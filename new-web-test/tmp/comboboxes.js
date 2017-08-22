var schema = ['Gender','Year','Major','GPA','FamilyIncome','Age','NoOfCourses','Country','Id'];
var schemax = ["NoOfCourses", "Year"];
var schemay = ["GPA"];
var predicates = []
//Schema for now is a static variable, should be replaced by backend data

//Script For the comboboxes. Can refactor to separate js file later.
var ms_operator;
var ms_attribute;
var ms_z_axis;
var ms_outlier;
var formdata; 
var y_aggr_ms;

var xmin=0	   
var xmax=10;
var ymin=0;
var ymax=10;

function Query(method,Yaxis,Xaxis,groupBy,aggrFunc,aggrVar,outlierCount,dataX,dataY) {
	this.method = method;
	this.yAxis = Yaxis;
	this.xAxis = Xaxis;
	this.groupBy = groupBy;
	this.aggrFunc = aggrFunc;
	this.aggrVar = aggrVar;
	this.outlierCount = outlierCount;
	this.dataX = dataX;
	this.dataY = dataY;
	this.sketchPoints=null;
	this.outputNormalized=false;
}


function add_operator(num){
    ms_operator = $('#pred_cond'+num).magicSuggest({
    placeholder: 'Select Operator',
    allowFreeEntries: false,
    data: ['=','!=','>','<','>=','<=','BETWEEN','LIKE','IN'],
    maxSelection: 1,
    required: true,
    resultAsString: true
    });
}
function add_attribute(num){
    ms_attribute = $('#pred_col'+num).magicSuggest({
    placeholder: 'Select Attribute',
    allowFreeEntries: false,
    data:predicates,
    maxSelection: 1,
    required: true,
    resultAsString: true
    });
}


function add_z_axis_combobox(){
    ms_z_axis = $('#z_axis_combobox').magicSuggest({
    placeholder: 'Select Attribute',
    allowFreeEntries: false,
    data: schema,
    maxSelection: 1,
    required: true,
    resultAsString: true
    });
    $('#z_axis_combobox').hide();
}

function add_outlier_count_combobox(){
    ms_outlier = $('#outlier_count_combobox').magicSuggest({
    placeholder: 'Select Number of Outliers',
    allowFreeEntries: false,
    data: ['1','2','3','4','5','6'],
    maxSelection: 1,
    required: true,
    resultAsString: true
    });
    $('#outlier_count_combobox').hide();
}

var ms_data = $('#data_combobox').magicSuggest({
	  placeholder: 'Select DataSet',
	  allowFreeEntries: false,
	  data: ['student','income'],
	  maxSelection: 1,
	  required: true,
	  resultAsString: true
	  });

$(ms_data).on(
	    'selectionchange', function(){
	       //alert(this.getValue());
	       if(this.getValue()==''){
	         $('#z_axis_combobox').hide();
	       }
	       else{
	          var interfaceFormData=getInterfaceFormData(ms_data.getValue()[0]);
	          console.log(interfaceFormData);
	         
	       }
	    });

var ms_method = $('#method_combobox').magicSuggest({
  placeholder: 'Select Method',
  allowFreeEntries: false,
  data: ['Outlier','SimilaritySearch','RepresentativeTrends','DissimilaritySearch'],
  maxSelection: 1,
  required: true,
  resultAsString: true
  });

$(ms_method).on(
	    'selectionchange', function(){
	       //alert(this.getValue());
	       if(this.getValue()==''){
	         $('#z_axis_combobox').hide();
	       }
	       else{
	         $('#z_axis_combobox').show();
	       }
	       if(this.getValue()=='Outlier'){
	         $('#outlier_count_combobox').show();
	       }      
	       else{
	         $('#outlier_count_combobox').hide();
	       }
	       if(this.getValue()=='SimilaritySearch' || this.getValue()=='DissimilaritySearch' ){
	          showCanvas();
	        
	        }
	       else{
	          hideCanvas();
	        
	       }
	    });

y_aggr_ms = $('#y_aggregate_combobox').magicSuggest({
    placeholder: 'Select Aggregate',
    allowFreeEntries: false,
    data: ['Average','Sum'],
    maxSelection: 1,
    required: true,
    resultAsString: true,
    maxSelectionRenderer: function(data){ return "One Aggregate Only";}
    });


var y_ms = $('#y_combobox').magicSuggest({
    placeholder: 'Select Y axis',
    allowFreeEntries: false,
    data: schemay,
    maxSelection: 1,
    required: true,
    resultAsString: true,
    maxSelectionRenderer: function(data){return "One Column Only";}
    });

$(y_ms).on(
	    'selectionchange', function(){
	       //alert(this.getValue());
	    	   console.log("in y");
	       if(y_ms.getValue()!='' && x_ms.getValue()!=''){
	    	   console.log("in y in");
	    	   ymetadata=formdata.yAxisColumns[y_ms.getValue()[0]];
	    	   xmetadata=formdata.xAxisColumns[x_ms.getValue()[0]];    	   
               xmin=xmetadata.min;	   
	    	   xmax=xmetadata.max;
	    	   ymin=ymetadata.min
	    	   ymax=ymetadata.max;
                  changeScaleBlankChart(xmin,xmax,ymin,ymax);
	    	 //    changeScaleBlankChart(1,10,2.5,3.5)

	       }
      
	    });

