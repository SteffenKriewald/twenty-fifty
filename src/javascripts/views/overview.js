window.twentyfifty.views.overview = function() {

  this.setup = function() {
    $("#results").append('<div class="viewToggle"><div id="vT1" class="vT1 vTactive" onclick="showView1()">Emissions / PEC</div><div id="vT2" class="vT2" onclick="showView2()">Cumulative / FEC</div></div>')
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
    .max_value(1000)
    .min_value(-100)
    .css_for_label({
      "Dedicated GHG Removal": "Dedicated-GHG-Removal",
      "Electricity Generation": "Electricity-Generation",
      "Hydrogen Production": "Hydrogen-Production",
      "Other Energy Supply": "Other-Energy-Supply",
      "Waste Management": "Waste-Management",
      "Agriculture & Land Use": "Agriculture-Land-Use",
      "Industry": "Industry",
      "Buildings-Residential": "Buildings-Residential",
      "Buildings-Non-Residential": "Buildings-Non-Residential",
      "Transport-Domestic": "Transport-Domestic",
      "Transport-International": "Transport-International",
      "Total Emissions": "Total-Emissions",
      "Carbon Budgets": "Carbon-Budgets"
    });

    this.emissions_cumulative_chart = lineChart()
    .title("Cumulative UK Greenhouse Gas Emissions")
    .unit('Mt.CO2e')
    //      .css_for_label(css_for_labels)
    .max_value(50000);


    this.energy_consumption_chart = timeSeriesStackedAreaChart()
    .title("Primary Energy Consumption")
    .unit('TWh/yr')
    //      .css_for_label(css_for_labels)
    .max_value(5000)
    .css_for_label({
      "Coal":"Coal",
      "Oil":"Oil",
      "Natural gas":"Natural-gas",
      "Waste":"Waste",
      "Bioenergy":"Bioenergy",
      "Hydrogen imports":"Hydrogen-imports",
      "Electricity imports":"Electricity-imports",
      "Nuclear":"Nuclear",
      "Environmental heat":"Environmental-heat",
      "Wind":"Wind",
      "Solar":"Solar",
      "Tidal, Wave, Hydro":"Tidal-Wave-Hydro",
      "Total":"Total"
    });

    this.final_energy_consumption_chart = timeSeriesStackedAreaChart()
    .title("Final Energy Consumption")
    .unit('TWh/yr')
    //      .css_for_label(css_for_labels)
    .max_value(5000)
    .css_for_label({
      "Dedicated GHG Removal":"Dedicated-GHG-Removal",
      "Agriculture":"Agriculture",
      "Industry":"Industry",
      "Buildings-Residential":"Buildings-Residential",
      "Buildings-Non-Residential":"Buildings-Non-Residential",
      "Transport-Domestic":"Transport-Domestic",
      "Transport-International":"Transport-International",
       "Total":"Total"
});

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

    this.updateResults = function(pathway, mode) {
        var _mode = (mode == 2050 || mode == 2100) ? mode : 2050;
        this.pathway = pathway;
        this.choices = twentyfifty.choices;
        updateGauge(pathway);
        // construct the data
        // connect the containers with charts and data
        //console.log('this.emissions_by_sector_chart ', this.emissions_by_sector_chart, pathway.ghg);

        classView1 = document.getElementById("view1").getAttribute('class');
        classView2 = document.getElementById('view2').getAttribute('class');
        document.getElementById('view1').setAttribute('class', 'overview visible');
        document.getElementById('view2').setAttribute('class', 'overview visible');

        this.emissions_by_sector_chart.setMode(_mode);
        this.emissions_cumulative_chart.setMode(_mode);
        this.energy_consumption_chart.setMode(_mode);
        this.final_energy_consumption_chart.setMode(_mode);


        //temporary fix, because Carbon Budgets are still malformatted
        for(var i=pathway.emissions_sector.length-1; i>=0; i--) {
          if(pathway.emissions_sector[i][0]==="Carbon Budgets") {
            pathway.emissions_sector.splice(i,1);
            break;
          }
        }

        d3.select('#top_container_1')
    	  .datum(convert_capacity_table_to_hash(pathway.emissions_sector))
    	  .call(this.emissions_by_sector_chart);

        d3.select('#top_container_2')
    	  .datum(convert_capacity_table_to_hash(pathway.emissions_cumulative))
    	  .call(this.emissions_cumulative_chart);

        d3.select('#bottom_container_1')
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
