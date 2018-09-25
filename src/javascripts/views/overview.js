window.twentyfifty.views.overview = function() {

  this.setup = function() {
      $("#results").append("<div id='overview'><div id='overview_emissions' class='overview'></div><div id='overview_energy' class='overview'></div><div class='clear'></div></div>");

      containers_1 = d3.select("#overview_emissions").selectAll(".chart")
	  .data(['top_container_1', 'bottom_container_1']);

      containers_2 = d3.select("#overview_energy").selectAll(".chart")
	  .data(['top_container_2', 'bottom_container_2']);

      containers_1.enter()
	  .append('div')
	  .attr('id', Object)
	  .attr('class', 'chart');

      containers_2.enter()
	  .append('div')
	  .attr('id', Object)
	  .attr('class', 'chart');
      
      
      this.emissions_by_sector_chart = timeSeriesStackedAreaChart()
	  .title("Emissions by Sector")
	  .unit('Mt.CO2e')
      //      .css_for_label(css_for_labels)
          .max_value(200);

//      this.cumulative_emissions_chart = lineGraphChart()
//	  .title("Cumulative Emissions")
//	  .unit('');
//      //      .css_for_label(css_for_labels)
//      //      .max_value(4000);
//      
      this.primary_energy_supply_chart = timeSeriesStackedAreaChart()
	  .title("Primary Energy Supply")
	  .unit('')
//      .css_for_label(css_for_labels)
//      .min_value(-500)
      	  .max_value(200);
      this.final_energy_demand_chart = timeSeriesStackedAreaChart()
	  .title("Final Energy Demand")
	  .unit('');
//      .css_for_label(css_for_labels)
//      .min_value(-500)
//	  .max_value(1000);
  };

  this.teardown = function() {
      $("#results").empty();
      this.emissions_by_sector_chart = null;
      //      cumulative_emissions_chart
      this.primary_energy_supply_chart = null;
      this.final_energy_demand_chart = null;
  };

    // This is used to convert the table from how it looks in Excel
    // into the format needed to plot a chart
    convert_buggy_emission_table_to_hash = function(table) {
	hash = d3.map();
    
    // Do not Skip any header row, and put the table into
    // a Hash table with the key being the first column and the
    // value being the rest

	console.log(table);
	//((table.slice(79,87)).concat(table.slice(124,128))).forEach(function(row) {
	((table.slice(0,12))).forEach(function(row) {
	    hash.set(row[0], row.slice(1));
	});
	return hash;
    }

    // This is used to convert the table from how it looks in Excel
    // into the format needed to plot a chart
    convert_capacity_table_to_hash = function(table) {
	hash = d3.map();
    
    // Do not Skip any header row, and put the table into
    // a Hash table with the key being the first column and the
    // value being the rest
	table.slice(0).forEach(function(row) {
	    hash.set(row[0], row.slice(1));
	});
	return hash;
    }

    
  this.updateResults = function(pathway) {
      this.pathway = pathway;
      console.log("updateResults:");
      console.log(pathway);
      this.choices = twentyfifty.choices;
      // construct the data      
      // connect the containers with charts and data
      d3.select('#top_container_1')
	  .datum(convert_buggy_emission_table_to_hash(pathway.ghg))
	  .call(this.emissions_by_sector_chart);

      d3.select('#bottom_container_1')
	  .datum(convert_capacity_table_to_hash(pathway.electricity.capacity))
	  .call(this.primary_energy_supply_chart);


  };

  return this;

}.call({});
