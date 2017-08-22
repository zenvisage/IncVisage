 var ms_data = $('#data_combobox').magicSuggest({
 placeholder: 'Select DataSet',
 allowFreeEntries: false,
 data: ['Airline','Sensor'],
 maxSelection: 1,
 required: true,
 resultAsString: true
 });
var ychange=0;
var xchange=0;
var measchange=0;
var dataset = "";
var dimension1 = "";
var dimension2 = "";
var measure = "";
var filter = "";
var filterValue = "";
var samplingType = "without";
var currentDataset="";
var tmpDataset;
var answer = [];
var question = [];
var conversion = [];
var qType = []
var qTime = []
var questionNo = 0;
var scoreV1 = 0;
var scoreV2 = 0;
var scoreV3 = 0;

var dataFile;
var dataFile;
var TotalScore = 0;
var TotalPoints = 0;
var scoreQ = 0;
var numQ = 0;
var QA = true;
var submissions = [];

var maxPause = 0;

var startTime = 0;
var endTime = 0;
var currentResponsTime = [];
var correctAnswer = [];
var points = [];
var accuracyArr = [];
var userAnswer = [];
var randomArray = [];
function convertMonthToDay(data)
{
	var day_  = 0;
	for(var i=0;i<data;i++)
	  day_ += days[i];
        return day_; 
}
/*
function shuffle() {
  var m = question.length, t, i;

  // While there remain elements to shuffle…
  while (m) {

    // Pick a remaining element…
    i = Math.floor(Math.random() * m--);

    // And swap it with the current element.
    t = question[m];
    question[m] = question[i];
    question[i] = t;

    t = answer[m];
    answer[m] = answer[i];
    answer[i] = t;

    t = conversion[m];
    conversion[m] = conversion[i];
    conversion[i] = t;

    t = qType[m];
    qType[m] = qType[i];
    qType[i] = t;
  }

  //return array;
}


function checkAnswer(){
  playFlag = false;
  endTime = Date.now();

  var timeDiff = (1.0*(endTime-startTime))/1000;

  currentResponsTime[questionNo] = timeDiff;

  var currentAnswer = document.getElementById("answer").value;

  userAnswer[questionNo] =  currentAnswer;
  if(currentAnswer == "" || currentAnswer == null)
  {
    alert("Please submit an answer.");
    return false;
  }
  
  var score = 0;
  var accuracyV1 = 0;
  var accuracyV2 = 0;
  var accuracyV3 = 0;
  document.getElementById("answer").value = "";

  console.log("pauseIter",maxPause);
  var valCurrent; 
  var valCorrect;
  if(qType[questionNo].toString() == "N")
  {
      if(conversion[questionNo].toString().toLowerCase() == "month")
      {
        var currAns = currentAnswer.toString().toLowerCase().split(" "); 
        var corrAns = answer[questionNo].toString().toLowerCase().split(" ");
        console.log(currAns);
        console.log(corrAns);
        valCurrent = convertMonthToDay(monthIndex[currAns[0]])+parseInt(currAns[1]);
        valCorrect = convertMonthToDay(monthIndex[corrAns[0]])+parseInt(corrAns[1]);
      }

      else if(conversion[questionNo].toString().toLowerCase() == "yearly")
      {
        var currAns = currentAnswer.toString().toLowerCase().split(" "); 
        var corrAns = answer[questionNo].toString().toLowerCase().split(" ");
        console.log(currAns);
        console.log(corrAns);
        var base = 0;
        if(dimension1.toString().toLowerCase() == "year")
        	base =  xMin;
        else
        	base = yMin;
        console.log("base:",base);
        valCurrent = (monthIndex[currAns[0]]+1)+(parseInt(currAns[1])-base+1)*12;
        valCorrect = (monthIndex[corrAns[0]]+1)+(parseInt(corrAns[1])-base+1)*12;
      }
      else if(conversion[questionNo].toString().toLowerCase() == "year")
      {
        if(dimension1.toString().toLowerCase() == "year")
          base =  xMin;
        else
          base = yMin;

        valCurrent = parseInt(currentAnswer)-base+1;
        valCorrect = parseInt(answer[questionNo])-base+1;
      }
      else if(conversion[questionNo].toString().toLowerCase() == "week")
      {
        valCurrent = parseInt(currentAnswer);
        valCorrect = parseInt(answer[questionNo]);
      }
      
      if(valCurrent == valCorrect)
      {
        console.log("correct");
        document.getElementById('questionNo').innerHTML = "You're right!";
        accuracyV1 = 1;

        correctAnswer[questionNo] = 1;
      }

      else
      {
         correctAnswer[questionNo] = 0;
         if(Math.abs(valCurrent-valCorrect) >= valCorrect)
          accuracyV1 = 0;
        //else if(isNaN(parseInt(currAns[1])))
          //accuracyV1 = 0;
         else
          accuracyV1 = 1 - (Math.abs(valCurrent-valCorrect)*1.0)/valCorrect;
         document.getElementById('questionNo').innerHTML = "The correct answer was: " + answer[questionNo]+"." ;
         
      }


  }
  else
  {
    valCurrent = currentAnswer.toString().toLowerCase();
    valCorrect = answer[questionNo].toString().toLowerCase();

    if(valCurrent.toString() == valCorrect.toString())
    {
      console.log("correct");
      document.getElementById('questionNo').innerHTML = "You're right! The correct answer was: " + answer[questionNo]+"." ;
     
      accuracyV3 = 1;
      correctAnswer[questionNo] = 1;
      
    }
    else
    {
       correctAnswer[questionNo] = 0;
       accuracyV3 = 0;
       document.getElementById('questionNo').innerHTML = "The correct answer was: " + answer[questionNo]+"." ;
       
    }

  }
  
  //scoreV2 = Math.log10((1.0*(xMax-pauseIter+1))).toFixed(2)/Math.log10(xMax).toFixed(2)*accuracyV2*(100.0);
  

  document.getElementById("answer").style.display = "none";
  document.getElementById("submitBtn").style.display = "none";
  document.getElementById("nextBtn").style.display = "block";

  var normalizer = qTime[questionNo];
  var compareTime = qTime[questionNo]-timeDiff;
  if(compareTime < 0)
      compareTime = 0;
  //if(pauseIter==0)
  //	maxPause=normalizer;
  if(qType[questionNo].toString() == "N")
  {
    scoreV1 = (1.0*compareTime)/(normalizer)*accuracyV1*(100.0);
    scoreQ = scoreV1;
    TotalScore += scoreV1;
    //document.getElementById('score').innerHTML = scoreV1.toFixed(2);
    points[questionNo] = scoreV1;
    accuracyArr[questionNo] = accuracyV1;
  }
  else
  {
    scoreV3 = (1.0*compareTime)/(normalizer)*accuracyV3*(100.0);
    scoreQ = scoreV3;
    TotalScore += scoreV3;
    //document.getElementById('score').innerHTML = scoreV3.toFixed(2);
    points[questionNo] = scoreV3;
    accuracyArr[questionNo] = accuracyV3;
  }

  
    document.getElementById('score').innerHTML = TotalScore.toFixed(2)+" pts (Out of "+(questionNo+1)*100+" pts)";
  
    document.getElementById('questionText').innerHTML = "You received "+scoreQ.toFixed(2)+" pts.</br></br></br>"+question[questionNo];


  submissions[questionNo] =  maxPause;
  questionNo++;
}


function next(){
    maxPause = 0;
    reset();
    clearFields();
    console.log(questionNo,numQ);
    if(questionNo < numQ)
    {
      document.getElementById('answer').style.display = "block";
      document.getElementById('submitBtn').style.display = "block";
      document.getElementById('submitBtn').disabled = true;
      document.getElementById("nextBtn").style.display = "none";     
      document.getElementById('questionNo').innerHTML = "Question " + (questionNo +1).toString();
      document.getElementById('questionText').innerHTML = question[questionNo]; 
      //document.getElementById("doneBtn").disabled = true;
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
    if(document.getElementById("clear")!=null) 
        { 
          document.getElementById("filter1").style.display = "none";
          filter = "",
          filterValue = "";
          document.getElementById('addFilters').style.display = "block";
        } 
    }
    
    else 
    {
      document.getElementById('question').style.display = "none";
      document.getElementById('prompt').style.display = "block";
      document.getElementById('prompt').innerHTML = "The QA section is complete. Press Done to save your responses and continue to the next part of the study.";
      document.getElementById('moveBtn').style.display = "block";
          var savetime = {};
          savetime.question = question;
        savetime.answer = answer;
        savetime.userans = userAnswer;
    savetime.iter = submissions;
    savetime.resp = currentResponsTime;
    savetime.correct = correctAnswer;
    savetime.accuracy = accuracyArr;
    savetime.score = points;
    $.get('/storeResponseHeat', JSON.stringify(savetime))
        .fail(function() {
          
      
        });
    }
    
}
*/
function clearFields()
{
  $("#xAxis").empty();
    var select2 = document.getElementById('xAxis');
      opt = document.createElement('option');
      opt.text = "--select an option--";
      opt.value = xList[i];
      //opt.innerHTML = xList[i];
      select2.appendChild(opt);

    //console.log(select2);
    for (var i = 0; i<xList.length; i++){
      
      var opt = document.createElement('option');
      opt.text = xList[i];
      opt.value = xList[i];
      //opt.innerHTML = yList[i];
      select2.appendChild(opt);
      //console.log(yList[i]);
    }
    $("#xAxis").selectpicker('refresh');
    document.getElementById("xAxis").options[0].disabled = true;
    
  //}
  
  $("#xAxis").selectpicker('refresh');


  $("#yAxis").empty();
    var select2 = document.getElementById('yAxis');
      opt = document.createElement('option');
      opt.text = "--select an option--";
      opt.value = yList[i];
      //opt.innerHTML = xList[i];
      select2.appendChild(opt);

    //console.log(select2);
    for (var i = 0; i<yList.length; i++){
      if(new String(dimension1).valueOf()==new String(yList[i]).valueOf())
        continue;
      
      var opt = document.createElement('option');
      opt.text = yList[i];
      opt.value = yList[i];
      //opt.innerHTML = yList[i];
      select2.appendChild(opt);
      //console.log(yList[i]);
    }
    $("#yAxis").selectpicker('refresh');
    document.getElementById("yAxis").options[0].disabled = true;
    
  //}
  
  $("#yAxis").selectpicker('refresh');
  
   $("#measType").empty();
    var select2 = document.getElementById('measType');
      opt = document.createElement('option');
      opt.text = "--select an option--";
      opt.value = measList[i];
      //opt.innerHTML = xList[i];
      select2.appendChild(opt);

    //console.log(select2);
    for (var i = 0; i<measList.length; i++){
      var opt = document.createElement('option');
      opt.text = measList[i];
      opt.value = measList[i];
      //opt.innerHTML = yList[i];
      select2.appendChild(opt);
      //console.log(yList[i]);
    }
    $("#measType").selectpicker('refresh');
    document.getElementById("measType").options[0].disabled = true;
    
  //}
  
  $("#measType").selectpicker('refresh');



  /*$("#filterattr").empty();
  var select3 = document.getElementById('filterattr');
   opt = document.createElement('option');
    opt.text = "Attribute";
    opt.value = "----";
    select3.appendChild(opt); 
  for (var i = 0; i<predList.length; i++){
    if(new String(dimension1).valueOf()!=new String(predList[i]).valueOf())
    {
    var opt = document.createElement('option');
    opt.text = predList[i];
    opt.value = predList[i];
    //opt.innerHTML = yList[i];
    select3.appendChild(opt);
     // console.log(predList[i]);
    }
  }

 $("#filterattr").selectpicker('refresh');
 document.getElementById("filterattr").options[0].disabled = true;
$("#filterattr").selectpicker('refresh');
  
  $("#filterValue").empty();
  var select4 = document.getElementById('filterValue');
   opt = document.createElement('option');
      opt.text = "Value";
      opt.value = "0";
      select4.appendChild(opt); 
  $("#filterValue").selectpicker('refresh');
  filter = "",
  filterValue = "";*/
  
  document.getElementById("doneBtn").disabled = true; 
}
/*
function finishQA()
{
  QA = false;
  reset();
  document.getElementById('prompt').style.display = "none";
  document.getElementById('moveBtn').style.display = "none";
  document.getElementById('player').style.marginLeft = "10%";
  //document.getElementById('colorLegend').style.marginLeft = "10%";
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
    if(document.getElementById("clear")!=null) 
        { 
          document.getElementById("filter1").style.display = "none";
          filter = "",
          filterValue = "";
          document.getElementById('addFilters').style.display = "block";
        } 

}
*/


