var seekbar = document.getElementById('seekbar');
var pauseIter = xMin-1;
var playIter = xMin-1;
var forwardFlag=false;
var forwardIter = xMin-1;
var backwardIter = xMin-1;
var seekbarIter = xMin-1;
var playNoviz=false;
var pauseFlag=false;
var playFlag=false;
var simplayFlag=false;
var resetFlag = false;
var backFlag=false;
var seekbarMax=xMin-1;
var currAvgMax=0;
var currAvgMin=0;

seekbar.addEventListener("change", function() {
  seekbarIter = Math.ceil(seekbar.value);

  
  if(pauseFlag==true)
  {

  		console.log("-------IN seek pause------",seekbarMax);
  		
  		if(seekbarIter>seekbarMax)
  	 		seekbarIter=seekbarMax;
  		if(seekbarIter < xMax)
  			forwardFlag=true;
  		if(seekbarIter <= xMax && seekbarIter >xMin)
  			backFlag=true;

  		forwardIter=seekbarIter;
  		backwardIter = seekbarIter;
  		pauseIter = seekbarIter;
  		playIter = seekbarIter;

  		
			  
			  seekbar.value = seekbarIter;
			  render(seekbarIter, width, height, graph,segmentChain[seekbarIter],estimateChain[seekbarIter]);


			  if(seekbarIter < xMax)
  			  {
  			  		if(document.getElementById("pause")!=null) 
	   			{ 
	   				document.getElementById("pause").setAttribute("class","glyphicon glyphicon-play"); 
	   				document.getElementById("pause").setAttribute("id","play"); 
	   				document.getElementById("play").style.opacity = "1";
	   			}
	   			else
	   				document.getElementById("play").style.opacity = "1";
	   			document.getElementById("forward").style.opacity = "1";
	   			document.getElementById("back").style.opacity = "1";

  			  }
  		if(seekbarIter < xMax && seekbarIter >xMin)
  			{
  				if(document.getElementById("pause")!=null) 
	   			{ 
	   				document.getElementById("pause").setAttribute("class","glyphicon glyphicon-play"); 
	   				document.getElementById("pause").setAttribute("id","play"); 
	   				document.getElementById("play").style.opacity = "1";
	   			}
	   			else
	   				document.getElementById("play").style.opacity = "1";
	   			document.getElementById("forward").style.opacity = "1";
	   			document.getElementById("back").style.opacity = "1";
  			}
  			if(seekbarIter >= xMax)
  			{
  				if(document.getElementById("pause")!=null) 
	   			{ 
	   				document.getElementById("pause").setAttribute("class","glyphicon glyphicon-play"); 
	   				document.getElementById("pause").setAttribute("id","play"); 
	   				document.getElementById("play").style.opacity = "0.1";
	   			}
	   			else
	   				document.getElementById("play").style.opacity = "0.1";
	   			document.getElementById("forward").style.opacity = "0.1";
	   			document.getElementById("back").style.opacity = "1";
  			}
  			if(seekbarIter == xMin)
  			{
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
  			}
			  

  }

  if(playFlag==false && pauseFlag!=true)
  {

  		console.log("-------IN seek play------",seekbarMax);

  		if(seekbarIter>xMax)
  			seekbarIter=xMax;
  		if(seekbarIter<xMin)
  			seekbarIter=xMin;

  		if(seekbarIter>iteration)
  			seekbarIter = iteration;

  		if(seekbarIter < xMax)
  			forwardFlag=true;
  		if(seekbarIter <= xMax && seekbarIter >xMin)
  			backFlag=true;



  		forwardIter=seekbarIter;
  		backwardIter = seekbarIter;
  		pauseIter = seekbarIter;
  		playIter = seekbarIter;

  		seekbar.value = seekbarIter;
				  render(seekbarIter, width, height, graph,segmentChain[seekbarIter],estimateChain[seekbarIter]);

			  
			  if(document.getElementById("pause")!=null) 
	   			{ 
	   				document.getElementById("pause").setAttribute("class","glyphicon glyphicon-play"); 
	   				document.getElementById("pause").setAttribute("id","play"); 
	   				document.getElementById("play").style.opacity = "1";
	   			}
	   			else
	   				document.getElementById("play").style.opacity = "1";
	   			document.getElementById("forward").style.opacity = "1";
	   			document.getElementById("back").style.opacity = "1";

  }


if(seekbarIter==xMin)
  {

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
  }
  if(seekbarIter>=xMax)
  {

  	if(document.getElementById("pause")!=null) 
	   			{ 
	   				document.getElementById("pause").setAttribute("class","glyphicon glyphicon-play"); 
	   				document.getElementById("pause").setAttribute("id","play"); 
	   				document.getElementById("play").style.opacity = "0.1";
	   			}
	   			else
	   				document.getElementById("play").style.opacity = "0.1";
	   			document.getElementById("forward").style.opacity = "1";
	   			document.getElementById("back").style.opacity = "1";
  }
    

});

  function setupSeekbar(start, end){
    seekbar.min = start;
    seekbar.max = end;
  }


