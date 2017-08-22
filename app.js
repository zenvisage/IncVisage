  var incvisage = require("./needletail-code-base/build/Release/incvisage");
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
  var path = require('path');
  var commandLineArgs = require('minimist')(process.argv.slice(2));

  if (commandLineArgs._[0] === "help") {
    console.info("help: display help");
    console.info("--port n: run on port n");
    console.info("--debug n: run java in debug mode, connect with Eclipse on port n");
    process.exit(0);
  }



  // read debug command line option, and if it's present enable debug mode
  if (commandLineArgs.debug) {
    var debugPort = commandLineArgs.debug;

   // java.options.push("-Xdebug");
  //  java.options.push("-agentlib:jdwp=transport=dt_socket,address=" +
   //   debugPort + ",server=y,suspend=n");
  }

  
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

    if (pathname == '/getViz') {
    
        var type = decodeURI(urlObj.query);   
        console.log("in get viz:"+type);
        //sendResponse(groups);
        var dataset = JSON.parse(type);
        
        if(dataset.toString()=="Flight")
        {
          console.log("randomization for flight");
          if(vizType==1)
          {
            fs.readFile('/home/srahman7/incvisage-cpp/ta', 'utf8', function (err,d) {
              if (err) {
                return console.log(err);
              }
              console.log(d);

              if(d=="1")
              {
                var vizJson = {"viz":1};

                fs.writeFile("/home/srahman7/incvisage-cpp/ta", "2", function(err) {
                    if(err) {
                        return console.log(err);
                    }

                    
                }); 
                sendResponse(JSON.stringify(vizJson));
              }
              else if(d=="2")
              {
                var vizJson = {"viz":2};

                fs.writeFile("/home/srahman7/incvisage-cpp/ta", "1", function(err) {
                    if(err) {
                        return console.log(err);
                    }

                    
                }); 
                sendResponse(JSON.stringify(vizJson));
              }
            });
          }
          else if(vizType==2)
          {
            fs.readFile('/home/srahman7/incvisage-cpp/ha', 'utf8', function (err,d) {
              if (err) {
                return console.log(err);
              }
              console.log(d);

              if(d=="1")
              {
                var vizJson = {"viz":1};

                fs.writeFile("/home/srahman7/incvisage-cpp/ha", "2", function(err) {
                    if(err) {
                        return console.log(err);
                    }

                    
                }); 
                sendResponse(JSON.stringify(vizJson));
              }
              else if(d=="2")
              {
                var vizJson = {"viz":2};

                fs.writeFile("/home/srahman7/incvisage-cpp/ha", "1", function(err) {
                    if(err) {
                        return console.log(err);
                    }

                    
                });

                sendResponse(JSON.stringify(vizJson)); 
              }
            });
          }
          else if(vizType==3)
          {
            fs.readFile('/home/srahman7/incvisage-cpp/tb', 'utf8', function (err,d) {
              if (err) {
                return console.log(err);
              }
              console.log(d);

              if(d=="1")
              {
                var vizJson = {"viz":1};

                fs.writeFile("/home/srahman7/incvisage-cpp/tb", "2", function(err) {
                    if(err) {
                        return console.log(err);
                    }

                    
                }); 

                sendResponse(JSON.stringify(vizJson));
              }
              else if(d=="2")
              {
                var vizJson = {"viz":2};

                fs.writeFile("/home/srahman7/incvisage-cpp/tb", "1", function(err) {
                    if(err) {
                        return console.log(err);
                    }

                    
                }); 

                sendResponse(JSON.stringify(vizJson));
              }
            });
          }
          else
          {
            fs.readFile('/home/srahman7/incvisage-cpp/hb', 'utf8', function (err,d) {
              if (err) {
                return console.log(err);
              }
              console.log(d);

              if(d=="1")
              {
                var vizJson = {"viz":1};

                fs.writeFile("/home/srahman7/incvisage-cpp/hb", "2", function(err) {
                    if(err) {
                        return console.log(err);
                    }

                    
                }); 
                sendResponse(JSON.stringify(vizJson));
              }
              else if(d=="2")
              {
                var vizJson = {"viz":2};

                fs.writeFile("/home/srahman7/incvisage-cpp/hb", "1", function(err) {
                    if(err) {
                        return console.log(err);
                    }

                    
                }); 

                 sendResponse(JSON.stringify(vizJson));
              }
            });
          }
        }
        else
        {
          console.log("randomization for sensor");
          var vizJson = {"viz":vizType};
          sendResponse(JSON.stringify(vizJson));
        }
        
        

       
    }
    if (pathname == '/setViz') {
    
        var type = decodeURI(urlObj.query);   
        console.log("in set viz:"+type);
        //sendResponse(groups);
        var vizJson = {};
        if(type==1)
        {
          vizType = 1;

          vizJson = {"viz":vizType};
          var data = incvisage.setVizType(vizJson);
          console.log("Viz Types = " + data.viz);
          
        }
        else if(type==2)
        {
          vizType = 2;
          
          vizJson = {"viz":vizType};
          var data = incvisage.setVizType(vizJson);
          console.log("Viz Type = " + data.viz);
          
        }
        else if(type==3)
        {
          vizType = 3;

          vizJson = {"viz":vizType};
          var data = incvisage.setVizType(vizJson);
          console.log("Viz Types = " + data.viz);
          
        }
        else
        {
          vizType = 4;
          
          vizJson = {"viz":vizType};
          var data = incvisage.setVizType(vizJson);
          console.log("Viz Type = " + data.viz);
          
        }

        console.log("incvisage viz:"+incvisage.getVizType());

        
      
     }

     
     if (pathname == '/getloaddata') {
     // console.log(urlObj);
       // console.log(urlObj.query);
        var query = decodeURI(urlObj.query);
        console.log("in load data:"+query);

        var groups = incvisage.loadDataset(JSON.parse(query));  
        console.log(groups);
        
        sendResponse(JSON.stringify(groups));
     } 

     if (pathname == '/getpreddata') {
        console.log(urlObj);
        console.log(urlObj.query);
        var query = decodeURI(urlObj.query);
        console.log("in pred data");
    
        groups = incvisage.loadPredicateData(JSON.parse(query));
        //var output = vde.getInterfaceFomDataSync(query);
        //console.log(groups);
        sendResponse(JSON.stringify(groups));
     }

     if (pathname == '/getaxesdata') {
        console.log(urlObj);
        console.log(urlObj.query);
        var query = decodeURI(urlObj.query);
        console.log("in axes data");
    
        groups = incvisage.loadAxesData(JSON.parse(query));  
        
        //var output = vde.getInterfaceFomDataSync(query);
        //console.log(groups);
        sendResponse(JSON.stringify(groups));
     } 

     if (pathname == '/getiterativedata1') {
    
        
        console.log("in get iterative data1");
        //console.log(urlObj);
        var index = decodeURI(urlObj.query);
        console.log(index);
        
        var iteration = {"iter":JSON.parse(index)};
        console.log(iteration);

        if(vizType==1)
        {
          var output = incvisage.genChart(iteration);  
          var yrange = incvisage.getAxisrange(iteration);


          var result = {"sl":output,"y":yrange};
          //console.log(result);
          sendResponse(JSON.stringify(result));
        }
        else if(vizType==2)
        {
          var output = incvisage.genChart(iteration);  
          var yrange = incvisage.getAxisrange(iteration);
          
          if(index==1)
          {
            
            var cells = incvisage.getValidCells(iteration);

            
            var result = {"validCells":cells,"sl":output,"gl":"","y":yrange,"max":yrange.aMax,"min":yrange.aMin};
            //console.log(result);
            sendResponse(JSON.stringify(result));
          }
          else
          {
            var result = {"sl":output,"gl":"","y":yrange,"max":yrange.aMax,"min":yrange.aMin};
            //console.log(result);
            sendResponse(JSON.stringify(result));
          }
          
        }
        else if(vizType==3)
        {
          var output = incvisage.genChart(iteration);  
          var yrange = incvisage.getAxisrange(iteration);
          var isContinue = incvisage.toContinue(iteration);

          var result = {"sl":"","gl":output,"y":yrange,"continued":isContinue};
          //console.log(result);
          sendResponse(JSON.stringify(result));
        }
        else
        {
          var output = incvisage.genChart(iteration);  
          var yrange = incvisage.getAxisrange(iteration);
          var isContinue = incvisage.toContinue(iteration);
          
          if(index==1)
          {
            
            var cells = incvisage.getValidCells(iteration);

            var result = {"validCells":cells,"sl":"","gl":output,"y":yrange,"max":yrange.aMax,"min":yrange.aMin,"continued":isContinue};
            //console.log(result);
            sendResponse(JSON.stringify(result));
          }
          else
          {
            var result = {"sl":"","gl":output,"y":yrange,"max":yrange.aMax,"min":yrange.aMin,"continued":isContinue};
           // console.log(result);
            sendResponse(JSON.stringify(result));
          }
        }
        
    
      
   }
   
   if (pathname == '/getiterativedata2') {
    
        
      console.log("in get iterative data1");
      //console.log(urlObj);
      var index = decodeURI(urlObj.query);
      console.log(index);
      
      var output = incvisage.genChart(JSON.parse(index));  
      
      //console.log(output,length);
      sendResponse(JSON.stringify(output));
    
      
   }

   if (pathname == '/resetdata') {
      
    console.log("in reset data");
    
      var nada = {"nada":1};
      incvisage.reset(nada);
        //var output = vde.getInterfaceFomDataSync(query);
        //console.log(groups);
        sendResponse("{\"nothing\":1}");
     }

     if (pathname == '/storeResponseTrend') {
    
        
      console.log("in storeResponseTrend");
      //console.log(urlObj);
      var index = decodeURI(urlObj.query);
      console.log(index,index.length);
      var jsonobj = JSON.parse(index);
      var question = jsonobj.question;
      var answer = jsonobj.answer;
      var userans = jsonobj.userans;
      var prevarr = jsonobj.iter;
      var currarr = jsonobj.resp;
      var correctarr = jsonobj.correct;
      var accuracy = jsonobj.accuracy;
      var points = jsonobj.score;
      
      var fullPath = __dirname; 
    // console.log(fullPath);
      //fullPath+"/new-web-test/trend_p_flight.csv"
      var currentdate = new Date(); 
      var userid = currentdate.getDate() + "-"
                + (currentdate.getMonth()+1)  + " " 
                + currentdate.getHours() + ":"  
                + currentdate.getMinutes();
 
      var file = fs.createWriteStream(fullPath+"/new-web-test/Qbank/trend_"+userid+".csv");
      file.on('error', function(err) { console.log("file write error"); });
     file.write("No,Q,Ans,User Ans,Itr,Time (sec), Correct, Accuracy, Points\n");
      for(var i=0;i<prevarr.length;i++)
      {
        file.write((i+1)+",\""+question[i]+"\","+answer[i]+","+userans[i]+","+prevarr[i]+","+currarr[i]+","+correctarr[i]+","+accuracy[i]+","+points[i]+"\n");
        
      }
      file.end();

      var output = "success";
      sendResponse(output);
    
      
     }

     if (pathname == '/storeResponseHeat') {
    
        
       console.log("in storeResponseHeat");
      //console.log(urlObj);
      var index = decodeURI(urlObj.query);
      console.log(index,index.length);
      
      var jsonobj = JSON.parse(index);

      var question = jsonobj.question;
      var answer = jsonobj.answer;
      var userans = jsonobj.userans;
      var prevarr = jsonobj.iter;
      var currarr = jsonobj.resp;
      var correctarr = jsonobj.correct;
      var accuracy = jsonobj.accuracy;
      var points = jsonobj.score;
     // console.log(arr);
      var fullPath = __dirname; 
    // console.log(fullPath);
      //fullPath+"/new-web-test/"
      var currentdate = new Date(); 
      var userid = currentdate.getDate() + "-"
                + (currentdate.getMonth()+1)  + " " 
                + currentdate.getHours() + ":"  
                + currentdate.getMinutes() ;
      var file = fs.createWriteStream(fullPath+"/new-web-test/Qbank/heat_"+userid+".csv");
      file.on('error', function(err) { console.log("file write error"); });
      file.write("No,Q,Ans,User Ans,Itr,Time (sec), Correct, Accuracy, Points\n");
      for(var i=0;i<prevarr.length;i++)
      {
        file.write((i+1)+",\""+question[i]+"\","+answer[i]+","+userans[i]+","+prevarr[i]+","+currarr[i]+","+correctarr[i]+","+accuracy[i]+","+points[i]+"\n");
        
      }
      file.end();

      var output = "success";
      sendResponse(output);
    
      
     }
     
     
  })
  .listen(8080);
