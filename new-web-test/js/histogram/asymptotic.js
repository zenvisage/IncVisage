var iteration = 1;
var maxIteration = 100;
var snapshotW = 350, snapshotH = 220;
var segmentArr = [];
var estimateArr = [];
var pauseSegmentArr = [];
var pauseEstimateArr = [];
var segmentForwardChain = [];
var segmentBackwardChain = [];
var segmentChain = [];
var estimateChain = [];
var xTickList = [];
var yTickList = [];
var ychange=0;
var xchange=0;
var  predList;
var filterArray;
var margin = {top: 30, right: 40, bottom: 30, left: 40},
    width = 800 - margin.left - margin.right,
    height = 400 - margin.top - margin.bottom;

var xMin = 0;
var xMax = 0;
var yMin = [];
var yMax = [];

var doneFlag=false;
var imageId=0;

var series1 = [];
var series2 = [];

var monthsArray = ["JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"];
var samples = 50000;
function indexArray(param) {
    var a = [], i;
    for (i=0; i<param.length; i+=1) {
        a[param[i].index] = param[i].value;
    }
    return a;
}

var monthIndex = indexArray([
    { index: "jan", value : 0 },
    { index: "feb", value : 1 },
    { index: "mar", value : 2 },
    { index: "apr", value : 3 },
    { index: "may", value : 4 },
    { index: "jun", value : 5 },
    { index: "jul", value : 6 },
    { index: "aug", value : 7 },
    { index: "sep", value : 8 },
    { index: "oct", value : 9 },
    { index: "nov", value : 10 },
    { index: "dec", value : 11 }    
]);
	
var dayOfWeekArray = [ "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"];
var days = [0, 31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335, 366];

function dayText(day){
  for (x =0; x < days.length; x++){
    if (day <= days[x+1]){
      return monthsArray[x] + " " +(day - days[x]);
    }
  }
}

function drawAxes(x, y, viz){
	if(String(dimension)==String("Month") || String(dimension)==String("Day Of Week"))
		var xAxis = d3.svg.axis();//.scale(x);
	else
		var xAxis = d3.svg.axis().scale(x);
  var yAxis = d3.svg.axis().scale(y).ticks(5).orient("left");

  graph.selectAll("g.x.axis").remove();
  graph.selectAll("g.y.axis").remove();
  
  graph.selectAll("text.x.label").remove();
  graph.selectAll("text.y.label").remove();
  
  viz.append("g")            // Add the X Axis
    .attr("class", "x axis")
    .attr("transform", "translate(0," + height + ")")
    .call(xAxis);

  viz.append("g")
      .attr("class", "y axis")
      .call(yAxis);
	if(String(dimension)==String("Month") || String(dimension)==String("Day Of Week"))
	{
		var distance = width/xTickList.length;
		for(var i=0;i<xTickList.length;i++)
		{
				  viz.append("text")
				  .attr("class", "x label")
				  .attr("text-anchor", "middle")
				  .attr("x", i*distance) //+distance/2
				  .attr("y", heightWin + 15)
				  //.attr("transform", "translate("+((i+1)*gridItemWidth/2)+",0)")
				  .style("font-weight", 400)
				  .style("font-size", "10px")
				  .text(xTickList[i]);
				  
		}
	}
  

   viz.append("text")
      .attr("class", "x label")
      .attr("text-anchor", "middle")
      .attr("x", width*0.5)
      .attr("y", height + 40)
      .style("font-weight", 400)
      .style("font-size", "14px")
      .text(dimension);

  viz.append("text")
    .attr("class", "y label")
    .attr("text-anchor", "middle")
    .attr("x", -height*0.5)
    .attr("y", 6)
    .attr("dy", ".75em")
    .attr("transform", "translate("+(-50)+",0)rotate(-90)")
    .style("font-weight", 400)
    .style("font-size", "14px")
    .text(measure);
}

var errorBar = d3.select("#errorRect").append("svg")
    .style("overflow", 'visible')
    .style("margin-bottom", '20px')
    .attr("width", 100)
    .attr("height", 30);

var basic = d3.select(".visualization").append("svg")
    .style("overflow", 'visible')
    .style("margin-bottom", '20px')
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
	.attr("version", 1.1)
    .attr("xmlns", "http://www.w3.org/2000/svg");
  
 var graph =  basic.append("g")
    .attr("transform", 
          "translate(" + margin.left + "," + margin.top + ")");


var snapshot = d3.select(".snapshot").append("svg")
    .attr("width", snapshotW )
    .attr("height", snapshotH)
    .append("g");

