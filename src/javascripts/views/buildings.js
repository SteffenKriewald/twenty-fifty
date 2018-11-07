window.twentyfifty.views.buildings = function() {

  this.setup = function() {
  $("#results").append('<div class="viewToggle"><div id="vT1" class="vT1 vTactive" onclick="showView1()">Energy & Emissions</div><div id="vT2" class="vT2" onclick="showView2()">Heat</div></div>')
  $("#results").append("<div id='buildings' class='viewContainer'><div id='view1' class='overview visible'></div><div id='view2' class='overview hidden'></div><div class='clear'></div></div>");

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
    .max_value(200)
    .css_for_label({
      "Residential":"Residential",
      "Non-residential":"Non-residential",
      "Indirect emissions":"Indirect-emissions",
      "Total Direct":"Total-Direct"
    });


      this.buildings_energy_consumption_chart = timeSeriesStackedAreaChart()
	  .title("Energy Consumption - Buildings & Heat Networks")
	  .unit('TWh/yr')
    .max_value(2000)
    .css_for_label({
      "Waste Heat":"Waste-Heat",
      "Electricity":"Electricity",
      "Hydrogen":"Hydrogen",
      "Bioenergy":"Bioenergy",
      "Natural Gas":"Natural-Gas",
      "Oil":"Oil",
      "Coal":"Coal",
      "Environmental Heat":"Environmental-Heat",
      "Total":"Total"
    });

      this.buildings_heat_supply_chart = timeSeriesStackedAreaChart()
	  .title("Heat Supplied")
	  .unit('TWh/yr')
    .max_value(1000)
    .css_for_label({
      "District Heat Network ":"District-Heat-Network",
      "Heat Pump":"Heat-Pump",
      "Hybrid Heat Pump/Boiler ":"Hybrid-Heat-Pump-Boiler",
      "Electric Heater ":"Electric-Heater",
      "Gas Boiler ":"Gas-Boiler",
      "Oil/Solid Fuel Boiler":"Oil-Solid-Fuel-Boiler",
      "Total":"Total"
    });
//      .min_value(-500)
//	  .max_value(1000);

    this.buildings_heat_demand_chart = timeSeriesStackedAreaChart()
    .title("Heat Demand")
    .unit('TWh/yr')
    .max_value(1000)
    .css_for_label({
      "Residential Space Heat":"Residential-Space-Heat",
      "Residential Hot Water":"Residential-Hot-Water",
      "Non-Residential Space Heat":"Non-Residential-Space-Heat",
      "Non-Residential Hot Water":"Non-Residential-Hot-Water",
      "Total":"Total"
    });
    //      .min_value(-500)
    //	  .max_value(1000);


  };

  this.teardown = function() {
      $("#results").empty();
      this.emissions_from_buildings_chart = null;
      this.buildings_energy_consumption_chart = null;
      this.buildings_heat_supply_chart = null;
      this.buildings_heat_demand_chart = null;
  };

    this.updateResults = function(pathway, mode) {
      var _mode = (mode == 2050 || mode == 2100) ? mode : 2050;

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

      this.emissions_from_buildings_chart.setMode(_mode);
      this.buildings_energy_consumption_chart.setMode(_mode);
      this.buildings_heat_supply_chart.setMode(_mode);
      this.buildings_heat_demand_chart.setMode(_mode);

      d3.select('#top_container_1')
  	  .datum(convert_capacity_table_to_hash(pathway.buildings_emissions))
  	  .call(this.emissions_from_buildings_chart);

      d3.select('#bottom_container_1')
  	  .datum(convert_capacity_table_to_hash(pathway.buildings_energy))
  	  .call(this.buildings_energy_consumption_chart);

      //console.log('test ', convert_capacity_table_to_hash(pathway.buildings_heat_supply));
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
