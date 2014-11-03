class Variety < ActiveRecord::Base
  validates :variety, :factor, presence: true
  validates :factor, numericality: true
  validates :variety, uniqueness: true
  
  def self.is_pit?(variety)
    v = Variety.find_by_variety(variety)
    return v.is_pit || false # PE careful..should return false if no pit
  end
    
  def self.get_varieties
    varieties = []
    
    self.find_each do |v|
      varieties << v.variety
    end
    varieties
  end
  
  def self.get_factor(variety)
    v         = Variety.find_by_variety(variety)
    factor    = (v.factor == -1) ? 1 : v.factor # PE varieties without factors are stored with value -1
  end
end
