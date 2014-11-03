class Ticket < ActiveRecord::Base
  has_many :tank_entries
  
  accepts_nested_attributes_for :tank_entries, :reject_if => lambda { |te| te[:measured_fruit_depth].blank? }, allow_destroy: true
  validates_associated :tank_entries
  
  validates :grower_name, :total_tanks, :fruit_variety, :date, :weigher_initials, :grader_initials, :grade, presence: true
  validates :total_tanks, :total_cubic_feet, :weight, :grade, numericality: true
  validate :check_pit_num
  validate :check_total_tanks
  validate :archived?
  
  def self.archive_by_date( archive_date )
    Ticket.where( "date <= ?", archive_date ).each do |t|
      t.archived = true
      t.save!
    end
  end
  
  def self.active( variety, sort )
    Ticket.where(fruit_variety: variety, archived: false).order(sort)
  end
  
  def self.archived
    Ticket.where(archived: true).order("date DESC")
  end
  
  def unarchive
    self.archived = false
  end
  
  def get_pit
    return (self.pit_number.blank?) ? "--" : self.pit_number
  end
  
  def init_measurements
    self.weight = 0
    self.total_cubic_feet = 0
  end
  
  def calculate_measurements
  	#If there is no user-defined factor, use default
    if self.factor.blank?
  		self.factor                                         = Variety.get_factor(self.fruit_variety)
  	end
    self.total_cubic_feet                                   = 0
    
    #Calculate total_cubic_feet then multiply by factor for weight
    self.tank_entries.each do |t|
      #Remove extra empty tank_entries
  	  if t.measured_fruit_depth.blank?
  	    t.attributes[:_destroy]                             = "1"
  	  else
  		  self.total_cubic_feet += t.measured_fruit_depth
      end
  	end
    self.weight                                             = ( self.total_cubic_feet * self.factor ).round # PE round result after
    self.save!
  end

  private

  def check_pit_num
    variety                                                 = Variety.find_by_variety( self.fruit_variety )
    unless variety.is_pit == false
      if self.pit_number.nil? || self.pit_number == ""
        errors.add(:pit_number, ": This variety of cherry requires a pit number. Please enter a pit number.")
      end
    end
  end

  def check_total_tanks
  	tank_count                                              = 0
  	self.tank_entries.each do |te|
  		if !te.measured_fruit_depth.blank?
  		   tank_count += 1
  		end
  	end
  	if self.total_tanks != tank_count
  		errors.add(:total_tanks, ": Total number of tanks is incorrect. Please check.")
  	end
  end
  
  def archived?
    self.archived = false unless self.archived
  end
  
  def self.flatten_date(hash)

  end
end
