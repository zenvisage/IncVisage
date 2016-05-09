
var width = 400;
var height = 200;
var padding_top = 10;
var padding_left = 30;
var padding_bottom = 30;
var padding_right = 10;


       
function createJSON(){
  var value = Math.random();
  width = 400*(value+1.2) //1->2 multiplier
  var json = {};
  json["width"] = width;
  json["height"] = 200;
  var padding= {"top": padding_top, "left": padding_left, "bottom": padding_bottom, "right": padding_right};
  json["padding"] = padding;

  //data----
  var data = [];
  var table_data = {"name":"table"};
  var table_data_values = [];
  end = 20+value*50;
  for(i=1; i<end; i++){
    table_data_values.push({"x":i, "y": Math.random()*100});
  }
  table_data["values"]= table_data_values;
  data.push(table_data);
  json["data"] = data;
  //end data-----

  json["scales"] = [
    {
      "name": "x",
      "type": "ordinal",
      "range": "width",
      "domain": {"data": "table", "field": "data.x"}
    },
    {
      "name": "y",
      "range": "height",
      "nice": true,
      "domain": {"data": "table", "field": "data.y"}
    }
  ];

  json["axes"] =  [
    {"type": "x", "scale": "x"},
    {"type": "y", "scale": "y"}
  ];

  json["marks"] =  [
    {
      "type": "rect",
      "from": {"data": "table"},
      "properties": {
        "enter": {
          "x": {"scale": "x", "field": "data.x"},
          "width": {"scale": "x", "band": true, "offset": -1},
          "y": {"scale": "y", "field": "data.y"},
          "y2": {"scale": "y", "value": 0}
        },
        "update": {
          "fill": {"value": "steelblue"}
        },
        "hover": {
          "fill": {"value": "red"}
        }
      }
    }
  ];
  return json;
}

function createBlank(){
  var json = {};
  json["width"] = width;
  json["height"] = 200;
  var padding= {"top": padding_top, "left": 50, "bottom": padding_bottom, "right": padding_right};
  json["padding"] = padding;

  //data----
  var data = [    {
      "name": "table",
      "values": [
        {
          "x": 3,
          "y": 4
        },
        {
        	"x":1,
        	"y":0
        }
      ]
    }];
  json["data"] = data;

  json["scales"] = [
    {
      "name": "x",
      "nice": true,
      "range": "width",
      "domainMin": 0.0,
      "domainMax": 4.0,
      "zero": false
    },
    {
      "name": "y",
      "range": "height",
      "nice": true,
      "domainMin": 0.0,
      "domainMax": 4.0,
      "zero": false
    }
  ];

  json["axes"] =  [
    {"type": "x", "scale": "x"},
    {"type": "y", "scale": "y"}
  ];
	
 

  json["marks"] =  [
    {
      "type": "rect",
      "from": {"data": "table"},
      "properties": {
        "enter": {
          "x": {"scale": "x", "field": ""},
          "width": {"scale": "x", "offset": -1},
          "y": {"scale": "y", "field": ""},
          "y2": {"scale": "y", "value": 0}
        },
        "update": {
          "fill": {"value": "steelblue"}
        },
        "hover": {
          "fill": {"value": "red"}
        }
      }
    }
  ];
  return json;
}


function createBlank(xmin,xmax,ymin,ymax){
	  var json = {};
	  json["width"] = width;
	  json["height"] = 200;
	  var padding= {"top": padding_top, "left": 50, "bottom": padding_bottom, "right": padding_right};
	  json["padding"] = padding;

	  //data----
	  var data = [    {
	      "name": "table",
	      "values": [
	        {
	          "x": 3,
	          "y": 4
	        },
	        {
	        	"x":1,
	        	"y":0
	        }
	      ]
	    }];
	  json["data"] = data;

	  json["scales"] = [
	    {
	      "name": "x",
	      "nice": true,
	      "range": "width",
	      "domainMin": xmin,
	      "domainMax": xmax,
	      "zero": false
	    },
	    {
	      "name": "y",
	      "range": "height",
	      "nice": true,
	      "domainMin": ymin,
	      "domainMax": ymax,
	      "zero": false
	    }
	  ];

	  json["axes"] =  [
	    {"type": "x", "scale": "x"},
	    {"type": "y", "scale": "y"}
	  ];
		
	 

	  json["marks"] =  [
	    {
	      "type": "rect",
	      "from": {"data": "table"},
	      "properties": {
	        "enter": {
	          "x": {"scale": "x", "field": ""},
	          "width": {"scale": "x", "offset": -1},
	          "y": {"scale": "y", "field": ""},
	          "y2": {"scale": "y", "value": 0}
	        },
	        "update": {
	          "fill": {"value": "steelblue"}
	        },
	        "hover": {
	          "fill": {"value": "red"}
	        }
	      }
	    }
	  ];
	  return json;
	}


