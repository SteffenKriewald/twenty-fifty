require_relative 'model'
require_relative 'model_version'

class ModelChoice
  attr_accessor :number
  attr_accessor :name
  attr_accessor :dstartdate
  attr_accessor :denddate
  attr_accessor :type
  attr_accessor :descriptions
  attr_accessor :lever_descriptions
  attr_accessor :long_descriptions
  attr_accessor :incremental_or_alternative
  attr_accessor :levels
  attr_accessor :doc
end

class DataFromModel
  attr_accessor :pathway

  # This connects to model.rb which
  # connects to model.c which is a
  # translation of model.xlsx
  def excel
    @excel ||= Model.new
  end

  # Data that changes as the user makes choices
  # The code should be in the form i0g2dd2pp1121f1i032211p004314110433304202304320420121
  # Where each letter or digit corresponds to a choice to be set in the Excel
  def calculate_pathway(code)
    # Need to make sure the Excel is ready for a new calculation
    excel.reset
    # Turn the i0g2dd2pp1121f1i032211p004314110433304202304320420121 into something like
    # [1.8,0.0,1.6,2.0,1.3,1.3,..]
    number_of_non_empty_levers = code.length / 3

    choices = convert_letters_to_float(code[0..number_of_non_empty_levers -1].split(''))
    starts = convert_letters_to_dates(code[number_of_non_empty_levers..(2 * number_of_non_empty_levers) -1].split(''))
    ends = convert_letters_to_dates(code[(2 * number_of_non_empty_levers)..(3 * number_of_non_empty_levers) -1].split(''))
    puts "\n================================================================================\n" +
         code[0..number_of_non_empty_levers -1]
         "\n================================================================================\n"
    puts "\n================================================================================\n" +
         code[number_of_non_empty_levers..(2 * number_of_non_empty_levers) -1]
         "\n================================================================================\n"
    puts "\n================================================================================\n" +
         code[2 * number_of_non_empty_levers..(3 * number_of_non_empty_levers) -1]
         "\n================================================================================\n"

    # Set the spreadsheet controls (input.choices is a named reference in the Excel)
    ## deactivatety dynamics (model calculations)
         excel.input_lever_ambition = choices
         excel.input_lever_start = starts
         excel.input_lever_end = ends
    # Read out the results, where each of these refers to a named reference in the Excel
    # (e.g. excel.output_impots_quantity refers to the output.imports.quantity named reference)
    {
      '_id' => code,
      'choices' => choices,
      'choicesDescriptions' => excel.output_lever_descriptions,
	# Gauge
	'mEreduction' => excel.output_metric_emissions_reduction_twentyfifty,
	'mEyrZero' => excel.output_metric_emissions_yrzero,
	# Warnings
	'warningsL4' => excel.output_warning_l4chosen, # ["Icon on?", 1] ["Warning Text", "..."]
	'warningsBio' => excel.output_warning_bio_imports,
	'warningsEP' => excel.output_warning_elec_peak,
	'warningsEx' => excel.output_warning_exceedl4_rate,
	'warningsLand' => excel.output_warning_land,
      'sankey' => excel.output_flows, # output.flows in the Excel
#      'ghg' => excel.output_emissions_by_sector, # output.emissions.by.sector in Excel
      'ghg' => excel.output_emissions_sector, # output.emissions.by.sector in Excel
      'electricity' => {
        'capacity' => excel.output_electricity_capacity_type
      },
      # Overview
      'emissions_sector' => excel.output_emissions_sector,
      'emissions_cumulative' => excel.output_emissions_cumulative,
      'energy_consumption' => excel.output_primary_energy_consumption,
      'final_energy_consumption' => excel.output_final_energy_consumption,
      # Buildings
      'buildings_emissions' => excel.output_buildings_emissions,
      'buildings_energy' => excel.output_buildings_energy_consumption,
      'buildings_heat_supply' => excel.output_buildings_heat_supply,
      'buildings_heat_demand' => excel.output_buildings_heat_demand,
      # Transport
      'tra_emissions' => excel.output_tra_emissions,
      'tra_energy' => excel.output_tra_energy_consumption,
      'tra_pass' => excel.output_tra_pass_distance,
      'tra_energy_con' => excel.output_tra_energy_consumption_vehicle,
      # Industry
      'ind_emissions' => excel.output_ind_emissions,
      'ind_energy_consumption' => excel.output_ind_energy_consumption,
      # co2 Reduction
      'emissions_removal' => excel.output_emissions_removal,
      'emissions_stored' => excel.output_emissions_stored_cumulative,
      'gas_grid' => excel.output_gas_grid_dist_supply,
      'hydrogen_production' => excel.output_hydrogen_production,
      # electricity
      'electricity_emissions' => excel.output_electricity_emissions,
      'electricity_generation' => excel.output_electricity_generation_type,
      'electricity_capacity' => excel.output_electricity_capacity_type,
      'electricity_peak' => excel.output_electricity_peak_demand,
      # land use
      'land_emissions' => excel.output_land_bio_emissions,
      'land_trade_off' => excel.output_land_area_trade_off,
      'land_bioenergy' => excel.output_land_bioenergy_production,
      'bioenergy_imports' => excel.output_bioenergy_imports,
      # import flow map
      'security_import_energy' => excel.output_security_import_energy,
      'security_import_fraction' => excel.output_security_import_fraction,
      # map - land demand
      # index 8=2050; 18=2100
      'map' => excel.output_land_map_area,
      # map - length of wave front
      'map_distance' => excel.output_land_map_distance,
      # map - number of units (power plants)
      'map_units' => excel.output_land_map_numberunits,
    }
  end

  # Data that doesn't change with user choices (more structural)

  def choices
    @choices ||= generate_choices
  end

  def generate_choices
    choices = []
    types.each_with_index do |choice_type,i|
      next if choice_type == nil
      next if choice_type == 0.0
      incremental = choice_type =~ /[abcd]/i
      choice = ModelChoice.new
      choice.number = i
      choice.name = names[i]
      choice.dstartdate = dstartdates[i].round
      choice.denddate = denddates[i].round
      choice.type = choice_type
      choice.incremental_or_alternative =  incremental ? 'alternative' : 'incremental'
      choice.descriptions = descriptions[i].drop(1)
      choice.long_descriptions = descriptions[i][0]
      choice.levels = incremental ? 'A'.upto(choice_type.upcase) : 1.upto(choice_type.to_i)
