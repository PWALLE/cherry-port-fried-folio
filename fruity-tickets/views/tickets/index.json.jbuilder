json.array!(@tickets) do |ticket|
  json.extract! ticket, :id, :grower_name, :total_tanks, :fruit_variety, :pit_number, :factor, :total_cubic_feet, :weight, :weigher_initials, :grade, :grader_initials, :grade_comment, :date
  json.url ticket_url(ticket, format: :json)
end
