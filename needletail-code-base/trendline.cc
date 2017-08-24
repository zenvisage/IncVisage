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
#include "axes.h"

#include <set>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <cassert>
#include <cinttypes>
#include <sys/stat.h>
#include <getopt.h>
#include <pthread.h>
#include <utility>
#include <sys/resource.h>
#include <stdlib.h>
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <map>

///#include <boost/unordered_map.hpp>
///#include <boost/dynamic_bitset.hpp>
///#include <boost/random/mersenne_twister.hpp>
///#include <boost/random/uniform_int.hpp>
///#include <boost/random/variate_generator.hpp>

//#include <gperftools/profiler.h>
#include "axes.h"
//#include "trend.h"
///#include "csv.h"
///#include "query.h"
#include "schema.h"
#include "sdb.h"
///#include "swift_index.h"
///#include "utils.h"
///#include <sys/types.h>
///#include <sys/wait.h>
///#include <sys/mman.h>
#include <math.h>
///#include "eugenewu.h"
///#include "segment.h"


#define MAX_CSV_LINE_LEN (4096)

/* FIXME - This assuems we know the range of possible values for the
 * attr. */
#define MIN_ATTR (0)
#define MAX_ATTR (9)

/* FIXME - This assuems we know the number of rows. */
#define NUM_ROWS (20000)

#define NUM_THREADS (20)

#define DATASET_COUNT (1)

#define QUERIES_COUNT (0)

#define SAMPLING_RATES_COUNT (0)
using namespace v8;

int vizType = -1;
std::map<std::string, std::string> dimMap;
std::map<std::string, std::string> measMap;
std::map<std::string, std::string> filterMap;
std::string dataset;
std::string dimAttr;
std::string measAttr;
std::string filterAttr;
std::string filterVal;
bool firstIteration;

//code for getvizType
Schema schema1("/home/srahman7/incvisage-cpp/needletail-code-base/data/1/schema_1", 0);
SDB db1("/home/srahman7/incvisage-cpp/needletail-code-base/data/1/sdb_1",schema1);

void setMaps(std::string dataset)
{
  dataset =  dataset;


  if(dataset == "flight")
    {
      //load the sdb
    //const char* sdb_fname = "/home/srahman7/incvisage-cpp/needletail-code-base/data/1/sdb_1";
    //const char* schema_fname = "/home/srahman7/incvisage-cpp/needletail-code-base/data/1/schema_1";
    
    //Schema schema(schema_fname, 0);

    //db = new SDB(sdb_fname, schema);

    //db.clear();

      dimMap["Day"] = "d0";
      dimMap["Month"] = "d1";
      dimMap["Day Of Month"] = "d2";
      dimMap["Day Of Week"] = "d3";
      dimMap["Week Of Year"] = "d4";
      dimMap["Year"] = "d5";

      measMap["Departure Delay (minutes)"] = "c0";
      measMap["Taxi Out (minutes)"] = "c1";
      measMap["Taxi In (minutes)"] = "c2";
      measMap["Arrival Delay (minutes)"] = "c3";
      measMap["Carrier Delay (minutes)"] = "c4";
      measMap["Weather Delay (minutes)"] = "c5";
      measMap["NAS Delay (minutes)"] = "c6";
      measMap["Security Delay (minutes)"] = "c7";
      measMap["Aircraft Delay (minutes)"] = "c8";

      filterMap["Carrier"] = "d6";
      filterMap["Origin Airport"] = "d7";
      filterMap["Origin City"] = "d8";
      filterMap["Origin State"] = "d9";
      filterMap["Destination Airport"] = "d10";
      filterMap["Destination City"] = "d11";
      filterMap["Destination State"] = "d12";

    }
    else
    {

    }
}