$('#forward').click(function() {
		
	   if (this.style.opacity == 1.0){
	  //console.log(segmentArr.length,segmentArr);
	  if(iteration<xMin)
	  {
		  //playNoviz=true;
		  console.log("forward init clcik");
		  forwardFlag=true;
		  //backFlag=false;
		  iteration++;
		  forwardIter=iteration;
		  console.log("forward init",forwardIter);
		  document.getElementById("play").style.opacity = "1";
		  document.getElementById("back").style.opacity = "1";
			
			
			
	}
	  
	  if(forwardFlag==true && playFlag==false && pauseFlag == true)
	  {
		   
		  //forwardFlag=true;
		  backFlag=true;
		  forwardIter++;
		  if(forwardIter>seekbarMax)
		  	seekbarMax = forwardIter;
		  console.log(forwardIter+ ' inside of forward '+iteration);
		  if(forwardIter <xMax)
		  {
			  			  
			  seekbar.value = forwardIter;
			  render(forwardIter, width, height, graph,segmentChain[forwardIter],estimateChain[forwardIter]);
			  
		  }
		  else if(forwardIter==xMax)
		  {
			  seekbar.value = forwardIter;
			  render(forwardIter, width, height, graph,segmentChain[forwardIter],estimateChain[forwardIter]);
			  
		  }

		  if(forwardIter>xMax)
		  {
		  	forwardIter=xMax;
		  }

		  backwardIter=forwardIter;
		  playIter = forwardIter;
		  pauseIter=forwardIter;
			
	  }
	  
	  if(iteration<xMax && backFlag==true)
	  {
		  forwardFlag=true;
		  
	  }
	  
	  		  
	   if(forwardIter >=xMax)
	   {
		 document.getElementById("forward").style.opacity = "0.1";
		 if(document.getElementById("pause")!=null) 
   			{ 
   				document.getElementById("pause").setAttribute("class","glyphicon glyphicon-play"); 
   				document.getElementById("pause").setAttribute("id","play"); 
   				document.getElementById("play").style.opacity = "0.1";
   			}
   			else if (document.getElementById("play")!=null) 
   				document.getElementById("play").style.opacity = "0.1";
		}

		if(forwardIter<xMax)
	   {

	   		if(document.getElementById("pause")!=null) 
   			{ 
   				document.getElementById("pause").setAttribute("class","glyphicon glyphicon-play"); 
   				document.getElementById("pause").setAttribute("id","play"); 
   				document.getElementById("play").style.opacity = "1";
   			}
   			else if (document.getElementById("play")!=null) 
   				document.getElementById("play").style.opacity = "1";
   			
	   }
			
	   if(forwardIter<iteration-1)
	   		document.getElementById("forward").style.opacity = "1";
	   	else
	   		document.getElementById("forward").style.opacity = "0.1";
		document.getElementById("back").style.opacity = "1";
	  
	
  console.log(forwardIter+ ' outside of forward '+iteration);

}
});



$('#back').click(function() {
	  if (this.style.opacity == 1.0){
	//console.log(segmentBackwardChain);
	if(iteration==xMin)
	{
		backFlag=false;
		//console.log("bacik",iteration,segmentArr);
		
		//console.log(iteration,segmentArr);
		document.getElementById("back").style.opacity = "0.1";
		document.getElementById("forward").style.opacity = "1";
		
		//iteration--;
		seekbar.value = iteration;
		render(iteration, width, height, graph,segmentChain[iteration],estimateArr);
		return;

	}
	if(playFlag==false && backFlag == true)
	{
		forwardFlag = true;
		backwardIter--;

		console.log("IN BACK",backwardIter);

		if(backwardIter >= xMin)
		{

			
	  		seekbar.value = backwardIter;
			render(backwardIter, width, height, graph,segmentChain[backwardIter],estimateChain[backwardIter]);
			



		}
		
		forwardIter=backwardIter;
		if(backwardIter<xMin)
			forwardIter=xMin;

		playIter=forwardIter;
		pauseIter=forwardIter;

		
	}
	
	

	if(backwardIter <=xMin)
	{
		 document.getElementById("back").style.opacity = "0.1";
		 
		 
		}
			
	   if(backwardIter<xMax)
	   {

	   		if(document.getElementById("pause")!=null) 
   			{ 
   				document.getElementById("pause").setAttribute("class","glyphicon glyphicon-play"); 
   				document.getElementById("pause").setAttribute("id","play"); 
   				document.getElementById("play").style.opacity = "1";
   			}
   			else if (document.getElementById("play")!=null) 
   				document.getElementById("play").style.opacity = "1";
   			
	   }
				
			//document.getElementById("pause").style.opacity = "0.1";
			document.getElementById("forward").style.opacity = "1";
		}
});



