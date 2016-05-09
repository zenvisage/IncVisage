var iteration = 1;
var maxIteration = 100;
var snapshotW = 350, snapshotH = 220;
var segmentArr = [];
var estimateArr = [];
var pauseSegmentArr = [];
var pauseEstimateArr = [];
var segmentForwardChain = [];
var segmentBackwardChain = [];
var estimateChain = [];
var ychange=0;
var xchange=0;


var margin = {top: 30, right: 40, bottom: 30, left: 40},
    width = 800 - margin.left - margin.right,
    height = 400 - margin.top - margin.bottom;

var xMin = 0;
var xMax = 0;
var yMin = 0;
var yMax = 0;

function drawAxes(x, y, viz){
  var xAxis = d3.svg.axis().scale(x);
  var yAxis = d3.svg.axis().scale(y).ticks(5).orient("left");

  graph.selectAll("g.x.axis").remove();
  graph.selectAll("g.y.axis").remove();
  
  viz.append("g")            // Add the X Axis
    .attr("class", "x axis")
    .attr("transform", "translate(0," + height + ")")
    .call(xAxis);

  viz.append("g")
      .attr("class", "y axis")
      .call(yAxis);

   viz.append("text")
      .attr("class", "x label")
      .attr("text-anchor", "end")
      .attr("x", width*0.55)
      .attr("y", height + 40)
      .style("font-weight", 400)
      .style("font-size", "14px")
      .text(dimension1);

  viz.append("text")
    .attr("class", "y label")
    .attr("text-anchor", "end")
    .attr("y", 6)
    .attr("dy", ".75em")
    .attr("transform", "rotate(-90)")
    .style("font-weight", 400)
    .style("font-size", "14px")
    .text(dimension2);
}


var basic = d3.select(".visualization").append("svg")
    .style("overflow", 'visible')
    .style("margin-bottom", '20px')
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
  
 var graph =  basic.append("g")
    .attr("transform", 
          "translate(" + margin.left + "," + margin.top + ")");


var snapshot = d3.select(".snapshot").append("svg")
    .attr("width", snapshotW )
    .attr("height", snapshotH)
    .append("g");

var div = d3.select("body").append("div")   
    .attr("class", "tooltip")               
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

    //console.log(jsondata);
    //console.dir(jsondata);
        

        //xStart= data[0].start;
        //xEnd = data[data.length -1].end;

        setupSeekbar(xMin, xMax);

        var x = d3.scale.linear().range([0, width])
                                .domain([xMin, xMax]);
        var y = d3.scale.linear().range([height, 0]).domain([yMin,yMax]);


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

      console.log(segmentArr_);
      console.log(estimateArr_);
      

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
            .transition().duration(100).style("opacity", (itr+10)/(maxIteration +10))
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
            var timestamp = x.invert(currentPos-2),
                index = bisect(hoverSegmentArr, timestamp);
                dataPoint = hoverSegmentArr[index-1]; //find the datapoint with that xValue
                console.log(index,dataPoint);
                if (timestamp > dataPoint.b && timestamp < dataPoint.e){
                  valueY = y(dataPoint.a);                  
                }
                else{
                  startDatum = hoverSegmentArr[index - 1],
                  endDatum = hoverSegmentArr[index],
                  interpolate = d3.interpolateNumber(startDatum.a, endDatum.a),
                  range = x(endDatum.b) - x(startDatum.b),
                  distance = currentPos - x(startDatum.b);
                  closest = distance / range;
                  if (closest > 1) {closest = 1}
                  valueY = y(interpolate(closest));
                 
                }
                console.log("timestamp: " + timestamp + " valueY: " + valueY+ " index: " + index); 
                marker.attr('cx', currentPos);
                marker.attr('cy', valueY);


                div.transition()
              .duration(500)
              .style("opacity", 0.9);
