 var ms_data = $('#data_combobox').magicSuggest({
 placeholder: 'Select DataSet',
 allowFreeEntries: false,
 data: ['Airline','Sensor'],
 maxSelection: 1,
 required: true,
 resultAsString: true
 });

var dataset = "airline";
var dimension = "Day";
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

    //if dataset changed, update the x-axis and y-axis options
  });
  
 $('#xAxis').on('change', function(){
    dimension = $(this).find("option:selected").val();
  //  console.log(dimension);
    xchange=1;
	
	var select = document.getElementById('xAxis');
		var value = select.options[select.selectedIndex].text;
		
		
		
	
	
//$("#yAxis option[value='temp2']").remove();
$("#filterattr").empty();
var select3 = document.getElementById('filterattr');
 opt = document.createElement('option');
    opt.text = "Attribute";
    opt.value = "----";
    select3.appendChild(opt); 
for (var i = 0; i<predList.length; i++){
	if(String(predList[i]).valueOf() != String(value).valueOf())
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
	
	
    if((xchange+ychange)==2)
       document.getElementById("doneBtn").disabled = false; 
  });

 $('#yAxis').on('change', function(){
    measure = $(this).find("option:selected").val();
    //console.log(measure);
    ychange=1;

    if((xchange+ychange)==2)
	     document.getElementById("doneBtn").disabled = false; 
  });
  
  $('#filterattr').on('change', function(){
    filter = $(this).find("option:selected").val();
    
    loadPredicateColumns();
    
  });

  $('#filterValue').on('change', function(){
    filterValue = $(this).find("option:selected").val();
    
  });
  
   $('#sampType').on('change', function(){
    samplingType = $(this).find("option:selected").val();
  //  console.log(samplingType);
    if (samplingType == "Without Range"){
		samplingType = "without";
    }
	else if (samplingType == "Without Range"){
		samplingType = "with";
    }
  });
  


$('.btn-default').on('pressed', function(){
    //console.log("pressed");
    //if dataset changed update the x-axis and y-axis options
  });