function play(){

	if(resetFlag==false)
	{
		document.getElementById('submitBtn').disabled = false;
		iteration++;

		if(iteration <= 1 && QA == true)
		{
			
			startTime = Date.now();
		}


		if(iteration<=xMax && playFlag==true)
		{
			//console.log("iteration:",iteration);
	
			$.get('/getiterativedata1', JSON.stringify(iteration), genCharts,'json')
			.fail(function() {
				
				console.log("Failed")
				alert('Request failed: /getiterativedata1');
		
			});

		}
		/*else if(playIter>=xMin && playIter<iteration)
		{

			simplayFlag = true;
			playIter--;
			simulatePlay();
		}*/


		else
		{
			//console.log(segmentArr);
			if(simplayFlag==false)
			{

				backFlag = true;
			//forwardFlag = false;
			if(pauseFlag==false)
				backwardIter = xMax;
			

			

			
			if(playFlag==true)
			{
				if(document.getElementById("pause")!=null)
				{
					
					document.getElementById("pause").style.opacity="0.1";
				}
				else if(document.getElementById("play")!=null)
				{
					
					document.getElementById("play").style.opacity="0.1";
				}

				document.getElementById("forward").style.opacity="0.1";
				document.getElementById("back").style.opacity="1";

			}



			playFlag = false;
			}
			
		}
	}
	else
		reset();
				
		
		
	


}


  
function genCharts(output)
{
	//var jsonData = JSON.parse(output);
	//console.log(ouput.length);
	//console.dir(output);
	
	if(resetFlag==false)	 
	{
		var segment = output.sl;
		var estimate = output.gl;
		var yRange = output.y;

		if(dataset == "sensordata")
		{
			console.log(iteration);
			yMin[iteration] = yRange.yMin;
			yMax[iteration] = yRange.yMax;

		}
		else
		{
			if(iteration <10)
			{
				currAvgMax = yRange.yMax;
				currAvgMin=yRange.yMin;
			}
			
			
			else if(iteration==15)
			{
				currAvgMax = yRange.yMax;
				currAvgMin=yRange.yMin;
			}
			else if(iteration==25)
			{
				currAvgMax = yRange.yMax;
				currAvgMin=yRange.yMin;
			}
			
			//console.log(currAvgMin,currAvgMax);
			yMin[iteration] = currAvgMin;
			yMax[iteration] = currAvgMax;

		}
		

		if(iteration==1)
		{
			console.log(output);
			xMax = output.xMax;
			xMin = output.xMin;
			iteration = xMin;
			console.log(output.xMax,output.xMin);
		}

		for(var i=0;i<segment.length;i++)
		{
			if(i==0)
			{
				segmentArr.splice(segment[i].n,1,segment[i].s);
			}
			else
				segmentArr.splice(segment[i].n,0,segment[i].s);
			
		}
		
		segmentChain[iteration] = [];
		for(var i=0;i<segmentArr.length;i++)
		{
			segmentChain[iteration][i] = segmentArr[i];
		}

		//console.log(segmentChain);
		for(var i=0;i<estimate.length;i++)
		{
			if(iteration==xMin)
				estimateArr[i] = estimate[i];
			else 
				estimateArr[estimate[i].g-xMin] = estimate[i];
			
		}
		
		estimateChain[iteration]=[];
		for(var i=0;i<estimateArr.length;i++)
		{
			estimateChain[iteration][i] = estimateArr[i];
		}
			
			//setTimeout(play, 100);
			 if(playNoviz==false && simplayFlag==false && resetFlag == false)
			 {
				render(iteration, width, height, graph,segmentArr,estimateArr);
				seekbar.value = iteration;
			 }
				
				if (iteration <= xMax && resetFlag == false){
					//iteration++;
					//console.log("checkplay",iteration,xMax);
					if(forwardFlag==false)
					{
						//ranOnce=true;
						setTimeout(play,getDelay(iteration));
					}
					else 
					{
						//ranOnce=true;
						playNoviz=true;
						setTimeout(play,getDelay(iteration));
					}
					
				}
	}
	else
		reset();		
	
	
}