function createScatter(){
  var json = {};
  json["width"] = 200;
  json["height"] = 200;
  var padding= {"top": padding_top, "left": padding_left, "bottom": padding_bottom, "right": padding_right};
  //json["padding"] = padding;
  //json["viewport"]=[width+50, height+50]

  //data----
  var data = [
  {
    "name":"iris",
    "values": [
      {"sepalLength": 5.1, "sepalWidth": 3.5, "petalLength": 1.4, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 4.9, "sepalWidth": 3.0, "petalLength": 1.4, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 4.7, "sepalWidth": 3.2, "petalLength": 1.3, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 4.6, "sepalWidth": 3.1, "petalLength": 1.5, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 5.0, "sepalWidth": 3.6, "petalLength": 1.4, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 5.4, "sepalWidth": 3.9, "petalLength": 1.7, "petalWidth": 0.4, "species": "setosa"},
      {"sepalLength": 4.6, "sepalWidth": 3.4, "petalLength": 1.4, "petalWidth": 0.3, "species": "setosa"},
      {"sepalLength": 5.0, "sepalWidth": 3.4, "petalLength": 1.5, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 4.4, "sepalWidth": 2.9, "petalLength": 1.4, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 4.9, "sepalWidth": 3.1, "petalLength": 1.5, "petalWidth": 0.1, "species": "setosa"},
      {"sepalLength": 5.4, "sepalWidth": 3.7, "petalLength": 1.5, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 4.8, "sepalWidth": 3.4, "petalLength": 1.6, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 4.8, "sepalWidth": 3.0, "petalLength": 1.4, "petalWidth": 0.1, "species": "setosa"},
      {"sepalLength": 4.3, "sepalWidth": 3.0, "petalLength": 1.1, "petalWidth": 0.1, "species": "setosa"},
      {"sepalLength": 5.8, "sepalWidth": 4.0, "petalLength": 1.2, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 5.7, "sepalWidth": 4.4, "petalLength": 1.5, "petalWidth": 0.4, "species": "setosa"},
      {"sepalLength": 5.4, "sepalWidth": 3.9, "petalLength": 1.3, "petalWidth": 0.4, "species": "setosa"},
      {"sepalLength": 5.1, "sepalWidth": 3.5, "petalLength": 1.4, "petalWidth": 0.3, "species": "setosa"},
      {"sepalLength": 5.7, "sepalWidth": 3.8, "petalLength": 1.7, "petalWidth": 0.3, "species": "setosa"},
      {"sepalLength": 5.1, "sepalWidth": 3.8, "petalLength": 1.5, "petalWidth": 0.3, "species": "setosa"},
      {"sepalLength": 5.4, "sepalWidth": 3.4, "petalLength": 1.7, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 5.1, "sepalWidth": 3.7, "petalLength": 1.5, "petalWidth": 0.4, "species": "setosa"},
      {"sepalLength": 4.6, "sepalWidth": 3.6, "petalLength": 1.0, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 5.1, "sepalWidth": 3.3, "petalLength": 1.7, "petalWidth": 0.5, "species": "setosa"},
      {"sepalLength": 4.8, "sepalWidth": 3.4, "petalLength": 1.9, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 5.0, "sepalWidth": 3.0, "petalLength": 1.6, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 5.0, "sepalWidth": 3.4, "petalLength": 1.6, "petalWidth": 0.4, "species": "setosa"},
      {"sepalLength": 5.2, "sepalWidth": 3.5, "petalLength": 1.5, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 5.2, "sepalWidth": 3.4, "petalLength": 1.4, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 4.7, "sepalWidth": 3.2, "petalLength": 1.6, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 4.8, "sepalWidth": 3.1, "petalLength": 1.6, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 5.4, "sepalWidth": 3.4, "petalLength": 1.5, "petalWidth": 0.4, "species": "setosa"},
      {"sepalLength": 5.2, "sepalWidth": 4.1, "petalLength": 1.5, "petalWidth": 0.1, "species": "setosa"},
      {"sepalLength": 5.5, "sepalWidth": 4.2, "petalLength": 1.4, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 4.9, "sepalWidth": 3.1, "petalLength": 1.5, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 5.0, "sepalWidth": 3.2, "petalLength": 1.2, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 5.5, "sepalWidth": 3.5, "petalLength": 1.3, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 4.9, "sepalWidth": 3.6, "petalLength": 1.4, "petalWidth": 0.1, "species": "setosa"},
      {"sepalLength": 4.4, "sepalWidth": 3.0, "petalLength": 1.3, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 5.1, "sepalWidth": 3.4, "petalLength": 1.5, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 5.0, "sepalWidth": 3.5, "petalLength": 1.3, "petalWidth": 0.3, "species": "setosa"},
      {"sepalLength": 4.5, "sepalWidth": 2.3, "petalLength": 1.3, "petalWidth": 0.3, "species": "setosa"},
      {"sepalLength": 4.4, "sepalWidth": 3.2, "petalLength": 1.3, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 5.0, "sepalWidth": 3.5, "petalLength": 1.6, "petalWidth": 0.6, "species": "setosa"},
      {"sepalLength": 5.1, "sepalWidth": 3.8, "petalLength": 1.9, "petalWidth": 0.4, "species": "setosa"},
      {"sepalLength": 4.8, "sepalWidth": 3.0, "petalLength": 1.4, "petalWidth": 0.3, "species": "setosa"},
      {"sepalLength": 5.1, "sepalWidth": 3.8, "petalLength": 1.6, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 4.6, "sepalWidth": 3.2, "petalLength": 1.4, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 5.3, "sepalWidth": 3.7, "petalLength": 1.5, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 5.0, "sepalWidth": 3.3, "petalLength": 1.4, "petalWidth": 0.2, "species": "setosa"},
      {"sepalLength": 7.0, "sepalWidth": 3.2, "petalLength": 4.7, "petalWidth": 1.4, "species": "versicolor"},
      {"sepalLength": 6.4, "sepalWidth": 3.2, "petalLength": 4.5, "petalWidth": 1.5, "species": "versicolor"},
      {"sepalLength": 6.9, "sepalWidth": 3.1, "petalLength": 4.9, "petalWidth": 1.5, "species": "versicolor"},
      {"sepalLength": 5.5, "sepalWidth": 2.3, "petalLength": 4.0, "petalWidth": 1.3, "species": "versicolor"},
      {"sepalLength": 6.5, "sepalWidth": 2.8, "petalLength": 4.6, "petalWidth": 1.5, "species": "versicolor"},
      {"sepalLength": 5.7, "sepalWidth": 2.8, "petalLength": 4.5, "petalWidth": 1.3, "species": "versicolor"},
      {"sepalLength": 6.3, "sepalWidth": 3.3, "petalLength": 4.7, "petalWidth": 1.6, "species": "versicolor"},
      {"sepalLength": 4.9, "sepalWidth": 2.4, "petalLength": 3.3, "petalWidth": 1.0, "species": "versicolor"},
      {"sepalLength": 6.6, "sepalWidth": 2.9, "petalLength": 4.6, "petalWidth": 1.3, "species": "versicolor"},
      {"sepalLength": 5.2, "sepalWidth": 2.7, "petalLength": 3.9, "petalWidth": 1.4, "species": "versicolor"},
      {"sepalLength": 5.0, "sepalWidth": 2.0, "petalLength": 3.5, "petalWidth": 1.0, "species": "versicolor"},
      {"sepalLength": 5.9, "sepalWidth": 3.0, "petalLength": 4.2, "petalWidth": 1.5, "species": "versicolor"},
      {"sepalLength": 6.0, "sepalWidth": 2.2, "petalLength": 4.0, "petalWidth": 1.0, "species": "versicolor"},
      {"sepalLength": 6.1, "sepalWidth": 2.9, "petalLength": 4.7, "petalWidth": 1.4, "species": "versicolor"},
      {"sepalLength": 5.6, "sepalWidth": 2.9, "petalLength": 3.6, "petalWidth": 1.3, "species": "versicolor"},
      {"sepalLength": 6.7, "sepalWidth": 3.1, "petalLength": 4.4, "petalWidth": 1.4, "species": "versicolor"},
      {"sepalLength": 5.6, "sepalWidth": 3.0, "petalLength": 4.5, "petalWidth": 1.5, "species": "versicolor"},
      {"sepalLength": 5.8, "sepalWidth": 2.7, "petalLength": 4.1, "petalWidth": 1.0, "species": "versicolor"},
      {"sepalLength": 6.2, "sepalWidth": 2.2, "petalLength": 4.5, "petalWidth": 1.5, "species": "versicolor"},
      {"sepalLength": 5.6, "sepalWidth": 2.5, "petalLength": 3.9, "petalWidth": 1.1, "species": "versicolor"},
      {"sepalLength": 5.9, "sepalWidth": 3.2, "petalLength": 4.8, "petalWidth": 1.8, "species": "versicolor"},
      {"sepalLength": 6.1, "sepalWidth": 2.8, "petalLength": 4.0, "petalWidth": 1.3, "species": "versicolor"},
      {"sepalLength": 6.3, "sepalWidth": 2.5, "petalLength": 4.9, "petalWidth": 1.5, "species": "versicolor"},
      {"sepalLength": 6.1, "sepalWidth": 2.8, "petalLength": 4.7, "petalWidth": 1.2, "species": "versicolor"},
      {"sepalLength": 6.4, "sepalWidth": 2.9, "petalLength": 4.3, "petalWidth": 1.3, "species": "versicolor"},
      {"sepalLength": 6.6, "sepalWidth": 3.0, "petalLength": 4.4, "petalWidth": 1.4, "species": "versicolor"},
      {"sepalLength": 6.8, "sepalWidth": 2.8, "petalLength": 4.8, "petalWidth": 1.4, "species": "versicolor"},
      {"sepalLength": 6.7, "sepalWidth": 3.0, "petalLength": 5.0, "petalWidth": 1.7, "species": "versicolor"},
      {"sepalLength": 6.0, "sepalWidth": 2.9, "petalLength": 4.5, "petalWidth": 1.5, "species": "versicolor"},
      {"sepalLength": 5.7, "sepalWidth": 2.6, "petalLength": 3.5, "petalWidth": 1.0, "species": "versicolor"},
      {"sepalLength": 5.5, "sepalWidth": 2.4, "petalLength": 3.8, "petalWidth": 1.1, "species": "versicolor"},
      {"sepalLength": 5.5, "sepalWidth": 2.4, "petalLength": 3.7, "petalWidth": 1.0, "species": "versicolor"},
      {"sepalLength": 5.8, "sepalWidth": 2.7, "petalLength": 3.9, "petalWidth": 1.2, "species": "versicolor"},
      {"sepalLength": 6.0, "sepalWidth": 2.7, "petalLength": 5.1, "petalWidth": 1.6, "species": "versicolor"},
      {"sepalLength": 5.4, "sepalWidth": 3.0, "petalLength": 4.5, "petalWidth": 1.5, "species": "versicolor"},
      {"sepalLength": 6.0, "sepalWidth": 3.4, "petalLength": 4.5, "petalWidth": 1.6, "species": "versicolor"},
      {"sepalLength": 6.7, "sepalWidth": 3.1, "petalLength": 4.7, "petalWidth": 1.5, "species": "versicolor"},
      {"sepalLength": 6.3, "sepalWidth": 2.3, "petalLength": 4.4, "petalWidth": 1.3, "species": "versicolor"},
      {"sepalLength": 5.6, "sepalWidth": 3.0, "petalLength": 4.1, "petalWidth": 1.3, "species": "versicolor"},
      {"sepalLength": 5.5, "sepalWidth": 2.5, "petalLength": 4.0, "petalWidth": 1.3, "species": "versicolor"},
      {"sepalLength": 5.5, "sepalWidth": 2.6, "petalLength": 4.4, "petalWidth": 1.2, "species": "versicolor"},
      {"sepalLength": 6.1, "sepalWidth": 3.0, "petalLength": 4.6, "petalWidth": 1.4, "species": "versicolor"},
      {"sepalLength": 5.8, "sepalWidth": 2.6, "petalLength": 4.0, "petalWidth": 1.2, "species": "versicolor"},
      {"sepalLength": 5.0, "sepalWidth": 2.3, "petalLength": 3.3, "petalWidth": 1.0, "species": "versicolor"},
      {"sepalLength": 5.6, "sepalWidth": 2.7, "petalLength": 4.2, "petalWidth": 1.3, "species": "versicolor"},
      {"sepalLength": 5.7, "sepalWidth": 3.0, "petalLength": 4.2, "petalWidth": 1.2, "species": "versicolor"},
      {"sepalLength": 5.7, "sepalWidth": 2.9, "petalLength": 4.2, "petalWidth": 1.3, "species": "versicolor"},
      {"sepalLength": 6.2, "sepalWidth": 2.9, "petalLength": 4.3, "petalWidth": 1.3, "species": "versicolor"},
      {"sepalLength": 5.1, "sepalWidth": 2.5, "petalLength": 3.0, "petalWidth": 1.1, "species": "versicolor"},
      {"sepalLength": 5.7, "sepalWidth": 2.8, "petalLength": 4.1, "petalWidth": 1.3, "species": "versicolor"},
      {"sepalLength": 6.3, "sepalWidth": 3.3, "petalLength": 6.0, "petalWidth": 2.5, "species": "virginica"},
      {"sepalLength": 5.8, "sepalWidth": 2.7, "petalLength": 5.1, "petalWidth": 1.9, "species": "virginica"},
      {"sepalLength": 7.1, "sepalWidth": 3.0, "petalLength": 5.9, "petalWidth": 2.1, "species": "virginica"},
      {"sepalLength": 6.3, "sepalWidth": 2.9, "petalLength": 5.6, "petalWidth": 1.8, "species": "virginica"},
      {"sepalLength": 6.5, "sepalWidth": 3.0, "petalLength": 5.8, "petalWidth": 2.2, "species": "virginica"},
      {"sepalLength": 7.6, "sepalWidth": 3.0, "petalLength": 6.6, "petalWidth": 2.1, "species": "virginica"},
      {"sepalLength": 4.9, "sepalWidth": 2.5, "petalLength": 4.5, "petalWidth": 1.7, "species": "virginica"},
      {"sepalLength": 7.3, "sepalWidth": 2.9, "petalLength": 6.3, "petalWidth": 1.8, "species": "virginica"},
      {"sepalLength": 6.7, "sepalWidth": 2.5, "petalLength": 5.8, "petalWidth": 1.8, "species": "virginica"},
      {"sepalLength": 7.2, "sepalWidth": 3.6, "petalLength": 6.1, "petalWidth": 2.5, "species": "virginica"},
      {"sepalLength": 6.5, "sepalWidth": 3.2, "petalLength": 5.1, "petalWidth": 2.0, "species": "virginica"},
      {"sepalLength": 6.4, "sepalWidth": 2.7, "petalLength": 5.3, "petalWidth": 1.9, "species": "virginica"},
      {"sepalLength": 6.8, "sepalWidth": 3.0, "petalLength": 5.5, "petalWidth": 2.1, "species": "virginica"},
      {"sepalLength": 5.7, "sepalWidth": 2.5, "petalLength": 5.0, "petalWidth": 2.0, "species": "virginica"},
      {"sepalLength": 5.8, "sepalWidth": 2.8, "petalLength": 5.1, "petalWidth": 2.4, "species": "virginica"},
      {"sepalLength": 6.4, "sepalWidth": 3.2, "petalLength": 5.3, "petalWidth": 2.3, "species": "virginica"},
      {"sepalLength": 6.5, "sepalWidth": 3.0, "petalLength": 5.5, "petalWidth": 1.8, "species": "virginica"},
      {"sepalLength": 7.7, "sepalWidth": 3.8, "petalLength": 6.7, "petalWidth": 2.2, "species": "virginica"},
      {"sepalLength": 7.7, "sepalWidth": 2.6, "petalLength": 6.9, "petalWidth": 2.3, "species": "virginica"},
      {"sepalLength": 6.0, "sepalWidth": 2.2, "petalLength": 5.0, "petalWidth": 1.5, "species": "virginica"},
      {"sepalLength": 6.9, "sepalWidth": 3.2, "petalLength": 5.7, "petalWidth": 2.3, "species": "virginica"},
      {"sepalLength": 5.6, "sepalWidth": 2.8, "petalLength": 4.9, "petalWidth": 2.0, "species": "virginica"},
      {"sepalLength": 7.7, "sepalWidth": 2.8, "petalLength": 6.7, "petalWidth": 2.0, "species": "virginica"},
      {"sepalLength": 6.3, "sepalWidth": 2.7, "petalLength": 4.9, "petalWidth": 1.8, "species": "virginica"},
      {"sepalLength": 6.7, "sepalWidth": 3.3, "petalLength": 5.7, "petalWidth": 2.1, "species": "virginica"},
      {"sepalLength": 7.2, "sepalWidth": 3.2, "petalLength": 6.0, "petalWidth": 1.8, "species": "virginica"},
      {"sepalLength": 6.2, "sepalWidth": 2.8, "petalLength": 4.8, "petalWidth": 1.8, "species": "virginica"},
      {"sepalLength": 6.1, "sepalWidth": 3.0, "petalLength": 4.9, "petalWidth": 1.8, "species": "virginica"},
      {"sepalLength": 6.4, "sepalWidth": 2.8, "petalLength": 5.6, "petalWidth": 2.1, "species": "virginica"},
      {"sepalLength": 7.2, "sepalWidth": 3.0, "petalLength": 5.8, "petalWidth": 1.6, "species": "virginica"},
      {"sepalLength": 7.4, "sepalWidth": 2.8, "petalLength": 6.1, "petalWidth": 1.9, "species": "virginica"},
      {"sepalLength": 7.9, "sepalWidth": 3.8, "petalLength": 6.4, "petalWidth": 2.0, "species": "virginica"},
      {"sepalLength": 6.4, "sepalWidth": 2.8, "petalLength": 5.6, "petalWidth": 2.2, "species": "virginica"},
      {"sepalLength": 6.3, "sepalWidth": 2.8, "petalLength": 5.1, "petalWidth": 1.5, "species": "virginica"},
      {"sepalLength": 6.1, "sepalWidth": 2.6, "petalLength": 5.6, "petalWidth": 1.4, "species": "virginica"},
      {"sepalLength": 7.7, "sepalWidth": 3.0, "petalLength": 6.1, "petalWidth": 2.3, "species": "virginica"},
      {"sepalLength": 6.3, "sepalWidth": 3.4, "petalLength": 5.6, "petalWidth": 2.4, "species": "virginica"},
      {"sepalLength": 6.4, "sepalWidth": 3.1, "petalLength": 5.5, "petalWidth": 1.8, "species": "virginica"},
      {"sepalLength": 6.0, "sepalWidth": 3.0, "petalLength": 4.8, "petalWidth": 1.8, "species": "virginica"},
      {"sepalLength": 6.9, "sepalWidth": 3.1, "petalLength": 5.4, "petalWidth": 2.1, "species": "virginica"},
      {"sepalLength": 6.7, "sepalWidth": 3.1, "petalLength": 5.6, "petalWidth": 2.4, "species": "virginica"},
      {"sepalLength": 6.9, "sepalWidth": 3.1, "petalLength": 5.1, "petalWidth": 2.3, "species": "virginica"},
      {"sepalLength": 5.8, "sepalWidth": 2.7, "petalLength": 5.1, "petalWidth": 1.9, "species": "virginica"},
      {"sepalLength": 6.8, "sepalWidth": 3.2, "petalLength": 5.9, "petalWidth": 2.3, "species": "virginica"},
      {"sepalLength": 6.7, "sepalWidth": 3.3, "petalLength": 5.7, "petalWidth": 2.5, "species": "virginica"},
      {"sepalLength": 6.7, "sepalWidth": 3.0, "petalLength": 5.2, "petalWidth": 2.3, "species": "virginica"},
      {"sepalLength": 6.3, "sepalWidth": 2.5, "petalLength": 5.0, "petalWidth": 1.9, "species": "virginica"},
      {"sepalLength": 6.5, "sepalWidth": 3.0, "petalLength": 5.2, "petalWidth": 2.0, "species": "virginica"},
      {"sepalLength": 6.2, "sepalWidth": 3.4, "petalLength": 5.4, "petalWidth": 2.3, "species": "virginica"},
      {"sepalLength": 5.9, "sepalWidth": 3.0, "petalLength": 5.1, "petalWidth": 1.8, "species": "virginica"}
    ]
  }
  ];

  json["data"] = data;
  //end data-----

  json["scales"] = [
    {
      "name": "x",
      "nice": true,
      "range": "width",
      "domain": {"data": "iris", "field": "data.sepalWidth"}
    },
    {
      "name": "y",
      "nice": true,
      "range": "height",
      "domain": {"data": "iris", "field": "data.petalLength"}
    },
    {
      "name": "c",
      "type": "ordinal",
      "domain": {"data": "iris", "field": "data.species"},
      "range": ["#800", "#080", "#008"]
    }
  ];

  json["axes"] =  [
    {"type": "x", "scale": "x", "offset": 5, "ticks": 5, "title": "Sepal Width"},
    {"type": "y", "scale": "y", "offset": 5, "ticks": 5, "title": "Petal Length"}
  ];

  json["legends"] =  [
    {
      "fill": "c",
      "title": "Species",
      "offset": 0,
      "properties": {
        "symbols": {
          "fillOpacity": {"value": 0.5},
          "stroke": {"value": "transparent"}
        }
      }
    }
  ];

  json["marks"] = [
    {
      "type": "symbol",
      "from": {"data": "iris"},
      "properties": {
        "enter": {
          "x": {"scale": "x", "field": "data.sepalWidth"},
          "y": {"scale": "y", "field": "data.petalLength"},
          "fill": {"scale": "c", "field": "data.species"},
          "fillOpacity": {"value": 0.5}
        },
        "update": {
          "size": {"value": 100},
          "stroke": {"value": "transparent"}
        },
        "hover": {
          "size": {"value": 300},
          "stroke": {"value": "white"}
        }
      }
    }
  ];
  return json;

}

