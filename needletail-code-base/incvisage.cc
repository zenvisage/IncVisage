#include <node.h>
#include <v8.h>
#include <uv.h>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
//#include histo and heatmap.h
#include "sdb.h"
#include "schema.h"
#include "axes.h"
#include "trend.h"
#include "heat.h"
#include "resultobject.h"
#include "swift_index.h"
#include "swift_map.h"
#include "utils.h"
using namespace v8;

int vizType = -1;

int heat_xmax = 0;
int heat_xmin = 0;
int heat_ymin = 0;
int heat_ymax = 0;

Schema schema("/home/srahman7/incvisage-cpp/needletail-code-base/data/1/schema_1", 0);
SDB db("/home/srahman7/incvisage-cpp/needletail-code-base/data/1/sdb_1",schema);

bool old_swi = false;
bool new_swi = false;

const char* swi_fname = "swi_file";

SwiftMapFactory::map_type_t bmap_type = SwiftMapFactory::EWAH_BITMAP; 

Binner binner;

              
SwiftIndex swi(db, schema, binner, bmap_type,
      old_swi ? swi_fname : NULL);


Trend trend(swi);
Heat heat(swi);
std::string dataset;
//code for getvizType

void getVizType(const v8::FunctionCallbackInfo<v8::Value>& args) {
   
  Isolate* isolate = args.GetIsolate();
  Local<Number> retval = v8::Number::New(isolate, vizType);
  args.GetReturnValue().Set(retval);
}

//code for select viz type
void setVizType(const v8::FunctionCallbackInfo<v8::Value>& args) {

  Isolate* isolate = args.GetIsolate();

  Handle<Object> obj1 = Handle<Object>::Cast(args[0]);
  Handle<Value> val = obj1->Get(String::NewFromUtf8(isolate,"viz"));

  vizType = val->NumberValue();


  Local<Object> obj = Object::New(isolate);

  if (vizType == 1)
  {
    // load histo object
    obj->Set(String::NewFromUtf8(isolate, "viz"), 
                            String::NewFromUtf8(isolate, "Trend"));
  }
  else if(vizType==2)
  {
    //load heatmap object
    obj->Set(String::NewFromUtf8(isolate, "viz"), 
                            String::NewFromUtf8(isolate, "Heatmap"));
  }
  else if(vizType==3)
  {
    //load heatmap object
    obj->Set(String::NewFromUtf8(isolate, "viz"), 
                            String::NewFromUtf8(isolate, "Trend Base"));
  }
  else
  {
    //load heatmap object
    obj->Set(String::NewFromUtf8(isolate, "viz"), 
                            String::NewFromUtf8(isolate, "Heat Base"));
  }
  
  args.GetReturnValue().Set(obj);
}
//code for select dataset and load dim and measure
void loadDataset(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = args.GetIsolate();

  Handle<Object> obj1 = Handle<Object>::Cast(args[0]);
  Handle<Value> val = obj1->Get(String::NewFromUtf8(isolate,"dataset"));
  v8::String::Utf8Value str(val);
  dataset = std::string(*str);
  
  std::string dims;
  std::string meas;
  std::string filter;

  std::cout << "Dataset: " << dataset << std::endl;
  if(dataset == "flight")
  {
    //gen dims
      
    if(vizType==1 || vizType==3)
      dims = "Day";
    else
      dims = "Day Of Month,Day Of Week,Month,Week Of Year,Year";

    //gen measures

    meas = "Aircraft Delay (minutes),Arrival Delay (minutes),Carrier Delay (minutes),Departure Delay (minutes),NAS Delay (minutes),Security Delay (minutes),Taxi Out (minutes),Taxi In (minutes),Weather Delay (minutes)";

    //gen preds

    filter = "Carrier,Destination Airport,Destination City,Destination State,Origin Airport,Origin City,Origin State,Year";

    
    
    

  }
  else
  {
    //gen dims
    
    dims = "Day,Hour";

    //gen measures

    meas = "Humidity,Light,Temperature,Voltage";

    //gen preds

    filter = "Day,Hour";
  }
  
  

  Local<Object> obj = Object::New(isolate);

  if(vizType==1 || vizType==3)
  {
    trend.setMaps(dataset);

    obj->Set(String::NewFromUtf8(isolate, "dimension"), 
                              String::NewFromUtf8(isolate, dims.c_str()));
    obj->Set(String::NewFromUtf8(isolate, "measure"), 
                              String::NewFromUtf8(isolate, meas.c_str()));
    obj->Set(String::NewFromUtf8(isolate, "predicate"), 
                              String::NewFromUtf8(isolate, filter.c_str()));
  }
  else 
  {
    heat.setMaps(dataset);

    obj->Set(String::NewFromUtf8(isolate, "dimension1"), 
                              String::NewFromUtf8(isolate, dims.c_str()));
    obj->Set(String::NewFromUtf8(isolate, "dimension2"), 
                              String::NewFromUtf8(isolate, dims.c_str()));
    obj->Set(String::NewFromUtf8(isolate, "measure"), 
                              String::NewFromUtf8(isolate, meas.c_str()));
    obj->Set(String::NewFromUtf8(isolate, "predicate"), 
                              String::NewFromUtf8(isolate, filter.c_str()));
  }
  

  args.GetReturnValue().Set(obj);
}
//code for load axes data

