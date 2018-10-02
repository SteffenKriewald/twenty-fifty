window.twentyfifty.views.flows_map_imports = function() {

  this.setup = function() {
    $("#results").append('<div class="viewToggle"><div id="vT1" class="vT1 vTactive" onclick="showViewA()">Imports</div><div id="vT2" class="vT2" onclick="showViewB()">Map</div><div id="vT3" class="vT3" onclick="showViewC()">Flows</div></div>')
    $("#results").append("<div id='flows_map_imports' class='viewContainer'><div id='view1' class='overview visible'></div><div id='view2' class='overview hidden'></div><div id='view3' class='overview hidden'></div><div class='clear'></div></div>");

    containers_1 = d3.select("#view1").selectAll(".chart")
	  .data(['top_container_1', 'bottom_container_1']);

      containers_1.enter()
	  .append('div')
	  .attr('id', Object)
	  .attr('class', 'chart');

    this.security_import_energy_chart = timeSeriesStackedAreaChart()
    .title("Energy Imports")
    .unit('TWh/yr')
    .max_value(5000);

    this.security_import_fraction_chart = lineChart()
    .title("Energy Import Dependence")
    .unit('Percent')
    .max_value(1);



      containers_2 = d3.select("#fmi_map").selectAll(".chart")
	  .data(['top_container_2']);

      containers_2.enter()
	  .append('div')
	  .attr('id', Object)
	  .attr('class', 'chart');

      containers_3 = d3.select("#fmi_imports").selectAll(".chart")
	  .data(['top_container_3']);

      containers_3.enter()
	  .append('div')
	  .attr('id', Object)
	  .attr('class', 'chart');

      //      this.fmi_flow_chart = sankey_somehow();
      //      this.fmi_map_chart = map();

//      this.fmi_import_chart = lineGraphChart()
//	  .title("Share of each fuel that is imported")
//	  .unit('');
//      //      .css_for_label(css_for_labels)
//      //      .max_value(4000);

  };

  this.teardown = function() {
      $("#results").empty();
      this.security_import_energy_chart = null;
      this.security_import_fraction_chart = null;
  };

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
      classView3 = document.getElementById('view3').getAttribute('class');
      document.getElementById('view1').setAttribute('class', 'overview visible');
      document.getElementById('view2').setAttribute('class', 'overview visible');
      document.getElementById('view3').setAttribute('class', 'overview visible');

      d3.select('#top_container_1')
      .datum(convert_capacity_table_to_hash(pathway.security_import_energy))
      .call(this.security_import_energy_chart);

      d3.select('#bottom_container_1')
      .datum(convert_capacity_table_to_hash(pathway.security_import_fraction))
      .call(this.security_import_fraction_chart);

/*      d3.select('#top_container_2')
      .datum(convert_capacity_table_to_hash(pathway.tra_pass))
      .call(this.passenger_distance_travelled_by_mode_chart);

      d3.select('#bottom_container_2')
      .datum(convert_capacity_table_to_hash(pathway.tra_energy_con))
      .call(this.car_share_of_demand_by_drivetrain_chart);*/

      document.getElementById('view1').setAttribute('class', classView1);
      document.getElementById('view2').setAttribute('class', classView2);
      document.getElementById('view3').setAttribute('class', classView3);

    };

    lineChart();

    return this;

  }.call({});
