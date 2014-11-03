json.array!(@varieties) do |variety|
  json.extract! variety, :id, :variety, :is_pit, :factor
  json.url variety_url(variety, format: :json)
end