//code for load predicate data
void loadPredicateData(const v8::FunctionCallbackInfo<v8::Value>& args) {
  
  //load filter attribute values
  Isolate* isolate = args.GetIsolate();

  Handle<Object> obj1 = Handle<Object>::Cast(args[0]);
  Handle<Value> val = obj1->Get(String::NewFromUtf8(isolate,"predicate"));
  v8::String::Utf8Value str(val);
  std::string predicate = std::string(*str);
  
  std::string filters;
  

  std::cout << "predicate: " << predicate << std::endl;

  // call individual predicate data according to viztype
  // also need a map for the filter attributes to some dictionary
  // set filter attribute within the object
  if(predicate.find("City") != std::string::npos)
  {
    filters = "Atlanta- GA,Chicago- IL,Denver- CO,Houston- TX,Los Angeles- CA,New York- NY,Oklahoma- OH";
  }
  else if(predicate.find("State") != std::string::npos)
  {

    filters = "AK,CA,FL,GA,IL,LA,MN,NY,OH,PA,TX";
  }
  else if(predicate.find("Airport") != std::string::npos)
  {
    filters = "ATL,DEN,DFW,JFK,LAX,LGA,ORD,SFO,CTL,LAS,PHX";
  }
  else if (predicate.find("Carrier") != std::string::npos)
  {
    filters = "AA,AS,CO,OH(1),UA,US";
  }
  else if (predicate.find("Year") != std::string::npos)
  {
    filters = "2003,2004,2005,2006,2007,2008,2009,2010,2011,2012,2013,2014,2015";
  }
  else if (predicate.find("Day") != std::string::npos)
  {
    filters = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38";
    //"59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96";
  }
  else if (predicate.find("Hour") != std::string::npos)
  {
    filters = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24";
  }
  


  Local<Object> obj = Object::New(isolate);

  obj->Set(String::NewFromUtf8(isolate, "predicate"), 
                            String::NewFromUtf8(isolate, filters.c_str()));
  
  args.GetReturnValue().Set(obj);
}

