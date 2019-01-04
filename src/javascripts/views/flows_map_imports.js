window.twentyfifty.views.flows_map_imports = function() {

  // sankey
  name_conversions = {
    "UK land based bioenergy": "Forests & biocrops",
    "Bio-conversion": "Biomass processing",
    "H2 conversion": "Hydrogen production",
    "H2": "Hydrogen",
    "Useful district heat": "Delivered heat",
    "Heating and cooling - homes": "Home heating & cooling",
    "Heating and cooling - commercial": "Office heating & cooling",
    "Lighting & appliances - homes": "Home lighting & appliances",
    "Lighting & appliances - commercial": "Office lighting & appliances"
  };

  convert_name = function(name) {
    return name_conversions[name] || name;
  };
  // sanky end

  // MAP

  // Define some new Raphael primitives for different types of labelled square
  Raphael.fn.upiabeled_square = function(x, y, label, area, colour) {
    var side, sq;
    side = Math.sqrt(area);
    sq = this.rect(x, y - side, side, side).attr({
      stroke: colour,
      fill: colour,
      'fill-opacity': "0.5"
    });
    label = this.text(x - 2, y - (side / 2), label).attr({
      'text-anchor': 'end',
      fill: 'black'
    });
    label.hide();
    return {
      square: sq,
      label: label
    };
  };

  Raphael.fn.up_rightiabeled_square = function(x, y, label, area, colour) {
    var side, sq;
    side = Math.sqrt(area);
    sq = this.rect(x - (side / 2), y - side, side, side).attr({
      stroke: colour,
      fill: colour,
      'fill-opacity': "0.5"
    });
    label = this.text(x + (side / 2) + 4, y - (side / 2), label).attr({
      'text-anchor': 'start',
      fill: 'black'
    });
    return {
      square: sq,
      label: label
    };
  };

  Raphael.fn.downiabeled_square = function(x, y, label, area, colour) {
    var side, sq;
    side = Math.sqrt(area);
    sq = this.rect(x - side, y, side, side).attr({
      stroke: colour,
      fill: colour,
      'fill-opacity': "0.5"
    });
    label = this.text(x + 4, y + (side / 2), label).attr({
      'text-anchor': 'start',
      fill: 'black'
    });
    return {
      square: sq,
      label: label
    };
  };

  // All the settings
  displayin_element = 'map';
  display_width = 1200;
  display_height = 800;
  mapimage_url = '/assets/images/uk.png';
  map_width = 492;
  map_height = 725;
  map_offset_x = 150;
  map_offset_y = 0;
  km = 0.69;
  m = km / 1000.0;
  m2 = m * m;
  ha = 10000 * m2;
  km2 = 1e6 * m2;
  Mha = 1e6 * ha;
  GW = 2;
  MW = GW / 1000;


/*colours = {
    'III.a.2': '#ff0000',
    'III.a.1': '#ff0000',
    'IV.c': '#aa0000',
    'VI.a.Biocrop': '#00ff00',
    'VI.a.Forestry': '#408000',
    'VI.c': '#00ff00',
    'V.b': '#00ff00',
    'IV.a': '#ffff00',
    'IV.b': '#cccc00',
    'VII.a': '#ffff00',
    'III.b': '#0000ff',
    'III.c.TidalRange': '#0000ff',
    'III.c.TidalStream': '#aaaaff',
    'I.a': '#666',
    'I.b': '#CCC',
    'II.a': '#FF0',
    'III.d': '#F00',
    'VII.c': '#008000',
    'VI.b': '#F00'
  };*/

  colours = {
    'Onshore Wind': '#ff0000',
    'Hydroelectric': '#ff0000',
    'Solar PV': '#aa0000',
    'Bioenergy Growth': '#00ff00',
    'Forest/Woodland': '#408000',
    'Offshore Wind': '#00ff00',
    'Tidal Stream': '#00ff00',
    'Tidal Range': '#ffff00',
    'Bioenergy Growth Overseas': '#cccc00'
  };

/*labels = {
    'III.a.2': 'Offshore wind',
    'III.a.1': 'Onshore wind',
    'IV.c': 'Micro wind',
    'VI.a.Biocrop': 'Energy crops',
    'VI.a.Forestry': 'Forest',
    'VI.c': 'Marine algae',
    'V.b': 'Biocrops',
    'IV.a': 'Solar PV',
    'IV.b': 'Solar thermal',
    'VII.a': 'Solar PV',
    'III.b': 'Hydro',
    'III.c.TidalRange': 'Tidal range',
    'III.c.TidalStream': 'Tidal stream',
    'I.a': '2 GW coal gas or biomass power stations without CCS',
    'I.b': '1.2 GW coal gas or biomass power stations with CCS',
    'II.a': '3 GW nuclear power station',
    'III.d': '0.01 GW geothermal stations',
    'VII.c': '1 GW gas standby power stations',
    'VI.b': '215 kt/y waste to energy conversion facilities'
  };*/

  labels = {
    'Onshore Wind': 'Onshore Wind',
    'Hydroelectric': 'Hydroelectric',
    'Solar PV': 'Solar PV',
    'Bioenergy Growth': 'Bioenergy Growth',
    'Forest/Woodland': 'Forest/Woodland',
    'Offshore Wind': 'Offshore Wind',
    'Tidal Stream': 'Tidal Stream',
    'Tidal Range': 'Tidal Range',
    'Bioenergy Growth Overseas': 'Bioenergy Growth Overseas'
  };

  pointSizes = {
    'Onshore Wind': 2,
    'Hydroelectric': 1.2,
    'Solar PV': 3,
    'Bioenergy Growth': 0.01,
    'Forest/Woodland': 1,
    'Offshore Wind': 0.01,
    'Tidal Stream': 1,
    'Tidal Range': 1,
    'Bioenergy Growth Overseas': 1
  };

  // MAP ENDS

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
    .max_value(5000)
    .css_for_label({
      "Coal":"Coal",
      "Oil":"Oil",
      "Gas":"Gas-f",
      "Bioenergy":"Bioenergy",
      "Electricity":"Electricity",
      "Hydrogen":"Hydrogen",
      "Nuclear (Uranium)":"Nuclear",
      "Total":"Total"
    });

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

// SANKEY

      $('#view3').append("<div id='sankey'></div>");
      this.s = s = new Sankey();
      s.stack(0,["Environmental Energy","Waste","UK Biomass","Fossil Fuel Reserves","Imports"]);
      s.stack(1,["Environmental Heat","Solar" ,"Wind","Tidal","Wave","Hydro","Electricity imports","Nuclear","Waste-EfW","Hydrogen Imports","Bio-Conversion","Coal","Oil","Hydrogen Production" ],"Imports");
      s.stack(2,["Natural Gas"],"Imports");
      s.stack(3,["CHP-Heat Network"],"Imports");
      s.stack(4,["Heat Network"],"Imports");
      s.stack(5,["Thermal Generation"],"Imports");
      s.stack(6,["Electricity Transmission"],"Imports");
      s.stack(7,["Electricity Distribution"],"Imports");
      s.stack(8,["Exports","Heat-Residential","Heat-Non-Residential","Appliances-Residential","Appliances-Non-Residential","Industry","Agriculture","Road Transport","Rail and Shipping","Aviation","Fossil Fuel Processing","Losses","GHG Removal"]);
      //s.stack(1,[]);
/*      s.stack(0, ["Pumped heat", "Solar", "Wind", "Tidal", "Wave", "Geothermal", "Hydro", "Electricity imports", "Nuclear", "Coal reserves", "Coal imports", "Biomass imports", "Gas reserves", "Gas imports", "Oil reserves", "Oil imports", "Biofuel imports", "UK land based bioenergy", "Agricultural 'waste'", "Other waste", "Marine algae"]);
      s.stack(1, ["Coal"], "Coal reserves");
      s.stack(1, ["Natural Gas"], "Gas reserves");
      s.stack(1, ["Oil"], "Oil reserves");
      s.stack(1, ["Bio-conversion"], "UK land based bioenergy");
      s.stack(2, ["Solar Thermal", "Solar PV"], "Solar");
      s.stack(2, ["Solid", "Gas", "Liquid"], "Coal");
      s.stack(3, ["Thermal generation", "CHP"], "Nuclear");
      s.stack(4, ["Electricity grid", "District heating"], "Wind");
      s.stack(5, ["H2 conversion"], "Electricity grid");
      s.stack(6, ["H2"], "H2 conversion");
      s.stack(7, ["Heating and cooling - homes", "Heating and cooling - commercial", "Lighting & appliances - homes", "Lighting & appliances - commercial", "Industry", "Road transport", "Rail transport", "Domestic aviation", "International aviation", "National navigation", "International shipping", "Agriculture", "Geosequestration", "Over generation / exports", "Losses"]);
*/
      s.nudge_boxes_callback = function() {
      //  this.boxes["Losses"].y = this.boxes["Marine algae"].b() - this.boxes["Losses"].size();
      };

      s.setColors({
        "Coal reserves": "#8F6F38",
        "Coal": "#8F6F38",
        "Coal imports": "#8F6F38",
        "Oil reserves": "#A99268",
        "Oil": "#A99268",
        "Oil imports": "#A99268",
        "Gas reserves": "#DDD4C4",
        "Natural Gas": "#DDD4C4",
        "Gas imports": "#DDD4C4",
        "Solar": "#F6FF00",
        "Solar Thermal": "#F6FF00",
        "Solar PV": "#F6FF00",
        "UK land based bioenergy": "#30FF00",
        "Bio-conversion": "#30FF00",
        "Marine algae": "#30FF00",
        "Agricultural 'waste'": "#30FF00",
        "Other waste": "#30FF00",
        "Biomass imports": "#30FF00",
        "Biofuel imports": "#30FF00",
        "Solid": "#557731",
        "Liquid": "#7D9763",
        "Gas": "#BCC2AD",
        "Electricity grid": "#0000FF",
        "Thermal generation": "#0000FF",
        "CHP": "#FF0000",
        "Nuclear": "#E2ABDB",
        "District heating": "#FF0000",
        "Pumped heat": "#FF0000",
        "Useful district heat": "#FF0000",
        "CHP Heat": "#FF0000",
        "Electricity imports": "#0000FF",
        "Wind": "#C7E7E6",
        "Tidal": "#C7E7E6",
        "Wave": "#C7E7E6",
        "Geothermal": "#C7E7E6",
        "Hydro": "#C7E7E6",
        "H2 conversion": "#FF6FCF",
        "Final electricity": "#0000FF",
        "Over generation / exports": "#0000FF",
        "H2": "#FF6FCF",
        "!!! HERE WE GO AGAIN !!!": "#ffa500",
        "Environmental Energy": "#009900",
        "Waste": "#521515",
        "UK Biomass": "#731d1d",
        "Fossil Fuel Reserves": "#808080",
        "Imports": "#e5e500",
        "Environmental Heat": "#990000",
        "Solar": "#009900",
        "Wind": "#00e500",
        "Tidal": "#004c00",
        "Wave": "#004c00",
        "Hydro": "#004c00",
        "Electricity Imports": "#e5e500",
        "Nuclear": "#ffa500",
        "Waste-EfW": "#521515",
        "Hydrogen Imports": "#730073",
        "Bio-Conversion": "#731d1d",
        "Natural Gas": "#808080",
        "Coal": "#4c4c4c",
        "Oil": "#666666",
        "Thermal Generation": "#e5e500",
        "Electricity Transmission": "#e5e500",
        "Hydrogen Production": "#730073",
        "CHP-Heat Network": "#e5e500",
        "Electricity Distribution": "#e5e500",
        "Heat Network": "#990000",
        "last": "#ffa500"
      });

      s.nudge_colours_callback = function() {
        this.recolour(this.boxes["Environmental Heat"].left_lines, "#990000");
        this.recolour(this.boxes["Nuclear"].left_lines, "#ffa500");
        this.recolour(this.boxes["Hydrogen Imports"].left_lines, "#730073");
        this.recolour(this.boxes["Bio-Conversion"].left_lines, "#731d1d");
        this.recolour(this.boxes["Natural Gas"].left_lines, "#808080");
        this.recolour(this.boxes["Coal"].left_lines, "#4c4c4c");
        this.recolour(this.boxes["Oil"].left_lines, "#666666");
        this.recolour(this.boxes["Losses"].left_lines, "#7f7fff");
        this.recolour(this.boxes["Heat Network"].left_lines, "#990000");
      };

      pixels_per_TWh = $('#sankey').height() / 10000;

      s.y_space = Math.round(100 * pixels_per_TWh);
      s.right_margin = 250;
      s.left_margin = 150;

      s.convert_flow_values_callback = function(flow) {
        return flow * pixels_per_TWh;
      };

      s.convert_flow_labels_callback = function(flow) {
        return Math.round(flow);
      };

      s.convert_box_value_labels_callback = function(flow) {
        return "" + Math.round(flow) + " TWh/y";
      };
      // SANKEY ENDS

      // Map
      $('#view2').append("<div id='map-container'>"
                            +"<div id='map'></div>"
                            +"<div id='power-stations'><div id='power-stations-title'>Illustration of the number of thermal power stations in 2050</div></div>"
                          +"</div>");
      this.r = r = Raphael(displayin_element, display_width, display_height);

      // The map itself is a bitmap image in public/assets/images
      r.image(mapimage_url, map_offset_x, map_offset_y, map_width, map_height);

      // Title
      r.text(20, 10, "Illustration of scale of land and sea use in 2050 (positions are arbitrary)").attr({
        'font-weight': 'bold',
        'text-anchor': 'start'
      });

      // The wave line
      this.wave = {
        line: r.path([]).attr({ stroke: 'blue', 'stroke-width': 2 }),
        label: r.text(98, 34, "Wave").attr({ 'text-anchor': 'end' })
      };
      this.wave.label.hide();

      // Now draw the boxes, starting at the map bottom-center
      x = (map_width / 2) + map_offset_x;
      y = map_height + map_offset_y - 100;
      this.land_boxes = {};
      // land_box_names = ['III.a.1', 'III.b', 'IV.a', 'IV.b', 'IV.c', 'VI.a.Biocrop', 'VI.a.Forestry'];
      //land_box_names = ['III.a.1', 'III.b', 'IV.a', 'IV.b', 'IV.c', 'VI.a.Biocrop', 'Test'];
      land_box_names = ['Bioenergy Growth', 'Bioenergy Growth Overseas', 'Forest/Woodland', 'Hydroelectric', 'Onshore Wind', 'Solar PV'];


      for (i = 0, len = land_box_names.length; i < len; i++) {
        name = land_box_names[i];
        console.log('xy '+x+' '+y+': '+ labels[name] + colours[name]);
        this.land_boxes[name] = r.upiabeled_square(x, y, labels[name], 0, colours[name]);
      }

      // Now draw the sea boxes, starting at the top center
      x = (map_width / 2) + map_offset_x + 250;
      y = 30;
      this.sea_boxes = {};
      //sea_box_names = ['III.a.2', 'III.c.TidalStream', 'III.c.TidalRange', 'VI.c'];
      sea_box_names = ['Offshore Wind', 'Tidal Stream', 'Tidal Range']

      for (i = 0, len = sea_box_names.length; i < len; i++) {
        name = sea_box_names[i];
        this.sea_boxes[name] = r.downiabeled_square(x, y, labels[name], 0, colours[name]);
      }

      // Draw the half circle with label to indicate land area overseas
      r.path("M244,695 q0,-200 -200,-200").attr({ 'stroke': '#ccc' });
      //r.text(44, 510, "Imports").attr({ 'fill': '#ccc', 'font-weight': 'bold', 'text-anchor': 'start' });
      r.text(44, 260, "Imports").attr({ 'fill': '#ccc', 'font-weight': 'bold', 'text-anchor': 'start' }); //cheap 'solution'


      // Now draw the land area overseas boxes, starting map bottom-left
      x = map_offset_x - 105;
      y = map_height + map_offset_y - 30;
      this.overseasiand_boxes = {};
      //overseasiand_box_names = ['V.b', 'VII.a'];
      overseasiand_box_names = ['Bioenergy Growth Overseas'];
      for (i = 0, len = overseasiand_box_names.length; i < len; i++) {
        name = overseasiand_box_names[i];
        this.overseasiand_boxes[name] = r.upiabeled_square(x, y, labels[name], 0, colours[name]);
      }

      // Now deal with the objects that aren't on the map. In this case a stack of circles
      //this.points = r.set();
      //r.text(700, 10, "Illustration of the number of thermal power stations in 2050 (scales and positions are arbitrary)").attr({ 'font-weight': 'bold', 'text-anchor': 'start' });
    //MAP ENDS



  };
  // MAP
  // This is a helper method for drawing the blocks of circles representating power stations of a particular type
  this.point_stack = function(x, y, number, colour, label, size) {
    var i, x_count, width, x_step, y_step;

    x_count = 0;
    if (size < 10) {
      x_step = 5;
      y_step = 5;
    } else {
      x_step = 1.5 * size;
      y_step = 1.5 * size;
    }
    width = 100;
    if (number > 0) {
      this.r.text(x - 20, y, label).attr({ 'text-anchor': 'end' });
    }
    for (i = 1; 1 <= number ? i <= number : i >= number; 1 <= number ? i++ : i--) {
      this.r.circle(x + (x_count * x_step), y, size * km).attr({ 'stroke-width': 0, 'fill': colour });
      this.r.circle(x + (x_count * x_step), y, 1).attr({ 'stroke-width': 0, 'fill': 'black' });
      x_count = x_count + 1;
      if ((x_count * x_step) > width) {
        x_count = 0;
        y = y + y_step;
      }
    }
    return y = y + 30;
  }
  // MAP ENDS


  this.teardown = function() {
      $("#results").empty();
      this.security_import_energy_chart = null;
      this.security_import_fraction_chart = null;
      s =  null;
  };

  this.updateResults = function(pathway, mode) {
      var _mode = (mode == 2050 || mode == 2100) ? mode : 2050;

      /*todo:
        - what about map['III.c.Wave'] ?
        - pathway.map doesn't change on lever changes (e.g. offshore & onshore wind)
          - they change in the new excel, which isn't compiled yet
        - what's to be shown on the right side of the map?

        - done:
          - move overseas to the left
          - add 'imports' label to quarter circle as in original tool
      */

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

      this.security_import_energy_chart.setMode(_mode);
      this.security_import_fraction_chart.setMode(_mode);

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

      // SANKEY
      // Expects the flow table to be in the form of
      // [
      // ["From", "To", 2007, 2010, ..., 2050],
      // ["Coal Reserves", "Coal", 124, 128, ..., 64],
      // ...
      // ]

      // Look for the indices of the columns we want:
      header = pathway.sankey[0];
      from_column = header.indexOf("From");
      to_column = header.indexOf("To");
      flow_column = header.indexOf(2100); // We only care about 2050 data at the moment

      // Check the table is ok
      if(from_column == -1 || to_column == -1 || flow_column == -1) {
        console.log("Energy flow table in unexpected format");
      }

      // Turn it into the form that the sankey library requires:
      // [[from, flow, to]]
      data = pathway.sankey.slice(1).map(function(row) { // slice(1) to skip header row
        return [row[from_column], row[flow_column], row[to_column]]
      });

      this.s.updateData(data);
      this.s.redraw();
      max_y = s.boxes['Losses'].b();
      if ($('#sankey').height() < max_y) {
        $('#sankey').height(max_y);
        this.s.r.setSize($('#sankey').width(), max_y);
      }
      //SANKEY ENDS

      // MAP
      var i, len, map, values, value, x, y, box, side;

      map = {};
      // The data is supplied as a table, with values for every year

      // The first row is the header, look for the 2050 column
      //console.log('pathway', pathway);
      console.log('pathway.map', pathway);
      console.log(pathway.map[0]);
     //column_index = pathway.map[0].indexOf(2050);
      column_index = 1; //8:  2050, 18: 2100
      //column_index = 8; //should be last index (18 for 2050), although that never changes
      // Then skip the header and loop through the rows
      // pathway.map.slice(1).forEach(function(row) {
      pathway.map.forEach(function(row) {
        // Extract the labels and 2050 values into an object
        map[row[0]] = row[column_index];
        console.log('extract map ', map[row[0]]);
      });
      console.log('map map ', map);

      // Draw the line for wave machines
      if (map['III.c.Wave'] > 0) {
        this.wave.label.show();
      } else {
        this.wave.label.hide();
      };
      this.wave.line.attr({ path: ["M", 100, 30, "l", 0, map['III.c.Wave'] * km] });

      // Now draw the land boxes in the right spot
      y = map_height + map_offset_y - 100;
      values = [];

      for (name in this.land_boxes) {
        console.log('name map '+name);
        values.push({ name: name, value: map[name] });
      }
      values.sort(function(a, b) { return b.value - a.value; });
      console.log('values ', values, map);

      for (i = 0, len = values.length; i < len; i++) {
        value = values[i];
        box = this.land_boxes[value.name];
        side = Math.sqrt(value.value * km2);
        box.square.attr({ y: y - side, width: side, height: side });
        box.label.attr({ y: y - (side / 2) });
        // Dont bother showing small boxes
        if (value.value > 10) {
          box.label.show();
        } else {
          box.label.hide();
        }
        y = y - side - 5;
      }

      // Now draw the sea boxes
      x = (map_width / 2) + map_offset_x + 250;
      x = 250;
      y = 30;
      values = [];

      for (name in this.sea_boxes) {
        values.push({ name: name, value: map[name] });
      }
      values.sort(function(a, b) { return b.value - a.value; });

      for (i = 0, len = values.length; i < len; i++) {
        value = values[i];
        box = this.sea_boxes[value.name];
        side = Math.sqrt(value.value * km2);
        box.square.attr({ x: x - side, y: y, width: side, height: side });
        box.label.attr({ x: x + 4, y: y + (side / 2) });
        if (value.value > 10) {
          box.label.show();
        } else {
          box.label.hide();
        }
        y = y + side + 5;
      }

      // Now draw the overseas land boxes
      y = map_height + map_offset_y - 30;
      values = [];

      for (name in this.overseasiand_boxes) {
        values.push({ name: name, value: map[name] });
      }
      values.sort(function(a, b) { return b.value - a.value; });

      for (i = 0, len = values.length; i < len; i++) {
        value = values[i];
        box = this.overseasiand_boxes[value.name];
        side = Math.sqrt(value.value * km2);
        box.square.attr({ y: y - side, width: side, height: side });
        box.label.attr({ y: y - (side / 2) });
        if (value.value > 10) {
          box.label.show();
        } else {
          box.label.hide();
        }
        y = y - side - 5;
      }


      // Now draw the points for particular power stations
/*
      console.log('pathway[map_units]', pathway['map_units']);
      var units = d3.select("#power-stations").selectAll("div")
              .data(pathway['map_units']).enter().append("p").text("hello ");
*/

      /*if (this.points != null) { this.points.remove(); }
      this.r.setStart();
      y = 35;
      x = 1055;
      values = [];


      //  I.a 2 GW coal gas or biomass power stations without CCS
      //  I.b 1.2 GW coal gas or biomass power stations with CCS
      //  III.d 0.01 GW geothermal stations
      //  VII.c 1 GW gas standby power stations
      //  VI.b 215 kt/y waste to energy conversion facilities

      point_cluster_names = ['I.a', 'I.b', 'II.a', 'III.d', 'VII.c', 'VI.b'];
      for (i = 0, len = point_cluster_names.length; i < len; i++) {
        name = point_cluster_names[i];
        values.push({ name: name, value: map[name] });
      }
      values.sort(function(a, b) { return b.value - a.value; });

      for (i = 0, len = values.length; i < len; i++) {
        value = values[i];
        if (value.value >= 1) {
          size = Math.round(Math.sqrt(pointSizes[value.name]) * 10);
          y = this.point_stack(x, y, value.value, colours[value.name], "" + (Math.round(value.value)) + " x " + labels[value.name], size);
        }
      }
      this.points = this.r.setFinish();*/
      // MAP ENDS
/**/
      document.getElementById('view1').setAttribute('class', classView1);
      document.getElementById('view2').setAttribute('class', classView2);
      document.getElementById('view3').setAttribute('class', classView3);

    };



    lineChart();

    return this;

  }.call({});