var div = d3.select("body").append("div")   
    .attr("class", "tooltip")
    .attr("id","tip")	
    .style("opacity", 0);
  
var bisect = d3.bisector(function(datum) {
    return datum.b;
  }).right; 

var marker = graph.append('circle')
    .attr('r', 7)
    .style('display', 'none')
    .style('fill', '#FFFFFF')
    .style('pointer-events', 'none')
    .style('stroke', '#FB5050')
    .style('stroke-width', '3px');
  
function render(itr, width, height, viz, segmentArr_,estimateArr_){

    //var file = "data/" + itr +".csv";
    //var file = "100/" + itr +".json";

//d3.json(file, function(error, json) {
//        if (error){
//          itr = maxIteration; //update to the most recent
//          var seekbar = document.getElementById('seekbar');
//          seekbar.value = itr;
//          render(itr, width, height, viz);
//          return itr;
//          return false;
//        } 
    //console.log("render:",estimateArr_);




    if(itr >= xMin)
      document.getElementById("currentIter").innerHTML = "Iteration " + (itr-xMin+1);

    else
      document.getElementById("currentIter").innerHTML = "Iteration ";
        if(viz == snapshot){
          document.getElementById("snapshotNo").innerHTML = "Iteration " + (itr-xMin+1);
        }



        setupSeekbar(xMin, xMax);

        var x = d3.scale.linear().range([0, width])
                                .domain([xMin, xMax]);
        var y = d3.scale.linear().range([height, 0]).domain([yMin[itr],yMax[itr]]);


        //if (itr == xMin){
      if(viz != snapshot){
        drawAxes(x, y, viz);
      }
        //}

        var trend = [];
		  for(var i=0, len=segmentArr_.length; i<len; i++){
      //console.log(segmentArr_[i].b,segmentArr_[i].e,segmentArr_[i].a);
          trend.push({
              start: segmentArr_[i].b,
              average: segmentArr_[i].a
          });
          trend.push({
              start: segmentArr_[i].e,
              average: segmentArr_[i].a
          });
		  
		  
		  
        }                   


		        var valueline = d3.svg.line()
            .x(function(d) { return x(d.start); })
            .y(function(d) { return y(d.average); });

        if ((itr == xMin) || (viz == snapshot)){
          viz.append("path")        // Add the valueline path.
            .attr("d", valueline(trend))
            .attr("id", "trend")
      .attr("class", "line")
            .attr("transform", null)
              .transition()
            .attr('stroke', 'black')
            .attr('stroke-width', 2)
            .attr('fill', 'none');
        }

        viz.selectAll("path#trend")        // Add the valueline path.
          .attr("d", valueline(trend))
          .attr("transform", null)
            .transition().duration(100).style("opacity", (itr+50)/(maxIteration +10))
          .attr('stroke', 'black')
          .attr('stroke-width', 2)
          .attr('fill', 'none');

          var hoverSegmentArr = [];

          for(var i=0;i<segmentArr_.length;i++)
            hoverSegmentArr[i] = segmentArr_[i];

      basic.on('mouseover', function(){
        marker.style('display', 'inherit');})
        .on('mouseout', function() {
            marker.style('display', 'none');})
        .on('mousemove', function() {
          var mouse = d3.mouse(this);
          var currentPos = mouse[0] - margin.left;

          if (currentPos > 0 && currentPos < width){

            var timestamp = x.invert(currentPos);
            var xVal = d3.round(timestamp);
            tooltipIndex = bisect(hoverSegmentArr, xVal);
            tooltipDatapoint = hoverSegmentArr[tooltipIndex-1];

            //index = bisect(hoverSegmentArr, timestamp);
            //dataPoint = hoverSegmentArr[index-1];

            valueY = y(tooltipDatapoint.a);  
            /*if (timestamp >= dataPoint.b && timestamp <= dataPoint.e){ //if the value falls within the range
              valueY = y(dataPoint.a);             
            }
            else{
              startDatum = hoverSegmentArr[index - 1],
              endDatum = hoverSegmentArr[index],
              interpolate = d3.interpolateNumber(startDatum.a, endDatum.a),
              range = x(endDatum.b) - x(startDatum.e),
              distance = currentPos - x(startDatum.e);

              closest = distance / range;
              if (closest > 1) {closest = 1}
              valueY = y(interpolate(closest));
            }*/
               // console.log("timestamp: " + timestamp + " valueY: " + valueY+ " index: " + index); 
                marker.attr('cx', x(xVal));
                marker.attr('cy', valueY);

            div.transition()
              .duration(500)
              .style("opacity", 0.9);

            if (dimension.toString() == "Day"){
              xVal = dayText(xVal);
            }
            div.html("x: " + xVal + "<br/>"  + "y: " + d3.format(".2f")(tooltipDatapoint.a) )  
              .style("left", "90%")     
              .style("top", 130 + "px"); 


            }





          });

                            
        
  /*      xStart= xMin;
        xEnd = xMax;

        var x = d3.scale.linear().range([0, width])
                                .domain([xStart, xEnd]);
        var y = d3.scale.linear().range([height, 0]).domain([yMin[itr], yMax[itr]]);

       // console.log(estimateArr_);
        var xMap = function(d) { return x(d.g);}, // value -> display
            yMap = function(d) {  return y(d.a);};

        viz.selectAll(".dot").remove();

        var circle = viz.selectAll(".dot")
          .data(estimateArr_);

          circle.enter().append("circle")
            .attr("class", "dot")
            .style("fill", "orange")
            .attr("r", 2)
            .attr("cx", xMap)
            .attr("cy", yMap)
            .style("opacity", 0.5); */
	 doneFlag=true;
	 
	
	 
	 
    return itr;
         
}


