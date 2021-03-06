window.twentyfifty.views.land_use = function() {

  this.setup = function() {
  $("#results").append('<div class="viewToggle"><div id="vT1" class="vT1 vTactive" onclick="showView1()">Emissions & Land</div><div id="vT2" class="vT2" onclick="showView2()">Bioenergy</div></div>')
  $("#results").append("<div id='land_use' class='viewContainer'><div id='view1' class='overview visible'></div><div id='view2' class='overview hidden'></div><div class='clear'></div></div>");

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

      this.emissions_from_land_chart = timeSeriesStackedAreaChart()
	  .title("Emissions - Land Use and Biofuels")
	  .unit('Mt.CO2e/yr')
    .max_value(100)
    .css_for_label({
      "Forestry":"Forestry",
      "Land":"Land",
      "Agricultural practices":"Agricultural-practices",
      "Bio-Conversion":"Bio-Conversion",
      "Waste Management":"Waste-Management",
      "Indirect emissions":"Indirect-emissions",
      "Total Direct":"Total-Direct"
    });

      this.land_trade_off_chart = timeSeriesStackedAreaChart()
	  .title("Land Area Trade Off")
	  .unit('km²')
    .max_value(50000)
    .css_for_label({
      "Forest":"Forest",
      "Bioenergy Land":"Bioenergy-Land",
      "Max Available Land":"Max-Available-Land",
      "Total Potential Land":"Total-Potential-Land"
    });

      this.land_bioenergy_chart = timeSeriesStackedAreaChart()
	  .title("Domestic Bioenergy Production")
	  .unit('TWh/yr')
    .max_value(100)
    .css_for_label({
      "Solid Biomass":"Solid-Biomass",
      "Dry Waste":"Dry-Waste",
      "Wet Waste":"Wet-Waste",
      "Total":"Total"
    });

    this.bioenergy_imports_chart = lineChart()
  .title("Imported Bioenergy")
  .unit('TWh/yr')
  .setCustomStyles({
    '0': {color: "black"},
    '1': {color: "#7f7fff", dashPattern: "4 4"},
    '2': {color: "#0000b2", dashPattern: "4 4"},
    '3': {color: "#020342", dashPattern: "4 4"}
  })
  .min_value(-100)
  .max_value(100);

  };

  this.teardown = function() {
      $("#results").empty();
      this.emissions_from_land_chart = null;
      this.land_trade_off_chart = null;
      this.land_bioenergy_chart = null;
      this.bioenergy_imports_chart = null;
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

      this.emissions_from_land_chart.setMode(_mode);
      this.land_trade_off_chart.setMode(_mode);
      this.land_bioenergy_chart.setMode(_mode);
      this.bioenergy_imports_chart.setMode(_mode);

      d3.select('#top_container_1')
      .datum(convert_capacity_table_to_hash(pathway.land_emissions))
      .call(this.emissions_from_land_chart);

      d3.select('#bottom_container_1')
      .datum(convert_capacity_table_to_hash(pathway.land_trade_off))
      .call(this.land_trade_off_chart);

      d3.select('#top_container_2')
      .datum(convert_capacity_table_to_hash(pathway.land_bioenergy))
      .call(this.land_bioenergy_chart);

      d3.select('#bottom_container_2')
      .datum(convert_capacity_table_to_hash(pathway.bioenergy_imports))
      .call(this.bioenergy_imports_chart);

      document.getElementById('view1').setAttribute('class', classView1);
      document.getElementById('view2').setAttribute('class', classView2);

    };

    lineChart();

    return this;

  }.call({});
