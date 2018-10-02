window.twentyfifty.views.transport = function() {

  this.setup = function() {
  $("#results").append('<div class="viewToggle"><div id="vT1" class="vT1 vTactive" onclick="showView1()">Energy & Emissions</div><div id="vT2" class="vT2" onclick="showView2()">Demand & Technology</div></div>')
  $("#results").append("<div id='transport' class='viewContainer'><div id='view1' class='overview visible'></div><div id='view2' class='overview hidden'></div><div class='clear'></div></div>");

    containers_1 = d3.select("#view1").selectAll(".chart")
    .data(['top_container_1', 'bottom_container_1']);

    containers_2 = d3.select("#view2").selectAll(".chart")
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
	  .title("Emissions - Transport")
	  .unit('Mt.CO2e/yr');
      //      .css_for_label(css_for_labels)
      //      .max_value(4000);

      this.energy_consumption_for_transport_chart = timeSeriesStackedAreaChart()
	  .title("Energy Consumption - Transport")
	  .unit('TWh/yr');
      //      .css_for_label(css_for_labels)
      //      .max_value(4000);

      this.passenger_distance_travelled_by_mode_chart = timeSeriesStackedAreaChart()
	  .title("Passenger Distance Travelled")
	  .unit('Billion Passenger km/yr');
//      .css_for_label(css_for_labels)
//      .min_value(-500)
      //	  .max_value(1000);
      this.car_share_of_demand_by_drivetrain_chart = timeSeriesStackedAreaChart()
	  .title("All Transport Energy Consumption by Vehicle")
	  .unit('TWh/yr');
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
      updateGauge(pathway);
      // construct the data
      // connect the containers with charts and data
      //console.log('pathway ', pathway.ghg, twentyfifty.choices);
      //console.log('this.emissions_by_sector_chart ', this.emissions_by_sector_chart, pathway.ghg);

      classView1 = document.getElementById("view1").getAttribute('class');
      classView2 = document.getElementById('view2').getAttribute('class');
      document.getElementById('view1').setAttribute('class', 'overview visible');
      document.getElementById('view2').setAttribute('class', 'overview visible');

      d3.select('#top_container_1')
  	  .datum(convert_capacity_table_to_hash(pathway.tra_emissions))
  	  .call(this.emissions_from_transport_chart);

      d3.select('#bottom_container_1')
  	  .datum(convert_capacity_table_to_hash(pathway.tra_energy))
  	  .call(this.energy_consumption_for_transport_chart);

      d3.select('#top_container_2')
      .datum(convert_capacity_table_to_hash(pathway.tra_pass))
      .call(this.passenger_distance_travelled_by_mode_chart);

      d3.select('#bottom_container_2')
      .datum(convert_capacity_table_to_hash(pathway.tra_energy_con))
      .call(this.car_share_of_demand_by_drivetrain_chart);

      document.getElementById('view1').setAttribute('class', classView1);
      document.getElementById('view2').setAttribute('class', classView2);

    };

    lineChart();

    return this;

}.call({});