var view_count = 1; //How many views are there?

/*Button on front end links to this javascript function that is full of vega tests
Tests for scatter plots, bar charts, and testing back end.
*/
function testVega(){
  removeViews();
  view_count = 1;
  spec1 = createJSON();
  vg.parse.spec(spec1, function(chart) {
  self.view1 = chart({
    el: "#view1",
    hover: false
  }).update();
});
  //view_count++;
  addChart();
  testScatter();
  testBackEnd();
}

//Creates the Blank Chart displayed center of front end, to help users visualize
//what they are inputting
function blankChart(){
  spec1 = createBlank();
  vg.parse.spec(spec1, function(chart) {
  self.view1 = chart({
    el: "#blankChart",
    hover: false
  }).update();
});
}



function changeScaleBlankChart(xmin,xmax,ymin,ymax){
	  spec1 = createBlank(xmin,xmax,ymin,ymax);
	  vg.parse.spec(spec1, function(chart) {
	  self.view1 = chart({
	    el: "#blankChart",
	    hover: false
	  }).update();
	});
}

//Function that creates a scatter plot definied in the createScatter() function
function testScatter(){
  //Legend has an issue, it's clipped
  spec1 = createScatter();
  vg.parse.spec(spec1, function(chart) {
  self.view1 = chart({
    el: "#view"+view_count,
    hover: false
  }).update();
});
}