var x_ms = $('#x_combobox').magicSuggest({
      placeholder: 'Select X axis',
      allowFreeEntries: false,
      data: schemax,
      maxSelection: 1,
      required: true,
      resultAsString: true,
      maxSelectionRenderer: function(data){return "One Column Only";}
      });

 $(x_ms).on(
	    'selectionchange', function(){
	       //alert(this.getValue());
	    	   console.log("in x ");
	       if(y_ms.getValue()!='' && x_ms.getValue()!=''){
	    	   console.log("in x in");
			   console.log(formdata.yAxisColumns);
			   ycol=formdata.yAxisColumns
			   console.log(ycol["GPA"]);
	    	   ymetadata=formdata.yAxisColumns[y_ms.getValue()[0]];
	    	   xmetadata=formdata.xAxisColumns[x_ms.getValue()[0]];  
	    	   xmin=xmetadata.min;	   
	    	   xmax=xmetadata.max;
	    	   ymin=ymetadata.min
	    	   ymax=ymetadata.max;
	    	   changeScaleBlankChart(xmin,xmax,ymin,ymax);
	    	//   changeScaleBlankChart(5,10,1,3.5)

	       }
      
	    });
 
/* var output_type_ms = $('#output_type_combobox').magicSuggest({
     placeholder: 'Select Output Type',
     allowFreeEntries: false,
     data: ['Normalized','Unnormalized'],
     maxSelection: 1,
     required: true,
     resultAsString: true,
     maxSelectionRenderer: function(data){return "One Column Only";}
     });

*/


  //Function for new form submit using magicsuggest results
  function onSubmit(){
    removeViews();
    if(ms_method.getValue()=='Outlier' || ms_method.getValue()=='RepresentativeTrends'){
    	console.log('submit called');
        getVegaOutlierData();
    }
    else if(ms_method.getValue()=="SimilaritySearch"|| ms_method.getValue()=="DissimilaritySearch"){
    	trendAnalysis();
    }
  }
  //This function should properly format the input from user selected boxes to
  //The proper values to send to back end.
  function getVegaOutlierData(){
    var query = "select ";
    var aggregate = "";
    if(y_aggr_ms.getValue()=='Average'){
      aggregate = "avg(" + y_ms.getValue() + ")";
    }
    /*query += ms_z_axis.getValue() + "," + x_ms.getValue() + "," +
      aggregate + " from db.ontime group by " + ms_z_axis.getValue() +
       "," + x_ms.getValue(); */
    console.log(ms_method.getValue(),ms_method.getValue()[0]);
    var query = new Query(ms_method.getValue()[0],y_ms.getValue()[0],x_ms.getValue()[0],ms_z_axis.getValue()[0],y_aggr_ms.getValue()[0],y_ms.getValue()[0],parseInt(ms_outlier.getValue()),[],[]);
    var query = new Query(ms_method.getValue()[0],y_ms.getValue()[0],x_ms.getValue()[0],ms_z_axis.getValue()[0],y_aggr_ms.getValue()[0],y_ms.getValue()[0],parseInt(ms_outlier.getValue()),[],[]);
   /*  if(output_type_ms.getValue()[0]=="Normalized"){
    	 query.outputNormalized=true;
     }
   */
    
    // var outlier_count = parseInt(ms_outlier.getValue());
    //var method="2phase";
    //alert(query);
    getData(query);
  }

  function getSimilarTrendData(){
	     var query = new Query(ms_method.getValue()[0],y_ms.getValue()[0],x_ms.getValue()[0],
	      ms_z_axis.getValue()[0],y_aggr_ms.getValue()[0],y_ms.getValue()[0],parseInt(ms_outlier.getValue()), [], []);
	   /*  if(output_type_ms.getValue()[0]=="Normalized"){
	    	 query.isOutputNormalized=true;
	     }*/
	     return query;

	  }

function showCanvas(){
	   document.getElementById("myCanvas").style.display = "block";
	    document.getElementById("tools_sketch").style.display = "block";
	   
	}
	  function hideCanvas(){
	    document.getElementById("myCanvas").style.display = "none";
	    document.getElementById("tools_sketch").style.display = "none";
	  }
	 


 function processFormData(interfaceFormData){
	 formdata=interfaceFormData;
	 console.log(interfaceFormData.xAxisColumns);
	 schemax=[]
	 schemay=[]
	 schema=[]
	 predicates=[]
	 for(xAxis in interfaceFormData.xAxisColumns){
		schemax.push(xAxis); 
	 }
	 
     
	 for(yAxis in interfaceFormData.yAxisColumns){
		 schemay.push(yAxis);
	 }
	 
     for(zAxis in interfaceFormData.zAxisColumns){
		 schema.push(zAxis);
	 }
     

     for(predicate in interfaceFormData.predicateColumns){
		 predicates.push(predicate);
	 }
     
     y_ms.setData(schemay);
     x_ms.setData(schemax);
     ms_z_axis.setData(schema);
     predicates.setData(schema);
     
     
     
  //   changeScaleBlankChart(1,10,2.5,3.5)

     	 
 }