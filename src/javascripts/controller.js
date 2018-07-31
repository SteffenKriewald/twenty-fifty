(function() {

  // This object contains a series of properties, where the name of the property
  // matches the name of the view as used in the URl and the value of the property
  // is a function that manages that view and is defined in the views/ folder.
  // e.g.: views.primary_energy = { setup: ..., teardown: ..., updateResults: ... } // defined in views/primary_energy.js
  views = {};
  // This property contains the currently active view. That object is defined in the views/ folder
  // and will respond to setup(), teardown() and updateResults(new_pathway)
  active_view = null;

  // FIXME: Wrap this in a state object
    intermediate_classification_map = {
	"Buildings Temperature Behaviour"					:"Buildings-Heat module",
	"Average temperature of homes"						:"Buildings-Heat module",
	"Home insulation"							:"Buildings-Heat module",
	"Home heating electrification"						:"Buildings-Heat module",
	"Home heating that isn"							:"Buildings-Heat module",
	"Home lighting & appliances"						:"Buildings-Heat module",
	"Electrification of home cooking"					:"Buildings-Heat module",
	"Heating Buildings - Thermal Efficiency"				:"Buildings-Heat module",
	"Heating Buildings - District heat share"				:"Buildings-Heat module",
	"Heating Buildings - Heat Pump share"					:"Buildings-Heat module",
	"Heating Buildings - Hybrid Heat Pump share"				:"Buildings-Heat module",
	"Lighting & Appliance Efficiency"					:"Buildings-Non-Heat module ",
	"Network Heat Supply Heat Pump Output"					:"Network Heat Source module",
	"Network Heat Supply Bio CHP Output"					:"Network Heat Source module",
	"Domestic transport behaviour"						:"Transport Demand",
	"International Aviation Demand"						:"Transport Demand",
	"Shift to zero emission transport"					:"Transport Demand",
	"Choice of zero-emission technology"					:"Transport Demand",
	"Domestic freight"							:"Transport Demand",
	"International aviation"						:"Transport Demand",
	"International shipping"						:"Transport Demand",
	"Light Vehicle - Electric vehicle distance share"			:"Transport (Excluding Aviation)",
	"Light Vehicle - Hydrogen vehicle distance share"			:"Transport (Excluding Aviation)",
	"Light Vehicle - Plug-in Hybrid Electric vehicle distance share"	:"Transport (Excluding Aviation)",
	"Light Vehicle - Biofuel share of liquid fuel"				:"Transport (Excluding Aviation)",
	"Heavy Vehicle - Electric vehicle distance share"			:"Transport (Excluding Aviation)",
	"Heavy Vehicle - Hydrogen vehicle distance share"			:"Transport (Excluding Aviation)",
	"Heavy Vehicle - Plug-in Hybrid Electric vehicle distance share"	:"Transport (Excluding Aviation)",
	"Heavy Vehicle - Biofuel share of liquid fuel"				:"Transport (Excluding Aviation)",
	"Aviation - Efficiency & Hybridisation"					:"Transport Aviation",
	"Aviation - Biofuel share of liquid fuel"				:"Transport Aviation",
	"Growth in industry"							:"Industry",
	"Energy intensity of industry"						:"Industry",
	"Energy Intensity"							:"Industry",
	"Industry Electricity Fuel Share"					:"Industry",
	"Industry Biomass Fuel Share"						:"Industry",
	"Industry Gas Fuel Share"						:"Industry",
	"Industry CCS - share of emissions with CCS"				:"Industry",
	"Hydrogen share of gas grid"						:"Gas Grid Source",
	"Biomethane share of gas grid"						:"Gas Grid Source",
	"Hydrogen Produced form Bio Gasification CCS"				:"Hydrogen Production",
	"Hydrogen Produced form SMR CCS"					:"Hydrogen Production",
	"Hydrogen from Zero Carbon Imports"					:"Hydrogen Production",
	"Emissions Removal from Other GGR"					:"Greenhouse Gas Removal (GGR)",
	"Bio Transformation with CCS share"					:"Greenhouse Gas Removal (GGR)",
	"CCS Capture rate development"						:"Greenhouse Gas Removal (GGR)",
	"Land dedicated to Woodland"						:"Farming & Forestry",
	"Land dedicated to bioenergy and share of agricultural waste collected"	:"Farming & Forestry",
	"Agricultural emissions intensity and yield improvement"		:"Farming & Forestry",
	"Waste reduction and diversion"						:"Bioenergy & Waste Transformations",
	"Short term electricity storage capacity"				:"Electricity Storage and Balancing",
	"Seasonal electricity storage capacity"					:"Electricity Storage and Balancing",
	"Biomass CCS Generation Capacity"					:"Electricity Generation",
	"Nuclear Generation Capacity"						:"Electricity Generation",
	"Wind Generation Capacity"						:"Electricity Generation",
	"Solar Generation Capacity"						:"Electricity Generation",
	"Marine (composite) Generation Capacity"				:"Electricity Generation",
	"Gas CCS Generation Capacity"                                           :"Electricity Generation"
    };

    controller = null;
    choices = null;
    startdatechoices = null;
    enddatechoices = null;
    old_startdatechoices = null;
    old_enddatechoices = null;

    view = null;
    sector = null; // FIXME: Rename to sub_view.
    comparator = null;
    old_choices = [];

  // FIXME: Where is the right spot for this?
  cache = {};

  // This is the first thing that gets called when everything has been loaded.
  // It wires up the controls, sets up the initial view and loads the first
  // pathway.
    $(document).ready( function() {
	setupBootSideMenu();
	accordionLeverToggle();
	checkSVGWorks();
	setUpControls();
	setVariablesFromURL();
	switchView(view);
	loadMainPathway();
    });

    accordionLeverToggle = function() {
	$('#accordion').find('.accordion-toggle').click(function(){
	this.classList.toggle("active"); /*new*/
	//Expand or collapse this panel
	$(this).next().slideToggle('fast');
	// Show or not Show Sparklines for group
	var spark = document.getElementById($(this).next().attr('id')+"db");
	if (spark.style.visibility == 'hidden') {spark.style.visibility='visible'} else {spark.style.visibility='hidden'}
	});
    }


//       setupBootSideMenu = function() {
//   	$('#lever_nav_panel').BootSideMenu({
//   	    side: "left",
//   	    width: "400px"
//   	});
//       }

    setupBootSideMenu = function() {
	$('#lever_nav_panel').BootSideMenu({
	    side: "left",
	    width: "465px",
	    closeOnClick: false,
//    pushBody: true,
	    icons: {
		left: '',
		right: '',
		down: ''
	    },
	    onStartup: function() {
		$('.toggler').addClass("hide");
	    }
	});
	$('#lever_nav_panel').BootSideMenu.open();
    }

  // Some of the graphs require SVG, which is only supported in modern browsers (Internet Explorer >8)
  // This function checks that SVG is supported, and if not reveals a warning block that is
  // in src/index.html.erb
  checkSVGWorks = function() {
    if (!!document.createElementNS && !!document.createElementNS('http://www.w3.org/2000/svg', "svg").createSVGRect) { return; }
    $("#svgWarn").show();
  };

  // The controls are a series of tables in src/index.html.erb in the #classic_controls block
  // This method attaches javascript function to those tables to trigger other jascript
  // methods in this file when they are clicked.
  setUpControls = function() {
    // All links with titles have the title turned into a tooltip. The tooltip is styled
    // in src/stylesheets/tooltip.css
      $("a[title]").tooltip({ delay: 0, trigger : 'hover', position: 'top left', offset: [3, 3], tip: '#tooltip' });

    // This turns the cells that are labeled '1' '2' '3' '4' into controls that select
    // a new pathway. The cell is expected to have a data-choicenumber attribute that
    // indicates whether it is nuclear, CCS, home heating etc and a data-choicelevel
    // attribute that indicates whether it
    $("a.choiceLink").on('click touchend', function(event) {
      event.preventDefault();
      t = $(event.target);
      c = t.data().choicenumber;
      l = t.data().choicelevel;
      go(c, l);
	g = t.data().group;
	// create (FIXME) - Update spark plot depending on group
	if ( g == 'demand') { var levelData = choices.slice(25, 30).concat(choices.slice(32, 35),choices.slice(37, 38),choices.slice(40, 41));}
	if ( g == 'supply') { var levelData = choices.slice(0, 0).concat(choices.slice(2, 15),choices.slice(17, 22));}
	if ( g == 'other') { var levelData = choices.slice(50, 52);}
	$("."+g+"db").sparkline(levelData, {
		type: 'bar', barColor: '#7f99b2', disableInteraction: true,
		barWidth:'2px', height:'75px', barSpacing:'0px',
		chartRangeMin: 0, chartRangeMax: 4}
	);
	document.getElementById(g+'db').style.visibility="hidden";
    });

      $("a.leverNameLink").html(function(index, leverName) {
	  truncated = leverName.trim().substring(0, 25);
	  if (truncated.length < leverName.trim().length) {
	      truncated += "..."
	  }
	  return truncated;
    });

      // This turns links containing dates '2010' .. '2100' into
      // controls that select new pathways. The link is expected to have
      // a data-choicenumber attribute that indicates start or end date
      // of implementation for a pathway
      $("a.dateChoiceLink").on('click touchend', function(event) {
	  //	  alert("dateChoiceLink event triggered")
	  event.preventDefault();
	  t = $(event.target);
	  c = t.data().choicenumber;
	  d = t.data().datechoice;

	  if (t.parent().parent().find("button").hasClass("sd")) {
	      old_startdatechoices = startdatechoices.slice(0);
	      startdatechoices[c] = d;
        if (startdatechoices[c] > enddatechoices[c]-10) {
          old_enddatechoices = enddatechoices.slice(0);
          //enddatechoices[c] = d;
          /* minimal implementation time */
          // What happens with border-limitation (2025-10 < min 2020; 2095 +10 > max 2100)
          enddatechoices[c] = d + 10;
        }

	  }
	  else {
	      old_enddatechoices = enddatechoices.slice(0);
	      enddatechoices[c] = d;
        if (enddatechoices[c] < startdatechoices[c]+10) {
          old_startdatechoices = startdatechoices.slice(0);
          //startdatechoices[c] = d;
          /* minimal implementation time */
          // What happens with border-limitation (2025-10 < min 2020; 2095 +10 > max 2100)
          startdatechoices[c] = d - 10;
        }


	  }


	// destroy old slider and create new one with updated dates and colors
	window['sliderSlider'+c].noUiSlider.destroy()
	noUiSlider.create(window['sliderSlider'+c], {
		start: [ startdatechoices[c], enddatechoices[c]],
		step: 5,
		connect: true,
		range: {'min':  2020,'max':  2100},
		cssPrefix: 'noUi-', // defaults to 'noUi-',
		cssClasses: {
			// Full list of classnames to override.
			// Does NOT extend the default classes.
			// Have a look at the source for the full, current list:
			// https://github.com/leongersen/noUiSlider/blob/master/src/js/options.js#L398
				target: 'target',
				base: 'base',
				origin: 'origin',
				handle: 'handle-my',
				handleLower: 'handle-lower',
				handleUpper: 'handle-upper',
				horizontal: 'horizontal',
				vertical: 'vertical',
				background: 'background',
				connect: 'connect-my',
				connects: 'connects',
				ltr: 'ltr',
				rtl: 'rtl',
				draggable: 'draggable',
				drag: 'state-drag',
				tap: 'state-tap',
				active: 'active',
				tooltip: 'tooltip',
				pips: 'pips',
				pipsHorizontal: 'pips-horizontal',
				pipsVertical: 'pips-vertical',
				marker: 'marker',
				markerHorizontal: 'marker-horizontal',
				markerVertical: 'marker-vertical',
				markerNormal: 'marker-normal',
				markerLarge: 'marker-large',
				markerSub: 'marker-sub',
				value: 'value',
				valueHorizontal: 'value-horizontal',
				valueVertical: 'value-vertical',
				valueNormal: 'value-normal',
				valueLarge: 'value-large',
				valueSub: 'value-sub'
		}
	});
	window['sliderSlider'+c].setAttribute('disabled', true);

	loadMainPathway();
      });



    $("a.view").on('click touchend', function(event) {
      var t, v;
      event.preventDefault();
      t = $(event.target);
      v = t.data().view;
      return switchView(v);
    });


    $(".newdropdown").on('click', function(event) {
      var d, o, space, t;
      event.preventDefault();
      t = $(event.target);
      d = $(t.data().dropdown);
      if (d.hasClass("showdropdown")) {
        return d.removeClass("showdropdown");
      } else {
        d.addClass("showdropdown");
        o = t.offset();
        o.top = o.top + t.height();
        space = $(document).width() - o.left - d.width();
        if (space < 0) {
          o.left = o.left + space;
        }
        return d.offset(o);
      }
    });

//      $("#classic_controls table tr").on('mouseover', function() {
//	  $(this).find("a.choiceLink").css("color","#000");
//	  $(this).find("td.choice2").css("display","block");
//	  });
//
//      $("#classic_controls table tr").on('mouseout', function() {
//	  $(this).find("a.choiceLink").css("color","#FFF");
//	  $(this).find("td.choice2").css("display","none");
//	  });
//
//

    // This triggers the interface to loop through levels 1 to 4
    // when the user hovers their mouse over a choice.
    d3.selectAll('td.name a')
      .datum(function() { return this.dataset })
      .on('mouseover', function(d,i) { startDemo(d.choicenumber); })
      .on('mouseout', function(d,i) { stopDemo(d.choicenumber); });


    // This forces the view to be redrawn if the user resizes their
    // browser window. It uses a timer to only trigger the redraw
    // half a second after the user has stopped resizing.
    // FIXME: The redrawing sometimes appears buggy.
    windowResizeDebounceTimer = null;
    $(window).resize(function(event) {
      clearTimeout(windowResizeDebounceTimer);
      windowResizeDebounceTimer = setTimeout(function() {
        // FIXME: Refactor out the cache[codeForChoices()] call
        active_view.updateResults(cache[codeForChoices()]);
      }, 500);
    });

      _old_intermediate_category = null;
      _intermediate_category = null;
      _color_intermediate_category = "lever-grey";

      controls = $('#classic_controls');
      // loop over lever "hidden" categories
      _demandIterator = [];
      //      for (i=25; i<42; i == 31 ? i+=2 : i++) _demandIterator.push(i);
      //      for (i in _demandIterator) {
      for (i=25; i<42; (i == 30||i == 35||i == 38) ? i+=2 : i++) {
//	  alert("i is '" + i + "'");
	  row = controls.find("tr#r" + i);
	  _original_title = row.find("a.leverNameLink").attr("data-original-title");
	  _intermediate_category = intermediate_classification_map[_original_title];
//	  if (_intermediate_category === undefined)
//	  alert(_original_title);
	  if (_old_intermediate_category == null) {
	      _old_intermediate_category = _intermediate_category;
	  }

	  if (_intermediate_category !== _old_intermediate_category) {
	      if (_color_intermediate_category === "lever-grey") {
		  _color_intermediate_category = "lever-lightgrey";
	      } else {
		  _color_intermediate_category = "lever-grey";
	      }
	  }
	  row.find("td.name").addClass(_color_intermediate_category);
	  _old_intermediate_category = _intermediate_category;
      }

  };

  setVariablesFromURL = function() {
      var url_elements;
      url_elements = window.location.pathname.split('/');
      controller = url_elements[1] || "pathways";
      choices = choicesForCode(url_elements[2] || twentyfifty.default_pathway);
      startdatechoices = [];
      for (i=1; i <=53; i++) {
	  startdatechoices.push(2020);
      }
      enddatechoices = [];
      for (i=1; i <=53; i++) {
	  enddatechoices.push(2050);
      }
      old_startdatechoices = startdatechoices.slice(0);
      old_enddatechoices = enddatechoices.slice(0);
      //      view = url_elements[3] || "primary_energy_chart";
      view = url_elements[3] || "overview";
      if (view === 'costs_compared_within_sector') {
	  sector = url_elements[4];
      }
      if (url_elements[4] === 'comparator') {
	  return comparator = url_elements[5];
      }
  };

  float_to_letter_map = {
    "": "0",
    1.0: "1",
    1.1: "b",
    1.2: "c",
    1.3: "d",
    1.4: "e",
    1.5: "f",
    1.6: "g",
    1.7: "h",
    1.8: "i",
    1.9: "j",
    2.0: "2",
    2.1: "l",
    2.2: "m",
    2.3: "n",
    2.4: "o",
    2.5: "p",
    2.6: "q",
    2.7: "r",
    2.8: "s",
    2.9: "t",
    3.0: "3",
    3.1: "v",
    3.2: "w",
    3.3: "x",
    3.4: "y",
    3.5: "z",
    3.6: "A",
    3.7: "B",
    3.8: "C",
    3.9: "D",
    0.0: "0",
    4.0: "4"
  };

  codeForChoices = function(c) {
    var cd, choice;
    if (c == null) {
      c = choices;
    }
    cd = (function() {
      var _i, _len, _results;
      _results = [];
      for (_i = 0, _len = c.length; _i < _len; _i++) {
        choice = c[_i];
        _results.push(float_to_letter_map[choice]);
      }
      return _results;
    })();
    return cd.join('');
  };

  letter_to_float_map = {
    "1": 1.0,
    "b": 1.1,
    "c": 1.2,
    "d": 1.3,
    "e": 1.4,
    "f": 1.5,
    "g": 1.6,
    "h": 1.7,
    "i": 1.8,
    "j": 1.9,
    "2": 2.0,
    "l": 2.1,
    "m": 2.2,
    "n": 2.3,
    "o": 2.4,
    "p": 2.5,
    "q": 2.6,
    "r": 2.7,
    "s": 2.8,
    "t": 2.9,
    "3": 3.0,
    "v": 3.1,
    "w": 3.2,
    "x": 3.3,
    "y": 3.4,
    "z": 3.5,
    "A": 3.6,
    "B": 3.7,
    "C": 3.8,
    "D": 3.9,
    "0": 0.0,
    "4": 4.0
  };

  choicesForCode = function(newCode) {
    var choice, _i, _len, _ref, _results;
    _ref = newCode.split('');
    _results = [];
    for (_i = 0, _len = _ref.length; _i < _len; _i++) {
      choice = _ref[_i];
      _results.push(letter_to_float_map[choice]);
    }
    return _results;
  };

  url = function(options) {
    var s;
    if (options == null) {
      options = {};
    }
    s = jQuery.extend({
      controller: controller,
      code: codeForChoices(),
      view: view,
      sector: sector,
      comparator: getComparator()
    }, options);
    if (s.view === 'costs_compared_within_sector' && (s.sector != null)) {
      return "/" + s.controller + "/" + s.code + "/" + s.view + "/" + s.sector;
    } else if (s.comparator != null) {
      return "/" + s.controller + "/" + s.code + "/" + s.view + "/comparator/" + s.comparator;
    } else {
      return "/" + s.controller + "/" + s.code + "/" + s.view;
    }
  };

  go = function(index, level) {
    old_choices = choices.slice(0);
    if (index <= 15 && index !== 3 && level > 1 && Math.ceil(choices[index]) === level) {
      choices[index] = Math.round((choices[index] - 0.1) * 10) / 10;
    } else {
      choices[index] = level;
    }
    return loadMainPathway();
  };

//    goWithDates = function(index, date, ) {
//      old_startdatechoices = startdatechoices.slice(0);
//      startdatechoices[index] = date;
//      old_enddatechoices = enddatechoices.slice(0);
//      enddatechoices[index] = date;
////      alert("date choice is: " + startdatechoices[index]);
//      return loadMainPathway();
//  };
//

  demoTimer = null;

  demoOriginalLevel = null;

  startDemo = function(choice) {
    var demoLevel, demoMaximum;
    demoLevel = 1;
    demoOriginalLevel = choices[choice];
    demoMaximum = window.twentyfifty.choice_sizes[choice];
    return demoTimer = setInterval((function() {
      go(choice, demoLevel);
      demoLevel = demoLevel + 1;
      if (demoLevel > demoMaximum) {
        demoLevel = 1;
      }
      return false;
    }), 1000);
  };

  stopDemo = function(choice) {
    if (demoTimer != null) {
      clearInterval(demoTimer);
    }
    if ((demoOriginalLevel != null) && demoOriginalLevel !== choices[choice]) {
      return go(choice, demoOriginalLevel);
    }
  };

  switchView = function(new_view) {
    var c, data;
    $('.showdropdown').removeClass("showdropdown");
    if (view === new_view && (active_view != null)) {
      return false;
    }
    if (active_view != null) {
      active_view.teardown();
    }
    view = new_view;
    active_view = views[view];
    $("a.selectedView").removeClass("selectedView");
    $("a.view[data-view='" + view + "']").addClass("selectedView");
    if (view === "costs_in_context") {
      $("#cost_choice").addClass("selectedView").text("Costs: context");
    } else if (view === "costs_compared_overview") {
      $("#cost_choice").addClass("selectedView").text("Costs: compared");
    } else if (view === "costs_sensitivity") {
      $("#cost_choice").addClass("selectedView").text("Costs: sensitivity");
    } else {
      $("#cost_choice").text("Costs");
    }
    active_view.setup();
    c = codeForChoices();
    data = cache[c];
    if (data != null) {
      active_view.updateResults(data);
    }

    if(active_view.updateComparator != undefined) {
      updateComparator();
    }

    if (history['pushState'] != null) {
      return history.pushState(choices, c, url());
    }
  };

  switchPathway = function(new_code) {
    return setChoices(choicesForCode(new_code));
  };

  setChoices = function(new_choices) {
    $('.showdropdown').removeClass("showdropdown");
    old_choices = choices.slice(0);
    choices = new_choices;
    loadSparkLines();
    return loadMainPathway();
  };

  loadMainPathway = function(pushState) {
    var fetch, main_code;
    if (pushState == null) {
      pushState = true;
    }
    if (choices.join('') === old_choices.join('')) {
      return false;
    }
    updateControls(old_choices, choices);
    main_code = codeForChoices();
    if (history['pushState'] != null) {
      history.pushState(choices, main_code, url());
    }
    if (cache[main_code] != null) {
      active_view.updateResults(cache[main_code]);
      return $('#calculating').hide();
    } else {
      $('#calculating').show();
      fetch = function() {
        return $.getJSON(url({
          code: main_code,
          view: 'data',
          sector: null,
          comparator: null
        }), function(data) {
          if (data != null) {
            cache[data._id] = data;
            if (data._id === codeForChoices()) {
              active_view.updateResults(data);
              return $('#calculating').hide();
            }
          }
        });
      };
      return fetch();
    }
  };

  loadSecondaryPathway = function(secondary_code, callback) {
    var fetch;
    if (cache[secondary_code] != null) {
      return callback(cache[secondary_code]);
    } else {
      fetch = (function(_this) {
        return function() {
          return $.getJSON(url({
            code: secondary_code,
            view: 'data',
            sector: null,
            comparator: null
          }), function(data) {
            if (data != null) {
              cache[data._id] = data;
              return callback(data);
            }
          });
        };
      })(this);
      return fetch();
    }
  };

  window.onpopstate = function(event) {
    var url_elements;
    if (!event.state) {
      return false;
    }
    url_elements = window.location.pathname.split('/');
    setChoices(choicesForCode(url_elements[2]));
    switchView(url_elements[3]);
    if (view === 'costs_compared_within_sector') {
      switchSector(url_elements[4]);
    }
    if (url_elements[4] === 'comparator') {
      return switchComparator(url_elements[5]);
    }
  };

    // TODO [2018-06-19 Tue 13:43]: return value respects only the old interface, not the updated one with dates and intermediate categories
    updateControls = function(old_choices, choices) {

	var c, choice, choice_frview, choice_whole, controls, i, old_choice, old_choice_frview, old_choice_whole, row, _i, _j, _len, _ref, _ref1, _results, _button, _startdate, _old_startdate, _enddate, _old_enddate, _original_title, _color_intermediate_category, _old_intermediate_category, intermediate_category;
	this.choices = choices;
	controls = $('#classic_controls');
	_ref = this.choices;
	_results = [];

	// loop over lever levels
	for (i = _i = 0, _len = _ref.length; _i < _len; i = ++_i) {
	    choice = _ref[i];
	    old_choice = old_choices[i];
	    if (choice !== old_choices[i]) {
		old_choice_whole = Math.ceil(old_choice);
		old_choice_frview = parseInt((old_choice % 1) * 10);
		choice_whole = Math.ceil(choice);
		choice_frview = parseInt((choice % 1) * 10);
		row = controls.find("tr#r" + i);
		_color_intermediate_category = $("#classic_controls tr#r" + i + " td.name").hasClass("lever-grey")?"lever-grey":"lever-lightgrey";
		row.find(".selected, .level" + old_choice_whole + ", .level" + old_choice_whole + "_" + old_choice_frview).removeClass("selected level" + old_choice_whole + " level" + old_choice_whole + "_" + old_choice_frview + " " + _color_intermediate_category);

		if (old_choice_frview !== 0) {
		    controls.find("#c" + i + "l" + old_choice_whole).text(old_choice_whole);
		}
		row.find("#c" + i + "l" + choice_whole).addClass('selected');
		for (c = _j = 1, _ref1 = choice_whole - 1; 1 <= _ref1 ? _j <= _ref1 : _j >= _ref1; c = 1 <= _ref1 ? ++_j : --_j) {
		    controls.find("#c" + i + "l" + c).addClass("level" + choice_whole + " " + _color_intermediate_category);
		}
		if (choice_frview !== 0) {
		    controls.find("#c" + i + "l" + choice_whole).text(choice);
		    _results.push(controls.find("#c" + i + "l" + choice_whole).addClass("level" + choice_whole + "_" + choice_frview));
		} else {
		    _results.push(controls.find("#c" + i + "l" + choice_whole).addClass("level" + choice_whole + " " +  _color_intermediate_category));
		}
	    } else {
		_results.push(void 0);
	    }
	}

	// loop over date choices
	for (i = 0; i < startdatechoices.length; i++) {
	    _startdate = startdatechoices[i];
	    _enddate = enddatechoices[i];
	    _old_startdate = old_startdatechoices[i];
	    _old_enddate = old_enddatechoices[i];
	    if ((_startdate !== _old_startdate)||(_enddate !== _old_enddate)) {
		_button = controls.find("#cd" + i);
    // show updated years - via class change (years will be shown permanently)
		_button.removeClass("update date-choice-mode-2100-default");
		_button.addClass("update date-choice-mode-2100-edited");
	    }
	    if (_startdate !== _old_startdate) {
		_button.find(".sd").text(_startdate);
	    }
	    if (_enddate !== _old_enddate) {
		_button.find(".ed").text(_enddate);
	    }

	}



	return _results;
    };

  getSector = function() {
    return parseInt(sector);
  };

  switchSector = function(new_sector) {
    sector = new_sector;
    if (history['pushState'] != null) {
      history.pushState(choices, codeForChoices(), url());
    }
    switchView('costs_compared_within_sector');
    active_view.teardown();
    return active_view.updateResults(cache[codeForChoices()]);
  };

  getComparator = function() {
    return comparator || twentyfifty.default_comparator_code;
  };

  updateComparator = function() {
    loadSecondaryPathway(getComparator(), function(comparator) {
      active_view.updateComparator(comparator);
    });
  }

  switchComparator = function(new_comparator) {
    comparator = new_comparator;
    if (history['pushState'] != null) {
      history.pushState(choices, codeForChoices(), url());
    }
    updateComparator();
  };

  pathwayName = function(pathway_code, default_name) {
    if (default_name == null) {
      default_name = null;
    }
    return window.twentyfifty.pathway_names_hash[pathway_code] || default_name;
  };

  pathwayDescriptions = function(pathway_code, default_description) {
    if (default_description == null) {
      default_description = null;
    }
    return window.twentyfifty.pathway_descriptions_hash[pathway_code] || default_description;
  };

  pathwayWikiPages = function(pathway_code, default_page) {
    if (default_page == null) {
      default_page = null;
    }
    return "http://2050-calculator-tool-wiki.decc.gov.uk/pages/" + (window.twentyfifty.pathway_wiki_pages_hash[pathway_code] || default_page);
  };

  getChoices = function() {
    return choices;
  };

  window.twentyfifty.code = codeForChoices;

  window.twentyfifty.getChoices = getChoices;

  window.twentyfifty.setChoices = setChoices;

  window.twentyfifty.getSector = getSector;

  window.twentyfifty.switchSector = switchSector;

  window.twentyfifty.getComparator = getComparator;

  window.twentyfifty.switchComparator = switchComparator;

  window.twentyfifty.url = url;

  window.twentyfifty.loadMainPathway = loadMainPathway;

  window.twentyfifty.loadSecondaryPathway = loadSecondaryPathway;

  window.twentyfifty.switchView = switchView;

  window.twentyfifty.switchPathway = switchPathway;

  window.twentyfifty.pathwayName = pathwayName;

  window.twentyfifty.pathwayDescriptions = pathwayDescriptions;

  window.twentyfifty.pathwayWikiPages = pathwayWikiPages;

  window.twentyfifty.startDemo = startDemo;

  window.twentyfifty.stopDemo = stopDemo;

  window.twentyfifty.views = views;

}).call(this);