void loadAxesData(const v8::FunctionCallbackInfo<v8::Value>& args) {
  // depending on visualization call specific load axes data
  Isolate* isolate = args.GetIsolate();

  Handle<Object> obj1 = Handle<Object>::Cast(args[0]);
  
  Handle<Value> val = obj1->Get(String::NewFromUtf8(isolate,"measure"));
  v8::String::Utf8Value str1(val);
  std::string meas = std::string(*str1);

  val = obj1->Get(String::NewFromUtf8(isolate,"predicate"));
  v8::String::Utf8Value str2(val);
  std::string filter = std::string(*str2);

  val = obj1->Get(String::NewFromUtf8(isolate,"predicatevalue"));
  v8::String::Utf8Value str3(val);
  std::string filterVal = std::string(*str3);

  Axes axes;

  swi.clear();


  if(vizType==1 || vizType==3)
  {
    val = obj1->Get(String::NewFromUtf8(isolate,"dimension"));
    v8::String::Utf8Value str(val);
    std::string dim = std::string(*str);

    
    trend.loadAxesData(dim,meas,filter,filterVal);
   
    Local<Object> obj = Object::New(isolate);

    obj->Set(String::NewFromUtf8(isolate, "xMin"), 
                            v8::Number::New(isolate, trend.xMin));
    obj->Set(String::NewFromUtf8(isolate, "xMax"), 
                            v8::Number::New(isolate, trend.xMax));
    args.GetReturnValue().Set(obj);
  }
  else
  {
    val = obj1->Get(String::NewFromUtf8(isolate,"dimension1"));
    v8::String::Utf8Value str(val);
    std::string dim1 = std::string(*str);

    val = obj1->Get(String::NewFromUtf8(isolate,"dimension2"));
    v8::String::Utf8Value str1(val);
    std::string dim2 = std::string(*str1);


    heat.loadAxesData(dim1,dim2,meas,filter,filterVal);
    
    heat_xmin = axes.xMin;
    heat_xmax = axes.xMax;
    heat_ymin = axes.yMin;
    heat_ymax = axes.yMax;

    Local<Object> obj = Object::New(isolate);

    obj->Set(String::NewFromUtf8(isolate, "xMin"), 
                            v8::Number::New(isolate, heat.xMin));
    obj->Set(String::NewFromUtf8(isolate, "xMax"), 
                            v8::Number::New(isolate, heat.xMax));
    obj->Set(String::NewFromUtf8(isolate, "yMin"), 
                            v8::Number::New(isolate, heat.yMin));
    obj->Set(String::NewFromUtf8(isolate, "yMax"), 
                            v8::Number::New(isolate, heat.yMax));
    args.GetReturnValue().Set(obj);
  }


}
//get data

void pack_segment_result(v8::Isolate* isolate, v8::Local<v8::Object> & target, bool isSecond){
  if(!isSecond)
  {
    int seg_no = trend.split_seg;

    if(trend.split_seg==-1)
      seg_no = 0;

    int start = trend.groupMap[trend.firstNewSegment.get_start()];
    int end = trend.groupMap[trend.firstNewSegment.get_end()];
    float mean = trend.firstNewSegment.get_mean();

    //std::cout << seg_no << ",(" << start << "," << end << ") =>" << mean << std::endl;

    target->Set(String::NewFromUtf8(isolate, "n"), Integer::New(isolate, seg_no));
    target->Set(String::NewFromUtf8(isolate, "b"), Integer::New(isolate, start));
    target->Set(String::NewFromUtf8(isolate, "e"), Integer::New(isolate, end));
    target->Set(String::NewFromUtf8(isolate, "a"), Number::New(isolate, mean));
  }
  else
  {
    int seg_no = trend.split_seg+1;

    int start = trend.groupMap[trend.secondNewSegment.get_start()];
    int end = trend.groupMap[trend.secondNewSegment.get_end()];
    float mean = trend.secondNewSegment.get_mean();

    
    //std::cout << seg_no << ",(" << start << "," << end << ") =>" << mean << std::endl;

    target->Set(String::NewFromUtf8(isolate, "n"), Integer::New(isolate, seg_no));
    target->Set(String::NewFromUtf8(isolate, "b"), Integer::New(isolate, start));
    target->Set(String::NewFromUtf8(isolate, "e"), Integer::New(isolate, end));
    target->Set(String::NewFromUtf8(isolate, "a"), Number::New(isolate, mean));
  }
}

