window.twentyfifty.views.story = function() {

  this.setup = function() {
  $("#results").append('<div class="viewToggle"><div id="vT1" class="vT1 vTactive" onclick="showView1()">Emissions/Generation</div><div id="vT2" class="vT2" onclick="showView2()">Capacity & Peak</div></div>')
  $("#results").append("<div id='stories' class='viewContainer'><div id='view1' class='overview visible'></div><div id='view2' class='overview hidden'></div><div class='clear'></div></div>");

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

    this.emissions_from_electricity_chart = timeSeriesStackedAreaChart()
	  .title("Emissions - Electricity Generation")
	  .unit('Mt.CO2e/yr')
    .max_value(400)
    .css_for_label({
      "Biomass & Waste CCS":"Biomass-Waste-CCS",
      "Biomass & Waste":"Biomass-Waste",
      "Gas CCS":"Gas-CCS",
      "Gas":"Gas",
      "Coal":"Coal",
      "Total":"Total"
    });

    this.electricity_supply_chart = timeSeriesStackedAreaChart()
	  .title("Electricity Supply")
	  .unit('TWh/yr')
    .max_value(1000)
    .css_for_label({
      "Biomass & Waste CCS":"Biomass-Waste-CCS",
      "Nuclear":"Nuclear",
      "Wind":"Wind",
      "Solar":"Solar",
      "Tidal, Wave, Hydro":"Tidal-Wave-Hydro",
      "Biomass & Waste":"Biomass-Waste",
      "Gas CCS":"Gas-CCS",
      "Gas":"Gas",
      "Coal":"Coal",
      "CHP":"CHP",
      "Net Imports":"Net-Imports",
      "Total":"Total"
    });

    this.electricity_capacity_chart = timeSeriesStackedAreaChart()
	  .title("Electrical Capacity")
	  .unit('GW')
    .max_value(400)
    .css_for_label({
      "Biomass & Waste CCS":"Biomass-Waste-CCS",
      "Nuclear":"Nuclear",
      "Wind":"Wind",
      "Solar":"Solar",
      "Tidal, Wave, Hydro":"Tidal-Wave-Hydro",
      "Biomass & Waste":"Biomass-Waste",
      "Gas CCS":"Gas-CCS",
      "Gas":"Gas",
      "Coal":"Coal",
      "Interconnectors":"Interconnectors",
      "Seasonal Storage":"Seasonal-Storage",
      "DSR & Batteries":"DSR-Batteries",
      "Total":"Total",
    });

    this.peak_gate_chart = timeSeriesStackedAreaChart()
	  .title("Peak Gate Power Demand")
	  .unit('GW')
    .max_value(400)
    .css_for_label({
      "Agriculture":"Agriculture",
      "Industry":"Industry",
      "Transport":"Transport",
      "Buildings Other":"Buildings-Other",
      "Buildings Heat":"Buildings-Heat",
      "Energy Supply & GGR":"Energy-Supply-GGR",
      "Total":"Total"
    });

  };

  this.teardown = function() {
      $("#results").empty();
      this.emissions_from_electricity_chart = null;
      this.electricity_supply_chart = null;
      this.electricity_capacity_chart = null;
      this.peak_gate_chart = null;
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

      this.emissions_from_electricity_chart.setMode(_mode);
      this.electricity_supply_chart.setMode(_mode);
      this.electricity_capacity_chart.setMode(_mode);
      this.peak_gate_chart.setMode(_mode);

      d3.select('#top_container_1')
  	  .datum(convert_capacity_table_to_hash(pathway.electricity_emissions))
  	  .call(this.emissions_from_electricity_chart);
      d3.select('#bottom_container_1')
  	  .datum(convert_capacity_table_to_hash(pathway.electricity_generation))
  	  .call(this.electricity_supply_chart);

      d3.select('#top_container_2')
      .datum(convert_capacity_table_to_hash(pathway.electricity_capacity))
      .call(this.electricity_capacity_chart);

      d3.select('#bottom_container_2')
      .datum(convert_capacity_table_to_hash(pathway.electricity_peak))
      .call(this.peak_gate_chart);

      document.getElementById('view1').setAttribute('class', classView1);
      document.getElementById('view2').setAttribute('class', classView2);

    };

    lineChart();

    return this;

}.call({});
