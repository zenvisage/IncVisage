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
var dataset = "airline";
var dimension1 = "Month";
var dimension2 = "DayOfMonth";
var measure = "ArrDelay";
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
		document.getElementById("loadbtn").disabled = false;
		document.getElementById("doneBtn").disabled = true;
	}
	else 
	{	
		document.getElementById("loadbtn").disabled = true;
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
if (dataset == "NYC Taxi"){
	tmpDataset=dataset;
    dataset = "taxitrip2013";
	
  }
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
		opt.value = "----";
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
    
    loadPredicateColumns();
    
  });

  $('#filterValue').on('change', function(){
    filterValue = $(this).find("option:selected").val();
    
  });
  

$('.btn-default').on('pressed', function(){
    console.log("pressed");
    //if dataset changed update the x-axis and y-axis options
  });