void pack_cell_result(v8::Isolate* isolate, v8::Local<v8::Object> & target, int cell_no){
  int quad_no = heat.split_quad;
  int xb = 0;
  int xe = 0;
  int yb = 0;
  int ye = 0;
  float mean = 0;

  if(cell_no==1)
  {
    if(heat.split_quad==-1)
      quad_no = 0;

    xb = heat.groupXMap[heat.firstNewQuadrant.get_xStart()];
    xe = heat.groupXMap[heat.firstNewQuadrant.get_xEnd()];
    yb = heat.groupYMap[heat.firstNewQuadrant.get_yStart()];
    ye = heat.groupYMap[heat.firstNewQuadrant.get_yEnd()];
    mean = heat.firstNewQuadrant.get_mean();

    
  }
  else if(cell_no==2)
  {
    quad_no = heat.split_quad+1;

    xb = heat.groupXMap[heat.secondNewQuadrant.get_xStart()];
    xe = heat.groupXMap[heat.secondNewQuadrant.get_xEnd()];
    yb = heat.groupYMap[heat.secondNewQuadrant.get_yStart()];
    ye = heat.groupYMap[heat.secondNewQuadrant.get_yEnd()];
    mean = heat.secondNewQuadrant.get_mean();

    
  }
  else if(cell_no==3)
  {
    quad_no = heat.split_quad+2;

    xb = heat.groupXMap[heat.thirdNewQuadrant.get_xStart()];
    xe = heat.groupXMap[heat.thirdNewQuadrant.get_xEnd()];
    yb = heat.groupYMap[heat.thirdNewQuadrant.get_yStart()];
    ye = heat.groupYMap[heat.thirdNewQuadrant.get_yEnd()];
    mean = heat.thirdNewQuadrant.get_mean();

    
  }
  else
  {
    quad_no = heat.split_quad+3;

    xb = heat.groupXMap[heat.fourthNewQuadrant.get_xStart()];
    xe = heat.groupXMap[heat.fourthNewQuadrant.get_xEnd()];
    yb = heat.groupYMap[heat.fourthNewQuadrant.get_yStart()];
    ye = heat.groupYMap[heat.fourthNewQuadrant.get_yEnd()];
    mean = heat.fourthNewQuadrant.get_mean();

   
  }

  target->Set(String::NewFromUtf8(isolate, "n"), Integer::New(isolate, quad_no));
  target->Set(String::NewFromUtf8(isolate, "xb"), Integer::New(isolate, xb));
  target->Set(String::NewFromUtf8(isolate, "xe"), Integer::New(isolate, xe));
  target->Set(String::NewFromUtf8(isolate, "yb"), Integer::New(isolate, yb));
  target->Set(String::NewFromUtf8(isolate, "ye"), Integer::New(isolate, ye));
  target->Set(String::NewFromUtf8(isolate, "a"), Number::New(isolate, mean));

  
  
}

