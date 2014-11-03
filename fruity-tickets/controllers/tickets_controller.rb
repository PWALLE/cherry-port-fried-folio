class TicketsController < ApplicationController
  require 'spreadsheet' # PE exporting to spreadsheets now
  
  before_filter :is_varieties?
  
  before_action :set_ticket, only: [:show, :edit, :update, :destroy]
  
  SORT_CATEGORIES = {
    'Date' => 'date DESC',
    'Grower' => 'grower_name, date DESC',
    'Pit Number' => 'pit_number, date DESC',
    'Grade' => 'grade DESC, date DESC'
  }.freeze
  
  EXCEL_SORT_CATEGORIES = {
    'Date' => 'date',
    'Grower' => 'grower_name, date',
    'Pit Number' => 'pit_number, date',
    'Grade' => 'grade DESC, date'
  }.freeze # PE excel dates : oldest -> newest
  
  # --------------------------------- ACTIONS ------------------------------------ #

  # POST /tickets/unarchive_ticket
  # Unarchives selected ticket
  def unarchive
    @ticket    = Ticket.find(params[:id])
    @ticket.unarchive
    @ticket.save!
    redirect_to archived_tickets_path, notice: "Ticket unarchived."
  end

  # POST /tickets/archive_by_date
  # Archives tickets on or before date param
  def archive_by_date
    archive_date = parse_date_params(params[:archive_date])
    Ticket.archive_by_date(archive_date)
    redirect_to tickets_path, notice: "All tickets recorded before " + archive_date.to_s + " have been archived."
  end

  # GET /tickets/archived
  # Displays the currently archived tickets and provides archive controls (PE 7/1/14)
  def archived
    @tickets = Ticket.archived
  end

  # POST /tickets/export
  # Exports current variety to an excel spreadsheet (PE 6/26/14)
  def export
    #Initialize workbook, get current variety
    book                                  = Spreadsheet::Workbook.new
    book.default_format                   = Spreadsheet::Format.new :size => 12
    sheets                                = []
    variety                               = params[:variety]
    format                                = Spreadsheet::Format.new :weight => :bold, :size => 12
	
    #Create separate spreadsheets ordered by date, grower, and grade
    EXCEL_SORT_CATEGORIES.each do |k, v|
      sheet                               = book.create_worksheet
      sheet.name                          = "Sort By " + k
      sheet.row(0).default_format         = format
      sheet.row(0).concat [ 'Date', 'Grower Name', 'Ticket No.', 'Pit No.', 'Total Tanks', 'Grade', 'Weight' ]
      row = 1
      
      
      Ticket.active( variety, v ).each do |t|
        sheet.update_row row, t.date.to_time.getlocal.strftime( "%m/%d/%y" ), t.grower_name, t.id, t.get_pit, t.total_tanks, t.grade.to_s + "%", t.weight
        row += 1
      end
      sheets << sheet
    end

    #Save the workbook
    spreadsheet                            = StringIO.new 
    book.write spreadsheet 
    send_data spreadsheet.string, :filename => "#{variety}_#{Time.now.getlocal.strftime( "%m-%d-%y" )}.xls", :type =>  "application/vnd.ms-excel"
  end
  

  # GET /tickets
  # GET /tickets.json
  # Main Page (PE 6/26/14)
  def index
    #See private callbacks
    set_variety(params[:variety])
    @varieties       = Variety.get_varieties
    set_sort(params[:sort])
    @sort_categories = SORT_CATEGORIES

    @tickets         = Ticket.active( session[:variety], SORT_CATEGORIES[session[:sort]] )
  end


  # GET /tickets/1
  # GET /tickets/1.json
  def show
    @count = 1 # PE @count enables clumsy but pretty color scheme in views
  end


  # GET /tickets/new
  # Initialize a new Ticket with default 60 tank_entries (PE 6/26/14)
  def new
    @ticket    = Ticket.new
    60.times { @ticket.tank_entries.build }
	  @varieties = Variety.get_varieties # PE for variety dropdown in view..helper?
    @count     = 1
  end
  

  # GET /tickets/1/edit
  # Retrieve requested Ticket and pad extra tank_entries for editing PE 6/26/14
  def edit
    @ticket    = Ticket.find(params[:id])
    ( 60 - @ticket.tank_entries.count ).times { @ticket.tank_entries.build }
	  @varieties = Variety.get_varieties
    @count     = 1
  end


  # POST /tickets
  # POST /tickets.json
  # Retrieve newly filled Ticket and save to db (PE 6/26/14)
  def create
    @ticket                  = Ticket.new(ticket_params)
    @ticket.unarchive         # PE default = unarchived
    @ticket.init_measurements # PE a little ugly, but need to provide default values
    
    respond_to do |format|
      if @ticket.save && @ticket.calculate_measurements
        format.html { redirect_to @ticket, notice: 'Ticket was successfully created.' }
        format.json { render :show, status: :created, location: @ticket }
      else
        #Reinitialize form for error'd view
        ( 60 - @ticket.tank_entries.count ).times { @ticket.tank_entries.build }
    	  @varieties            = Variety.get_varieties
        @count                = 1
        format.html { render :new }
        format.json { render json: @ticket.errors, status: :unprocessable_entity }
      end
    end
  end


  # PATCH/PUT /tickets/1
  # PATCH/PUT /tickets/1.json
  # Retrieve updated Ticket and save to db (PE 6/26/14)
  def update
    respond_to do |format|
      if @ticket.update(ticket_params) && @ticket.calculate_measurements
        format.html { redirect_to @ticket, notice: 'Ticket was successfully updated.' }
        format.json { render :show, status: :ok, location: @ticket }
      else
        #Reinitialize form for error'd view
        ( 60 - @ticket.tank_entries.count ).times { @ticket.tank_entries.build }
    	  @varieties = Variety.get_varieties
        @count     = 1
        format.html { render :edit }
        format.json { render json: @ticket.errors, status: :unprocessable_entity }
      end
    end
  end


  # DELETE /tickets/1
  # DELETE /tickets/1.json
  def destroy
    @ticket.destroy
    respond_to do |format|
      format.html { redirect_to tickets_url, notice: 'Ticket was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  #######
  private
  #######
  
    # Use callbacks to share common setup or constraints between actions.
    def set_ticket
      @ticket             = Ticket.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def ticket_params
      params.require(:ticket).permit(:grower_name, :total_tanks, :fruit_variety, :pit_number, :factor, :total_cubic_feet, :weight, :weigher_initials, :grade, :grader_initials, :grade_comment, :date, tank_entries_attributes: [:id, :measured_fruit_depth, :tank_number])
    end
    
    # Massage date params into Date object (PE 7/19/14)
    def parse_date_params( date )
      Date.new( *(%w(1 2 3).map { |e| date["date(#{e}i)"].to_i }) )
    end
    
    # Allow sorting by varieties on index page (PE 6/26/14)
    def set_variety(arg)
      if arg
        session[:variety] = Variety.find_by_variety(arg) ? Variety.find_by_variety(arg).variety : Variety.first.variety
      else
        session[:variety] ||= Variety.first.variety
      end
    end
    
    # Allow sorting by fields on index page (PE 6/26/14)
    def set_sort(arg)
      if arg
        session[:sort]    = SORT_CATEGORIES.key?(arg) ? arg : SORT_CATEGORIES.keys[0]
      else
        session[:sort]    ||= SORT_CATEGORIES.keys[0]
      end
    end
    
    # Assert that <1 variety has been created (PE 6/26/14)
    def is_varieties?
      if Variety.count < 1
        flash[:notice]    = "Welcome to the ticket program. Please add some varieties."
        redirect_to varieties_path
      end
    end
end