#      choice.doc = one_page_note_filenames[i]
      choices << choice
    end
    choices
  end

  def reported_calculator_version
    excel.output_version
  end

  def types
    # no longer needed (?), no named range in excel
    @types ||= [4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4]
  end


  def choice_sizes
    sizes = {}
    choices.each do |choice|
      sizes[choice.number] = choice.levels.to_a.size
    end
    sizes
  end

  def names
    @names ||= excel.output_lever_names.flatten
  end

  def dstartdates
    @dstartdates ||= excel.output_lever_default_start.flatten
  end

  def denddates
    @denddates ||= excel.output_lever_default_end.flatten
  end

  def d_end_dates_letters
    @d_end_dates_letters ||= convert_dates_to_letters(denddates.map(&:to_i).map(&:to_s))
  end

  def d_start_dates_letters
    @d_start_dates_letters ||= convert_dates_to_letters(dstartdates.map(&:to_i).map(&:to_s))
  end

  def descriptions
    # @descriptions ||= excel.input_descriptions
    @descriptions ||= excel.output_lever_descriptions
  end

  def long_descriptions
    @long_descriptions ||= excel.input_long_descriptions
  end

  def example_pathways
        @example_pathways ||= generate_example_pathways
#        @example_pathways = []
  end

  def one_page_note_filenames