function simulatePlay()
{
		 
	if(resetFlag==false)	 
	{
		
		 playIter++;
		 console.log("simplay",playIter,iteration);
		 if(playIter<iteration)
		 {


		 	genSimCharts();

		 }
		  
		 else
		 {

		  	playFlag = true;
		  	iteration--;
		  	playNoviz=false;
		  	simplayFlag=false;
		  	play();
	  		
		/*  backFlag = true;
			//forwardFlag = false;
			if(pauseFlag==false)
				backwardIter = xMax;
			

		if(document.getElementById("pause")!=null)
				{
					
					document.getElementById("pause").style.opacity="0.1";
				}
				else if(document.getElementById("play")!=null)
				{
					
					document.getElementById("play").style.opacity="0.1";
				}

				document.getElementById("forward").style.opacity="0.1";
				document.getElementById("back").style.opacity="1";

			


			forwardIter=xMax;
			playFlag = false;*/
		}
	}
	else
		reset();

}


function genSimCharts()
{
	if(resetFlag==false)	 
	{

		console.log(pauseIter,playIter,segmentChain);
		 if(playNoviz==false)
		  		{
		  			console.log("in draw");
		  			seekbar.value = playIter;
					render(playIter, width, height, graph,segmentChain[playIter],estimateChain[playIter]);
			    }

				if(simplayFlag==true && pauseFlag==false)
					setTimeout(simulatePlay,getDelay(playIter));
	}
	else
		reset();

}

function getDelay(itr)
{
	/*if(itr <=9)
		return 1000 - 75*(itr-1);
	else if(itr>9 && itr < xMax/2)
		return 400;
	else return 200;*/

	return 200;

}

function reset()
{
	
	console.log("in reset");
	//console.log(segmentArr);
	//console.log(estimateArr);
	$.get('/resetdata', JSON.stringify(iteration),'json')
			.fail(function() {
				
				console.log("Failed")
				alert('Request failed: /reset');
		
			});
	console.log("Data reset");
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
	//resetFlag = false;
	backFlag=false;
	//resetFlag = false;
	
	doneFlag = false;
	
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
	
	iteration = xMin-1;
	seekbar.value = iteration;
	segmentArr.length=0;
	estimateArr.length=0;
	segmentBackwardChain.length=0;
	segmentForwardChain.length=0;
	console.log(segmentArr,estimateArr);
	graph.selectAll("g.x.axis").remove();
	graph.selectAll("g.y.axis").remove();
	initGraph();
    div = d3.select("body").append("div")   
    .attr("class", "tooltip")
    .attr("id","tip")	
    .style("opacity", 0); 
	render(iteration, width, height, graph,[],[]);

	graph.selectAll("text.x.label").remove();
	graph.selectAll("text.y.label").remove();

	//document.getElementById("snapshotNo").setAttribute('text', "Click on Save for a snapshot of the current iteration");
	$("#snapshotNo").text("Click on Save for a snapshot of the current iteration");
}