function renderSubmit(){
  console.log("Load data");

  if((xchange+ychange)>1)
  {

    $('#xAxis')
    .find('option')
    .remove()
    .end()
    .append('<option value="temp1">----</option>')
    .val('temp1');

    $('#yAxis')
    .find('option')
    .remove()
    .end()
    .append('<option value="temp2">----</option>')
    .val('temp2');

    //document.getElementById("loadbtn").disabled = true;
    document.getElementById("doneBtn").disabled = true;
    if(document.getElementById("pause")!=null) 
    { 
      document.getElementById("pause").setAttribute("class","glyphicon glyphicon-play"); 
      document.getElementById("pause").setAttribute("id","play"); 
      document.getElementById("play").style.opacity = "0.1";
    }
    else if (document.getElementById("play")!=null) 
       document.getElementById("play").style.opacity = "0.1"; 
    document.getElementById("forward").style.opacity = "0.1";
    document.getElementById("back").style.opacity = "0.1"; 
	
	$("#xAxis").empty();
	var select1 = document.getElementById('xAxis');
	  opt = document.createElement('option');
	  opt.text = "----";
	  opt.value = 'tmp1';
	  select1.appendChild(opt);
	  $("#xAxis").selectpicker('refresh');
	
	$("#yAxis").empty();
	var select2 = document.getElementById('yAxis');
	  opt = document.createElement('option');
	  opt.text = "----";
	  opt.value = 'tmp2';
	  select2.appendChild(opt);
	  $("#yAxis").selectpicker('refresh');
	  
	$("#filterattr").empty();
	var select3 = document.getElementById('filterattr');
    opt = document.createElement('option');
    opt.text = "Attribute";
    opt.value = "----";
    select3.appendChild(opt); 

    $("#filterattr").selectpicker('refresh');
 
  }

  xchange=0;
  ychange=0;


  var info = { "dataset": dataset};//"dataset": "sensordata", "dimension": "Day", "measure": "Temperature"
  $.get('/getloaddata',JSON.stringify(info),processMetaData,'json')
      .fail(function() {
        console.log("Failed")
        alert('Request failed: /LoadData');
      });
  
}

function processMetaData(data){
  console.log(data);

  var xColumns = data.dimension;
  var yColumns = data.measure;
  var predColumns = data.predicate;
  
  var xList = xColumns.split(",");
  var yList = yColumns.split(",");
  predList = predColumns.split(",");
predList.sort();
  //console.log(predList);
  
  var select1 = document.getElementById('xAxis');
  //select1.parentNode.removeChild("temp1");
  //$("#xAxis option[value='temp1']").remove();
  $("#xAxis").empty();
  opt = document.createElement('option');
    opt.text = "--select an option--";
    opt.value = xList[i];
    //opt.innerHTML = xList[i];
    select1.appendChild(opt);

//console.log(select1);
for (var i = 0; i<xList.length; i++){
    opt = document.createElement('option');
    opt.text = xList[i];
    opt.value = xList[i];
    //opt.innerHTML = xList[i];
    select1.appendChild(opt);

   // console.log(xList[i]);
}
$("#xAxis").selectpicker('refresh');
document.getElementById("xAxis").options[0].disabled = true;
$("#xAxis").selectpicker('refresh');

var select2 = document.getElementById('yAxis');

//$("#yAxis option[value='temp2']").remove();
$("#yAxis").empty();
  opt = document.createElement('option');
    opt.text = "--select an option--";
    opt.value = xList[i];
    //opt.innerHTML = xList[i];
    select2.appendChild(opt);

//console.log(select2);
for (var i = 0; i<yList.length; i++){
    var opt = document.createElement('option');
    opt.text = yList[i];
    opt.value = yList[i];
    //opt.innerHTML = yList[i];
    select2.appendChild(opt);
    //console.log(yList[i]);

}
 $("#yAxis").selectpicker('refresh');
 document.getElementById("yAxis").options[0].disabled = true;
$("#yAxis").selectpicker('refresh');


var select3 = document.getElementById('filterattr');
$("#filterattr").empty();
 opt = document.createElement('option');
    opt.text = "Attribute";
    opt.value = "----";
    select3.appendChild(opt); 
for (var i = 0; i<predList.length; i++){
  
    var opt = document.createElement('option');
    opt.text = predList[i];
    opt.value = predList[i];
    //opt.innerHTML = yList[i];
    select3.appendChild(opt);
   // console.log(predList[i]);
 
}

 $("#filterattr").selectpicker('refresh');
 document.getElementById("filterattr").options[0].disabled = true;
$("#filterattr").selectpicker('refresh');


  
}

