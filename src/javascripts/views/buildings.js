window.twentyfifty.views.buildings = function() {

  this.setup = function() {
  $("#results").append('<div class="viewToggle"><div id="vT1" class="vT1 vTactive" onclick="showView1()">Energy & Emissions</div><div id="vT2" class="vT2" onclick="showView2()">Heat</div></div>')
  $("#results").append("<div id='buildings' class='buildings'><div id='view1' class='overview visible'></div><div id='view2' class='overview hidden'></div><div class='clear'></div></div>");

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

      this.emissions_from_buildings_chart = timeSeriesStackedAreaChart()
	  .title("Emissions - Buildings")
	  .unit('Mt.CO2e/yr')
    .max_value(20);
    //      .css_for_label(css_for_labels)


      this.buildings_energy_consumption_chart = timeSeriesStackedAreaChart()
	  .title("Energy Consumption - Buildings & Heat Networks")
	  .unit('TWh/yr')
    .max_value(2000);
      //      .css_for_label(css_for_labels)

      this.buildings_heat_supply_chart = timeSeriesStackedAreaChart()
	  .title("Heat Supplied")
	  .unit('TWh/yr')
    .max_value(2000);
//      .css_for_label(css_for_labels)
//      .min_value(-500)
//	  .max_value(1000);

    this.buildings_heat_demand_chart = timeSeriesStackedAreaChart()
    .title("Heat Demand")
    .unit('TWh/yr')
    .max_value(2000);
    //      .css_for_label(css_for_labels)
    //      .min_value(-500)
    //	  .max_value(1000);


  };

  this.teardown = function() {
      $("#results").empty();
      this.emissions_from_buildings_chart = null;
      this.buildings_energy_consumption_chart = null;
      this.heat_supplied_by_technology_chart = null;
//      this.total_heat_demand_chart = null;
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
  	  .datum(convert_capacity_table_to_hash(pathway.buildings_emissions))
  	  .call(this.emissions_from_buildings_chart);

      d3.select('#bottom_container_1')
  	  .datum(convert_capacity_table_to_hash(pathway.buildings_energy))
  	  .call(this.buildings_energy_consumption_chart);

      d3.select('#top_container_2')
      .datum(convert_capacity_table_to_hash(pathway.buildings_heat_supply))
      .call(this.buildings_heat_supply_chart);

      d3.select('#bottom_container_2')
      .datum(convert_capacity_table_to_hash(pathway.buildings_heat_demand))
      .call(this.buildings_heat_demand_chart);

      document.getElementById('view1').setAttribute('class', classView1);
      document.getElementById('view2').setAttribute('class', classView2);

    };

    lineChart();

    return this;

}.call({});