#    @one_page_note_filenames ||= excel.input_onepagenotes.flatten
        @one_page_note_filenames = []
  end

  def generate_example_pathways
    # Transpose the data so that every row is an example pathway
    # data = excel.input_example_pathways.transpose
    data = excel.output_lever_example_ambition.transpose
    data = data.map do |pathway_data|
      {
        name: pathway_data[0],
        code: convert_float_to_letters(pathway_data[1..-4]).join,
        description: wrap(pathway_data[-3]),
        wiki: pathway_data[-2],
        cost_comparator: (c = pathway_data[-1]; c.is_a?(Numeric) ? c : nil )
      }
    end
  end

  def cost_comparator_pathways
    example_pathways.find_all do |e|
      e[:cost_comparator]
    end.sort_by do |e|
      e[:cost_comparator]
    end.map do |e|
      e[:code]
    end
  end

  # FIXME: Only wraps one line into two
  def wrap(string, wrap_at_length = 45)
    return "" unless string
    string = string.to_s
    length_so_far = 0
    string.split.partition do |word|
      length_so_far = length_so_far + word.length + 1 # +1 for the trailing space
      length_so_far > wrap_at_length
    end.reverse.map { |a| a.join(" ") }.join("\n")
  end

  # Set the 9 decimal points between 1.1 and 3.9
  FLOAT_TO_LETTER_MAP = Hash["abcdefghijklmnopqrstuvwxyzABCD".split('').map.with_index { |l,i| [(i/10.0)+1,l] }]
  FLOAT_TO_LETTER_MAP[0.0] = '0'
  FLOAT_TO_LETTER_MAP[1.0] = '1'
  FLOAT_TO_LETTER_MAP[2.0] = '2'
  FLOAT_TO_LETTER_MAP[3.0] = '3'
  FLOAT_TO_LETTER_MAP[4.0] = '4'

  LETTER_TO_FLOAT_MAP = FLOAT_TO_LETTER_MAP.invert


  LETTER_TO_DATES_MAP = Hash[]
  LETTER_TO_DATES_MAP['a'] = '2020'
  LETTER_TO_DATES_MAP['b'] = '2025'
  LETTER_TO_DATES_MAP['c'] = '2030'
  LETTER_TO_DATES_MAP['d'] = '2035'
  LETTER_TO_DATES_MAP['e'] = '2040'
  LETTER_TO_DATES_MAP['f'] = '2045'
  LETTER_TO_DATES_MAP['g'] = '2050'
  LETTER_TO_DATES_MAP['h'] = '2055'
  LETTER_TO_DATES_MAP['i'] = '2060'
  LETTER_TO_DATES_MAP['j'] = '2065'
  LETTER_TO_DATES_MAP['k'] = '2070'
  LETTER_TO_DATES_MAP['l'] = '2075'
  LETTER_TO_DATES_MAP['m'] = '2080'
  LETTER_TO_DATES_MAP['n'] = '2085'
  LETTER_TO_DATES_MAP['o'] = '2090'
  LETTER_TO_DATES_MAP['p'] = '2095'
  LETTER_TO_DATES_MAP['q'] = '2100'

  DATES_TO_LETTER_MAP = LETTER_TO_DATES_MAP.invert

  def convert_float_to_letters(array)
    array.map do |entry|
      case entry
      when Float; FLOAT_TO_LETTER_MAP[entry] || entry
      when nil; 0
      else entry
      end
    end
  end

  def convert_letters_to_float(array)
    array.map do |entry|
      LETTER_TO_FLOAT_MAP[entry].to_f || entry.to_f
    end
  end

  def convert_letters_to_dates(array)
    array.map do |entry|
      LETTER_TO_DATES_MAP[entry].to_i || entry.to_i
    end
  end

  def convert_dates_to_letters(array)
    array.map do |entry|
      DATES_TO_LETTER_MAP[entry] || entry
    end
  end



end

if __FILE__ == $0
  g = DataFromModel.new
  initial_choices = g.excel.input_choices.flatten

  tests = 100
  t = Time.now
  a = []
  c = initial_choices.map { rand(4)+1 }.join
  tests.times do
    a << g.calculate_pathway(c)
  end
  te = Time.now - t
  puts "Problem" if a.any? { |r| r != a.first }
  puts "#{te/tests} seconds per run"
  puts "#{tests/te} runs per second"
end
