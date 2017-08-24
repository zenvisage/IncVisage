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

#include <boost/unordered_map.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <yaml-cpp/yaml.h>

//#include <gperftools/profiler.h>

#include "trend.h"
#include "axes.h"
#include "csv.h"
#include "query.h"
#include "schema.h"
#include "sdb.h"
#include "swift_index.h"
#include "utils.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <math.h>
#include "eugenewu.h"
#include "segment.h"
#include "config.h"


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

using namespace std;

//Schema schema1("/home/srahman7/incvisage-cpp/needletail-code-base/data/1/schema_1", 0);
//SDB db1("/home/srahman7/incvisage-cpp/needletail-code-base/data/1/sdb_1",schema1);

vector<Segment> k_Best[366];
double groupAvgs[366]; 
int groupNums[366];

int opt_start = 10000;
float opt_alpha = 1.02;

int nonSingleGroups = 366;
double avg_map[366][366]={0};

int record_id[366];

bool use_query = false;
uint64_t num_rows = 10000000;
uint64_t dataset_count = DATASET_COUNT;
const char* randomness = NULL;
double density = 0.10;
double clustering_rate=0.10;
string distribution = "clustered";
string indextype = "density";
const char* bmap_impl = NULL;

double conv_rate = 1.0;

float block_read_time_trend = 2;

//float alloted_time = 1000;

//SwiftMapFactory::map_type_t bmap_type_trend = SwiftMapFactory::EWAH_BITMAP;    
SwiftMap::sample_type_t sample_type_trend = SwiftMap::SEQUENTIAL;

Trend::Trend(SwiftIndex& swi):swi(swi)
{

}

void Trend::reset()
{
	for(int i=0;i<this->card;i++)
	{
		vector<Segment>().swap(k_Best[i]);

		groupAvgs[i] = 0; 
		groupNums[i] = 0;
		
		nonSingleGroups = 366;
		
		for(int j=0;j<this->card;j++)
		{
			avg_map[i][j]=0;
		}

		record_id[i] = 0;
	}

	//this->dimAttr = "";
	//this->measAttr = "";
	//this->filterAttr = "";
	//this->filterVal = "";

	this->firstIteration =  true;

	//this->groupMap.clear();
	this->groupAvgMap.clear();
	//this->card = -1;
	this->split_seg = -1;

	this->firstNewSegment.set_start(0);
	this->firstNewSegment.set_end(0);
	this->firstNewSegment.set_mean(0);

	this->secondNewSegment.set_start(0);
	this->secondNewSegment.set_end(0);
	this->secondNewSegment.set_mean(0);

	//this->queryType = -1;

	this->isContinue = 1;

	this->avgMin = 10000000;
	this->avgMax = -10000000;
}