function loadAxesColumns(){
  console.log("Load Axes");
  currentDataset=tmpDataset; 
  
  document.getElementById("title").innerHTML = measure + " vs " + dimension;
  //should indicate what we're displaying


  var info = {"dimension": dimension, "measure": measure, "predicate": filter, "predicatevalue":filterValue, "sampling":samples,"epsilon": 8, "alpha": 1};
  $.get('/getaxesdata',JSON.stringify(info),processAxesData,'json')
      .fail(function() {
        console.log("Failed")
        alert('Request failed: /Axesdata');
      });
  
}

function loadPredicateColumns(){
  console.log("Load Predicate");
    
  var info = {"predicate": filter};
  $.get('/getpreddata',JSON.stringify(info),processPredicateData,'json')
      .fail(function() {
        console.log("Failed")
        alert('Request failed: /predData');
      });
  
}
function sortNumber(a,b) {
    return a - b;
}
function processPredicateData(data){
	//console.log(data);
	var filterValues = data.predicate;
	var filterList = filterValues.split(",");
	
	if(!isNaN(filterList[0]))  // value is a number
	{
		//convert and then sort
		for(var i=0;i<filterList.length;i++)
		{
			filterList[i] = Number(filterList[i]);			
		}
		
		filterList.sort(sortNumber);
		
	}
	else
		filterList.sort();
	
	var select3 = document.getElementById('filterValue');
//$("#yAxis option[value='temp2']").remove();
$("#filterValue").empty();
 opt = document.createElement('option');
    opt.text = "Value";
    opt.value = "Value";
    select3.appendChild(opt); 
for (var i = 0; i<filterList.length; i++){
	
    var opt = document.createElement('option');
    opt.text = filterList[i];
    opt.value = filterList[i];
    //opt.innerHTML = yList[i];
    select3.appendChild(opt);
    //console.log(filterList[i]);
	
}

 $("#filterValue").selectpicker('refresh');
 document.getElementById("filterValue").options[0].disabled = true;
$("#filterValue").selectpicker('refresh');
	
}

function processAxesData(data){

  //console.log(data);
	
 	
  xMin = data.xMin;
  xMax = data.xMax;//;
  //yMin = data.yMin;
  //yMax = data.yMax;
 
	  xTickList.length = 0;

	  for(var i=xMin,j=0;i<=xMax;i++,j++)
	  {
		  if(String(dimension)==String("Month"))
			{		
				xTickList[j] = monthsArray[i];
			}
			else if(String(dimension)==String("Day Of Week"))
			{		
				xTickList[j] = dayOfWeekArray[i];
			}
			else 
				xTickList[j] = i;
		  
	  }


  document.getElementById("playerButtons").className = document.getElementById("playerButtons").className.replace
      ( /(?:^|\s)disabled(?!\S)/g , 'abled' );

if(document.getElementById("pause")!=null) 
          { 
            document.getElementById("pause").setAttribute("class","glyphicon glyphicon-play"); 
            document.getElementById("pause").setAttribute("id","play"); 
            document.getElementById("play").style.opacity = "1";
          }
          else
            document.getElementById("play").style.opacity = "1";
          document.getElementById("forward").style.opacity = "0.1";
          document.getElementById("back").style.opacity = "0.1";
		 //document.getElementById("loadbtn").disabled = true;

//console.log("doneflag",doneFlag);
/*if(doneFlag==true)
{
	imageId++;
	
	//working version
	var html = basic.node().parentNode.innerHTML.trim(); 
  console.log(html);
  	var canvas = document.createElement('canvas');
	canvas.setAttribute('width', width + margin.left + margin.right);
	canvas.setAttribute('height', height + margin.top + margin.bottom);
	//canvas.height = width + margin.left + margin.right; 
	//canvas.width = height + margin.top + margin.bottom;
	canvg(canvas, html);
	var dataURL = canvas.toDataURL('image/png');
	//console.log(dataURL);
	var info = {"id": imageId, "value": dataURL};
	console.log(info);
	$.get('/saveimagedata',JSON.stringify(info),'json')
      .fail(function() {
        console.log("Failed")
        alert('Request failed: /saveimage');
      });
	  
	  $(".canvas").remove();
	  console.log("canvas removed");

	
}*/


resetOnLoad();
// var xMin, xMax, yMin, yMax;
  //display axes ticks
  //play enabled
  //render(iteration, width, height, graph);
}


