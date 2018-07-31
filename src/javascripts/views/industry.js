window.twentyfifty.views.industry = function() {

  this.setup = function() {
      $("#results").append("<div id='industry'><div id='industry_energy_emissions' class='industry'></div><div class='clear'></div></div>");

      containers_1 = d3.select("#industry_energy_emissions").selectAll(".chart")
	  .data(['top_container_1', 'bottom_container_1']);

      containers_1.enter()
	  .append('div')
	  .attr('id', Object)
	  .attr('class', 'chart');
     
      this.emissions_from_industry_chart = timeSeriesStackedAreaChart()
	  .title("Emissions from Industry")
	  .unit('');
      //      .css_for_label(css_for_labels)
      //      .max_value(4000);

      this.energy_consumption_for_industry_chart = timeSeriesStackedAreaChart()
	  .title("Energy Consumption")
	  .unit('');
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

  };


  return this;

}.call({});