void Trend::setMaps(std::string dataset)
{
	this->dataset =  dataset;


	if(dataset == "flight")
  	{
  		//load the sdb
		//const char* sdb_fname = "/home/srahman7/incvisage-cpp/needletail-code-base/data/1/sdb_1";
		//const char* schema_fname = "/home/srahman7/incvisage-cpp/needletail-code-base/data/1/schema_1";
		
		//Schema schema(schema_fname, 0);

		//db = new SDB(sdb_fname, schema);

		//db.clear();

  		this->dimMap["Day"] = "d0";
  		this->dimMap["Month"] = "d1";
  		this->dimMap["Day Of Month"] = "d2";
  		this->dimMap["Day Of Week"] = "d3";
  		this->dimMap["Week Of Year"] = "d4";
  		this->dimMap["Year"] = "d5";

  		this->measMap["Departure Delay (minutes)"] = "c0";
  		this->measMap["Taxi Out (minutes)"] = "c1";
  		this->measMap["Taxi In (minutes)"] = "c2";
  		this->measMap["Arrival Delay (minutes)"] = "c3";
  		this->measMap["Carrier Delay (minutes)"] = "c4";
  		this->measMap["Weather Delay (minutes)"] = "c5";
  		this->measMap["NAS Delay (minutes)"] = "c6";
  		this->measMap["Security Delay (minutes)"] = "c7";
  		this->measMap["Aircraft Delay (minutes)"] = "c8";

  		this->filterMap["Carrier"] = "d6";
  		this->filterMap["Origin Airport"] = "d7";
  		this->filterMap["Origin City"] = "d8";
  		this->filterMap["Origin State"] = "d9";
  		this->filterMap["Destination Airport"] = "d10";
  		this->filterMap["Destination City"] = "d11";
  		this->filterMap["Destination State"] = "d12";

  		this->filterValueMap["AA"] = 1;
	    this->filterValueMap["AS"] = 20;//2;
	    this->filterValueMap["CO"] = 30;//3;
	    this->filterValueMap["OH(1)"] = 40;//4;
	    this->filterValueMap["UA"] = 2;//5;
	    this->filterValueMap["US"] = 3;//6;

	    this->filterValueMap["Atlanta- GA"] = 1;
	    this->filterValueMap["Chicago- IL"] = 2;
	    this->filterValueMap["Denver- CO"] = 30;//3;
	    this->filterValueMap["Houston- TX"] = 3;//4;
	    this->filterValueMap["Los Angeles- CA"] = 5;
	    this->filterValueMap["New York- NY"] = 6;
	    this->filterValueMap["Oklahoma- OH"] = 7;

	    this->filterValueMap["AK"] = 10;//1;
	    this->filterValueMap["CA"] = 2;
	    this->filterValueMap["FL"] = 3;
	    this->filterValueMap["GA"] = 4;
	    this->filterValueMap["IL"] = 5;
	    this->filterValueMap["LA"] = 6;
	    this->filterValueMap["MN"] = 7;
	    this->filterValueMap["NY"] = 1;//8;
	    this->filterValueMap["OH"] = 9;
	    this->filterValueMap["PA"] = 10;
	    this->filterValueMap["TX"] = 11;
	    
	    this->filterValueMap["ATL"] = 1;
	    this->filterValueMap["DEN"] = 2;
	    this->filterValueMap["DFW"] = 3;
	    this->filterValueMap["JFK"] = 40;//4;
	    this->filterValueMap["LAX"] = 50;//5;
	    this->filterValueMap["LGA"] = 4;//6;
	    this->filterValueMap["ORD"] = 5;//7;
	    this->filterValueMap["SFO"] = 8;
	    this->filterValueMap["CTL"] = 9;
	    this->filterValueMap["LAS"] = 10;
	    this->filterValueMap["PHX"] = 11;

  	}
  	else
  	{
  		this->dimMap["Day"] = "d0";
  		this->dimMap["Hour"] = "d1";

  		this->measMap["Temperature"] = "c0";
  		this->measMap["Humidity"] = "c1";
  		this->measMap["Light"] = "c2";
  		this->measMap["Voltage"] = "c3";

  		this->filterMap["Day"] = "d0";
  		this->filterMap["Hour"] = "d1";

  		this->filterValueMap["1"] = 1;
	    this->filterValueMap["2"] = 2;//2;
	    this->filterValueMap["3"] = 3;//3;
	    this->filterValueMap["4"] = 4;//4;
	    this->filterValueMap["5"] = 5;//5;
	    this->filterValueMap["6"] = 6;//6;

	    this->filterValueMap["7"] = 7;//6;
	    this->filterValueMap["8"] = 8;//6;
	    this->filterValueMap["9"] = 9;//6;
	    this->filterValueMap["10"] = 10;//6;
	    this->filterValueMap["11"] = 11;//6;
	    this->filterValueMap["12"] = 12;//6;
	    
	    this->filterValueMap["13"] = 13;//6;
	    this->filterValueMap["14"] = 14;//6;
	    this->filterValueMap["15"] = 15;//6;
	    this->filterValueMap["16"] = 16;//6;
	    this->filterValueMap["17"] = 17;//6;
	    this->filterValueMap["18"] = 18;//6;

	    this->filterValueMap["19"] = 19;//6;
	    this->filterValueMap["20"] = 20;//6;
	    this->filterValueMap["21"] = 21;//6;
	    this->filterValueMap["22"] = 22;//6;
	    this->filterValueMap["23"] = 23;//6;
	    this->filterValueMap["24"] = 24;//6;
  	}
}