//Adds a new div to store a new chart, in order
function addChart(){
  var lastView = $("#view"+view_count++);
  var view = "view"+view_count;
  lastView.after(" <div id="+ view+ " class='mod'></div>")
}

//Tests Back end functionality by using a set query
function testBackEnd(){
  getDataNow();
}

//Adds a graph to the div specified by view count
function addGraph(data){
  spec1 = createLineGraph(data);
  var view = view_count;
  vg.parse.spec(spec1, function(chart) {
    chart({
      el: "#view"+view,
      hover: false
    }).update();
  });
}

function processesBackEndData(data) {
  //Take json data
  //alert(data.length);
  //alert(data);
  //alert(data);
  console.log("vega:"+data)
  
  
  
  for(index = 1; index <= data; index++){
	$.get('/getiterativedata', JSON.stringify(index), genCharts,'json')
      .fail(function() {
        console.log("Failed")
        alert('Request failed: /getiterativedata');
      });
  }
  //Scroll to graphs automatically! (After stuff loads)
  
}


function genCharts(output)
{
	//var jsonData = JSON.parse(output);
	//console.log(ouput.length);
	//console.dir(output);
	var outputs = output[0];
	
	var segments = outputs.segment;
	var estimates = outputs.estimate;
	
	//console.dir(segments);
	//console.dir(estimates);
	console.log(segments.length);
	//for (var i = 0; i < segments.length; i++) {
	//	var counter = segments[i];
	//	console.log(counter.average);
	//}
	//console.log(segment);
	console.log(estimates.length);
	//for (var i = 0; i < estimates.length; i++) {
	//	var counter = estimates[i];
	//	console.log(counter.average);
	//}
	
	
}
//Removes all the views and graphs we created
function removeViews(){
  //When view_count = 1, we have 1 view. 1 based, not 0 based.
  for(var i = 2; i <=view_count; i++){
    //But we need to keep first view alive
    $("#view"+i).remove();
  }
  $("#view1").empty();
  view_count = 1; //view1 will always be empty as the "anchor" div. (Unless with the test version)
}

