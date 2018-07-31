#!/usr/bin/env ruby
begin
  require 'excel_to_code'
rescue LoadError
  puts "You need to install excel_to_code"
  puts "sudo gem install bundler"
  puts "bundle"
  exit
end

root_directory = File.expand_path(File.join(File.dirname(__FILE__),'..'))
model_directory = File.join(root_directory, 'model')

command = ExcelToC.new

#command.excel_file = File.join(model_directory, "model_mockup.xlsm")
excel_file = "model.xlsm"

puts "input is '" + excel_file + "'"

command.excel_file = File.join(model_directory, excel_file)
command.output_directory = model_directory
command.output_name = 'model'

command.named_references_that_can_be_set_at_runtime = ['input.lever.ambition', 'input.lever.end', 'input.lever.start']

command.named_references_to_keep = lambda do |named_reference|
  named_reference =~ /^(input|output)\./i
end

command.create_makefile = false
command.create_rakefile = true
# Trying to track down a bug
command.dump_steps ||= {[:'Power_DRIVERS', :U444] => true}

command.actually_compile_code = true
command.actually_run_tests = true
command.treat_external_references_as_local = true
command.allow_unknown_functions = true

command.go!
