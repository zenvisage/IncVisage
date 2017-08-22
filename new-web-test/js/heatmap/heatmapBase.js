var iteration = 1;
var maxIteration = 1;
var snapshotW = 350, snapshotH = 220;
var segmentArr = [];
var estimateArr = [];
var pauseSegmentArr = [];
var pauseEstimateArr = [];
var segmentForwardChain = [];
var segmentChain = [];
var segmentBackwardChain = [];
var estimateChain = [];
var xTickList = [];
var yTickList = [];
var ychange=0;
var xchange=0;
var xList;
var yList;
var measList;
var predList;
var margin = {top: 30, right: 40, bottom: 30, left: 40},
    widthWin = 800 - margin.left - margin.right,
    heightWin = 400 - margin.top - margin.bottom;
	
var xMin = 0;
var xMax = 0;
var yMin = 0;
var yMax = 0;
var avgMax=[];
var avgMin=[];
var currAvgMax=0;
var currAvgMin=0;
var validCells;
var samples = 25000;
var iterationLow=0;

var monthsArray = ["JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"];
var days = [0, 31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335, 366];
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

var basic = d3.select("#chart").append("svg")
    .style("overflow", 'visible')
    .style("margin-bottom", '20px')
    .attr("width", widthWin + margin.left + margin.right)
    .attr("height", heightWin + margin.top + margin.bottom);
  
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

//var colors = ["#ffffd9","#edf8b1","#c7e9b4","#7fcdbb","#41b6c4","#1d91c0","#225ea8","#253494","#081d58"];
var grayBlue = d3.hcl(92.425, 7.555, 254.549);
var darkBlue = d3.hcl(20.93, 39.892, 287.421);
console.log("maxITr:",maxIteration);

//var heatmapColor = d3.scale.linear()
//  .domain(d3.range(0, 1, 1.0 / (colors.length - 1)))
//  .range(colors);			   
	
var basicLegend = d3.select("#colorLegend").append("svg")
	.style("overflow", 'visible')
    .style("margin-bottom", '20px')
    .attr("width", widthWin + margin.left + margin.right).attr("height", 30);
    


	
function drawAxes(x, y, gridItemHeight, gridItemWidth, viz){
	
	//console.log(xTickList.length,yTickList.length);
 // var xAxis = d3.svg.axis().scale(x).ticks(0);//ticks(xTickList.length);
 // var yAxis = d3.svg.axis().scale(y).ticks(0);//ticks(yTickList.length).orient("left");

  graph.selectAll("g.x.axis").remove();
  graph.selectAll("g.y.axis").remove();
  
  graph.selectAll("text.x.label").remove();
  graph.selectAll("text.y.label").remove();
  
  /*viz.append("g")            // Add the X Axis
    .attr("class", "x axis")
    .attr("transform", "translate(0," + heightWin + ")")
    .call(xAxis);

  viz.append("g")
      .attr("class", "y axis")
      .call(yAxis);*/
	  
   for(var i=0;i<xTickList.length;i++)
	{
			  viz.append("text")
			  .attr("class", "x label")
			  .attr("text-anchor", "middle")
			  .attr("x", i*gridItemWidth+gridItemWidth/2)
			  .attr("y", heightWin + 15)
			  //.attr("transform", "translate("+((i+1)*gridItemWidth/2)+",0)")
			  .style("font-weight", 400)
			  .style("font-size", "10px")
			  .text(xTickList[i]);
			  
	}
   viz.append("text")
      .attr("class", "x label")
      .attr("text-anchor", "middle")
      .attr("x", widthWin*0.50)
      .attr("y", heightWin + 40)
      .style("font-weight", 400)
      .style("font-size", "14px")
      .text(dimension1);
  
  for(var i=0;i<yTickList.length;i++)
	{
			  viz.append("text")
			  .attr("class", "y label")
			  .attr("text-anchor", "middle")
			  .attr("x", -12)//widthWin+12)
			  .attr("y", heightWin - (i*gridItemHeight+gridItemHeight/4))
			  //.attr("transform", "rotate(-90)")
			  .style("font-weight", 400)
			  .style("font-size", "10px")
			  .text(yTickList[i]);
			  
	}
  
  viz.append("text")
    .attr("class", "y label")
    .attr("text-anchor", "middle")
    .attr("x", -heightWin*0.50)
    .attr("y", 6)
    .attr("dy", ".75em")
    .attr("transform", "translate("+(-50)+",0)rotate(-90)")//"translate("+(widthWin+40)+",0)rotate(-90)")
    .style("font-weight", 400)
    .style("font-size", "14px")
    .text(dimension2);
	
  
	
}

