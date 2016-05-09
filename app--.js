  var java = require("java");
  var io = require("socket.io");
  var express = require("express");
  var hbs = require("express-hbs");
  var connectAssets = require("connect-assets");
  var fs = require('fs');
  var connect = require('connect');
  var serveStatic = require('serve-static'); 
  var querystring = require('querystring');
  var logger = require('morgan');
  var url= require('url');

  var commandLineArgs = require('minimist')(process.argv.slice(2));

  if (commandLineArgs._[0] === "help") {
    console.info("help: display help");
    console.info("--port n: run on port n");
    console.info("--debug n: run java in debug mode, connect with Eclipse on port n");
    process.exit(0);
  }

  java.classpath.push("lib/json-simple-1.1.1.jar");
  java.classpath.push("lib/postgresql-9.2-1000.jdbc4.jar");
  java.classpath.push("lib/py4j0.8.jar");
  java.classpath.push("lib/SizeOf.jar");
  java.classpath.push("lib/jackson-annotations-2.3.0.jar");
  java.classpath.push("lib/jackson-core-2.3.0.jar");
  java.classpath.push("lib/jackson-databind-2.3.0.jar");
  java.classpath.push("lib/guava-15.0.jar");
  java.classpath.push("lib/commons-math3-3.4.1.jar");
  java.classpath.push("lib/javaml-0.1.5.jar");
  java.classpath.push("lib/hamcrest-core-1.3.jar");
  java.classpath.push("lib/jackson-core-asl-1.8.5.jar");
  java.classpath.push("lib/jackson-mapper-asl-1.8.5.jar");
  java.classpath.push("lib/JavaEWAH-1.0.0.jar");
  java.classpath.push("lib/opencsv-3.2.jar");
  java.classpath.push("bin/");

  java.options.push("-Xmx14g");

  // read debug command line option, and if it's present enable debug mode
  if (commandLineArgs.debug) {
    var debugPort = commandLineArgs.debug;

    java.options.push("-Xdebug");
    java.options.push("-agentlib:jdwp=transport=dt_socket,address=" +
      debugPort + ",server=y,suspend=n");
  }

  var viz =  java.newInstanceSync("api.Visualization");
  var approxHeatMap = java.newInstanceSync("api.ApproxHeatMap");
  var approxHisto = java.newInstanceSync("api.ApproxHisto");
  
  
  //var iHisto = java.newInstanceSync("approxHeatMap.IHisto");
  var groups = 0;
  var vizType = -1;
  //  console.log(vde.sumSync(1,1));

  //vde.loadOutlierDataSync();
  console.log("here");
  var app = connect()
  .use(logger())
  .use(serveStatic('new-web-test')) //new-web-test
  .use(function(request, response) {
    request.socket.setTimeout(1000000); // ms
    var urlObj = url.parse(request.url);
    var pathname = urlObj.pathname;
    console.log(pathname);

    
    function sendResponse(body) {
      response.writeHead(200, {
        'Content-Type': 'text/json',
        'Content-Length': body.length,
      });
	  //console.log(body.length);
      response.write(body);
      response.end();
    }

    if (pathname == '/storeResponseTrend') {
    
        
      console.log("in storeResponseTrend");
      //console.log(urlObj);
      var index = decodeURI(urlObj.query);
      console.log(index,index.length);
      var jsonobj = JSON.parse(index);
      var prevarr = jsonobj.prev;
      var currarr = jsonobj.curr;
      
      var fullPath = __dirname; 
    // console.log(fullPath);
      //fullPath+"/new-web-test/trend_p_flight.csv"
      var file = fs.createWriteStream(fullPath+"/new-web-test/trend_p_flight.csv");
      file.on('error', function(err) { console.log("file write error"); });
      file.write("q,t\n");
      for(var i=0;i<prevarr.length;i++)
      {
        if(prevarr[i] < currarr[i])
          file.write((i+1)+","+prevarr[i]+"\n");
        else
          file.write((i+1)+","+currarr[i]+"\n");
      }
      file.end();

      var output = "";
      sendResponse(output);
    
      
     }

     if (pathname == '/storeResponseHeat') {
    
        
       console.log("in storeResponseHeat");
      //console.log(urlObj);
      var index = decodeURI(urlObj.query);
      console.log(index,index.length);
      
      var jsonobj = JSON.parse(index);
      var prevarr = jsonobj.prev;
      var currarr = jsonobj.curr;
     // console.log(arr);
      var fullPath = __dirname; 
    // console.log(fullPath);
      //fullPath+"/new-web-test/"
      var file = fs.createWriteStream(fullPath+"/new-web-test/heat_p_flight.csv");
      file.on('error', function(err) { console.log("file write error"); });
      file.write("q,t\n");
      for(var i=0;i<prevarr.length;i++)
      {
        if(prevarr[i] < currarr[i])
          file.write((i+1)+","+prevarr[i]+"\n");
        else
          file.write((i+1)+","+currarr[i]+"\n");
      }
      file.end();

      var output = "";
      sendResponse(output);
    
      
     }
	
	if (pathname == '/setViz') {
		
	  var type = decodeURI(urlObj.query);	  
	  console.log("in set viz:"+type);
	  //sendResponse(groups);
	  
	  if(type==1)
	  {
		  vizType = 1;
		  approxHisto.setDbSetsSync(viz.dbSets);
		  
	  }
	  else if(type==2)
	  {
		  vizType = 2;
		  approxHeatMap.setDbSetsSync(viz.dbSets);
		  
	  }


      
     }
	
	if (pathname == '/getgroups') {
		
	  	  
	  console.log("in get get groups");
	  sendResponse(groups);

      
     }
	 
	 if (pathname == '/getiterativedata1') {
		
	  	  
	  console.log("in get iterative data1");
	  //console.log(urlObj);
	  var index = decodeURI(urlObj.query);
	  console.log(index);
	  
	  if(vizType == 1)
		var output = approxHisto.genHistoSync(index,"0");  
	  else if(vizType == 2)
		var output = approxHeatMap.genHeatMapSync(index,"0");
	  //console.log(output,length);
	  sendResponse(output);
	  
      
     }
	 
	 if (pathname == '/getiterativedata2') {
		
	  	  
	  console.log("in get iterative data2");
	  //console.log(urlObj);
	  var index = decodeURI(urlObj.query);
	  console.log(index);
	  if(vizType == 1)
		var output = approxHisto.genHistoSync(index,"0");  
	  else if(vizType == 2)
		var output = approxHeatMap.genHeatMapSync(index,"1");
	  console.log(output.length);
	  sendResponse(output);
	  
      
     }
    
  /*  if (pathname == '/getformdata') {
    	console.log(urlObj);
        console.log(urlObj.query);
        var query = decodeURI(urlObj.query);
		console.log("in load data");
		
		
		groups = approxHeatMap.loadDataSync(query);
        //var output = vde.getInterfaceFomDataSync(query);
        //console.log(groups);
        sendResponse(groups);
       }*/
	   

   
   if (pathname == '/getloaddata') {
     // console.log(urlObj);
       // console.log(urlObj.query);
        var query = decodeURI(urlObj.query);
    console.log("in load data:"+vizType);
    
    if(vizType == 1)
		groups = approxHisto.loadDataSync(query);  
	  else if(vizType == 2)
		groups = approxHeatMap.loadDataSync(query);
        //var output = vde.getInterfaceFomDataSync(query);
        //console.log(groups);
        sendResponse(groups);
       } 
	   
	   if (pathname == '/getpreddata') {
      console.log(urlObj);
        console.log(urlObj.query);
        var query = decodeURI(urlObj.query);
    console.log("in pred data");
    
    if(vizType == 1)
		groups = approxHisto.loadPredicateDataSync(query);  
	  else if(vizType == 2)
		groups = approxHeatMap.loadPredicateDataSync(query);
        //var output = vde.getInterfaceFomDataSync(query);
        //console.log(groups);
        sendResponse(groups);
       } 

      if (pathname == '/getaxesdata') {
      console.log(urlObj);
        console.log(urlObj.query);
        var query = decodeURI(urlObj.query);
    console.log("in axes data");
    
    if(vizType == 1)
		groups = approxHisto.loadAxesDataSync(query);  
	  else if(vizType == 2)
		groups = approxHeatMap.loadAxesDataSync(query);
        //var output = vde.getInterfaceFomDataSync(query);
        //console.log(groups);
        sendResponse(groups);
       } 
           
	if (pathname == '/resetdata') {
    	
		console.log("in reset data");
		
		if(vizType == 1)
		approxHisto.resetSync();  
	  else if(vizType == 2)
			approxHeatMap.resetSync();
        //var output = vde.getInterfaceFomDataSync(query);
        //console.log(groups);
        sendResponse("{\"nothing\":1}");
       }
	   
	   
  })
  .listen(8080);
// .listen(8999, '128.174.244.56');
