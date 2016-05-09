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
//dataset is the new dataset that the user has selected
$('#dataset').on('change', function(){
	dataset = $(this).find("option:selected").val();
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
    
  renderSubmit();
    //if dataset changed, update the x-axis and y-axis options
  });
  
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
	
	$("#filterattr").empty();
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
	
	
	
    if((xchange+ychange+measchange)==3)
       document.getElementById("doneBtn").disabled = false; 
  });

 $('#yAxis').on('change', function(){
    dimension2 = $(this).find("option:selected").val();
    //console.log(dimension2);
    ychange=1;

	$("#filterattr").empty();
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
  
   //$('#sampType').on('change', function(){
    //samplingType = $(this).find("option:selected").val();
    //console.log(samplingType);
    //if (samplingType == "Without Range"){
		//samplingType = "without";
    //}
	//else if (samplingType == "Without Range"){
		//samplingType = "with";
    //}
  //});
  
$('#filterattr').on('change', function(){
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
    
  });
  

$('.btn-default').on('pressed', function(){
    console.log("pressed");
    //if dataset changed update the x-axis and y-axis options
  });


var filterCounter = 1;

function duplicate() {

}


$('#addFilters').click(function() {
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
});

$('#clear').click(function() {
  $(this).parent().parent().parent()[0].style.display = "none";
  filter = "",
  filterValue = "";
  document.getElementById('addFilters').style.display = "block";
  console.log($(this).parent().parent().parent()[0])
});