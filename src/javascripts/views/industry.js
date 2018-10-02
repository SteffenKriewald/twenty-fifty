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
    .max_value(80);
      //      .css_for_label(css_for_labels)
      //      .max_value(4000);

      this.energy_consumption_for_industry_chart = timeSeriesStackedAreaChart()
	  .title("Energy Consumption - Industry")
	  .unit('TWh/yr')
    .max_value(400);
      //      .css_for_label(css_for_labels)
      //      .max_value(4000);

  };

  this.teardown = function() {
      $("#results").empty();
      this.emissions_from_industry_chart = null;
      this.energy_consumption_for_industry_chart = null;
  };

  this.updateResults = function(pathway) {
    this.pathway = pathway;
    this.choices = twentyfifty.choices;
    updateGauge(pathway);

    d3.select('#top_container_1')
    .datum(convert_capacity_table_to_hash(pathway.ind_emissions))
    .call(this.emissions_from_industry_chart);

    d3.select('#bottom_container_1')
    .datum(convert_capacity_table_to_hash(pathway.ind_energy_consumption))
    .call(this.energy_consumption_for_industry_chart);


  };


  return this;

}.call({});