void genChart(const v8::FunctionCallbackInfo<v8::Value>& args) {

  Isolate* isolate = args.GetIsolate();

  Handle<Object> obj1 = Handle<Object>::Cast(args[0]);
  Handle<Value> val = obj1->Get(String::NewFromUtf8(isolate,"iter"));

  int iteration = val->NumberValue();

  if(vizType==1)
  {
    trend.genHisto(iteration,db,schema);

    //std::cout << "result recived" << std::endl;

    Local<Array> result_list = Array::New(isolate);
    
    //std::cout << trend.split_seg << ",(" << trend.firstNewSegment.get_start() << "," << trend.firstNewSegment.get_end() << ") <=>" << trend.firstNewSegment.get_mean() << std::endl;
    Local<Object> result = Object::New(isolate);
    //std::cout << "send for packing1" << std::endl;
    pack_segment_result(isolate, result,false);
    result_list->Set(0, result);

    if(trend.split_seg != -1)
    {
      //std::cout << (trend.split_seg+1) << ",(" << trend.secondNewSegment.get_start() << "," << trend.secondNewSegment.get_end() << ") <=>" << trend.secondNewSegment.get_mean() << std::endl;
      Local<Object> result2 = Object::New(isolate);
      //std::cout << "send for packing2" << std::endl;
      pack_segment_result(isolate, result2,true);
      result_list->Set(1, result2);
    }
    
    

    // Return the list
    args.GetReturnValue().Set(result_list);
  }
  else if(vizType==2)
  {
    heat.genHeat(iteration,db,schema);

    Local<Array> result_list = Array::New(isolate);
    
    Local<Object> result = Object::New(isolate);
    
    pack_cell_result(isolate, result,1);
    result_list->Set(0, result);
    
    if(heat.split_quad==-1)
    {
      args.GetReturnValue().Set(result_list);
    }
    else if (heat.isOneCell==true)
    {
      // Return the list
      args.GetReturnValue().Set(result_list);
    }
    else
    {
      if(heat.isTwoSeg==true)
      {
        Local<Object> result1 = Object::New(isolate);
        //std::cout << "send for packing1" << std::endl;
        pack_cell_result(isolate, result1,2);
        result_list->Set(1, result1);
      }
      
      else
      {
        Local<Object> result2 = Object::New(isolate);
        //std::cout << "send for packing1" << std::endl;
        pack_cell_result(isolate, result2,2);
        result_list->Set(1, result2);

        Local<Object> result3 = Object::New(isolate);
        //std::cout << "send for packing1" << std::endl;
        pack_cell_result(isolate, result3,3);
        result_list->Set(2, result3);

        Local<Object> result4 = Object::New(isolate);
        //std::cout << "send for packing1" << std::endl;
        pack_cell_result(isolate, result4,4);
        result_list->Set(3, result4);
      }

      // Return the list
      args.GetReturnValue().Set(result_list);
    }
    
  }
  else if(vizType==3)
  {
    trend.genHistoBase(iteration,db,schema);

    Local<Array> result_list = Array::New(isolate);
    
    std::map<int,float>::iterator it;
    int seg_no=0;
    for ( it = trend.groupAvgMap.begin(); it != trend.groupAvgMap.end(); it++ )
    {
      Local<Object> result = Object::New(isolate);
      int group = trend.groupMap[it->first];
  
      float mean = it->second;

      result->Set(String::NewFromUtf8(isolate, "n"), Integer::New(isolate, seg_no));
      result->Set(String::NewFromUtf8(isolate, "b"), Integer::New(isolate, group));
      result->Set(String::NewFromUtf8(isolate, "e"), Integer::New(isolate, group));
      result->Set(String::NewFromUtf8(isolate, "a"), Number::New(isolate, mean));
      result_list->Set(seg_no, result);

      seg_no++;
         
    }

    args.GetReturnValue().Set(result_list);
    
  }
  else
  {
    using namespace std;
    heat.genHeatBase(iteration,db,schema);

    Local<Array> result_list = Array::New(isolate);
    
    map<string,float>::iterator it;
    int quad_no=0;
    for ( it = heat.groupAvgMap.begin(); it != heat.groupAvgMap.end(); it++ )
    {
      Local<Object> result = Object::New(isolate);
      string key = it->first;
      vector<string> v;
      char * dup = strdup(key.c_str());
      char * token = strtok(dup, ",");
      while(token != NULL){
          v.push_back(string(token));
          // the call is treated as a subsequent calls to strtok:
          // the function continues from where it left in previous invocation
          token = strtok(NULL, ",");
      }
      free(dup);

      int groupX = atoi(v[0].c_str());
      int groupY =  atoi(v[1].c_str());
      float mean = it->second;

      int xb,xe,yb,ye;


      xb = groupX;
      xe = groupX;
      yb = groupY;
      ye = groupY;

      
      result->Set(String::NewFromUtf8(isolate, "n"), Integer::New(isolate, quad_no));
      result->Set(String::NewFromUtf8(isolate, "xb"), Integer::New(isolate, xb));
      result->Set(String::NewFromUtf8(isolate, "xe"), Integer::New(isolate, xe));
      result->Set(String::NewFromUtf8(isolate, "yb"), Integer::New(isolate, yb));
      result->Set(String::NewFromUtf8(isolate, "ye"), Integer::New(isolate, ye));
      result->Set(String::NewFromUtf8(isolate, "a"), Number::New(isolate, mean));
      

      result_list->Set(quad_no, result);

      quad_no++;
         
    }

    args.GetReturnValue().Set(result_list);

  }

}