void Trend::loadAxesData(std::string dim,std::string meas,std::string filter,std::string filterValue)
{
	this->dimAttr = this->dimMap[dim];
	this->measAttr = this->measMap[meas];
	this->filterAttr = this->filterMap[filter];
	if(this->dataset=="flight")
		this->filterVal = this->filterValueMap[filterValue];
	else
		this->filterVal = atoi(filterValue.c_str());

	this->firstIteration =  true;

	if(this->filterAttr.empty())
	{
		this->queryType = 1;

	}
	else
	{
		this->queryType = 2;
	}

	if(!this->groupMap.empty())
		groupMap.clear();



	string fname = "/home/srahman7/incvisage-cpp/needletail-code-base/data/";
	if(this->dataset == "flight" )
	{
		fname = fname+"1/schema_1.csv";
		
	}
	else
	{
		fname = fname+"2/schema_2.csv";
	}
	//cout << "print group maps" << endl;
	std::ostringstream temp;
	temp.str("");
	temp.clear();
	temp << fname;
	char* q = new char[temp.str().length() + 1];
	std::strcpy(q, temp.str().c_str());
	std::ifstream data(q);

    std::string line;
    while(std::getline(data,line))
    {
        vector<string> v;
	      char * dup = strdup(line.c_str());
	      char * token = strtok(dup, ",");
	      while(token != NULL){
	          v.push_back(string(token));
	          // the call is treated as a subsequent calls to strtok:
	          // the function continues from where it left in previous invocation
	          token = strtok(NULL, ",");
	      }
	      free(dup);

	      if(v[1] == this->dimAttr)
	      {
	      	this->xMin = atoi(v[2].c_str());
      		this->xMax =  atoi(v[3].c_str());
      		break;
	      }
    }

    this->card = this->xMax-this->xMin+1;
	for(int i = this->xMin,j=0;i<=this->xMax;i++,j++)
	{
		groupMap[j] = i;
		
		//cout << j << "=>" << groupMap[j] << endl;
	}
	isContinue = 1;

}