//dataset is the new dataset that the user has selected
$('#dataset').on('change', function(){
	dataset = $(this).find("option:selected").val();
  var info =1;
  $.get('/getViz',JSON.stringify(dataset),loadQuestion,'json')
      .fail(function() {
        console.log("Failed")
        alert('Request failed: /LoadData');
      });
	 
    //if dataset changed, update the x-axis and y-axis options
  });

 function loadQuestion(vizVal)
 {
    vizType =  vizVal.viz;
    

    if(String(currentDataset)!=String(dataset))
    {
      //document.getElementById("loadbtn").disabled = false;
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

      if(document.getElementById("clear")!=null) 
          { 
            document.getElementById("filter1").style.display = "none";
            filter = "",
            filterValue = "";
            document.getElementById('addFilters').style.display = "block";
          }
    }
    else 
    { 
      //document.getElementById("loadbtn").disabled = true;
      document.getElementById("doneBtn").disabled = false;
    }
     // console.log(String(currentDataset),String(dataset));
     if (dataset == "Flight"){
      tmpDataset=dataset;
      dataset = "flight";
      

      }
      if (dataset == "Airline"){
      tmpDataset=dataset;
      dataset = "airline";
      

      }
      if (dataset == "Sensor"){
      tmpDataset=dataset;
      dataset = "sensordata";
    
   // $("#sampType")
   //     .html('<option>Without Range</option><option>With Range</option>')
   //     .selectpicker('refresh');
     }

     if (dataset == "EPA"){
      tmpDataset=dataset;
    dataset = "epa";
        
   // $("#sampType")
   //     .html('<option>Without Range</option><option>With Range</option>')
    //    .selectpicker('refresh');
      }
  if (dataset == "NYC Taxi-2011"){
    tmpDataset=dataset;
      dataset = "taxitrip2011";
    
    }
    
  if (dataset == "NYC Taxi-2012"){
    tmpDataset=dataset;
      dataset = "taxitrip2012";
    
    }
    
  if (dataset == "NYC Taxi-2013"){
    tmpDataset=dataset;
      dataset = "taxitrip2013";
    
    }
    
    if(vizType==1)
    {
      dataFile = "heat_qA_"+dataset+".csv";

      
    
    }
    else
    {
      dataFile = "heat_qB_"+dataset+".csv";

      
    
    }
    
   /* console.log(vizType,dataFile);

    d3.csv(dataFile, function(data) {
       //console.log(dataFile);
      for(var i=0;i<data.length;i++)
      {
        //console.log(data[i]);
              
        question[i] = data[i].Q;
        answer[i] = data[i].A;
        conversion[i] = data[i].C;  
        qType[i] =  data[i].T; 
        qTime[i] = data[i].S;   
        randomArray[i] = i;
      }
      numQ = question.length;

      questionNo = 0;
      shuffle();
       

  //console.log(conversion);
     // question = ["How long was the shortest delay (in minutes) in 2009?", "When was the longest delay in 1980?"];
    //  answer = ["15.2", "Dec 2"];
      document.getElementById('question').style.display = "block";
      document.getElementById('player').style.marginLeft = "17%";
      //document.getElementById('colorLegend').style.marginLeft = "17%";
      document.getElementById('questionNo').innerHTML = "Question " + (questionNo +1).toString();
      document.getElementById('questionText').innerHTML = question[questionNo]; 
      
      
    });*/

    renderSubmit();


 }


 $('#xAxis').on('change', function(){
    dimension1 = $(this).find("option:selected").val();
    //console.log(dimension1);
    xchange=1;
	//remove the x-dim from yList
	/*if(ychange!=1)
	{
		var select = document.getElementById('xAxis');
		var value = select.selectedIndex;
		
		var select1 = document.getElementById('yAxis');
		//console.log(select[value],value,select1[value]);
		select1.removeChild(select1[value]);
		
	}
	else
	{*/


		$("#yAxis").empty();
		var select2 = document.getElementById('yAxis');
		  opt = document.createElement('option');
			opt.text = "--select an option--";
			opt.value = yList[i];
			//opt.innerHTML = xList[i];
			select2.appendChild(opt);

		//console.log(select2);
		for (var i = 0; i<yList.length; i++){
			if(new String(dimension1).valueOf()==new String(yList[i]).valueOf())
				continue;
			
			var opt = document.createElement('option');
			opt.text = yList[i];
			opt.value = yList[i];
			//opt.innerHTML = yList[i];
			select2.appendChild(opt);
			//console.log(yList[i]);
		}
		$("#yAxis").selectpicker('refresh');
		document.getElementById("yAxis").options[0].disabled = true;
		
	//}
	
	$("#yAxis").selectpicker('refresh');
  if((xchange+ychange+measchange)==3)
       document.getElementById("doneBtn").disabled = false; 
	
	/*$("#filterattr").empty();
	var select3 = document.getElementById('filterattr');
	 opt = document.createElement('option');
		opt.text = "Attribute";
		opt.value = "----";
		select3.appendChild(opt); 
	for (var i = 0; i<predList.length; i++){
		if(new String(dimension1).valueOf()!=new String(predList[i]).valueOf())
		{
		var opt = document.createElement('option');
		opt.text = predList[i];
		opt.value = predList[i];
		//opt.innerHTML = yList[i];
		select3.appendChild(opt);
	   // console.log(predList[i]);
		}
	}

 $("#filterattr").selectpicker('refresh');
 document.getElementById("filterattr").options[0].disabled = true;
$("#filterattr").selectpicker('refresh');
	
	$("#filterValue").empty();
  var select4 = document.getElementById('filterValue');
   opt = document.createElement('option');
      opt.text = "Value";
      opt.value = "0";
      select4.appendChild(opt); 
  $("#filterValue").selectpicker('refresh');
	filter = "",
  filterValue = "";
	
    */
  });

 $('#yAxis').on('change', function(){
    dimension2 = $(this).find("option:selected").val();
    //console.log(dimension2);
    ychange=1;

    
	/*$("#filterattr").empty();
	var select3 = document.getElementById('filterattr');
	 opt = document.createElement('option');
		opt.text = "Attribute";
		opt.value = "0";
		select3.appendChild(opt); 
	for (var i = 0; i<predList.length; i++){
		if((new String(dimension1).valueOf()!=new String(predList[i]).valueOf()) && (new String(dimension2).valueOf()!=new String(predList[i]).valueOf()))
		{
		var opt = document.createElement('option');
		opt.text = predList[i];
		opt.value = predList[i];
		//opt.innerHTML = yList[i];
		select3.appendChild(opt);
	   // console.log(predList[i]);
		}
	}

 $("#filterattr").selectpicker('refresh');
 document.getElementById("filterattr").options[0].disabled = true;
$("#filterattr").selectpicker('refresh');
	
	$("#filterValue").empty();
  var select4 = document.getElementById('filterValue');
   opt = document.createElement('option');
      opt.text = "Value";
      opt.value = "0";
      select4.appendChild(opt); 
  $("#filterValue").selectpicker('refresh');
	filter = "",
  filterValue = "";*/

    if((xchange+ychange+measchange)==3)
	     document.getElementById("doneBtn").disabled = false; 
  });
  
  $('#measType').on('change', function(){
    measure = $(this).find("option:selected").val();
    //console.log(measure);
    measchange=1;

    if((xchange+ychange+measchange)==3)
       document.getElementById("doneBtn").disabled = false; 
  });
  
  
