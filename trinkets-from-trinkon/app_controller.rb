#The app_controller controls the interfaction between Facebook users and the app.  
#It is the interface for individual_users.  
#Travis Sheppard, Trinkon LLC, 2012
#[EDIT] Paul Elliott, independent contractor, 2013
#       -heavily modified to accommodate Facebook platform changes
#       -now handles mobile and page tab usage
#       -moved user auth pipeline back to send Trinkon checkpoint

# TODO
# * need to update documentation to follow TomDoc (http://tomdoc.org/)
class AppController < ApplicationController
  require 'will_paginate/array'
  skip_before_filter :verify_admin_user #white-list the admin verification.
                                        #-TS May 4, 2012
  
  #Filters..see protected methods --PE 10/30/14
  before_filter :set_p3p
  before_filter :app_portal, :only => [:index]
  before_filter :page_tab_app_portal, :only => [:index]
  before_filter :populate_friends_activity, :only => [
    :index, :show_all_organizations, :my_score, :show_sent_gifts, 
    :friends, :friend_by_last_name, :show_friend, :show_user_gift, :about
  ]
  before_filter :check_auth, :only => [
    :return_from_general_auth, :return_from_gift_auth
  ]
  
  #Rescues from errors..see private methods --PE 12/26/12
  rescue_from Errno::ECONNRESET, :with => :rescue_connection_reset
  rescue_from Koala::Facebook::APIError, :with => :rescue_api_error
  rescue_from NoMethodError, :with => :rescue_undefined_method
  
  #Gets configuration variables using settingslogic gem.  
  # Values are in config/config_variables.yml.  TS Mar 26, 2012
  APP_ID = Settings.app_id
  APP_SECRET = Settings.app_secret
  NUMBER_OF_TOP_TRINKONS = Integer(Settings.number_of_top_trinkons)
  FACEBOOK_URL = Settings.facebook_url
  CALLBACK_PREFIX = Settings.callback_prefix
  GIVE_CALLBACK = CALLBACK_PREFIX + "complete_gift_transaction?gift_state="
  GIFT_AUTH_CALLBACK = CALLBACK_PREFIX + "return_from_gift_auth"
  GEN_AUTH_CALLBACK = CALLBACK_PREFIX + "return_from_general_auth"

  #[Mobile] Redirects user returning to the application --PE 12/28/12
  def mobile_app_portal
    # log returning user into the app
    if params[:individual_user_id] && params[:secret]
      login(params[:individual_user_id], params[:secret])
    end

	  # if pathway param exists, follow it to the proper page
	  if params[:pathway]		
		  redirect_returning_user(params[:pathway],params)
	  else
		  redirect_to(:action => 'index')
	  end
  end

  #...other code...#

  #Return the user to Facebook app --PE 1/4/13
  def return_from_general_auth	
    # back from oauth wormhole..obtain facebook user and credentials
    oauth = Koala::Facebook::OAuth.new(APP_ID, APP_SECRET, GEN_AUTH_CALLBACK)
    session[:access_token] = oauth.get_access_token(params[:code])
    graph = Koala::Facebook::API.new(session[:access_token])
    facebook_user_id = graph.get_object("me")["id"]
    session[:individual_user_id] = 
      IndividualUser.login_or_create(facebook_user_id,session[:access_token])

    # determine where to redirect
    state, link = false, false
	  if params[:state]
      state = true
      decoded_state = ActiveSupport::JSON.decode(params[:state])
        
      # [Page Tab] return to the page tab
      if decoded_state['page_tab_id']
			  graph = Koala::Facebook::API.new
			  page_tab = graph.get_object("#{decoded_state['page_tab_id']}")
			  if page_tab["link"]
          link = true
				  encoded_page_state = URI.encode(ActiveSupport::JSON.encode(
            {:individual_user_id => session[:individual_user_id], 
              :secret => session[:access_token]
            }))
        end
      end
    end
    
    # handle the redirect
    if state && link
      redirect_to(page_tab["link"] + 
        "?sk=app_#{APP_ID}&app_data=#{encoded_page_state}")
    else
      redirect_to(create_fb_url_with_creds(session[:individual_user_id],
        session[:access_token]))
    end
    return
  end

  #...other code...#
  
  #Shows a list of your friends who have installed Trinkon, 
  # indexed by a specific letter --PE June 18, 2012
  def friend_by_last_name
	  @friend_alphabet.each do |openstruct_letter|
	    if openstruct_letter.letter.eql?(params[:sort_letter])
	      openstruct_letter.active_status = true
		  end
	  end
	  @friends.delete_if {|individual_user| individual_user.facebook_name.split(' ').last[0,1].upcase.eql?(params[:sort_letter]) == false}
  end

  #...other code...#

  #The gateway to sending Trinkons. 
  # User must select whether to send to self, or to a friend --PE 12/28/12
  def give_gift	
    @gift_template = GiftTemplate.find(params[:gift_template_id])
    @organization = @gift_template.organization
    @related_trinkons = @gift_template.similar_trinkons
  end
  
  #Redirects to authorization portal for basic permissions --PE 12/26/12
  def authorize_application
    # init auth state and oauth url..enter wormhole
    oauth = Koala::Facebook::OAuth.new(APP_ID, APP_SECRET, GIFT_AUTH_CALLBACK)
    encoded_auth_state = create_auth_state
    oauth_url = create_oauth_url(oauth.oauth_callback_url, encoded_auth_state)
    redirect_to(:action => "exit_portal", :url => oauth_url) and return
  end
  
  #Return to the Facebook application --PE 12/26/12
  def return_from_gift_auth
    # back from oauth wormhole..obtain facebook user and credentials
	  oauth = Koala::Facebook::OAuth.new(APP_ID, APP_SECRET, GIFT__AUTH_CALLBACK)
	  session[:access_token] = oauth.get_access_token(params[:code])
	  graph = Koala::Facebook::API.new(session[:access_token])
	  facebook_user_id = graph.get_object("me")["id"]
	  session[:individual_user_id] = IndividualUser.login_or_create(
      facebook_user_id,session[:access_token])

	  unless params[:state] # no state param..forgetaboutit
		  redirect_to(create_fb_url_with_creds(session[:individual_user_id],
        session[:access_token])) and return
	  end
	
	  #[Page Tab] determine if we came from page tab..if so, back to Kansas
	  decoded_state = ActiveSupport::JSON.decode(params[:state])
    page_id, link = false, false
	  if decoded_state['page_tab_id']
      page_id = true
		  graph = Koala::Facebook::API.new
		  page_tab = graph.get_object("#{decoded_state['page_tab_id']}")
		  if page_tab["link"]
        link = true
		    encoded_page_state = URI.encode(ActiveSupport::JSON.encode(
          {:pathway => decoded_state['pathway'], 
          :gift_template_id => decoded_state['gift_template_id'], 
          :individual_user_id => session[:individual_user_id], 
          :secret => session[:access_token]}))
		  end
	  end
    
    # handle the redirect
    if page_id && link
      redirect_to(page_tab["link"] + 
        "?sk=app_#{APP_ID}&app_data=#{encoded_page_state}")
    else
	    redirect_to(create_fb_url_with_creds(session[:individual_user_id], 
        session[:access_token], decoded_state))
    end
	  return
  end
 
  #...other code...#
  
  #The page from which a user sends a gift to a friend.  
  #Makes sure the gift can be sent, 
  #then provides a JSON of friends (to autocomplete specs), 
  #as well as 3 related Trinkons. --PE 12/28/12
  def give_gift_friends	
    @gift_template = GiftTemplate.find(params[:gift_template_id])
    @organization = @gift_template.organization
    @related_trinkons = @gift_template.similar_trinkons
    if (@organization.display_type == "unbranded")
	    @postTitle = @gift_template.title
    else
	    @postTitle = @organization.display_name + " " + @gift_template.title
    end
	  @app_id = APP_ID
	  @link = FACEBOOK_URL

    @gift_state = create_gift_state
    @redirect_uri = GIVE_CALLBACK
		
	  current_user = IndividualUser.find(session[:individual_user_id])
		
	  friend_list = []
	
	  current_user.friends_from_api(session[:access_token]).each do |friend| 
	    friend_list << Hash["label" => friend["name"], 
        "value" => friend["name"], "id" => friend["id"].to_i]
	  end
	  @friend_list_json = friend_list.to_json
	
	  # assures the gift can be sent (see gift_template.rb, can_be_sent), and that its quantity is > 0.  Provides proper error message if not.  -TS April 27, 2012
	  unless (@gift_template.can_be_sent(current_user.facebook_id) && 
      @gift_template.quantity_available > 0)
		  redirect_to(:action => "show_organization", 
        :organization_id => @gift_template.organization_id)
		  if @gift_template.status == "admin_visible"
		    flash[:notice] = "Sorry, that gift can only be sent by authorized senders."
		  else
		    flash[:notice] = "Sorry, that gift can't be sent right now."
	    end
	  end
  end
    
  #This method records which trinkon the user sent, and to whom it was sent.
  # Currently, it logs that the trinkon was sent regardless of whether
  # the user clicked send or cancel --PE 1/3/13
  def complete_gift_transaction
    # find gift and user from gift state..safely preserved thru the wormhole!
    decoded_gift_state = ActiveSupport::JSON.decode(params[:gift_state])
    gift_template = GiftTemplate.find(decoded_gift_state['gift_template_id'])
    current_individual_user = IndividualUser.find(
      decoded_gift_state['individual_user_id'])
    access_token = decoded_gift_state['access_token']
    if decoded_gift_state['recipient'] = "self"
      recipient_id = current_individual_user.id
    else
      recipient_id = IndividualUser.find_by_facebook_id(
        decoded_gift_state['recipient']).id
    end
	  is_private = decoded_gift_state['is_private']
  
    # log points and create record of sent gift
    current_individual_user.increment_user_points(1)
    user_gift = UserGift.new(:individual_user_id => recipient_id, 
      :sender_user_id => current_individual_user.id, 
      :gift_template_id => gift_template.id, :time_acquired => Time.now)
    user_gift.status = "sent"  
    user_gift.save
	
    if (gift_template.quantity_available < 10000)
      gift_template.decrease_quantity_available(1)
    end
	
	  # return user to app, preserving platform login creds,
    # and redirecting to proper organization storefront
	  if decoded_gift_state['page_tab_id']
		  graph = Koala::Facebook::API.new
	    page_tab = graph.get_object("#{decoded_gift_state['page_tab_id']}")
		  if page_tab["link"]
		    encoded_page_state = URI.encode(ActiveSupport::JSON.encode(
          {:pathway => "gift_sent", 
          :organization_id => gift_template.organization_id, 
          :individual_user_id => current_individual_user.id, 
          :secret => access_token}))
		    redirect_to(page_tab["link"] + 
          "?sk=app_#{APP_ID}&app_data=#{encoded_page_state}")
		  else #No link provided..user brought to main app
		    redirect_to "index" 
		  end
	  else
	    redirect_to(FACEBOOK_URL + "?pathway=#{'gift_sent'}&" +
        "organization_id=#{gift_template.organization_id}&" +
        "individual_user_id=#{current_individual_user.id}&" + 
        "secret=#{access_token}")
	  end
	  return
  
  # rescue from all the badness
  rescue
    flash[:notice] = "Your Trinkon could not be posted to Facebook."
    redirect_to(:controller => "app", 
      :action => "give_gift", :gift_template_id => gift_template.id)
    return
  end
   
  #...other code...#
  
  #######
  private
  #######

  #Sets a p3p header to allow cookies in iframes in IE browser. TS Mar 29, 2012
  def set_p3p
    response.headers["P3P"]='CP="NOI DSP LAW NID"'
  end
  
  #[Removed authorize_individual_user] deprecated --PE 12/26/12 
  
  #Properly redirect when returning to the application --PE 12/26/12
  def app_portal
	  if params[:individual_user_id] && params[:secret]
	    login(params[:individual_user_id], params[:secret])
	  end
	
    # pathway parameter indicates that the app should return to a specific page
    if params[:pathway]
	    redirect_returning_user(params[:pathway],params)
	  end
  end
  
  #[Page Tab] Detect page tab and redirect to proper page --PE 12/26/12
  def page_tab_app_portal
	  unless params[:signed_request]
	    return # no signed_request..forgetaboutit
    end
	
    oauth = Koala::Facebook::OAuth.new(APP_ID, APP_SECRET)
	  signed_request = oauth.parse_signed_request(params[:signed_request])
    unless signed_request["page"]
      return # no page in signed_request..forgetaboutit
    end
    
    # page tab detected, determine where to redirect
    session[:page_tab_id] = signed_request["page"]["id"]
    if signed_request["app_data"]
      decoded_page_state = ActiveSupport::JSON.decode(signed_request["app_data"])
        
      # login authorizing user 
      if decoded_page_state['individual_user_id'] && decoded_page_state['secret']
        login(decoded_page_state['individual_user_id'], 
          decoded_page_state['secret'])
      end

      # follow pathway param to proper page..back to Kansas
      if decoded_page_state['pathway']
        redirect_returning_user(decoded_page_state['pathway'],decoded_page_state)
      end
    end
    
    # catch the fallout..redirect to the right page tab [at least]
    organization_page_tab = OrganizationPageTab.find_by_page_tab_facebook_id(
      session[:page_tab_id])
    return unless organization_page_tab
    redirect_to(:action=>"show_organization",
      :organization_id=>organization_page_tab.organization_id)
  end
  
  #Redirect a user returning to the app to proper page --PE 1/3/13
  def redirect_returning_user(pathway, options = {})
    params = {}
    case pathway
	  when "gift_sent"
      params[:action] = "show_organization"
      params[:organization_id] = options['organization_id']
	  when "self"
      params[:action] = "give_gift_self"
      params[:gift_template_id] = options['gift_template_id']
	  when "friend"
      params[:action] = "give_gift_friends"
      params[:gift_template_id] = options['gift_template_id']
	  else
		  flash[:notice] = "We're sorry, but something went wrong. Please try again."
      params[:action] = "index"
	  end
    redirect_to(params) and return
  end
  
  #Redirect non-auth'ed user to app..'do not pass Go, do not collect $200'
  #--PE 10/30/14
  def check_auth
    unless params[:code]
	    redirect_to(FACEBOOK_URL) and return
    end
  end
  
  #Creates a JSON to safely pass page params thru Facebook's auth wormhole 
  #--PE September 23, 2012
  def create_auth_state
	  auth_state = {:pathway => params[:gift_template][:pathway], 
      :gift_template_id => params[:gift_template][:gift_template_id]}

    if session[:page_tab_id] #[Page Tab] include the page tab id
	    auth_state[:page_tab_id] = session[:page_tab_id]
	  end
	  return URI.encode(ActiveSupport::JSON.encode(auth_state))
  end
  
  #Creates a JSON of gift parameters --PE 12/27/12
  def create_gift_state
	  gift_state = {:gift_template_id => params[:gift_template_id], 
      :individual_user_id => session[:individual_user_id], 
      :access_token => session[:access_token]}
	
	  if session[:page_tab_id] #[Page Tab] include the page tab id
	    gift_state[:page_tab_id] = session[:page_tab_id]
	  end
	  return URI.encode(ActiveSupport::JSON.encode(gift_state))
  end
  
  #Creates facebook app url with user creds [and state] appended
  #--PE 10/30/14
  def create_fb_url_with_creds(indv_u_id, ac_token, decoded_state: nil)
    url = FACEBOOK_URL + "?individual_user_id=#{indv_u_id}&secret=#{ac_token}"
    if decoded_state
      url += "pathway=#{decoded_state['pathway']}" +
        "&gift_template_id=#{decoded_state['gift_template_id']}"  
  end
  
  #Creates facebook oauth dialog url
  #--PE 10/30/14
  def create_oauth_url(cb_url, encoded_auth_state)
    oauth_url = "https://www.facebook.com/dialog/oauth/?client_id=" +
      "#{APP_ID}&redirect_uri=#{cb_url}&state=#{encoded_auth_state}"
  end

  #########
  protected
  #########  

  #Renew the access_token after API failure --PE July 17, 2012
  def rescue_api_error
    session.delete(:access_token)
	  session.delete(:individual_user_id)
	  oauth = Koala::Facebook::OAuth.new(APP_ID, APP_SECRET, AUTH_CALLBACK)
	  redirect_to(:action => "exit_portal", :url => oauth.url_for_oauth_code)
  end

  #Redirect to app index page after connection is reset --PE July 23, 2012
  def rescue_connection_reset
    redirect_to(:action => "index")
    flash[:notice] = "Sorry, something went wrong (connection reset). " +
      "Please try again."
  end

  #Redirect to app index page after undefined method errors --PE July 23, 2012
  def rescue_undefined_method
    redirect_to(:action => "index")
    flash[:notice] = "Sorry, something went wrong (undefined method). " +
      "Please try again."
  end
end