var mode2010 = "switch to 2100 mode";
var mode2050 = "go back to 2050 mode";
function timeMode(){
    var mode = document.getElementById("modeSwitchID").value;
    if ( mode == mode2010 ) {
      var change = 'visible';
      document.getElementById("modeSwitchID").value = mode2050;
    } else {
        var conf = confirm("Are you sure you want to go back to 2050 mode, all timing information will be lost.");
        if (conf==true) {
          var change = 'hidden';
          document.getElementById("modeSwitchID").value = mode2010;
          location.reload();
        }
      }
    var good = document.getElementsByClassName('date-choice-mode-2100-wrapper');
    for (var i = 0; i < good.length; i++) { good[i].style.visibility = change; }
}

function loadSparkLines() {
  var groupArray = ["demand","supply","other"];
    for (var i = 0; i <groupArray.length; i++) {
      var g = groupArray[i];
      if ( g == 'demand') { var levelData = choices.slice(25, 30).concat(choices.slice(32, 35),choices.slice(37, 38),choices.slice(40, 41));}
      if ( g == 'supply') { var levelData = choices.slice(0, 0).concat(choices.slice(2, 15),choices.slice(17, 22));}
      if ( g == 'other') { var levelData = choices.slice(50, 52);}
      $("."+g+"db").sparkline(levelData, {
        type: 'bar', barColor: '#7f99b2', disableInteraction: true,
        barWidth:'2px', height:'75px', barSpacing:'0px',
        chartRangeMin: 0, chartRangeMax: 4}
      );
    }

}

window.onload = loadSparkLines;