void getAxisrange(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = args.GetIsolate();

    Handle<Object> obj1 = Handle<Object>::Cast(args[0]);
    Handle<Value> val = obj1->Get(String::NewFromUtf8(isolate,"iter"));

    int iteration = val->NumberValue();

    Local<Object> obj = Object::New(isolate);

    

    if(vizType==1 || vizType==3)
    {
      obj->Set(String::NewFromUtf8(isolate, "aMin"), 
                            v8::Number::New(isolate, trend.avgMin));
      obj->Set(String::NewFromUtf8(isolate, "aMax"), 
                            v8::Number::New(isolate, trend.avgMax));
      obj->Set(String::NewFromUtf8(isolate, "xMin"), 
                            v8::Number::New(isolate, trend.xMin));
      obj->Set(String::NewFromUtf8(isolate, "xMax"), 
                            v8::Number::New(isolate, trend.xMax));
    }
    else
    {
      obj->Set(String::NewFromUtf8(isolate, "aMin"), 
                            v8::Number::New(isolate, heat.avgMin));
      obj->Set(String::NewFromUtf8(isolate, "aMax"), 
                            v8::Number::New(isolate, heat.avgMax));
      obj->Set(String::NewFromUtf8(isolate, "xMin"), 
                            v8::Number::New(isolate, heat.xMin));
      obj->Set(String::NewFromUtf8(isolate, "xMax"), 
                            v8::Number::New(isolate, heat.xMax));
      obj->Set(String::NewFromUtf8(isolate, "yMin"), 
                            v8::Number::New(isolate, heat.yMin));
      obj->Set(String::NewFromUtf8(isolate, "yMax"), 
                            v8::Number::New(isolate, heat.yMax));

    }

    
    
    
    args.GetReturnValue().Set(obj);

}

void toContinue(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = args.GetIsolate();

    if(vizType==3)
    {
      Local<Number> retval = v8::Number::New(isolate, trend.isContinue);
      args.GetReturnValue().Set(retval);
    }
    else if (vizType == 4)
    {
      Local<Number> retval = v8::Number::New(isolate, heat.isContinue);
      args.GetReturnValue().Set(retval);
    }

    

}

void reset(const v8::FunctionCallbackInfo<v8::Value>& args) {

  if(vizType==1 || vizType==3)
    trend.reset();
  else
    heat.reset();

}

void getValidCells(const v8::FunctionCallbackInfo<v8::Value>& args) {

    Isolate* isolate = args.GetIsolate();

    Handle<Object> obj1 = Handle<Object>::Cast(args[0]);
    Handle<Value> val = obj1->Get(String::NewFromUtf8(isolate,"iter"));

        
    Local<Array> result_list = Array::New(isolate);

    std::map<int, int>::iterator itX;
    std::map<int, int>::iterator itY;

    int cell=0;
    for(itX = heat.groupXMap.begin();itX!=heat.groupXMap.end();itX++)
    {
      for(itY = heat.groupYMap.begin();itY!=heat.groupYMap.end();itY++)
      {
        

        if(heat.validCells(itX->first,itY->first)==false)
        {
          //std::cout << itX->first << "," <<itY->first << std::endl;
          Local<Object> result = Object::New(isolate);
          result->Set(String::NewFromUtf8(isolate, "x"), Integer::New(isolate, itX->second));
          result->Set(String::NewFromUtf8(isolate, "y"), Integer::New(isolate, itY->second));

          result_list->Set(cell, result);
          cell++;
        }
      }
    } 

      // Return the list
      args.GetReturnValue().Set(result_list);

}

void init(Handle <Object> exports, Handle<Object> module) {
 // we'll register our functions to make them callable from node here..
  
  NODE_SET_METHOD(exports, "setVizType", setVizType);
  NODE_SET_METHOD(exports, "loadDataset", loadDataset);
  NODE_SET_METHOD(exports, "loadPredicateData", loadPredicateData);
  NODE_SET_METHOD(exports, "getVizType", getVizType);
  NODE_SET_METHOD(exports, "loadAxesData", loadAxesData);
  NODE_SET_METHOD(exports, "genChart", genChart);
  NODE_SET_METHOD(exports, "getAxisrange", getAxisrange);
  NODE_SET_METHOD(exports, "reset", reset);
  NODE_SET_METHOD(exports, "getValidCells", getValidCells);
  NODE_SET_METHOD(exports, "toContinue", toContinue);
}

// associates the module name with initialization logic
NODE_MODULE(incvisage, init) 

