
var seekbar = document.getElementById('seekbar');
var pauseIter = iterationLow-1;
var playIter = iterationLow-1;
var forwardFlag=false;
var forwardIter = iterationLow-1;
var backwardIter = iterationLow-1;
var seekbarIter = iterationLow-1;
var playNoviz=false;
var pauseFlag=false;
var playFlag=false;
var simplayFlag=false;
var resetFlag = false;
var backFlag=false;
var resetFlag = false;
var seekbarMax=iterationLow-1;


seekbar.addEventListener("change", function() {
  seekbarIter = Math.ceil(seekbar.value);

  
  if(pauseFlag==true)
  {
	    if(seekbarIter>seekbarMax)
			seekbarIter=seekbarMax;
  		if(seekbarIter < pauseIter)
  			forwardFlag=true;
  		if(seekbarIter <= pauseIter && seekbarIter > iterationLow)
  			backFlag=true;

  		forwardIter=seekbarIter;
  		backwardIter = seekbarIter;
  		pauseIter = seekbarIter;
  		playIter = seekbarIter;

  		
	  
	  seekbar.value = seekbarIter;
	  render(seekbarIter, widthWin, heightWin, graph,segmentChain[seekbarIter],estimateChain[seekbarIter]);


	  if(seekbarIter < maxIteration)
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
  		if(seekbarIter < maxIteration && seekbarIter >iterationLow)
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
  			if(seekbarIter >= maxIteration)
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
  			if(seekbarIter == iterationLow)
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
	   if(seekbarIter>maxIteration)
  			seekbarIter=maxIteration;
  		if(seekbarIter<iterationLow)
  			seekbarIter=iterationLow;

  		if(seekbarIter>iteration)
  			seekbarIter = iteration;

  		if(seekbarIter < maxIteration)
  			forwardFlag=true;
  		if(seekbarIter <= maxIteration && seekbarIter >iterationLow)
  			backFlag=true;



  		forwardIter=seekbarIter;
  		backwardIter = seekbarIter;
  		pauseIter = seekbarIter;
  		playIter = seekbarIter;

  		
				  seekbar.value = seekbarIter;
				  render(seekbarIter, widthWin, heightWin, graph,segmentChain[seekbarIter],estimateChain[seekbarIter]);

			  
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


if(seekbarIter==iterationLow)
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
  if(seekbarIter>=maxIteration)
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
	  if(iteration<iterationLow)
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
			$.get('/getiterativedata1', JSON.stringify(iteration), genCharts,'json')
			.fail(function() {
				
				console.log("Failed")
				alert('Request failed: /getiterativedata1');
		
			});

			return;
			
	}
	  
	  if(forwardFlag==true && playFlag==false)
	  {
		   
		  //forwardFlag=true;
		  backFlag=true;
		  forwardIter++;
		  if(forwardIter>seekbarMax)
		  	seekbarMax = forwardIter;
		  console.log(forwardIter+ ' inside of forward '+iteration);
		  if(forwardIter <maxIteration)
		  {
			  
			  seekbar.value = forwardIter;
			  render(forwardIter, widthWin, heightWin, graph,segmentChain[forwardIter],estimateChain[forwardIter]);
			  
		  }
		  else if(forwardIter==maxIteration)
		  {
			  seekbar.value = forwardIter;
			  render(forwardIter, widthWin, heightWin, graph,segmentChain[forwardIter],estimateChain[forwardIter]);
			  
		  }

		  if(forwardIter>maxIteration)
		  {
		  	forwardIter=maxIteration;
		  }

		  backwardIter=forwardIter;
		  playIter = forwardIter;
		  pauseIter=forwardIter;
			
	  }
	  
	  if(iteration<maxIteration && backFlag==true)
	  {
		  forwardFlag=true;
		  
	  }
	  
	  		  
	   if(forwardIter >=maxIteration)
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

		if(forwardIter<maxIteration)
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
	if(iteration==iterationLow)
	{
		backFlag=false;
		//console.log("bacik",iteration,segmentArr);
		//console.log(iteration,segmentArr);
		document.getElementById("back").style.opacity = "0.1";
		document.getElementById("forward").style.opacity = "1";
		
		//iteration--;
		seekbar.value = iteration;
		render(iteration, widthWin, heightWin, graph,segmentChain[iteration]);
		return;

	}
	if(playFlag==false && backFlag == true)
	{
		forwardFlag = true;
		backwardIter--;

		console.log("IN BACK",backwardIter);

		if(backwardIter >= iterationLow)
		{

			
	  		seekbar.value = backwardIter;
			render(backwardIter, widthWin, heightWin, graph,segmentChain[backwardIter],estimateChain[backwardIter]);
			



		}
		
		forwardIter=backwardIter;
		if(backwardIter<iterationLow)
			forwardIter=iterationLow;

		playIter=forwardIter;
		pauseIter=forwardIter;

		
	}
	
	

	if(backwardIter <=iterationLow)
	{
		 document.getElementById("back").style.opacity = "0.1";
		 
		 
		}
			
	   if(backwardIter<maxIteration)
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
		//document.getElementById('submitBtn').disabled = false;
		iteration++;
		
		if(iteration <= 1 && QA == true)
		{
			
			startTime = Date.now();
		}
		
		if(iteration<=maxIteration  && playFlag==true)
		{
			console.log("iteration:",iteration,"Max:",maxIteration);
	
			$.get('/getiterativedata1', JSON.stringify(iteration), genCharts,'json')
			.fail(function() {
				
				console.log("Failed")
				alert('Request failed: /getiterativedata1');
		
			});
			  
		
			
	  
  
		}
		/*else if(playIter > iterationLow && playIter < iteration)
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
				if(pauseFlag==false && simplayFlag==false)
				{
					backwardIter = maxIteration;
					playIter = maxIteration;
					pauseIter = maxIteration;
				}
				

				

				
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
				//simplayFlag=true;
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
		
		  		if(playNoviz==false)
		  		{
		  			//console.log("in draw");
		  			seekbar.value = playIter;
					render(playIter, widthWin, heightWin, graph,segmentChain[playIter],estimateChain[playIter]);
			}

				if(simplayFlag==true && pauseFlag==false)
					//setTimeout(simulatePlay,getDelay(playIter));
					simulatePlay();
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
		if(iteration==1)
		{
			validCells = output.validCells;
			//console.log("validCells",validCells);
			var axis = output.y;
			//console.log(axis);
			xMax = axis.xMax;
			xMin = axis.xMin;
			yMax = axis.yMax;
			yMin = axis.yMin;

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

			//console.log(output);
		}
		
		if(iteration<20)
		{
			currAvgMax = output.max;
			currAvgMin=output.min;
		}
		
		else if(iteration==30)
		{
			currAvgMax = output.max;
			currAvgMin=output.min;
		}
		else if(iteration==50)
		{
			currAvgMax = output.max;
			currAvgMin=output.min;
		}
		else if(iteration==100)
		{
			currAvgMax = output.max;
			currAvgMin=output.min;
		}
		

		avgMax[iteration] = currAvgMax;
		avgMin[iteration] = currAvgMin;
		console.log(output.max,currAvgMax,output.min,currAvgMin);
		
		if(segment.length>0)
		{
			
			
			var replacedSegmentArr = [];
			for(var i=0;i<segment.length;i++)
			{
				var quadObj = {"xb":segment[i].xb,"xe":segment[i].xe,"yb":segment[i].yb,"ye":segment[i].ye,"a":segment[i].a};
				if(i==0)
				{

					segmentArr.splice(segment[i].n,1,quadObj);
				}
				else
					segmentArr.splice(segment[i].n,0,quadObj);
				
			}
			
			segmentChain[iteration] = [];
			for(var i=0;i<segmentArr.length;i++)
			{
				segmentChain[iteration][i] = segmentArr[i];
			}
				
			//console.log(segmentChain,segmentArr);
			 if(playNoviz==false && simplayFlag==false && resetFlag == false)
			 {
				render(iteration, widthWin, heightWin, graph,segmentArr);
				seekbar.value = iteration;
			 }
				
				if (iteration <= maxIteration && resetFlag == false){
					//iteration++;
					//console.log("checkplay",iteration);
					if(forwardFlag==false)
					{
						//ranOnce=true;
						//setTimeout(play,getDelay(iteration));
						play();
					}
					else 
					{
						playNoviz=true;
						//setTimeout(play,getDelay(iteration));
						play();
					}
					
				}
			
		}
		
		else
			{
				
				iteration--;
				maxIteration=iteration;
				setupSeekbar(iterationLow,maxIteration);
				//console.log(segmentArr);
				backFlag = true;
				//forwardFlag = false;
				if(pauseFlag==false)
					backwardIter = iteration;
				

				

				
				if(playFlag==true && simplayFlag==false)
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

	return 300;

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
	
	
	//d3.selectAll("path.line").remove();
	graph.selectAll("rect").remove();
	snapshot.selectAll("rect").remove();
	basicLegend.selectAll("rect").remove();
	basicLegend.selectAll("text").remove();
	//graph.selectAll(".circle.dot").data([]).exit().remove();
	//d3.selectAll("svg > *").remove();
	

	iterationLow = 0;
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
	//resetFlag = false;
	backFlag=false;
	//resetFlag = false;
	
	maxIteration = (xMax-xMin+1)*(yMax-yMin+1);
  
	
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
		pauseIter = iteration-1;
		render(pauseIter, widthWin, heightWin, graph,segmentArr);
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

	  if(pauseIter > maxIteration)
	  {
		pauseIter=maxIteration;
				
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
	  console.log("iteration>=maxIteration",playIter);
	  if(iteration>=maxIteration)
	  {
		  //document.getElementById("forward").style.opacity = "1";
		  console.log("iteration>=maxIteration",playIter);
		  if(playIter<maxIteration && playIter>=iterationLow)
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
	  		render(maxIteration, widthWin, heightWin, graph,segmentArr);
			seekbar.value = maxIteration;
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
 	
 	render(snapIter, snapshotW, snapshotH, snapshot,segmentChain[snapIter],estimateChain[snapIter]);
 	render(snapIter, widthWin, heightWin, graph,segmentChain[snapIter],estimateChain[snapIter]);

 
  

});



//left pane - checkboxes
$('#trendlineCheckBox').click(function() {
    var $this = $(this);
    // $this will contain a reference to the checkbox
    if (this.checked) {
      d3.selectAll("#trend").transition()
        .duration(function(d, i){ return i * 30;})
        .style("opacity", (iteration+10)/(maxIteration + 10))
    } else {
      d3.selectAll("#trend").transition()
        .duration(function(d, i){ return i * 30;})
        .style("opacity", 0);
    }
});


$('#sampledCheckbox').click(function() {
    var $this = $(this);
    if (this.checked) {
      d3.selectAll("circle").transition()
        .duration(1000)
        .style("opacity", 0.5)
    } else {
      d3.selectAll("circle").transition()
        .duration(800)
        .style("opacity", 0);
      }
});