function resetOnLoad()
{
  
  //console.log("in reset");
  //console.log(segmentArr);
  //console.log(estimateArr);
  $.get('/resetdata', JSON.stringify(iteration),'json')
      .fail(function() {
        
        console.log("Failed")
        alert('Request failed: /reset');
    
      });
  var element = document.getElementById('tip');
   element.parentNode.removeChild(element);
  errorBar.selectAll("rect").remove();
  errorBar.selectAll("text").remove();
  graph.selectAll("path.line").remove();
  graph.selectAll("circle.dot").remove();
  snapshot.selectAll("circle.dot").remove();
  snapshot.selectAll("path.line").remove();
  //graph.selectAll(".circle.dot").data([]).exit().remove();
  //d3.selectAll("svg > *").remove();
  
  
  seekbarMax = xMin-1;
  seekbarIter = xMin-1;
  maxIteration = xMax;
  pauseIter = xMin-1;
  playIter = xMin-1;
  forwardFlag=false;
  orwardIter = xMin-1;
  backwardIter = xMin-1;
  playNoviz=false;
  pauseFlag=false;
  playFlag=false;
  simplayFlag=false;
  resetFlag = false;
  backFlag=false;
  resetFlag = false;
  
  
  
  //console.log("Reset: ", iteration);
  
  if(document.getElementById("pause")!=null) 
        { 
          document.getElementById("pause").setAttribute("class","glyphicon glyphicon-play"); 
          document.getElementById("pause").setAttribute("id","play"); 
          document.getElementById("play").style.opacity = "1";
        }
        else if (document.getElementById("play")!=null) 
          document.getElementById("play").style.opacity = "1";
  document.getElementById("forward").style.opacity = "0.1";
    document.getElementById("back").style.opacity = "0.1";
    //document.getElementById("play").style.opacity = "1";
    //document.getElementById("pause").style.opacity = "1";
  
  iteration = xMin-1;
  seekbar.value = iteration;
  segmentArr.length=0;
  estimateArr.length=0;
  
  graph.selectAll("g.x.axis").remove();
  graph.selectAll("g.y.axis").remove();
  initGraph();
  div = d3.select("body").append("div")   
    .attr("class", "tooltip")
    .attr("id","tip")	
    .style("opacity", 0); 
  render(iteration, width, height, graph,[],[]);

  doneFlag=false;
  
  graph.selectAll("text.x.label").remove();
  graph.selectAll("text.y.label").remove();
  
  graph.append("text")
      .attr("class", "x label")
      .attr("text-anchor", "middle")
      .attr("x", width*0.50)
      .attr("y", height + 40)
      .style("font-weight", 400)
      .style("font-size", "14px")
      .text(dimension);

  graph.append("text")
    .attr("class", "y label")
    .attr("text-anchor", "middle")
    .attr("x", -height*0.5)
    .attr("y", 6)
    .attr("dy", ".75em")
    .attr("transform", "translate("+(-50)+",0)rotate(-90)")
    .style("font-weight", 400)
    .style("font-size", "14px")
    .text(measure);
  
}

//initial setup
initGraph();

function initGraph(){
var x = d3.scale.linear().range([0, width])
                                .domain([0, 1]);
var y = d3.scale.linear().range([height, 0]).domain([0, 1]);


  var xAxis = d3.svg.axis().scale(x).ticks(0);
  var yAxis = d3.svg.axis().scale(y).ticks(0).orient("left");

  graph.append("g")            // Add the X Axis
    .attr("class", "x axis")
    .attr("transform", "translate(0," + height + ")")
    .call(xAxis);

  graph.append("g")
      .attr("class", "y axis")
      .call(yAxis);
}


   
