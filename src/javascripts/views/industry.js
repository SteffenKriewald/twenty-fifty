window.twentyfifty.views.industry = function() {

  this.setup = function() {
  $("#results").append('<div class="viewToggle"><div id="vT1" class="vT1_nohover vTactive">Energy & Emissions</div></div>')
  $("#results").append("<div id='industry' class='viewContainer'><div id='view1' class='overview visible'></div></div>");

    containers_1 = d3.select("#view1").selectAll(".chart")
    .data(['top_container_1', 'bottom_container_1']);

      containers_1.enter()
	  .append('div')
	  .attr('id', Object)
	  .attr('class', 'chart');

      this.emissions_from_industry_chart = timeSeriesStackedAreaChart()
	  .title("Emissions - Industry")
	  .unit('Mt.CO2e/yr')
    .max_value(80)
    .css_for_label({
      "Ferrous & non-ferrous metals":"Ferrous-non-ferrous-metals",
      "Cement, Ceramics, Glass":"Cement-Ceramics-Glass",
      "Chemicals":"Chemicals",
      "Other Industry":"Other-Industry",
      "Indirect emissions":"Indirect-emissions",
      "Total Direct":"Total-Direct"
    });
      //      .max_value(4000);

      this.energy_consumption_for_industry_chart = timeSeriesStackedAreaChart()
	  .title("Energy Consumption - Industry")
	  .unit('TWh/yr')
    .max_value(400)
    .css_for_label({
      "Electricity":"Electricity",
      "Hydrogen":"Hydrogen",
      "Bioenergy":"Bioenergy",
      "Natural gas":"Natural-gas-i",
      "Oil":"Oil",
      "Coal":"Coal",
      "Total":"Total"
    });
      //      .max_value(4000);

  };

  this.teardown = function() {
      $("#results").empty();
      this.emissions_from_industry_chart = null;
      this.energy_consumption_for_industry_chart = null;
  };

  this.updateResults = function(pathway, mode) {
    var _mode = (mode == 2050 || mode == 2100) ? mode : 2050;

    this.pathway = pathway;
    this.choices = twentyfifty.choices;
    updateGauge(pathway);

    this.emissions_from_industry_chart.setMode(_mode);
    this.energy_consumption_for_industry_chart.setMode(_mode);

    d3.select('#top_container_1')
    .datum(convert_capacity_table_to_hash(pathway.ind_emissions))
    .call(this.emissions_from_industry_chart);

    d3.select('#bottom_container_1')
    .datum(convert_capacity_table_to_hash(pathway.ind_energy_consumption))
    .call(this.energy_consumption_for_industry_chart);


  };


  return this;

}.call({});
