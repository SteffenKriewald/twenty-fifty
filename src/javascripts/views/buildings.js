window.twentyfifty.views.buildings = function() {

  this.setup = function() {
      $("#results").append("<div id='buildings'><div id='buildings_energy_emissions' class='buildings'></div><div id='buildings_heat' class='buildings'></div><div class='clear'></div></div>");

      containers_1 = d3.select("#buildings_energy_emissions").selectAll(".chart")
	  .data(['top_container_1', 'bottom_container_1']);

      containers_2 = d3.select("#buildings_heat").selectAll(".chart")
	  .data(['top_container_2', 'bottom_container_2']);

      containers_1.enter()
	  .append('div')
	  .attr('id', Object)
	  .attr('class', 'chart');

      containers_2.enter()
	  .append('div')
	  .attr('id', Object)
	  .attr('class', 'chart');
      
      this.emissions_from_buildings_chart = timeSeriesStackedAreaChart()
	  .title("Emissions from Buildings")
	  .unit('');
      //      .css_for_label(css_for_labels)
      //      .max_value(4000);

      this.buildings_energy_use_by_fuel_chart = timeSeriesStackedAreaChart()
	  .title("Buildings energy use by fuel")
	  .unit('');
      //      .css_for_label(css_for_labels)
      //      .max_value(4000);
      
      this.heat_supplied_by_technology_chart = timeSeriesStackedAreaChart()
	  .title("Heat Supplied by Technology")
	  .unit('');
//      .css_for_label(css_for_labels)
//      .min_value(-500)
      //	  .max_value(1000);
//      this.total_heat_demand_chart = lineGraphChart()
//	  .title("Total Heat Demand")
//	  .unit('');
////      .css_for_label(css_for_labels)
////      .min_value(-500)
////	  .max_value(1000);
      
  };

  this.teardown = function() {
      $("#results").empty();
      this.emissions_from_buildings_chart = null;
      this.buildings_energy_use_by_fuel_chart = null;
      this.heat_supplied_by_technology_chart = null;
//      this.total_heat_demand_chart = null;
  };

  this.updateResults = function(pathway) {
    this.pathway = pathway;
    this.choices = twentyfifty.choices;

  };


  return this;

}.call({});