$('#play').click(function() {
  var $this = $(this);

  if (this.style.opacity == 1.0){

  if (this.className == 'glyphicon glyphicon-pause'){

	  $('#play').attr("class",'glyphicon glyphicon-play');
	  $('#play').attr("id",'play');
	console.log("pause:",iteration);
	if(simplayFlag==true)
	{
		pauseIter=playIter;
		//simplayFlag=false;
	}
	else
	{
		pauseIter = iteration;
		render(pauseIter, width, height, graph,segmentArr,estimateArr);
		seekbar.value = pauseIter;
		simplayFlag=true;
	}
	if(pauseIter>seekbarMax)
		  	seekbarMax = pauseIter;
    pauseFlag = true;
    playFlag=false;
    playNoviz = true;
    backFlag = true;
    forwardFlag = true;

	console.log("without play:",pauseIter);

	  if(pauseIter > xMax)
	  {
		pauseIter=xMax;
				
	  }

	  if(document.getElementById("pause")!=null) 
   			{ 
   				document.getElementById("pause").setAttribute("class","glyphicon glyphicon-play"); 
   				document.getElementById("pause").setAttribute("id","play"); 
   				document.getElementById("play").style.opacity = "1";
   			}
   			else if (document.getElementById("play")!=null) 
   				document.getElementById("play").style.opacity = "1";


	  document.getElementById("back").style.opacity = "1";
	  
	  forwardIter = pauseIter;
	  backwardIter = pauseIter;
	  playIter = pauseIter;

	  if(forwardIter < iteration)
	  	document.getElementById("forward").style.opacity = "1";
	  else
	  	document.getElementById("forward").style.opacity = "0.1";
	  console.log("play:",playIter,"pause:",pauseIter,"pause:",forwardIter,"pause:",backwardIter);
	  
	  if (maxPause < pauseIter)
	  	maxPause = pauseIter;
	  
  }else{
	  $('#play').attr("class",'glyphicon glyphicon-pause');
	  $('#play').attr("id",'pause');



	  
	  forwardFlag=false;
	  backFlag=false;
	  pauseFlag = false;
	  playFlag=true;
	  playNoviz = false;
	  
	  if(resetFlag==true)
		  resetFlag=false;
	  
	  //playIter = iteration;
	  
	  if(iteration>=xMax)
	  {
		  //document.getElementById("forward").style.opacity = "1";
		  console.log("iteration>=xMax",playIter);
		  if(playIter<xMax && playIter>=xMin)
		  {
			  simplayFlag=true;
			  playIter--;
			document.getElementById("forward").style.opacity = "0.1";
			document.getElementById("back").style.opacity = "0.1";
		  	simulatePlay();
		  }
		  else
		  {

		  	document.getElementById("back").style.opacity = "1";
		  	//document.getElementById("pause").style.opacity = "0.1";
	  		render(xMax, width, height, graph,segmentArr,estimateArr);
			seekbar.value = xMax;
		  }
	  	
	  }
		else{
			
	  		if(simplayFlag==true)
			{
				//playIter--;
				document.getElementById("forward").style.opacity = "0.1";
				document.getElementById("back").style.opacity = "0.1";
				console.log(playIter,"------pasue clicked-----",iteration,"-----simplay but still play-----",pauseIter);
				simulatePlay();
			}
			else
			{
				document.getElementById("forward").style.opacity = "0.1";
				document.getElementById("back").style.opacity = "0.1";
				console.log("play:",iteration);
				play();
			}
	  
		}
	  
	  
   
		
	
	  
		
  }
  
}//if abled loop
  
 //$(this).toggleClass('glyphicon glyphicon-play').toggleClass('glyphicon glyphicon-pause');
  
});

$('#repeat').click(function() {
	resetFlag = true;
  reset();

});

$('#save').click(function() {
  
 	var snapIter;
 	if(backFlag==true)
 	{
 		snapIter=backwardIter;
 	}
 	else if(forwardFlag)
 	{

 		snapIter=forwardIter;

 	}
 	else if(simplayFlag==true)
 	{

 		snapIter=playIter;
 	}
	else
		snapIter=iteration;

 	
			  //seekbar.value = snapIter;
 	render(snapIter, snapshotW, snapshotH, snapshot,segmentChain[snapIter],estimateChain[snapIter]);
 	render(snapIter, width, height, graph,segmentChain[snapIter],estimateChain[snapIter]);

 
  

});


$('#trendlineLegend').click(function() {
	var setting = document.getElementById("trendWord").getAttribute("class");
    // $this will contain a reference to the checkbox
    if (setting == "off") {
      graph.selectAll("path.line").transition()
        .duration(function(d, i){ return i * 30;})
        .style("opacity", (iteration+10)/(maxIteration + 10))
        document.getElementById("trendWord").style.opacity = "1";
        document.getElementById("trendWord").setAttribute("class","on"); 
    } else {
      graph.selectAll("path.line").transition()
        .duration(function(d, i){ return i * 30;})
        .style("opacity", 0);
        document.getElementById("trendWord").style.opacity = "0.4";
        document.getElementById("trendWord").setAttribute("class","off"); 
    }
});



$('#estimateLegend').click(function() {
	var setting = document.getElementById("approxWord").getAttribute("class");
    if (setting == "off") {
     graph.selectAll("circle").transition()
        .duration(1000)
        .style("opacity", 0.5)
        document.getElementById("approxWord").style.opacity = "1";
        document.getElementById("approxWord").setAttribute("class","on"); 
    } else {
      graph.selectAll("circle").transition()
        .duration(800)
        .style("opacity", 0);
        document.getElementById("approxWord").style.opacity = "0.4";
        document.getElementById("approxWord").setAttribute("class","off"); 
      }
});


