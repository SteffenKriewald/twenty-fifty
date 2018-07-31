window.twentyfifty.views.flows_map_imports = function() {

  this.setup = function() {
      $("#results").append("<div id='fmi'><div id='fmi_flows' class='flows_map_imports'></div><div id='fmi_map' class='flows_map_imports'></div><div id='fmi_imports' class='flows_map_imports'></div><div class='clear'></div></div>");

      containers_1 = d3.select("#fmi_flows").selectAll(".chart")
	  .data(['top_container_1']);

      containers_1.enter()
	  .append('div')
	  .attr('id', Object)
	  .attr('class', 'chart');

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

  };

  this.updateResults = function(pathway) {
    this.pathway = pathway;
    this.choices = twentyfifty.choices;

  };


  return this;

}.call({});