function render(itr, width, height, viz, segmentArr_){


    if(itr >= iterationLow)
      document.getElementById("currentIter").innerHTML = "Iteration " + (itr-iterationLow+1);

    else
      document.getElementById("currentIter").innerHTML = "Iteration ";
        if(viz == snapshot){
          document.getElementById("snapshotNo").innerHTML = "Iteration " + (itr-iterationLow+1);
        }
       

        

							//.attr("transform", "translate(0," + 100) + ")");
     
       
		
	//viz.selectAll("rect").remove();

		var colors = d3.scale.linear()
    .domain([avgMin[itr],avgMax[itr]])
    .range(["lightblue", "darkblue"])
    .interpolate(d3.interpolateHcl);
		var xPos=0;
	  var yPos=0;
	  var rectW= 0;
	  var rectH=0;
	  var gridItemWidth = width / (xMax-xMin+1);
		var gridItemHeight = height / (yMax-yMin+1);
		var legendElementWidth = 14;//((widthWin + margin.left + margin.right)/2)/colors.length;
    var legendElementHeight = 28;

  console.log("render",segmentArr_);
    for(var i=0;i<segmentArr_.length;i++)
	  {
		  xPos = (segmentArr_[i].xb-xMin)*gridItemWidth;
		  yPos = (yMax-segmentArr_[i].ye)*gridItemHeight;//segmentArr_[i].yb-yMin
		  rectW = (segmentArr_[i].xe-segmentArr_[i].xb+1)*gridItemWidth;
		  rectH = (segmentArr_[i].ye-segmentArr_[i].yb+1)*gridItemHeight;
		  //console.log("x:",segmentArr_[i].xb-xMin,"y:",yMax-segmentArr_[i].ye,"w:",segmentArr_[i].xe-segmentArr_[i].xb+1,"h:",segmentArr_[i].ye-segmentArr_[i].yb+1,"gridHeight:",gridItemHeight);
		//  console.log("x:",xPos,"y:",yPos,"w:",rectW,"h:",rectH,"gridHeight:",gridItemHeight);
		  
		  var rectangle = viz.append("rect")
                            .attr("x", xPos)
                           .attr("y", yPos)
                           .attr("width", rectW)
						   .attr("height", rectH)
						   .style("fill", colors(segmentArr_[i].a)) //d3.interpolateHcl(paleYellow, darkBlue)(segmentArr_[i].a))//function(d) {     return heatmapColor(segmentArr_[i].a)})
						   .style("stroke", '#555');
		  rectangle.append("title").text
									(
										function(d)
                    {
                      var number=segmentArr_[i].a; 
                      var dimLabel1=dimension1+": ", dimLabel2=dimension2+": ";
                        //console.log(segmentArr_[i]);
                        if(segmentArr_[i].xb!= segmentArr_[i].xe)
                        {
                            dimLabel1 = dimLabel1+xTickList[segmentArr_[i].xb-xMin]+"-"+xTickList[segmentArr_[i].xe-xMin]+"\n"; 
                            
                        }

                        else 
                        {
                            dimLabel1 = dimLabel1+xTickList[segmentArr_[i].xb-xMin]+"\n";
                            
                        }

                         if(segmentArr_[i].yb != segmentArr_[i].ye)
                        {
                           dimLabel2 = dimLabel2+yTickList[segmentArr_[i].yb-yMin]+"-"+yTickList[segmentArr_[i].ye-yMin]+"\n";

                        }

                        else 
                        {
                            dimLabel2 = dimLabel2+yTickList[segmentArr_[i].yb-yMin]+"\n";

                        }
                      if(number==0)
                      {
                        

                        return dimLabel1+dimLabel2+measure+": "+number;
                      }
                        
                      else
                      {
                        return dimLabel1+dimLabel2+measure+": "+number.toFixed(2);
                      }
                        
                    }
									);
	  }	
		
	  //console.log("label print");
	  if (itr == iterationLow)
      {
        console.log("in itr: ", itr,iterationLow);
		  var x = d3.scale.linear().range([0, width])
                                .domain([xMin, xMax]);
          var y = d3.scale.linear().range([height, 0]).domain([yMin,yMax]);

          drawAxes(x, y, gridItemHeight, gridItemWidth, viz);
		
		
		
      }
	  
	  
	  
	  //console.log("Legend Print");
	  if(itr>=iterationLow)	  
	 {
		basicLegend.selectAll("rect").remove(); 
		basicLegend.selectAll("text").remove();
		var numLegends = 50;
    console.log(avgMin[itr],avgMax[itr]);
    for(var j=0;j<numLegends;j++)
      {
        
        var colorVal = avgMin[itr] + ((1.0*j)/(numLegends-1))*(avgMax[itr]- avgMin[itr]);
        //console.log("color:", colorVal);
        var rectangle = basicLegend.append("rect")
                .attr("x", legendElementWidth * j)
                 .attr("y", 0)
                 .attr("width", legendElementWidth)
                 .attr("height", legendElementHeight)
                 .style("fill", colors(colorVal)) //d3.interpolateHcl(paleYellow, darkBlue)(segmentArr_[i].a))//function(d) {     return heatmapColor(segmentArr_[i].a)})
                 .style("stroke", '#555')
                 .style("stroke-width", 0);;
        //rectangle.append("title").text(function(d){return segmentArr_[i].a});
      }
		
		basicLegend.append("text")
		.attr("class", "mono")
            .text(avgMin[itr].toFixed(2))
            .attr("x", -10)
            .attr("y", legendElementHeight+10);
    
		basicLegend.append("text")
		.attr("class", "mono")
            .text(avgMax[itr].toFixed(2))
            .attr("x", legendElementWidth*j)
            .attr("y", legendElementHeight+10);
		/*var	legend = basicLegend.selectAll(".legend")
              .data([0].concat(heatmapColor.quantiles()), function(d) {return d; })
              .enter().append("g")
              .attr("class", "legend");

          legend.append("rect")
            .attr("x", function(d, i) { return legendElementWidth * i; })
            .attr("y", 10)
            .attr("width", legendElementWidth)
            .attr("height", gridItemHeight)
            .style("fill", function(d, i) { return colors[i]; });
			
			legend.append("text")
            .attr("class", "mono")
            .text(function(d) { return "≥ " + Math.round(d); })
            .attr("x", function(d, i) { return legendElementWidth * i; })
            .attr("y", gridItemHeight+20);*/


      /*  for(var i=0;i<validCells.length;i++)
       {
          xPos = (validCells[i].x-xMin)*gridItemWidth;
          yPos = (yMax-validCells[i].y)*gridItemHeight;//segmentArr_[i].yb-yMin
          rectW = gridItemWidth;
          rectH = gridItemHeight;
          //console.log("x:",segmentArr_[i].xb-xMin,"y:",yMax-segmentArr_[i].ye,"w:",segmentArr_[i].xe-segmentArr_[i].xb+1,"h:",segmentArr_[i].ye-segmentArr_[i].yb+1,"gridHeight:",gridItemHeight);
          //console.log("x:",xPos,"y:",yPos,"w:",rectW,"h:",rectH,"gridHeight:",gridItemHeight);
          
          var rectangle = viz.append("rect")
                    .attr("x", xPos)
                   .attr("y", yPos)
                   .attr("width", rectW)
             .attr("height", rectH)
             .style("fill", '#E5E7E5') //d3.interpolateHcl(paleYellow, darkBlue)(segmentArr_[i].a))//function(d) {     return heatmapColor(segmentArr_[i].a)})
             .style("stroke", '#555');
          //rectangle.append("title").text();


       }*/
		 
	 }


	 

			
      //console.log(segmentArr_);
      //console.log(estimateArr_);
      



    return itr;
         
}