/*$('#filterattr').on('change', function(){
    filter = $(this).find("option:selected").val();
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
    if(String(filter) != String("0"))
    	loadPredicateColumns();
    else
    	$("#filterValue").selectpicker('refresh');
    
  });

  $('#filterValue').on('change', function(){
    filterValue = $(this).find("option:selected").val();
    if((xchange+ychange+measchange)==3)
    	document.getElementById("doneBtn").disabled = false;
    
  });*/
  

$('.btn-default').on('pressed', function(){
    console.log("pressed");
    //if dataset changed update the x-axis and y-axis options
  });


var filterCounter = 1;

/*$('#addFilters').click(function() {
  if (document.getElementById('filter1').style.display == "none"){
    document.getElementById('filter1').style.display = "block";
    document.getElementById('addFilters').style.display = "none";
  } else{
    var original = document.getElementById('filter1' );
    var clone = original.cloneNode(true); // "deep" clone
    clone.id = "filter" + ++filterCounter; // there can only be one element with an ID
    clone.onclick = duplicate; // event handlers are not cloned
    original.parentNode.appendChild(clone);
    console.log("duplicate");
  }
  //document.getElementById('addFilters').style.display = "none";
});*/

$('#clear').click(function() {
  $(this).parent().parent().parent()[0].style.display = "none";
  filter = "",
  filterValue = "";
  document.getElementById('addFilters').style.display = "block";
  console.log($(this).parent().parent().parent()[0])
});
