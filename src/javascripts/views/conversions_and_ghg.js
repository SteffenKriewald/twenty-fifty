window.twentyfifty.views.conversions_and_ghg = function() {

  this.setup = function() {
      $("#results").append("<div id='conversions_and_ghg'><div id='conversions_and_ghg_emissions' class='conversions_and_ghg'></div><div id='conversions_and_ghg_gas_grid' class='conversions_and_ghg'></div><div class='clear'></div></div>");

      containers_1 = d3.select("#conversions_and_ghg_emissions").selectAll(".chart")
	  .data(['top_container_1', 'bottom_container_1']);

      containers_2 = d3.select("#conversions_and_ghg_gas_grid").selectAll(".chart")
	  .data(['top_container_2', 'bottom_container_2']);

      containers_1.enter()
	  .append('div')
	  .attr('id', Object)
	  .attr('class', 'chart');

      containers_2.enter()
	  .append('div')
	  .attr('id', Object)
	  .attr('class', 'chart');
      
      this.emissions_from_conversions_and_ghg_chart = timeSeriesStackedAreaChart()
	  .title("Emissions from Energy Conversion & GHG Reduction")
	  .unit('');
      //      .css_for_label(css_for_labels)
      //      .max_value(4000);

      this.total_co2_captured_chart = timeSeriesStackedAreaChart()
	  .title("Total CO2 captured (CCS & GHG Reduction)")
	  .unit('');
      //      .css_for_label(css_for_labels)
      //      .max_value(4000);
      
      this.gas_grid_fuel_share_chart = timeSeriesStackedAreaChart()
	  .title("Gas Grid fuel Share")
	  .unit('');
//      .css_for_label(css_for_labels)
//      .min_value(-500)
      //	  .max_value(1000);
      this.hydrogen_production_by_technology_chart = timeSeriesStackedAreaChart()
	  .title("Hydrogen Production by Technology")
	  .unit('');
//      .css_for_label(css_for_labels)
//      .min_value(-500)
//	  .max_value(1000);
      
  };

  this.teardown = function() {
      $("#results").empty();
      this.emissions_from_conversions_and_ghg_chart = null;
      this.total_co2_captured_chart = null;
      this.gas_grid_fuel_share_chart = null;
      this.hydrogen_production_by_technology_chart = null;
  };

  this.updateResults = function(pathway) {
    this.pathway = pathway;
    this.choices = twentyfifty.choices;

  };


  return this;

}.call({});