function renderSubmit(){
  console.log("Load data");

  if((xchange+ychange+measchange)>2)
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
	  
    $("#measType").empty();
	var select3 = document.getElementById('measType');
	  opt = document.createElement('option');
	  opt.text = "----";
	  opt.value = 'tmp3';
	  select3.appendChild(opt);
	  $("#measType").selectpicker('refresh');
	  
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

  var xColumns = data.dimension1;
  var yColumns = data.dimension2;
  var measColumns = data.measure;
  var predColumns = data.predicate;
	predList = predColumns.split(",");
	predList.sort();
  xList = xColumns.split(",");
  yList = yColumns.split(",");
  measList = measColumns.split(",");

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
    opt.value = yList[i];
    //opt.innerHTML = xList[i];
    select2.appendChild(opt);

//console.log(select2);
for (var i = 0; i<yList.length; i++){
    var opt = document.createElement('option');
    opt.text = yList[i];
    opt.value = yList[i];
    //opt.innerHTML = yList[i];
    select2.appendChild(opt);
   // console.log(yList[i]);
}
 $("#yAxis").selectpicker('refresh');
 document.getElementById("yAxis").options[0].disabled = true;
$("#yAxis").selectpicker('refresh');

var select3 = document.getElementById('measType');
//$("#measType option[value='temp3']").remove();
$("#measType").empty();
  opt = document.createElement('option');
    opt.text = "--select an option--";
    opt.value = measList[i];
    //opt.innerHTML = xList[i];
    select3.appendChild(opt);

//console.log(select3);
for (var i = 0; i<measList.length; i++){
    var opt = document.createElement('option');
    opt.text = measList[i];
    opt.value = measList[i];
    //opt.innerHTML = yList[i];
    select3.appendChild(opt);
    //console.log(measList[i]);
}
 $("#measType").selectpicker('refresh');
 document.getElementById("measType").options[0].disabled = true;
$("#measType").selectpicker('refresh');

var select4 = document.getElementById('filterattr');
$("#filterattr").empty();
 opt = document.createElement('option');
    opt.text = "Attribute";
    opt.value = "----";
    select4.appendChild(opt); 
for (var i = 0; i<predList.length; i++){
  
    var opt = document.createElement('option');
    opt.text = predList[i];
    opt.value = predList[i];
    //opt.innerHTML = yList[i];
    select4.appendChild(opt);
   // console.log(predList[i]);
 
}

 $("#filterattr").selectpicker('refresh');
 document.getElementById("filterattr").options[0].disabled = true;
$("#filterattr").selectpicker('refresh');


  
}