// .listen(8999, '128.174.244.56');
/***

    if (pathname == '/storeResponseTrend') {
    
        
      console.log("in storeResponseTrend");
      //console.log(urlObj);
      var index = decodeURI(urlObj.query);
      console.log(index,index.length);
      var jsonobj = JSON.parse(index);
      var question = jsonobj.question;
      var answer = jsonobj.answer;
      var userans = jsonobj.userans;
      var prevarr = jsonobj.iter;
      var currarr = jsonobj.resp;
      var correctarr = jsonobj.correct;
      var accuracy = jsonobj.accuracy;
      var points = jsonobj.score;
      
      var fullPath = __dirname; 
    // console.log(fullPath);
      //fullPath+"/new-web-test/trend_p_flight.csv"
      var currentdate = new Date(); 
      var userid = currentdate.getDate() + "-"
                + (currentdate.getMonth()+1)  + " " 
                + currentdate.getHours() + ":"  
                + currentdate.getMinutes();
 
      var file = fs.createWriteStream(fullPath+"/new-web-test/trend_"+userid+".csv");
      file.on('error', function(err) { console.log("file write error"); });
     file.write("No,Q,Ans,User Ans,Itr,Time (sec), Correct, Accuracy, Points\n");
      for(var i=0;i<prevarr.length;i++)
      {
        file.write((i+1)+",\""+question[i]+"\","+answer[i]+","+userans[i]+","+prevarr[i]+","+currarr[i]+","+correctarr[i]+","+accuracy[i]+","+points[i]+"\n");
        
      }
      file.end();

      var output = "success";
      sendResponse(output);
    
      
     }

     if (pathname == '/storeResponseHeat') {
    
        
       console.log("in storeResponseHeat");
      //console.log(urlObj);
      var index = decodeURI(urlObj.query);
      console.log(index,index.length);
      
      var jsonobj = JSON.parse(index);

      var question = jsonobj.question;
      var answer = jsonobj.answer;
      var userans = jsonobj.userans;
      var prevarr = jsonobj.iter;
      var currarr = jsonobj.resp;
      var correctarr = jsonobj.correct;
      var accuracy = jsonobj.accuracy;
      var points = jsonobj.score;
     // console.log(arr);
      var fullPath = __dirname; 
    // console.log(fullPath);
      //fullPath+"/new-web-test/"
      var currentdate = new Date(); 
      var userid = currentdate.getDate() + "-"
                + (currentdate.getMonth()+1)  + " " 
                + currentdate.getHours() + ":"  
                + currentdate.getMinutes() ;
      var file = fs.createWriteStream(fullPath+"/new-web-test/heat_"+userid+".csv");
      file.on('error', function(err) { console.log("file write error"); });
      file.write("No,Q,Ans,User Ans,Itr,Time (sec), Correct, Accuracy, Points\n");
      for(var i=0;i<prevarr.length;i++)
      {
        file.write((i+1)+",\""+question[i]+"\","+answer[i]+","+userans[i]+","+prevarr[i]+","+currarr[i]+","+correctarr[i]+","+accuracy[i]+","+points[i]+"\n");
        
      }
      file.end();

      var output = "success";
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


***/
