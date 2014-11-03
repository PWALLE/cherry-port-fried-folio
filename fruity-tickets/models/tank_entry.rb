class TankEntry < ActiveRecord::Base
  belongs_to :ticket
  validate  :check_missing_value
  validates :measured_fruit_depth, numericality: true
  after_save { |te| te.destroy if te.measured_fruit_depth.blank? }

  def check_missing_value
    mfd = self.measured_fruit_depth.blank? ? 0 : 1
    tn  = self.tank_number.blank? ? 0 : 1
    if (mfd + tn == 1)
      errors.add(:tank_number, ": Some tanks are missing tank numbers or values. Please check.")
    end
  end

  def check_fruit_depth_numericality
  	unless self.measured_fruit_depth.blank?
  		if !(self.measured_fruit_depth.is_a? Numeric)
  			errors.add(:measured_fruit_depth, ": Fruit depth must be a number. Please correct.")
  		end
  	end
  end
end
