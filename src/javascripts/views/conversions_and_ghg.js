window.twentyfifty.views.conversions_and_ghg = function() {

  this.setup = function() {
  $("#results").append('<div class="viewToggle"><div id="vT1" class="vT1 vTactive" onclick="showView1()">Emissions Removal</div><div id="vT2" class="vT2" onclick="showView2()">Gas Grid & Hydrogen</div></div>')
  $("#results").append("<div id='conversions_and_ghg' class='viewContainer'><div id='view1' class='overview visible'></div><div id='view2' class='overview hidden'></div><div class='clear'></div></div>");

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

    this.emissions_from_conversions_and_ghg_chart = timeSeriesStackedAreaChart()
	  .title("Emissions Removal")
	  .unit('Mt.CO2e/yr')
    .max_value(50)
    .css_for_label({
      "Process CCS":"Process-CCS",
      "Direct Air Capture (CCS)":"Direct-Air-Capture-CCS",
      "Enhanced Weathering":"Enhanced-Weathering",
      "Forestry":"Forestry",
      "Total Sequestration":"Total-Sequestration",
    });
      //      .max_value(4000);

    this.total_co2_captured_chart = lineChart()
	  .title("Cumulative Emissions Stored")
	  .unit('Mt.CO2e');
      //      .css_for_label(css_for_labels)
      //      .max_value(4000);

    this.gas_grid_fuel_share_chart = timeSeriesStackedAreaChart()
	  .title("Gas Distribution Grid Energy Supplied")
	  .unit('TWh/yr')
    .max_value(1000)
    .css_for_label({
      "Hydrogen":"Hydrogen",
      "Biomethane":"Biomethane",
      "Natural Gas":"Natural-Gas",
      "Total":"Total"
    });
//      .min_value(-500)
      //	  .max_value(1000);
    this.hydrogen_production_by_technology_chart = timeSeriesStackedAreaChart()
	  .title("Hydrogen Supply")
	  .unit('TWh/yr')
    .max_value(.1)
    .css_for_label({
      "Biomass CCS":"Biomass-CCS",
      "Methane (SMR) CCS":"Methane-SMR-CCS",
      "Electrolysis":"Electrolysis",
      "Imports":"Imports",
      "Total":"Total",
    });
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

  this.updateResults = function(pathway, mode) {
      var _mode = (mode == 2050 || mode == 2100) ? mode : 2050;

      this.pathway = pathway;
      this.choices = twentyfifty.choices;
      updateGauge(pathway);

      classView1 = document.getElementById("view1").getAttribute('class');
      classView2 = document.getElementById('view2').getAttribute('class');
      document.getElementById('view1').setAttribute('class', 'overview visible');
      document.getElementById('view2').setAttribute('class', 'overview visible');

      this.emissions_from_conversions_and_ghg_chart.setMode(_mode);
      this.total_co2_captured_chart.setMode(_mode);
      this.gas_grid_fuel_share_chart.setMode(_mode);
      this.hydrogen_production_by_technology_chart.setMode(_mode);

      d3.select('#top_container_1')
  	  .datum(convert_capacity_table_to_hash(pathway.emissions_removal))
  	  .call(this.emissions_from_conversions_and_ghg_chart);

      d3.select('#bottom_container_1')
  	  .datum(convert_capacity_table_to_hash(pathway.emissions_stored))
  	  .call(this.total_co2_captured_chart);

      d3.select('#top_container_2')
      .datum(convert_capacity_table_to_hash(pathway.gas_grid))
      .call(this.gas_grid_fuel_share_chart);

      d3.select('#bottom_container_2')
      .datum(convert_capacity_table_to_hash(pathway.hydrogen_production))
      .call(this.hydrogen_production_by_technology_chart);

      document.getElementById('view1').setAttribute('class', classView1);
      document.getElementById('view2').setAttribute('class', classView2);

    };

    lineChart();

    return this;

}.call({});