void Trend::genHisto(int iteration,SDB& db1,Schema& schema1)
{
		// required variables
	cout << "in gen charts: " << iteration << endl;
	
	
	swi.isTrend = true;
	
    std::ostringstream temp;


	int i = iteration;
	string query_fname = "";
	string qt = "/home/srahman7/incvisage-cpp/needletail-code-base/scripts/";

	if(this->queryType==1)
	{
		query_fname = qt+"qt1";


	}
	else
	{
		query_fname = qt+"qt2";
		swi.isTrend = false;
	}

	temp.str("");
	temp.clear();
	temp << query_fname;
	char* q = new char[temp.str().length() + 1];
	std::strcpy(q, temp.str().c_str());

	YAML::Node aqry = YAML::LoadFile(q);

	
	float iter_samples = floor((opt_start*1.0)/pow(opt_alpha,i-1));
	//cout << "at first iteration: " << i << endl;
	double segment_avg = 0;
	vector<Segment> prev_vect;
		//---- load the segment structure from previous iteation
	if(this->firstIteration)
	{
		cout << "in first iteration" << endl;
		for(int rec = 0; rec < this->card ; rec++)
		{
			record_id[rec] = 0;
			
		}

		int start = 0; //xMinF
		int end = this->card-1; //xMaxF

		for(int g = start,am=0;g <= end;g++,am++)
		{
			//cout << this->dimAttr << "," << this->groupMap[g] << endl;
			conv_rate = 1.0;
			uint64_t conv_samples = 0;//floor(iter_samples/this->card);

			YAML::Node& qry = aqry;

			if(this->queryType==1)
			{
			
				qry["q"][this->dimAttr] = this->groupMap[g];


				uint64_t per_block_sample = ceil((swi.perGroupValCount[this->dimAttr][this->groupMap[g]]*1.0)/schema1.get_num_pages());

				float dim_comb_allotted_time = alloted_time/this->card;

				uint64_t num_blocks_allowed = ceil((dim_comb_allotted_time*1.0)/block_read_time_trend);

				conv_samples = per_block_sample*num_blocks_allowed;

				//conv_samples = 100; sensor datq

				//cout << "allowed" << conv_samples << endl;

			}
			else
			{
				//block_read_time_trend = block_read_time;
				qry["q"]["__nodes__"][this->dimAttr] = this->groupMap[g];
				qry["q"]["__nodes__"][this->filterAttr] = this->filterVal;

				uint64_t numerator = swi.perGroupValCount[this->dimAttr][this->groupMap[g]]*swi.perGroupValCount[this->filterAttr][filterVal];
				
				uint64_t denominator = schema1.get_num_rows()*schema1.get_num_pages();
				//cout << numerator << "," << denominator << endl;
				uint64_t per_block_sample = ceil((numerator*1.0)/denominator);
				//cout << per_block_sample << endl;
				float dim_comb_allotted_time = alloted_time/(this->card*(swi.perGroupValCount[this->filterAttr].size()-1)); //-1 because there is a 0 key
				//cout << dim_comb_allotted_time << "," << swi.perGroupValCount[this->filterAttr].size() << endl;
				uint64_t num_blocks_allowed = ceil((dim_comb_allotted_time*1.0)/block_read_time);
				//cout << num_blocks_allowed << endl;
				conv_samples = per_block_sample*num_blocks_allowed;

				//conv_samples = 10; sensor data

				//cout << "allowed" << conv_samples << endl;
			}

			
			//cout << "allowed--" << conv_samples << endl;
			//Query query(q, schema1);
			Query query(qry, schema1);
			query.set_num_samples(conv_samples);
			
			query.set_conv_rate(conv_rate);

			ResultSet results(query.get_targets().size());

			
			//ProfilerStart("cpu.out");
			//std::clock_t c_start = std::clock();
			//swi.run_query(query, results, sample_type_trend);
			record_id[g] = swi.run_query(query, results, sample_type_trend,record_id[g],firstIteration,this->groupMap[g]);
			if(swi.isRecordEmpty==true)
			{
				record_id[g] = -1;
				//cout << "epmty group: " << g << endl;
				continue;
			}
			std::vector<double> avgs = results.get_avgs();
			groupAvgs[g] = (groupAvgs[g]*groupNums[g] + avgs[0]*conv_samples)/(groupNums[g]+conv_samples);
			groupNums[g] += conv_samples;

			segment_avg += avgs[0];

		}
		
		segment_avg /= (end-start+1);

		vector<Segment> tmp;
		

		Segment tmpSeg(start,end,segment_avg);
		
		tmp.push_back(tmpSeg); 
		k_Best[i] = tmp;

		this->firstIteration = false;

		//vector<ResultObject> resVector;
		//ResultObject resObj(0,tmpSeg);

		//cout << resObj.seg_no << ",(" << resObj.seg.get_start() << "," << resObj.seg.get_end() << ",) =>" << resObj.seg.get_mean() << endl;

		//resVector.push_back(resObj);
		//return resVector;

		this->split_seg = -1;

		this->firstNewSegment.set_start(start);
		this->firstNewSegment.set_end(end);
		this->firstNewSegment.set_mean(segment_avg);

		this->avgMin = segment_avg;
		this->avgMax = segment_avg;

	}
		
	else
	{
		//cout << "assign prev k"<<endl;
		prev_vect = k_Best[i-1];
		double global_seg_mean = 0;
		int split_segment_no = -1;
		int split_segment_maxSplit = -1;
		double global_max = -1000000000;
		double global_best_mean1 = 0, global_best_mean2 = 0;
		//cout << "sampling"<<endl;
		for(int s=0;s<prev_vect.size();s++)
		{
			
			int start = prev_vect[s].get_start();
			int end = prev_vect[s].get_end();

			if(end - start < 1)
				continue;
			//---- ask for N samples from a non-single segment group for some target/measure and get the avg 
			
			//cout << "seg: "<< s << "=>(" << start << "," << end <<")"<<endl;
			for(int g = start,am=0;g <= end;g++,am++)
			{
				if(record_id[g] == -1)
				{
					continue;
				}
				conv_rate = 1.0;
				
				uint64_t conv_samples = floor(iter_samples/nonSingleGroups);

				//cout << "group: "<< g <<endl;
				YAML::Node& qry = aqry;

				if(this->queryType==1)
				{
				
					qry["q"][this->dimAttr] = this->groupMap[g];


					uint64_t per_block_sample = ceil(swi.perGroupValCount[this->dimAttr][this->groupMap[g]]/schema1.get_num_pages());

					float dim_comb_allotted_time = alloted_time/this->card;

					uint64_t num_blocks_allowed = ceil(dim_comb_allotted_time/block_read_time_trend);

					conv_samples = per_block_sample*num_blocks_allowed;

					//cout << "allowed" << conv_samples << endl;

				}
				else
				{
					//block_read_time_trend = block_read_time;
					qry["q"]["__nodes__"][this->dimAttr] = this->groupMap[g];
					qry["q"]["__nodes__"][this->filterAttr] = this->filterVal;

					uint64_t numerator = swi.perGroupValCount[this->dimAttr][this->groupMap[g]]*swi.perGroupValCount[this->filterAttr][filterVal];
					
					uint64_t denominator = schema1.get_num_rows()*schema1.get_num_pages();
					//cout << numerator << "," << denominator << endl;
					uint64_t per_block_sample = ceil((numerator*1.0)/denominator);
					//cout << per_block_sample << endl;
					float dim_comb_allotted_time = alloted_time/(this->card*(swi.perGroupValCount[this->filterAttr].size()-1)); //-1 because there is a 0 key
					//cout << dim_comb_allotted_time << "," << swi.perGroupValCount[this->filterAttr].size() << endl;
					uint64_t num_blocks_allowed = ceil((dim_comb_allotted_time*1.0)/block_read_time);
					//cout << num_blocks_allowed << endl;
					conv_samples = per_block_sample*num_blocks_allowed;

					//cout << "allowed" << conv_samples << endl;
				}

				//conv_samples = 100; sensor data
				//Query query(q, schema1);
				Query query(qry, schema1);
				query.set_num_samples(conv_samples);
				
				query.set_conv_rate(conv_rate);

				ResultSet results(query.get_targets().size());

				
				//ProfilerStart("cpu.out");
				//std::clock_t c_start = std::clock();
				//swi.run_query(query, results, sample_type_trend);
				record_id[g] = swi.run_query(query, results, sample_type_trend,record_id[g],firstIteration,this->groupMap[g]);
				if(swi.isRecordEmpty==true)
				{
					record_id[g] = -1;
					//cout << "sample 0: "<<g<<endl;
					continue;
				}

				std::vector<double> avgs = results.get_avgs();
				groupAvgs[g] = (groupAvgs[g]*groupNums[g] + avgs[0]*conv_samples)/(groupNums[g]+conv_samples);
				groupNums[g] += conv_samples;


				//cout << "avgmap "<< g <<endl;
				avg_map[am][am] = groupAvgs[g];
				for(int am_it = 0; am_it <am; am_it++)
				{
					avg_map[am_it][am] = avg_map[am_it][am-1]+groupAvgs[g];
				}
				
					
				
				
			}


		
			double alphaT = 0;
			double alphaU = 0;
			double seg_mean = 0;
			double best_mean1 = 0;
			double best_mean2 = 0;
			double tmp1, tmp2;
			double max = -1000000000;
			int maxSplit = -1;
			//cout << "compute mean "<< endl;
			for(int g = start, am=0;g <= end;g++,am++)
			{
				
				//cout << "find each split: " << g << endl;
				alphaT = (1.0 / (g - start + 1)) * avg_map[0][am]; // V_s
				tmp1 = alphaT;
				//seg_mean += alpha * alpha * (i - seg_start + 1);

				
				
				if (g == end)
					alphaU = 0;
				else
					alphaU = (1.0 / (end - (g + 1) + 1)) * avg_map[am+1][end - start]; // V_s
				tmp2 = alphaU;
				//seg_mean += alpha * alpha * (seg_end - (i + 1) + 1); // |S|sum(V_s)
				seg_mean = (g - start + 1)*(end - (g + 1) + 1)*(alphaT-alphaU)*(alphaT-alphaU)/(card*(end-start+1));
						
				if (seg_mean > max) {
					max = seg_mean;
					maxSplit = g;
					best_mean1 = tmp1;
					best_mean2 = tmp2;

					
				}

				seg_mean = 0;
				alphaT = 0;
				alphaU=0;
			}

			//cout << "find global max " << endl;
			//---- find the global best split point
			global_seg_mean = max;
		
			if (global_seg_mean > global_max) {
				global_max = global_seg_mean;

				split_segment_no = s;
				split_segment_maxSplit = maxSplit;

				global_best_mean1 = best_mean1;
				global_best_mean2 = best_mean2;

			}

			global_seg_mean = 0;

			
			
			

		}	

	
			//---- create new segment structure and push


		vector<Segment> tmp;
		Segment tmpSeg1(0, 0, 0);
		Segment tmpSeg2(0, 0, 0);
		nonSingleGroups = 0;

		//vector<ResultObject> resVector;
		
		//cout << "compute k-best "<< endl;
		for (int s = 0; s < prev_vect.size(); s++) {
			if (s == split_segment_no) {
				double tmp_mean = global_best_mean1;
				if(global_best_mean2 > tmp_mean)
					tmp_mean = global_best_mean2;
				if(this->avgMax < tmp_mean)
					this->avgMax = tmp_mean;
				
				tmp_mean = global_best_mean1;
				if(global_best_mean2 < tmp_mean)
					tmp_mean = global_best_mean2;

				if(this->avgMin > tmp_mean)
					this->avgMin = tmp_mean;
				
				//cout << "split pt: " << split_segment_maxSplit << " at segment: " << s << "(" << prev_vect[s].get_start() << "," << prev_vect[s].get_end() <<")" << endl;
				if (split_segment_maxSplit == prev_vect[s].get_end()) {

					tmpSeg1.set_start(prev_vect[s].get_start());
					tmpSeg1.set_end(split_segment_maxSplit - 1);
					tmpSeg1.set_mean(global_best_mean1);
					tmp.push_back(tmpSeg1); 

					//ResultObject resObj1(split_segment_no,tmpSeg1);
					//resVector.push_back(resObj1);


				//	cout << "seg 1: (" << tmpSeg1.get_start() << "," << tmpSeg1.get_end() << "," << tmpSeg1.get_mean() << ")" << endl; 

					if(tmpSeg1.get_end() - tmpSeg1.get_start() > 0)
						nonSingleGroups += (tmpSeg1.get_end() - tmpSeg1.get_start()+1);

					tmpSeg2.set_start(split_segment_maxSplit);
					tmpSeg2.set_end(prev_vect[s].get_end());
					tmpSeg2.set_mean(global_best_mean2);
					tmp.push_back(tmpSeg2); 

				//	cout << "seg 2: (" << tmpSeg2.get_start() << "," << tmpSeg2.get_end() << "," << tmpSeg2.get_mean() << ")" << endl; 

					//ResultObject resObj2(split_segment_no+1,tmpSeg2);
					//resVector.push_back(resObj2);

					if(tmpSeg2.get_end() - tmpSeg2.get_start() > 0)
						nonSingleGroups += (tmpSeg2.get_end() - tmpSeg2.get_start()+1);

					this->split_seg = split_segment_no;

					this->firstNewSegment.set_start(prev_vect[s].get_start());
					this->firstNewSegment.set_end(split_segment_maxSplit - 1);
					this->firstNewSegment.set_mean(global_best_mean1);

					this->secondNewSegment.set_start(split_segment_maxSplit);
					this->secondNewSegment.set_end(prev_vect[s].get_end());
					this->secondNewSegment.set_mean(global_best_mean2);
					
				} else {
					tmpSeg1.set_start(prev_vect[s].get_start());
					tmpSeg1.set_end(split_segment_maxSplit);
					tmpSeg1.set_mean(global_best_mean1);
					tmp.push_back(tmpSeg1); 

					//ResultObject resObj1(split_segment_no,tmpSeg1);
					//resVector.push_back(resObj1);
				//	cout << "seg 1: (" << tmpSeg1.get_start() << "," << tmpSeg1.get_end() << "," << tmpSeg1.get_mean() << ")" << endl; 

					if(tmpSeg1.get_end() - tmpSeg1.get_start() > 0)
						nonSingleGroups += (tmpSeg1.get_end() - tmpSeg1.get_start()+1);

					tmpSeg2.set_start(split_segment_maxSplit+1);
					tmpSeg2.set_end(prev_vect[s].get_end());
					tmpSeg2.set_mean(global_best_mean2);
					tmp.push_back(tmpSeg2); 

					//ResultObject resObj2(split_segment_no+1,tmpSeg2);
					//resVector.push_back(resObj2);

				//	cout << "seg 2: (" << tmpSeg2.get_start() << "," << tmpSeg2.get_end() << "," << tmpSeg2.get_mean() << ")" << endl; 

					if(tmpSeg2.get_end() - tmpSeg2.get_start() > 0)
						nonSingleGroups += (tmpSeg2.get_end() - tmpSeg2.get_start()+1);

					if(tmpSeg2.get_end() - tmpSeg2.get_start() > 0)
						nonSingleGroups += (tmpSeg2.get_end() - tmpSeg2.get_start()+1);

					this->split_seg = split_segment_no;

					this->firstNewSegment.set_start(prev_vect[s].get_start());
					this->firstNewSegment.set_end(split_segment_maxSplit);
					this->firstNewSegment.set_mean(global_best_mean1);

					this->secondNewSegment.set_start(split_segment_maxSplit+1);
					this->secondNewSegment.set_end(prev_vect[s].get_end());
					this->secondNewSegment.set_mean(global_best_mean2);
					
				}

			} else {
				tmp.push_back(prev_vect[s]); 

				if(prev_vect[s].get_end() - prev_vect[s].get_start() > 0)
					nonSingleGroups += (prev_vect[s].get_end() - prev_vect[s].get_start()+1);

				
				
			}
		
		

			


		}

	
		//cout << "assign k-best "<< endl;
		if(i < this->card)	
			k_Best[i] = tmp;

		
		vector<Segment>().swap(tmp);
		vector<Segment>().swap(prev_vect);

		/*cout << "k_best" << endl;

		for(int kb = 0; kb < k_Best[i].size() ; kb++)
			cout << "seg:" << kb <<" (" << k_Best[i][kb].get_start() << "," << k_Best[i][kb].get_end() << "," << k_Best[i][kb].get_mean() << ")" << endl; 

		cout << "objects returned" << endl;

		cout << "seg 1: (" << firstNewSegment.get_start() << "," << firstNewSegment.get_end() << "," << firstNewSegment.get_mean() << ")" << endl; 

		cout << "seg 2: (" << secondNewSegment.get_start() << "," << secondNewSegment.get_end() << "," << secondNewSegment.get_mean() << ")" << endl; 
		*/
		//return resVector;
	}
			


}

