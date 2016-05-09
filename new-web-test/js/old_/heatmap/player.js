
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
var maxPause=0;

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

  		var seekSegmentArr = [];
			  
	  for(var i=0;i<segmentArr.length;i++)
		  seekSegmentArr[i] = segmentArr[i];
	  ////console.log(segmentArr.length,segmentArr);
	  ////console.log(segmentForwardChain);
	  ////console.log(segmentBackwardChain);
	  var currentIter;
	  if(iteration==maxIteration)
		  currentIter = iteration;
	  else if(iteration < maxIteration)
		  currentIter = iteration-1;
	  else
		  currentIter = maxIteration;
	  
	  var toIterate = currentIter-seekbarIter;
	  
	  //console.log("seek",seekbarIter,"currentiter",currentIter, "toIterate",toIterate);
	  for(var i=currentIter,j=0;j<toIterate;j++,i--)
	  {
		  ////console.log(seekSegmentArr.length,seekSegmentArr);
		  ////console.log(i,segmentBackwardChain[i]);
		  seekSegmentArr.splice(segmentBackwardChain[i].n,segmentForwardChain[i].length,segmentBackwardChain[i].s);
		  ////console.log(seekSegmentArr.length,seekSegmentArr);
	  }
	  
	  seekbar.value = seekbarIter;
	  render(seekbarIter, widthWin, heightWin, graph,seekSegmentArr,estimateChain[seekbarIter]);


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

  		var seekSegmentArr = [];
			  
			  for(var i=0;i<segmentArr.length;i++)
				  seekSegmentArr[i] = segmentArr[i];
			  ////console.log(segmentArr.length,segmentArr);
			  ////console.log(segmentForwardChain);
			  ////console.log(segmentBackwardChain);
			  var currentIter;
			  if(iteration<=maxIteration)
				  currentIter = iteration;
			  else
				  currentIter = maxIteration;
			  var toIterate = currentIter-seekbarIter;
			  if(toIterate==0)
			  {
			  		seekbar.value = seekbarIter;
			  	render(seekbarIter, widthWin, heightWin, graph,segmentArr,estimateArr);

			  }
			  else
			  {
			  		//console.log("seek",seekbarIter,"currentiter",currentIter, "toIterate",toIterate);
				  for(var i=currentIter,j=0;j<toIterate;j++,i--)
				  {
					  ////console.log(forwardSegmentArr.length,forwardSegmentArr);
					  ////console.log(i,segmentBackwardChain[i]);
					  seekSegmentArr.splice(segmentBackwardChain[i].n,segmentForwardChain[i].length,segmentBackwardChain[i].s);
					  
				  }
				  
				  seekbar.value = seekbarIter;
				  render(seekbarIter, widthWin, heightWin, graph,seekSegmentArr,estimateChain[seekbarIter]);

			  }
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
	  ////console.log(segmentArr.length,segmentArr);
	  if(iteration<iterationLow)
	  {
		  //playNoviz=true;
		  //console.log("forward init clcik");
		  forwardFlag=true;
		  //backFlag=false;
		  iteration++;
		  forwardIter=iteration;
		  //console.log("forward init",forwardIter);
		  document.getElementById("play").style.opacity = "1";
		  document.getElementById("back").style.opacity = "1";
			$.get('/getiterativedata1', JSON.stringify(iteration), genCharts,'json')
			.fail(function() {
				
				//console.log("Failed")
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
		  //console.log(forwardIter+ ' inside of forward '+iteration);
		  if(forwardIter <maxIteration)
		  {
			  var forwardSegmentArr = [];
			  
			  for(var i=0;i<segmentArr.length;i++)
				  forwardSegmentArr[i] = segmentArr[i];
			  ////console.log(segmentArr.length,segmentArr);
			  ////console.log(forwardSegmentArr.length,forwardSegmentArr);
			  ////console.log(segmentBackwardChain);
			  var currentIter;
			  if(iteration==maxIteration)
				  currentIter = iteration;
			  else if(iteration<maxIteration)
				  currentIter = iteration-1;
			  else
				  currentIter = maxIteration;
			  var toIterate = currentIter-forwardIter;
			  
			  //console.log("forward",forwardIter,"currentiter",currentIter, "toIterate",toIterate);
			  for(var i=currentIter,j=0;j<toIterate;j++,i--)
			  {
				  ////console.log(forwardSegmentArr.length,forwardSegmentArr);
				  ////console.log(i,segmentBackwardChain[i]);
				  forwardSegmentArr.splice(segmentBackwardChain[i].n,segmentForwardChain[i].length,segmentBackwardChain[i].s);
				  
			  }
			  
			  seekbar.value = forwardIter;
			  render(forwardIter, widthWin, heightWin, graph,forwardSegmentArr,estimateChain[forwardIter]);
			  
		  }
		  else if(forwardIter==maxIteration)
		  {
			  seekbar.value = forwardIter;
			  render(forwardIter, widthWin, heightWin, graph,segmentArr,estimateChain[forwardIter]);
			  
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
			
	   
			document.getElementById("back").style.opacity = "1";
	  
	
  //console.log(forwardIter+ ' outside of forward '+iteration);

}
});

$('#back').click(function() {
	  if (this.style.opacity == 1.0){
	////console.log(segmentBackwardChain);
	if(iteration==iterationLow)
	{
		backFlag=false;
		////console.log("bacik",iteration,segmentArr);
		segmentArr.length=0;
		segmentArr.splice(0,0,segmentBackwardChain[iteration+1].s);
		////console.log(iteration,segmentArr);
		document.getElementById("back").style.opacity = "0.1";
		document.getElementById("forward").style.opacity = "1";
		
		//iteration--;
		seekbar.value = iteration;
		render(iteration, widthWin, heightWin, graph,segmentArr,estimateArr);
		return;

	}
	if(playFlag==false && backFlag == true)
	{
		forwardFlag = true;
		backwardIter--;

		//console.log("IN BACK",backwardIter);

		if(backwardIter >= iterationLow)
		{

			var backSegmentArr = [];
			  
		  for(var i=0;i<segmentArr.length;i++)
			  backSegmentArr[i] = segmentArr[i];
		  ////console.log(segmentArr.length,segmentArr);
		  ////console.log(forwardSegmentArr.length,forwardSegmentArr);
		  ////console.log(segmentBackwardChain);
		  var currentIter;
		  if(iteration==maxIteration)
			  currentIter = iteration;
		  else if(iteration<maxIteration)
			  currentIter = iteration-1;
		  else
			  currentIter = maxIteration;
		  var toIterate = currentIter-backwardIter;
		  
		  //console.log("backward",backwardIter,"currentiter",currentIter, "toIterate",toIterate);
		  for(var i=currentIter,j=0;j<toIterate;j++,i--)
		  {
			  ////console.log(forwardSegmentArr.length,forwardSegmentArr);
			  ////console.log(i,segmentBackwardChain[i]);
			  backSegmentArr.splice(segmentBackwardChain[i].n,segmentForwardChain[i].length,segmentBackwardChain[i].s);
			  
		  }

	  		seekbar.value = backwardIter;
			render(backwardIter, widthWin, heightWin, graph,backSegmentArr,estimateChain[backwardIter]);
			



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
		document.getElementById('submitBtn').disabled = false;
		iteration++;
		
		if(iteration <= 1 && competition == true)
		{
			
			startTime = Date.now();
		}
		
		if(iteration<=maxIteration)
			{
				//console.log("iteration:",iteration,"Max:",maxIteration);
		
				$.get('/getiterativedata1', JSON.stringify(iteration), genCharts,'json')
				.fail(function() {
					
					//console.log("Failed")
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
				////console.log(segmentArr);
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
		 //console.log(playIter,"-------------simplay-------",iteration);
		 
		 if(playIter<=maxIteration && resetFlag==false)
		 {

			
		 	genSimCharts();

		 }
		  
		 else
		 {

		  
	  		
			  backFlag = true;
				//forwardFlag = false;
				if(pauseFlag==false)
					backwardIter = maxIteration;
				

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

				


				forwardIter=maxIteration;
				playFlag = false;
				//simplayFlag = true;
			}
	}
	else
		reset(); 

}


function genSimCharts()
{

	if(resetFlag==false)
	{
		var playSegmentArr = [];
			  
		  for(var i=0;i<segmentArr.length;i++)
			  playSegmentArr[i] = segmentArr[i];
		  ////console.log(segmentArr.length,segmentArr);
		 // //console.log(playSegmentArr.length,playSegmentArr);
		 // //console.log(segmentForwardChain);
		//  //console.log(segmentBackwardChain);
		  var currentIter;
		  if(iteration==maxIteration)
			  currentIter = iteration;
		  else if(iteration < maxIteration)
			  currentIter = iteration-1;
		  else
			  currentIter = maxIteration;
		  var toIterate = currentIter-playIter;
		  if(toIterate < 0)
		 {
			 simplayFlag = false;
				return;
			 
		 }
		  //console.log("--------------play",playIter,"currentiter",currentIter, "toIterate--------------",toIterate);
		  for(var i=currentIter,j=0;j<toIterate;j++,i--)
		  {
			//  //console.log(playSegmentArr.length,playSegmentArr);
			 // //console.log(i,segmentBackwardChain[i]);
			  playSegmentArr.splice(segmentBackwardChain[i].n,segmentForwardChain[i].length,segmentBackwardChain[i].s);
			  
		  }

		  ////console.log("before get to currentplay");
		  		for(var j=0;j<segmentForwardChain[playIter].length;j++)
		  		{
		  			if(j==0)
					{
						playSegmentArr.splice(segmentForwardChain[playIter][j].n,1,segmentForwardChain[playIter][j].s);
					}
					else
						playSegmentArr.splice(segmentForwardChain[playIter][j].n,0,segmentForwardChain[playIter][j].s);


		  		}
		  		////console.log("before draw",playNoviz);
		  		if(playNoviz==false)
		  		{
		  			////console.log("in draw");
		  			seekbar.value = playIter;
					render(playIter, widthWin, heightWin, graph,playSegmentArr,estimateChain[playIter]);
			}

				if(simplayFlag==true && pauseFlag==false)
					setTimeout(simulatePlay,getDelay(playIter));
	}
	else
		reset();
}
  
function genCharts(output)
{
	//var jsonData = JSON.parse(output);
	////console.log(ouput.length);
	//console.dir(output);
	if(resetFlag==false)
	{
		var segment = output.sl;
		var estimate = output.gl;

		if(iteration==1)
		{
			validCells = output.validCells;

			xMax = output.xMax;
			xMin = output.xMin;
			yMax = output.yMax;
			yMin = output.yMin;

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
		
		if(iteration<6)
		{
			currAvgMax = output.max;
			currAvgMin=output.min;
		}
		
		else if(iteration==10)
		{
			currAvgMax = output.max;
			currAvgMin=output.min;
		}
		else if(iteration==15)
		{
			currAvgMax = output.max;
			currAvgMin=output.min;
		}
		else if(iteration==25)
		{
			currAvgMax = output.max;
			currAvgMin=output.min;
		}
		else if(iteration==50)
		{
			currAvgMax = output.max;
			currAvgMin=output.min;
		}

		avgMax[iteration] = currAvgMax;
		avgMin[iteration] = currAvgMin;
		//console.log(output.max,currAvgMax,output.min,currAvgMin);
		/*if(iteration==1)
		{
			avgMax[iteration] = output.max;
			avgMin[iteration] = output.min;

			currAvgMax = output.max;
			currAvgMin=output.min;
			//console.log(output.max,currAvgMax,output.min,currAvgMin);
		}
		else 
		{
			avgMax[iteration] = output.max;
			avgMin[iteration] = output.min;
			
			currAvgMax = output.max;
			currAvgMin=output.min;
			//console.log(output.max,avgMax,output.min,avgMin);
		}*/
		/*else
		{
			avgMax[iteration] = currAvgMax;
			avgMin[iteration] = currAvgMin;
			
			////console.log(output.max,avgMax,output.min,avgMin);
		}*/
		
		
		
		////console.log("length:",segment.length);
		if(segment.length>0)
		{
			
			
			var replacedSegmentArr = [];
			for(var i=0;i<segment.length;i++)
			{
				var replacedSegment = {};
				replacedSegment.n = segment[i].n;
				replacedSegment.s = segment[i].s;
				replacedSegmentArr[i] = replacedSegment;
				if(i==0)
				{
					var replacedSegment2 = {};
					replacedSegment2.n = segment[i].n;
					////console.log(segment[i].n,segmentArr[segment[i].n]);
					replacedSegment2.s = segmentArr[segment[i].n];
					segmentBackwardChain[iteration] = replacedSegment2;
					segmentArr.splice(segment[i].n,1,segment[i].s);
				}
				else
					segmentArr.splice(segment[i].n,0,segment[i].s);
				
			}
			segmentForwardChain[iteration] = replacedSegmentArr;
			
			////console.log(iteration,segmentArr);
			//onsole.log(estimate);
			
			/*for(var i=0;i<estimate.length;i++)
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
			}*/
			
			////console.log(segmentArr);
		//for(var i=0;i<estimateArr.length;i++)
		//{
		//	//console.log(i,estimateArr[i]);	
				
		//}
		
			
		////console.log("genCharts",iteration);
			
			//setTimeout(play, 100);
			 if(playNoviz==false && simplayFlag==false)
			 {
				render(iteration, widthWin, heightWin, graph,segmentArr,estimateArr);
				seekbar.value = iteration;
			 }
				
				if (iteration <= maxIteration){
					//iteration++;
					////console.log("checkplay",iteration);
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
			{
				
				iteration--;
				maxIteration=iteration;
				setupSeekbar(iterationLow,maxIteration);
				////console.log(segmentArr);
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
	if(itr <=4)
		return 1000 - 200*(itr-1);
	else if(itr>4 && itr < maxIteration/2)
		return 200;
	else return 150;

}

function reset()
{
	resetFlag = true;
	//console.log("in reset");
	////console.log(segmentArr);
	////console.log(estimateArr);
	$.get('/resetdata', JSON.stringify(iteration),'json')
			.fail(function() {
				
				//console.log("Failed")
				alert('Request failed: /reset');
		
			});
	//console.log("Data reset");
	
	resetFlag = true;
	//d3.selectAll("path.line").remove();
	graph.selectAll("rect").remove();
	snapshot.selectAll("rect").remove();
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
	//resetFlag = false;
	backFlag=false;
	//resetFlag = false;
	
	maxIteration = (xMax-xMin+1)*(yMax-yMin+1);
  
	resetFlag = true;
	//console.log("Reset: ", iteration);
	
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
	resetFlag = true;
	//document.getElementById("snapshotNo").setAttribute('text', "Click on Save for a snapshot of the current iteration");
	$("#snapshotNo").text("Click on Save for a snapshot of the current iteration");
}




$('#play').click(function() {
  var $this = $(this);

  if (this.style.opacity == 1.0){

  if (this.className == 'glyphicon glyphicon-pause'){

	  $('#play').attr("class",'glyphicon glyphicon-play');
	  $('#play').attr("id",'play');
	//console.log("pause:",iteration);
	if(simplayFlag==true)
	{
		pauseIter=playIter;
		//simplayFlag=false;
	}
	else
	{
		pauseIter = iteration-1;
		render(pauseIter, widthWin, heightWin, graph,segmentArr,estimateArr);
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

	//console.log("without play:",pauseIter);

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
	  document.getElementById("forward").style.opacity = "1";
	  forwardIter = pauseIter;
	  backwardIter = pauseIter;
	  playIter = pauseIter;
	  if (maxPause < pauseIter)
	  	maxPause = pauseIter;
	  //console.log("play:",playIter,"pause:",pauseIter,"pause:",forwardIter,"pause:",backwardIter);
	  

	  
	  
	  
	  
	  
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
	  //console.log("iteration>=maxIteration",playIter);
	  if(iteration>=maxIteration)
	  {
		  //document.getElementById("forward").style.opacity = "1";
		  //console.log("iteration>=maxIteration",playIter);
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
	  		render(maxIteration, widthWin, heightWin, graph,segmentArr,estimateArr);
			seekbar.value = maxIteration;
		  }
	  	
	  }
		else{
			
			if(simplayFlag==true)
			{
				playIter--;
				document.getElementById("forward").style.opacity = "0.1";
				document.getElementById("back").style.opacity = "0.1";
				//console.log(playIter,"------pasue clicked-----",iteration,"-----simplay but still play-----",pauseIter);
				simulatePlay();
			}
			else
			{
				document.getElementById("forward").style.opacity = "0.1";
				document.getElementById("back").style.opacity = "0.1";
				//console.log("play:",iteration);
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
 	var snapSegmentArr = [];
			  
	  for(var i=0;i<segmentArr.length;i++)
		  snapSegmentArr[i] = segmentArr[i];
	  
	  var currentIter;
	  if(iteration<=maxIteration)
		  currentIter = iteration;
	  else
		  currentIter = maxIteration;
	  var toIterate = currentIter-snapIter;
	  
	  //console.log("snap",snapIter,"currentiter",currentIter, "toIterate",toIterate);
	  for(var i=currentIter,j=0;j<toIterate;j++,i--)
	  {
		  snapSegmentArr.splice(segmentBackwardChain[i].n,segmentForwardChain[i].length,segmentBackwardChain[i].s);
	  }
	  
	  //seekbar.value = snapIter;
 	render(snapIter, snapshotW, snapshotH, snapshot,snapSegmentArr,estimateChain[snapIter]);
 	render(snapIter, widthWin, heightWin, graph,snapSegmentArr,estimateChain[snapIter]);

 
  

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