//backend_data is a dictionary, has "x_type", "y_type", "data"
function createBarGraph(backend_data){
  var json = {};
  json["width"] = width;
  json["height"] = 200;
  //json["viewport"]=[width+50, height+50]
  var padding= {"top": padding_top, "left": padding_left, "bottom": padding_bottom, "right": padding_right};
  //json["padding"] = padding;

  //data----
  var data = [];
  var table_data = {"name":"table"};
  var table_data_values = [];

  var backend_values = backend_data["data"]; //array for backend_values

  for(i=0; i<backend_values.length; i++){
    table_data_values.push({"x":backend_values[i][0], "y": backend_values[i][1]});
  }
  table_data["values"]= table_data_values;
  data.push(table_data);
  json["data"] = data;
  //end data-----

  json["scales"] = [
    {
      "name": "x",
      "type": "ordinal",
      "range": "width",
      "domain": {"data": "table", "field": "data.x"}
    },
    {
      "name": "y",
      "range": "height",
      "nice": true,
      "domain": {"data": "table", "field": "data.y"}
    }
  ];

  json["axes"] =  [
    {"type": "x", "scale": "x", "title": backend_data["x_type"]},
    {"type": "y", "scale": "y", "title": backend_data["y_type"]}
  ];

  json["marks"] =  [
    {
      "type": "rect",
      "from": {"data": "table"},
      "properties": {
        "enter": {
          "x": {"scale": "x", "field": "data.x"},
          "width": {"scale": "x", "band": true, "offset": -1},
          "y": {"scale": "y", "field": "data.y"},
          "y2": {"scale": "y", "value": 0}
        },
        "update": {
          "fill": {"value": "steelblue"}
        },
        "hover": {
          "fill": {"value": "red"}
        }
      }
    }
  ];
  return json;
}

