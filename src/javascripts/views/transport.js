window.twentyfifty.views.transport = function() {

  this.setup = function() {
      $("#results").append("<div id='transport'><div id='transport_energy_emissions' class='transport'></div><div id='transport_passenger' class='transport'></div><div class='clear'></div></div>");

      containers_1 = d3.select("#transport_energy_emissions").selectAll(".chart")
	  .data(['top_container_1', 'bottom_container_1']);

      containers_2 = d3.select("#transport_passenger").selectAll(".chart")
	  .data(['top_container_2', 'bottom_container_2']);

      containers_1.enter()
	  .append('div')
	  .attr('id', Object)
	  .attr('class', 'chart');

      containers_2.enter()
	  .append('div')
	  .attr('id', Object)
	  .attr('class', 'chart');
      
      this.emissions_from_transport_chart = timeSeriesStackedAreaChart()
	  .title("Emissions from Transport")
	  .unit('');
      //      .css_for_label(css_for_labels)
      //      .max_value(4000);

      this.energy_consumption_for_transport_chart = timeSeriesStackedAreaChart()
	  .title("Energy Consumption")
	  .unit('');
      //      .css_for_label(css_for_labels)
      //      .max_value(4000);
      
      this.passenger_distance_travelled_by_mode_chart = timeSeriesStackedAreaChart()
	  .title("Passenger Distance travelled by Mode")
	  .unit('');
//      .css_for_label(css_for_labels)
//      .min_value(-500)
      //	  .max_value(1000);
      this.car_share_of_demand_by_drivetrain_chart = timeSeriesStackedAreaChart()
	  .title("Car Share of Demand by Drivetrain")
	  .unit('');
//      .css_for_label(css_for_labels)
//      .min_value(-500)
//	  .max_value(1000);
      
  };

  this.teardown = function() {
      $("#results").empty();
      this.emissions_from_transport_chart = null;
      this.energy_consumption_for_transport_chart = null;
      this.passenger_distance_travelled_by_mode_chart = null;
      this.car_share_of_demand_by_drivetrain_chart = null;
  };

  this.updateResults = function(pathway) {
    this.pathway = pathway;
    this.choices = twentyfifty.choices;

  };


  return this;

}.call({});