Axes loadAxesData(std::string dim,std::string meas,std::string filter,std::string filterValue)
{
  dimAttr = dimMap[dim];
  measAttr = measMap[meas];
  filterAttr = filterMap[filter];
  filterVal = filterValue;

  firstIteration =  true;

  Axes x_axes;
  if(dataset == "flight" )
  {
    if(dimAttr == "d0")
    {
      x_axes.xMin = 1;
      x_axes.xMax = 366;
    }
  }
  else
  {

  }

  return x_axes;

}


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
  else
  {
    //load heatmap object
    obj->Set(String::NewFromUtf8(isolate, "viz"), 
                            String::NewFromUtf8(isolate, "Heatmap"));
  }
  
  args.GetReturnValue().Set(obj);
}
//code for select dataset and load dim and measure
void loadDataset(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = args.GetIsolate();

  Handle<Object> obj1 = Handle<Object>::Cast(args[0]);
  Handle<Value> val = obj1->Get(String::NewFromUtf8(isolate,"dataset"));
  v8::String::Utf8Value str(val);
  std::string dataset = std::string(*str);
  
  std::string dims;
  std::string meas;
  std::string filter;

  std::cout << "Dataset: " << dataset << std::endl;
  if(dataset == "flight")
  {
    //gen dims
      
    
    dims = "Day,Month,Day Of Month,Day Of Week,Week Of Year,Year";

    //gen measures

    meas = "Departure Delay (minutes),Taxi Out (minutes),Taxi In (minutes),Arrival Delay (minutes),Carrier Delay (minutes),Weather Delay (minutes),NAS Delay (minutes),Security Delay (minutes),Aircraft Delay (minutes)";

    //gen preds

    filter = "Carrier,Origin Airport,Origin City,Origin State,Destination Airport,Destination City,Destination State";


  }
  else
  {
    //gen dims
    
    dims = "Day,Hour";

    //gen measures

    meas = "Temperature,Humidity,Light,Voltage";

    //gen preds

    filter = "Day,Hour";
  }
  
  setMaps(dataset);

  Local<Object> obj = Object::New(isolate);

  obj->Set(String::NewFromUtf8(isolate, "dimension"), 
                            String::NewFromUtf8(isolate, dims.c_str()));
  obj->Set(String::NewFromUtf8(isolate, "measure"), 
                            String::NewFromUtf8(isolate, meas.c_str()));
  obj->Set(String::NewFromUtf8(isolate, "predicate"), 
                            String::NewFromUtf8(isolate, filter.c_str()));

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
  


  Local<Object> obj = Object::New(isolate);

  obj->Set(String::NewFromUtf8(isolate, "predicate"), 
                            String::NewFromUtf8(isolate, filters.c_str()));
  
  args.GetReturnValue().Set(obj);
}

void loadAxesData(const v8::FunctionCallbackInfo<v8::Value>& args) {
  // depending on visualization call specific load axes data
  Isolate* isolate = args.GetIsolate();

  Handle<Object> obj1 = Handle<Object>::Cast(args[0]);
  
  Handle<Value> val = obj1->Get(String::NewFromUtf8(isolate,"dimension"));
  v8::String::Utf8Value str(val);
  std::string dim = std::string(*str);

  val = obj1->Get(String::NewFromUtf8(isolate,"measure"));
  v8::String::Utf8Value str1(val);
  std::string meas = std::string(*str1);

  val = obj1->Get(String::NewFromUtf8(isolate,"predicate"));
  v8::String::Utf8Value str2(val);
  std::string filter = std::string(*str2);

  val = obj1->Get(String::NewFromUtf8(isolate,"predicatevalue"));
  v8::String::Utf8Value str3(val);
  std::string filterVal = std::string(*str3);

  Axes x_axes;
  if(vizType==1)
  {
    x_axes = loadAxesData(dim,meas,filter,filterVal);
    std::cout << dimAttr << "," << measAttr << "," << filterAttr << "," << filterVal << std::endl;

    Local<Object> obj = Object::New(isolate);

    obj->Set(String::NewFromUtf8(isolate, "xMin"), 
                            v8::Number::New(isolate, x_axes.xMin));
    obj->Set(String::NewFromUtf8(isolate, "xMax"), 
                            v8::Number::New(isolate, x_axes.xMax));
    args.GetReturnValue().Set(obj);
  }
  else
  {

  }


}
//get data
void genChart(const v8::FunctionCallbackInfo<v8::Value>& args) {


}


void init(Handle <Object> exports, Handle<Object> module) {
 // we'll register our functions to make them callable from node here..
  
  NODE_SET_METHOD(exports, "setVizType", setVizType);
  NODE_SET_METHOD(exports, "loadDataset", loadDataset);
  NODE_SET_METHOD(exports, "loadPredicateData", loadPredicateData);
  NODE_SET_METHOD(exports, "getVizType", getVizType);
  NODE_SET_METHOD(exports, "loadAxesData", loadAxesData);
  NODE_SET_METHOD(exports, "genChart", genChart);
}

// associates the module name with initialization logic
NODE_MODULE(trendline, init) 

