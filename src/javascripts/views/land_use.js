window.twentyfifty.views.land_use = function() {

  this.setup = function() {
      $("#results").append("<div id='land_use'><div id='land_use_emissions' class='land_use'></div><div id='land_use_bioenergy' class='land_use'></div><div class='clear'></div></div>");

      containers_1 = d3.select("#land_use_emissions").selectAll(".chart")
	  .data(['top_container_1', 'bottom_container_1']);

      containers_2 = d3.select("#land_use_bioenergy").selectAll(".chart")
	  .data(['top_container_2', 'bottom_container_2']);

      containers_1.enter()
	  .append('div')
	  .attr('id', Object)
	  .attr('class', 'chart');

      containers_2.enter()
	  .append('div')
	  .attr('id', Object)
	  .attr('class', 'chart');
      
      this.emissions_from_land_use_chart = timeSeriesStackedAreaChart()
	  .title("Emissions from Land Use & Biofuels")
	  .unit('');
      //      .css_for_label(css_for_labels)
      //      .max_value(4000);

      this.land_balance_chart = timeSeriesStackedAreaChart()
	  .title("Land Balance, forestry vs. Bioenergy")
	  .unit('');
      //      .css_for_label(css_for_labels)
      //      .max_value(4000);
      
      this.domestic_bioenergy_supply_chart = timeSeriesStackedAreaChart()
	  .title("domestic Bioenergy Resource Production")
	  .unit('');
//      .css_for_label(css_for_labels)
//      .min_value(-500)
      //	  .max_value(1000);
//      this.imported_bioenergy_chart = lineGraphChart()
//	  .title("imported Bioenergy")
//	  .unit('');
////      .css_for_label(css_for_labels)
////      .min_value(-500)
////	  .max_value(1000);
      
  };

  this.teardown = function() {
      $("#results").empty();
      this.emissions_from_land_use_chart = null;
      this.land_balance_chart = null;
      this.domestic_bioenergy_supply_chart = null;
//      this.imported_bioenergy_chart = null;
  };

  this.updateResults = function(pathway) {
    this.pathway = pathway;
    this.choices = twentyfifty.choices;

  };


  return this;

}.call({});