void Trend::genHistoBase(int iteration,SDB& db1,Schema& schema1)
{
		// required variables
	cout << "in gen charts base: " << iteration << endl;
	
	
	swi.isTrend = true;
	
    std::ostringstream temp;


	int i = iteration;
	string query_fname = "";
	string qt = "/home/srahman7/incvisage-cpp/needletail-code-base/scripts/";

	if(this->queryType==1)
		query_fname = qt+"qt1";
	else
	{		
		query_fname = qt+"qt2";
		swi.isTrend = false;
	}

	temp.str("");
	temp.clear();
	temp << query_fname;
	char* q = new char[temp.str().length() + 1];
	std::strcpy(q, temp.str().c_str());

	YAML::Node aqry = YAML::LoadFile(q);

	this->avgMin = 10000000;
    this->avgMax = -10000000;
	
		//---- load the segment structure from previous iteation
	if(this->firstIteration)
	{
		cout << "in first iteration" << endl;
		for(int rec = 0; rec < this->card ; rec++)
		{
			record_id[rec] = 0;
			
		}

		this->avgMax = -100000;
		this->avgMin = 100000;
	}

		int start = 0; //xMinF
		int end = this->card-1; //xMaxF

		for(int g = start,am=0;g <= end;g++,am++)
		{
			//cout << "g: " << g << endl;
			if(record_id[g] == -1)
			{
				
				continue;
			}
			conv_rate = 1.0;
			int conv_samples = 1;

			YAML::Node& qry = aqry;

			if(this->queryType==1)
			{
			
				qry["q"][this->dimAttr] = this->groupMap[g];//groupMap
				
			}
			else
			{
				qry["q"]["__nodes__"][this->dimAttr] = this->groupMap[g];
				qry["q"]["__nodes__"][this->filterAttr] = this->filterVal;
			}


			//Query query(q, schema1);
			Query query(qry, schema1);
			query.set_num_samples(conv_samples);
			
			query.set_conv_rate(conv_rate);

			ResultSet results(query.get_targets().size());

			
			//ProfilerStart("cpu.out");
			//std::clock_t c_start = std::clock();
			//swi.run_query(query, results, sample_type_trend);
			record_id[g] = swi.run_query(query, results, sample_type_trend,record_id[g],firstIteration,this->groupMap[g]);

			if(swi.isRecordEmpty==true)
			{
				record_id[g] = -1;
				//cout << "sample 0: "<<g<<endl;
				continue;
			}
			std::vector<double> avgs = results.get_avgs();
			groupAvgs[g] = (groupAvgs[g]*groupNums[g] + avgs[0]*conv_samples)/(groupNums[g]+conv_samples);
			groupNums[g] += conv_samples;

			this->groupAvgMap[g] = groupAvgs[g];

			if(this->avgMax < groupAvgs[g])
					this->avgMax = groupAvgs[g];
			

			if(this->avgMin > groupAvgs[g])
				this->avgMin = groupAvgs[g];
			

		}
		
		
	this->firstIteration = false;
	
	isContinue = toContinue();

			
}

int Trend::toContinue()
{
	for(int recX = 0; recX < this->card ; recX++)
	{
		if(record_id[recX] != -1)
			return 1; 
		
		
	}

	return 0;

}