function loadAxesColumns(){
  console.log("Load Axes");
  currentDataset=tmpDataset;  
  document.getElementById("title").innerHTML = measure;
  var info = {"dimension1": dimension1,"dimension2":dimension2, "measure": measure, "predicate": filter, "predicatevalue":filterValue,"sampling":samples};
  $.get('/getaxesdata',JSON.stringify(info),processAxesData,'json')
      .fail(function() {
        console.log("Failed")
        alert('Request failed: /Axesdata');
      });
  
}

function processAxesData(data){

   // console.log(data);
  xMin = data.xMin;
  xMax = data.xMax;//;
  yMin = data.yMin;
  yMax = data.yMax;
  xTickList.length = 0;
  yTickList.length = 0;
  
  for(var i=xMin,j=0;i<=xMax;i++,j++)
  {
	  if(String(dimension1)==String("Month"))
	 {
		xTickList[j] = monthsArray[i-1];
	 }
	 else if(String(dimension1)==String("Day Of Week"))
	 {
		xTickList[j] = dayOfWeekArray[i-1];
	}
			else 
				xTickList[j] = i;
	  
  }
  
  for(var i=yMin,j=0;i<=yMax;i++,j++)
  {
	  if(String(dimension2)==String("Month"))
	 {
		yTickList[j] = monthsArray[i-1];
	 }
	 else if(String(dimension2)==String("Day Of Week"))
	 {
		yTickList[j] = dayOfWeekArray[i-1];
	}
			else 
				yTickList[j] = i;
	  
  }
  
  if(iteration==1)
	 basicLegend=basicLegend.attr("height", heightWin/(yMax-yMin+1)).append("g");
          
  else
  {
	  basicLegend.selectAll("g").remove();
	  basicLegend.attr("height", heightWin/(yMax-yMin+1)).append("g");
  }

	
  //basicLegend=basicLegend.append("title").text("Color Legend");
  maxIteration = (xMax-xMin+1)*(yMax-yMin+1);
  iterationLow = 0;
  setupSeekbar(iterationLow,maxIteration);

  startX = 0;
  startY = 0;
    
  
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
resetOnLoad();
// var xMin, xMax, yMin, yMax;
  //display axes ticks
  //play enabled
  //render(iteration, width, height, graph);
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
	console.log(data);
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

function resetOnLoad()
{
  
  console.log("in reset on load");
  //console.log(segmentArr);
  //console.log(estimateArr);
  $.get('/resetdata', JSON.stringify(iteration),'json')
      .fail(function() {
        
        console.log("Failed")
        alert('Request failed: /reset');
    
      });
  
  //d3.selectAll("rect").remove();
  graph.selectAll("rect").remove();
  snapshot.selectAll("rect").remove();
  //console.log(basicLegend);
  basicLegend.selectAll("rect").remove();
  basicLegend.selectAll("text").remove();
  //graph.selectAll(".circle.dot").data([]).exit().remove();
  //d3.selectAll("svg > *").remove();
  iterationLow = 1;
  seekbarMax = iterationLow-1;
  seekbarIter = iterationLow-1;
  maxIteration = (xMax-xMin+1)*(yMax-yMin+1);
  pauseIter = iterationLow-1;
  playIter = iterationLow-1;
  forwardFlag=false;
  orwardIter = iterationLow-1;
  backwardIter = iterationLow-1;
  playNoviz=false;
  pauseFlag=false;
  playFlag=false;
  simplayFlag=false;
  resetFlag = false;
  backFlag=false;
  //resetFlag = false;
  
  
  setupSeekbar(iterationLow,maxIteration);
  
  console.log("Reset: ", iteration);
  
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
  
  iteration = iterationLow-1;
  seekbar.value = iteration;
  segmentArr.length=0;
  estimateArr.length=0;
  
  graph.selectAll("g.x.axis").remove();
  graph.selectAll("g.y.axis").remove();
  initGraph();

  render(iteration, widthWin, heightWin, graph,[],[]);

  graph.selectAll("text.x.label").remove();
  graph.selectAll("text.y.label").remove();
  
   graph.append("text")
      .attr("class", "x label")
      .attr("text-anchor", "end")
      .attr("x", widthWin*0.55)
      .attr("y", heightWin + 40)
      .style("font-weight", 400)
      .style("font-size", "14px")
      .text(dimension1);
	  
  graph.append("text")
    .attr("class", "y label")
    .attr("text-anchor", "middle")
    .attr("x", -heightWin*0.50)
    .attr("y", 6)
    .attr("dy", ".75em")
    .attr("transform", "translate("+(-50)+",0)rotate(-90)") // "translate("+(widthWin+40)+",0)rotate(-90)"
    .style("font-weight", 400)
    .style("font-size", "14px")
    .text(dimension2);
}

initGraph();
function initGraph(){
var x = d3.scale.linear().range([0, widthWin]).domain([0, 1]);
var y = d3.scale.linear().range([heightWin, 0]).domain([0, 1]);



  var xAxis = d3.svg.axis().scale(x).ticks(0);
  var yAxis = d3.svg.axis().scale(y).ticks(0).orient("left");

  graph.append("g")            // Add the X Axis
    .attr("class", "x axis")
    .attr("transform", "translate(0," + heightWin + ")")
    .call(xAxis);

  graph.append("g")
      .attr("class", "y axis")
      .call(yAxis);
	  
	  
}