function createLineGraph(backend_data){
  var json = {};
  json["width"] = width;
  json["height"] = 200;
  //json["viewport"]=[width+50, height+50]
  var padding= {"top": padding_top, "left": padding_left, "bottom": padding_bottom, "right": padding_right};
  //json["padding"] = padding;

  //data----
  var data = [];
  var table_data = {"name":"table"};
  var table_data_values = [];

  var backend_values = backend_data["data"]; //array for backend_values

  for(i=0; i<backend_values.length; i++){
    table_data_values.push({"x":backend_values[i][0], "y": backend_values[i][1]});
  }
  table_data["values"]= table_data_values;
  data.push(table_data);
  json["data"] = data;
  //end data-----
 
  json["scales"] = [
    {
      "name": "x",
      "type": "ordinal",
      "range": "width",
      "domain": {"data": "table", "field": "data.x"},
      "domainMin": xmin,
      "domainMax": xmax,
      "zero": false
    },
    {
      "name": "y",
      "range": "height",
      "nice": true,
      "domain": {"data": "table", "field": "data.y"},
      "domainMin": ymin,
      "domainMax": ymax,
       "zero": false
    },
    {
      "name": "color", "type": "ordinal", "range": "category10"
    }
  ];

  json["axes"] =  [
    {"type": "x", "scale": "x", "title": backend_data["x_type"], "titleOffset":60},
    {"type": "y", "scale": "y", "title": backend_data["y_type"], "titleOffset":60}
  ];

  json["marks"] =  [
    {
      "type": "line",
      "from": {"data": "table"},
      "properties": {
        "enter": {
          "x": {"scale": "x", "field": "data.x"},
          "y": {"scale": "y", "field": "data.y"},
          "stroke": {"scale": "color"},
          "strokeWidth": {"value": 2}
        }
      }
    }
  ];
  return json;
}