//(d3.event.pageX) - 50
//(valueY + 10)
              div.html("x: " + d3.round(timestamp) + "<br/>"  + "y: " + d3.format(".2f")(dataPoint.a))  
                .style("left", 500 + "px")     
                .style("top", 130 + "px"); 
            }





          });

                            
        
        xStart= xMin;
        xEnd = xMax;

        var x = d3.scale.linear().range([0, width])
                                .domain([xStart, xEnd]);
        var y = d3.scale.linear().range([height, 0]).domain([yMin, yMax]);

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
            .style("opacity", 0.5); 
    //if(itr==1)
    //{
      
    //  for (var i = 0; i < estData.length; i++) {
    //    var counter = estData[i];
    //    console.log(counter.g,counter.a);
    //  }
    //}

     // });

     //   console.log(itr+ ' outside of error');
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

    document.getElementById("doneBtn").disabled = true; 
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

  var xColumns = data.dimension1;
  var yColumns = data.dimension2;
  var measColumns = data.measure;

  var xList = xColumns.split(",");
  var yList = yColumns.split(",");
  var measList = measColumns.split(",");

  var select1 = document.getElementById('xAxis');
  //select1.parentNode.removeChild("temp1");
  $("#xAxis option[value='temp1']").remove();
  opt = document.createElement('option');
    opt.text = "--select an option--";
    opt.value = xList[i];
    //opt.innerHTML = xList[i];
    select1.appendChild(opt);

console.log(select1);
for (var i = 0; i<xList.length; i++){
    opt = document.createElement('option');
    opt.text = xList[i];
    opt.value = xList[i];
    //opt.innerHTML = xList[i];
    select1.appendChild(opt);

    console.log(xList[i]);
}
$("#xAxis").selectpicker('refresh');
document.getElementById("xAxis").options[0].disabled = true;
$("#xAxis").selectpicker('refresh');

var select2 = document.getElementById('yAxis');
$("#yAxis option[value='temp2']").remove();
  opt = document.createElement('option');
    opt.text = "--select an option--";
    opt.value = yList[i];
    //opt.innerHTML = xList[i];
    select2.appendChild(opt);

console.log(select2);
for (var i = 0; i<yList.length; i++){
    var opt = document.createElement('option');
    opt.text = yList[i];
    opt.value = yList[i];
    //opt.innerHTML = yList[i];
    select2.appendChild(opt);
    console.log(yList[i]);
}
 $("#yAxis").selectpicker('refresh');
 document.getElementById("yAxis").options[0].disabled = true;
$("#yAxis").selectpicker('refresh');

var select3 = document.getElementById('measType');
$("#measType option[value='temp3']").remove();
  opt = document.createElement('option');
    opt.text = "--select an option--";
    opt.value = measList[i];
    //opt.innerHTML = xList[i];
    select3.appendChild(opt);

console.log(select3);
for (var i = 0; i<measList.length; i++){
    var opt = document.createElement('option');
    opt.text = measList[i];
    opt.value = measList[i];
    //opt.innerHTML = yList[i];
    select3.appendChild(opt);
    console.log(measList[i]);
}
 $("#measType").selectpicker('refresh');
 document.getElementById("measType").options[0].disabled = true;
$("#measType").selectpicker('refresh');

  
}

function loadAxesColumns(){
  console.log("Load Axes");
  document.getElementById("title").innerHTML = measure     
  var info = {"dimension1": dimension1,"dimension2":dimension2, "measure": measure, "sampling":samplingType};
  $.get('/getaxesdata',JSON.stringify(info),processAxesData,'json')
      .fail(function() {
        console.log("Failed")
        alert('Request failed: /Axesdata');
      });
  
}

function processAxesData(data){

    console.log(data);
  xMin = data.xMin;
  xMax = data.xMax;//;
  yMin = data.yMin;
  yMax = data.yMax;

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
          document.getElementById("forward").style.opacity = "1";
          document.getElementById("back").style.opacity = "0.1";

resetOnLoad();
// var xMin, xMax, yMin, yMax;
  //display axes ticks
  //play enabled
  //render(iteration, width, height, graph);
}


function resetOnLoad()
{
  
  console.log("in reset");
  //console.log(segmentArr);
  //console.log(estimateArr);
  
  
  d3.selectAll("path.line").remove();
  graph.selectAll("circle.dot").remove();
  snapshot.selectAll("circle.dot").remove();
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
  resetFlag = false;
  backFlag=false;
  resetFlag = false;
  
  
  
  console.log("Reset: ", iteration);
  
  if(document.getElementById("pause")!=null) 
        { 
          document.getElementById("pause").setAttribute("class","glyphicon glyphicon-play"); 
          document.getElementById("pause").setAttribute("id","play"); 
          document.getElementById("play").style.opacity = "1";
        }
        else if (document.getElementById("play")!=null) 
          document.getElementById("play").style.opacity = "1";
  document.getElementById("forward").style.opacity = "1";
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

  render(iteration, width, height, graph,[],[]);

  graph.selectAll("text.x.label").remove();
  graph.selectAll("text.y.label").remove();
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


   