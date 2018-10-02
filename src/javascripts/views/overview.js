window.twentyfifty.views.overview = function() {

  this.setup = function() {
    $("#results").append('<div class="viewToggle"><div id="vT1" class="vT1 vTactive" onclick="showView1()">Emissions</div><div id="vT2" class="vT2" onclick="showView2()">Energy</div></div>')
    $("#results").append("<div id='overview' class='viewContainer'><div id='view1' class='overview visible'></div><div id='view2' class='overview hidden'></div><div class='clear'></div></div>");


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

    this.emissions_by_sector_chart = timeSeriesStackedAreaChart()
    .title("Greenhouse Gas Emissions")
    .unit('Mt.CO2e/yr')
    //      .css_for_label(css_for_labels)
    .max_value(1000);

    this.emissions_cumulative_chart = lineChart()
    .title("Cumulative UK Greenhouse Gas Emissions")
    .unit('Mt.CO2e')
    //      .css_for_label(css_for_labels)
    .max_value(50000);


    this.energy_consumption_chart = timeSeriesStackedAreaChart()
    .title("Primary Energy Consumption")
    .unit('TWh/yr')
    //      .css_for_label(css_for_labels)
    .max_value(5000);

    this.final_energy_consumption_chart = timeSeriesStackedAreaChart()
    .title("Final Energy Consumption")
    .unit('TWh/yr')
    //      .css_for_label(css_for_labels)
    .max_value(5000);

// Styling of line charts
/*
    this.emissions_by_sector_chart = lineChart()
	  .title("Greenhouse Gas Emissions")
	  .unit('Mt.CO2e/yr')
    //      .css_for_label(css_for_labels)
    .max_value(100)
    .setCustomStyles({
      //'0' is the key of the first dataset
      '0': {
        color: '#000',
        dashPattern: '5 1 1 1'
      },
      '1': {
        color: 'red'
      }
    });
*/


  };

  this.teardown = function() {
      $("#results").empty();
      this.emissions_by_sector_chart = null;
      this.emissions_cumulative_chart = null;
      this.energy_consumption_chart = null;
      this.final_energy_consumption_chart = null;
  };


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
    	  .datum(convert_capacity_table_to_hash(pathway.emissions_sector))
    	  .call(this.emissions_by_sector_chart);

        d3.select('#bottom_container_1')
    	  .datum(convert_capacity_table_to_hash(pathway.emissions_cumulative))
    	  .call(this.emissions_cumulative_chart);

        d3.select('#top_container_2')
        .datum(convert_capacity_table_to_hash(pathway.energy_consumption))
        .call(this.energy_consumption_chart);

        d3.select('#bottom_container_2')
        .datum(convert_capacity_table_to_hash(pathway.final_energy_consumption))
        .call(this.final_energy_consumption_chart);

        document.getElementById('view1').setAttribute('class', classView1);
        document.getElementById('view2').setAttribute('class', classView2);

      };

      lineChart();

      return this;

    }.call({});
