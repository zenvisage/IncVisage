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

#include "heat.h"
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
#include "quadrant.h"
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

vector<Quadrant> k_Best_heat[1000];
double groupAvgs_heat[366][366]; 
int groupNums_heat[366][366];

int opt_start_heat = 25000;
float opt_alpha_heat = 1.02;

int record_id_heat[366][366];

bool use_query_heat = false;
uint64_t num_rows_heat = 10000000;
uint64_t dataset_count_heat = dataset_count_heat;
const char* randomness_heat = NULL;
double density_heat = 0.10;
double clustering_rate_heat=0.10;
string distribution_heat = "clustered";
string indextype_heat = "density_heat";

double conv_rate_heat = 1.0;

float block_read_time_heat = 4;

SwiftMap::sample_type_t sample_type_heat = SwiftMap::SEQUENTIAL;

Heat::Heat(SwiftIndex& swi_heat):swi_heat(swi_heat)
{

}



double getMin(double a,double b)
{
	return a < b ? a:b;
}

double getMax(double a,double b)
{
	return a > b ? a:b;
}

void Heat::reset()
{
	for(int i=0;i<366;i++)
	{
		
		for(int j=0;j<366;j++)
		{
			groupAvgs_heat[i][j] = 0; 
			groupNums_heat[i][j] = 0;
			record_id_heat[i][j] = 0;
		}

		
	}

	for(int i=0;i<1000;i++)
		vector<Quadrant>().swap(k_Best_heat[i]);
	
	this->firstIteration =  true;
	this->isTwoSeg = false;
	this->isOneCell =  false;

	this->split_quad = -1;

	this->firstNewQuadrant.set_xStart(0);
	this->firstNewQuadrant.set_xEnd(0);
	this->firstNewQuadrant.set_yStart(0);
	this->firstNewQuadrant.set_yEnd(0);
	this->firstNewQuadrant.set_mean(0);

	this->secondNewQuadrant.set_xStart(0);
	this->secondNewQuadrant.set_xEnd(0);
	this->secondNewQuadrant.set_yStart(0);
	this->secondNewQuadrant.set_yEnd(0);
	this->secondNewQuadrant.set_mean(0);

	this->thirdNewQuadrant.set_xStart(0);
	this->thirdNewQuadrant.set_xEnd(0);
	this->thirdNewQuadrant.set_yStart(0);
	this->thirdNewQuadrant.set_yEnd(0);
	this->thirdNewQuadrant.set_mean(0);

	this->fourthNewQuadrant.set_xStart(0);
	this->fourthNewQuadrant.set_xEnd(0);
	this->fourthNewQuadrant.set_yStart(0);
	this->fourthNewQuadrant.set_yEnd(0);
	this->fourthNewQuadrant.set_mean(0);

	this->isContinue = 1;

	this->avgMin = 10000000;
	this->avgMax = -10000000;
}


void Heat::setMaps(std::string dataset)
{
	this->dataset =  dataset;


	if(dataset == "flight")
  	{
  		//load the sdb
		
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

void Heat::loadAxesData(std::string dim1,std::string dim2,std::string meas,std::string filter,std::string filterValue)
{
	this->dimXAttr = this->dimMap[dim1];
	this->dimYAttr = this->dimMap[dim2];
	this->measAttr = this->measMap[meas];
	this->filterAttr = this->filterMap[filter];
	if(this->dataset=="flight")
	{
		this->filterVal = this->filterValueMap[filterValue];

		if(this->dimXAttr=="d1" || this->dimXAttr=="d3" || this->dimXAttr=="d5")
		{
			if(this->dimYAttr=="d1" || this->dimYAttr=="d3" || this->dimYAttr=="d5")
			{
				block_read_time_heat = 2;
			}
		}

		if(this->dimXAttr=="d2" || this->dimXAttr=="d4" || this->dimYAttr=="d2" || this->dimYAttr=="d4")
		{
			block_read_time_heat = 8;
		}
	}
	else
		this->filterVal = atoi(filterValue.c_str());

	this->firstIteration =  true;

	if(this->filterAttr.empty())
		this->queryType = 1;
	else
		this->queryType = 2;

	if(!this->groupXMap.empty())
		groupXMap.clear();

	if(!this->groupYMap.empty())
		groupYMap.clear();

	if(!this->groupAvgMap.empty())
		groupAvgMap.clear();



	
	string fname = "./incvisage-cpp/needletail-code-base/data/";
	if(this->dataset == "flight" )
	{
		fname = fname+"1/schema_1.csv";
		
	}
	else
	{
		fname = fname+"2/schema_2.csv";
	}
	
	std::ostringstream temp;
	temp.str("");
	temp.clear();
	temp << fname;
	char* q = new char[temp.str().length() + 1];
	std::strcpy(q, temp.str().c_str());
	std::ifstream data(q);

    std::string line;
    int bothDimFound=0;
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

	      if(v[1] == this->dimXAttr)
	      {
	      	this->xMin = atoi(v[2].c_str());
      		this->xMax =  atoi(v[3].c_str());

      		cout << v[1] << xMin << "," << xMax << endl;
      		bothDimFound++;
      		if(bothDimFound==2)
      			break;
	      }
	      if(v[1] == this->dimYAttr)
	      {
	      	this->yMin = atoi(v[2].c_str());
      		this->yMax =  atoi(v[3].c_str());

      		cout << v[1] << yMin << "," << yMax << endl;
      		bothDimFound++;
      		if(bothDimFound==2)
      			break;
	      }
    }

    this->cardX = this->xMax-this->xMin+1;
    this->cardY = this->yMax-this->yMin+1;
	
	for(int i = this->xMin,j=0;i<=this->xMax;i++,j++)
	{
		groupXMap[j] = i;
		
	}

	for(int i = this->yMin,j=0;i<=this->yMax;i++,j++)
	{
		groupYMap[j] = i;
		
	}

	isContinue = 1;

	
}

void Heat::genHeat(int iteration,SDB& db11,Schema& schema11)
{
		// required variables
	cout << "in gen charts: " << iteration << endl;
	
	
	swi_heat.isTrend = false;
	
    std::ostringstream temp;


	int i = iteration;
	string query_fname = "";
	string qt = "./incvisage-cpp/needletail-code-base/scripts/";

	if(this->queryType==1)
		query_fname = qt+"qh1";
	else
		query_fname = qt+"qh2";

	temp.str("");
	temp.clear();
	temp << query_fname;
	char* q = new char[temp.str().length() + 1];
	std::strcpy(q, temp.str().c_str());

	YAML::Node aqry = YAML::LoadFile(q);

	
	float iter_samples = floor((opt_start_heat*1.0)/pow(opt_alpha_heat,i-1));
	//cout << "at first iteration: " << i << endl;
	double quadrant_avg = 0;
	vector<Quadrant> prev_vect;
		//---- load the Quadrant structure from previous iteation
	if(this->firstIteration)
	{
		cout << "in first iteration" << endl;
		for(int recX = 0; recX < this->cardX ; recX++)
		{
			for(int recY = 0; recY < this->cardY ; recY++)
			{
				record_id_heat[recX][recY] = 0; 
			}
			
		}

		int startX = 0; //xMinF
		int endX = this->cardX-1; //xMaxF

		int startY = 0; //xMinF
		int endY = this->cardY-1; //xMaxF

		for(int gX = startX;gX <= endX;gX++)
		{
			for(int gY = startY;gY <= endY;gY++)
			{
				conv_rate_heat = 1.0;
				uint64_t conv_samples = floor(iter_samples/(this->cardX*this->cardY));
				
				YAML::Node& qry = aqry;
				
				if(this->queryType==1)
				{
					qry["q"]["__nodes__"][this->dimXAttr] = this->groupXMap[gX];
					qry["q"]["__nodes__"][this->dimYAttr] = this->groupYMap[gY];

					uint64_t numerator = swi_heat.perGroupValCount[this->dimXAttr][this->groupXMap[gX]]*swi_heat.perGroupValCount[this->dimYAttr][this->groupYMap[gY]];
					
					uint64_t denominator = schema11.get_num_rows()*schema11.get_num_pages();
					
					uint64_t per_block_sample = ceil((numerator*1.0)/denominator);
					
					float dim_comb_allotted_time = alloted_time/(this->cardX*this->cardY); 
					
					uint64_t num_blocks_allowed = ceil((dim_comb_allotted_time*1.0)/block_read_time_heat);
					
					conv_samples = per_block_sample*num_blocks_allowed;
				}
							

				Query query(qry, schema11);
				query.set_num_samples(conv_samples);
				
				query.set_conv_rate(conv_rate_heat);

				ResultSet results(query.get_targets().size());

				
				record_id_heat[gX][gY] = swi_heat.run_query(query, results, sample_type_heat,record_id_heat[gX][gY],firstIteration,this->groupXMap[gX],this->groupYMap[gY]);
				if(swi_heat.isRecordEmpty==true)
				{
					record_id_heat[gX][gY] = -1;


					continue;
				}
				
				
				std::vector<double> avgs = results.get_avgs();
				groupAvgs_heat[gX][gY] = (groupAvgs_heat[gX][gY]*groupNums_heat[gX][gY] + avgs[0]*conv_samples)/(groupNums_heat[gX][gY]+conv_samples);
				groupNums_heat[gX][gY] += conv_samples;

				quadrant_avg += avgs[0];

			}
		}

		
		
		quadrant_avg = quadrant_avg/((endX-startX+1)*(endY-startY+1));

		vector<Quadrant> tmp;
		

		Quadrant tmpSeg(startX,endX,startY,endY,quadrant_avg);
		
		tmp.push_back(tmpSeg); 
		k_Best_heat[i] = tmp;

		this->firstIteration = false;

		this->split_quad = -1;

		this->firstNewQuadrant.set_xStart(startX);
		this->firstNewQuadrant.set_xEnd(endX);
		this->firstNewQuadrant.set_yStart(startY);
		this->firstNewQuadrant.set_yEnd(endY);
		this->firstNewQuadrant.set_mean(quadrant_avg);

		this->avgMin = quadrant_avg;
		this->avgMax = quadrant_avg;

	}
		
	else
	{
		prev_vect = k_Best_heat[i-1];
		double global_quad_mean = 0;
		int split_Quadrant_no = -1;
		int split_quad_maxSplitX = -1;
		int split_quad_maxSplitY = -1;
		double global_max = -1000000000;
		double global_best_mean1 = 0, global_best_mean2 = 0, global_best_mean3 = 0, global_best_mean4 = 0;
		
		for(int s=0;s<prev_vect.size();s++)
		{
			
			int startX = prev_vect[s].get_xStart();
			int endX = prev_vect[s].get_xEnd();
			int startY = prev_vect[s].get_yStart();
			int endY = prev_vect[s].get_yEnd();

			if((endX - startX) < 1 && (endY - startY) < 1)
				continue;
			//---- ask for N samples from a non-single Quadrant group for some target/measure and get the avg 
			

			for(int gX = startX;gX <= endX;gX++)
			{
				for(int gY = startY;gY <= endY;gY++)
				{
					if(record_id_heat[gX][gY]==-1)
						continue;
					conv_rate_heat = 1.0;
					uint64_t conv_samples = floor(iter_samples/(this->cardX*this->cardY));

					YAML::Node& qry = aqry;
					
					if(this->queryType==1)
					{
						qry["q"]["__nodes__"][this->dimXAttr] = this->groupXMap[gX];
						qry["q"]["__nodes__"][this->dimYAttr] = this->groupYMap[gY];

						uint64_t numerator = swi_heat.perGroupValCount[this->dimXAttr][this->groupXMap[gX]]*swi_heat.perGroupValCount[this->dimYAttr][this->groupYMap[gY]];
						
						uint64_t denominator = schema11.get_num_rows()*schema11.get_num_pages();
						uint64_t per_block_sample = ceil((numerator*1.0)/denominator);
						
						float dim_comb_allotted_time = alloted_time/(this->cardX*this->cardY); 
						
						uint64_t num_blocks_allowed = ceil((dim_comb_allotted_time*1.0)/block_read_time_heat);
						
						conv_samples = per_block_sample*num_blocks_allowed;

					}
					
					//Query query(q, schema11);
					Query query(qry, schema11);
					query.set_num_samples(conv_samples);
					
					query.set_conv_rate(conv_rate_heat);

					ResultSet results(query.get_targets().size());

					
					record_id_heat[gX][gY] = swi_heat.run_query(query, results, sample_type_heat,record_id_heat[gX][gY],firstIteration,this->groupXMap[gX],this->groupYMap[gY]);
					if(swi_heat.isRecordEmpty==true)
					{
						record_id_heat[gX][gY] = -1;
						continue;
					}
					std::vector<double> avgs = results.get_avgs();
					groupAvgs_heat[gX][gY] = (groupAvgs_heat[gX][gY]*groupNums_heat[gX][gY] + avgs[0]*conv_samples)/(groupNums_heat[gX][gY]+conv_samples);
					groupNums_heat[gX][gY] += conv_samples;

				}
			}



		}	
		
		for(int s=0;s<prev_vect.size();s++)
		{
			
			int startX = prev_vect[s].get_xStart();
			int endX = prev_vect[s].get_xEnd();
			int startY = prev_vect[s].get_yStart();
			int endY = prev_vect[s].get_yEnd();

			if((endX - startX) < 1 && (endY - startY) < 1)
				continue;
			//---- ask for N samples from a non-single Quadrant group for some target/measure and get the avg 
			
			double alphaT = 0;
			double alphaU = 0;
			double alphaV = 0;
			double alphaW = 0;
			double quad_mean = 0;
			double best_mean1 = 0;
			double best_mean2 = 0;
			double best_mean3 = 0;
			double best_mean4 = 0;
			double tmp1, tmp2, tmp3, tmp4;
			double max = -1000000000;
			int maxSplitX = -1;
			int maxSplitY = -1;

			for(int gX = startX;gX <= endX;gX++)
			{
				for(int gY = startY;gY <= endY;gY++)
				{
						
					if(record_id_heat[gX][gY]==-1)
						continue;
					// split in 4
					alphaT = 0;

					for (int kx = startX; kx <= gX; kx++) {
						for (int ky = gY; ky <= endY; ky++) {
							alphaT += groupAvgs_heat[kx][ky];
						}
					}

					alphaT = (1.0 / ((gX - startX + 1) * (endY - gY + 1))) * alphaT; // V_s

					tmp1 = alphaT;

					quad_mean += (alphaT * alphaT * (gX - startX + 1) * (endY - gY + 1));
					
					
					alphaU = 0;

					for (int kx = gX + 1; kx <= endX; kx++) {
						for (int ky = gY; ky <= endY; ky++) {
							alphaU += groupAvgs_heat[kx][ky];
						}
					}

					if(gX<endX)
						alphaU = (1.0 / ((endX - (gX + 1) + 1) * (endY - gY + 1))) * alphaU; // V_s
					else
						alphaU = 0;

					tmp2 = alphaU;

					quad_mean += (alphaU * alphaU * (endX - (gX + 1) + 1) * (endY - gY + 1));

					alphaV = 0;

						
					for (int kx = gX + 1; kx <= endX; kx++) {
						for (int ky = gY - 1; ky >= startY; ky--) {
							alphaV += groupAvgs_heat[kx][ky];
						}
					}

					
					if(gX<endX && gY>startY) 
						alphaV = (1.0 / ((endX - (gX + 1) + 1) * (gY-1 - startY + 1))) * alphaV; // V_s
					else
						alphaV = 0;
					
					tmp3 = alphaV;

					quad_mean += (alphaV * alphaV * (endX - (gX + 1) + 1) * (gY-1 - startY + 1));

					alphaW = 0;

					for (int kx = startX; kx <= gX; kx++) {
						for (int ky = gY - 1; ky >= startY; ky--) {
							alphaW += groupAvgs_heat[kx][ky];
						}
					}

					if(gY>startY)
						alphaW = (1.0 / ((gX - startX + 1) * (gY-1 - startY + 1))) * alphaW; // V_s
					else
						alphaW = 0;

					
					tmp4 = alphaW;

					quad_mean += (alphaW * alphaW * (gX - startX + 1) * (gY-1 - startY + 1));
							
					if (quad_mean > max) {
						max = quad_mean;
						maxSplitX = gX;
						maxSplitY = gY;
						best_mean1 = tmp1;
						best_mean2 = tmp2;
						best_mean3 = tmp3;
						best_mean4 = tmp4;

						
					}

					quad_mean = 0;
					alphaT = 0;
					alphaU=0;
					alphaV = 0;
					alphaW=0;
				}
			}

			//---- find the global best split point
			global_quad_mean = max;
		
			if (global_quad_mean > global_max) {
				global_max = global_quad_mean;

				split_Quadrant_no = s;
				split_quad_maxSplitX = maxSplitX;
				split_quad_maxSplitY = maxSplitY;

				global_best_mean1 = best_mean1;
				global_best_mean2 = best_mean2;
				global_best_mean3 = best_mean3;
				global_best_mean4 = best_mean4;

			}

			global_quad_mean = 0;

		}
			
			
		//---- create new Quadrant structure and push


		vector<Quadrant> tmp;
		Quadrant tmpSeg1(0, 0, 0, 0, 0);
		Quadrant tmpSeg2(0, 0, 0, 0,0);
		Quadrant tmpSeg3(0, 0, 0, 0, 0);
		Quadrant tmpSeg4(0, 0, 0, 0,0);
		//vector<ResultObject> resVector;
		

		for (int s = 0; s < prev_vect.size(); s++) {
			if (s == split_Quadrant_no) {
				
				if (split_quad_maxSplitX < prev_vect[s].get_xEnd()) {
					if (split_quad_maxSplitY == prev_vect[s].get_yStart()) {
						// split in two quad 1 and 2
						
						tmpSeg1.set_xStart(prev_vect[s].get_xStart());
						tmpSeg1.set_xEnd(split_quad_maxSplitX);
						tmpSeg1.set_yStart(prev_vect[s].get_yStart());
						tmpSeg1.set_yEnd(prev_vect[s].get_yEnd());
						tmpSeg1.set_mean(global_best_mean1);

						tmp.push_back(tmpSeg1);

						tmpSeg2.set_xStart(split_quad_maxSplitX+1);
						tmpSeg2.set_xEnd(prev_vect[s].get_xEnd());
						tmpSeg2.set_yStart(prev_vect[s].get_yStart());
						tmpSeg2.set_yEnd(prev_vect[s].get_yEnd());
						tmpSeg2.set_mean(global_best_mean2);
						
						tmp.push_back(tmpSeg2);

						this->split_quad = split_Quadrant_no;

						this->firstNewQuadrant.set_xStart(prev_vect[s].get_xStart());
						this->firstNewQuadrant.set_xEnd(split_quad_maxSplitX);
						this->firstNewQuadrant.set_yStart(prev_vect[s].get_yStart());
						this->firstNewQuadrant.set_yEnd(prev_vect[s].get_yEnd());
						this->firstNewQuadrant.set_mean(global_best_mean1);

						this->secondNewQuadrant.set_xStart(split_quad_maxSplitX+1);
						this->secondNewQuadrant.set_xEnd(prev_vect[s].get_xEnd());
						this->secondNewQuadrant.set_yStart(prev_vect[s].get_yStart());
						this->secondNewQuadrant.set_yEnd(prev_vect[s].get_yEnd());
						this->secondNewQuadrant.set_mean(global_best_mean2);

						this->isTwoSeg = true;

						
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
						
												

					} else {
						// split in four
												

						tmpSeg1.set_xStart(prev_vect[s].get_xStart());
						tmpSeg1.set_xEnd(split_quad_maxSplitX);
						tmpSeg1.set_yStart(split_quad_maxSplitY);
						tmpSeg1.set_yEnd(prev_vect[s].get_yEnd());
						tmpSeg1.set_mean(global_best_mean1);

						tmp.push_back(tmpSeg1);

						tmpSeg2.set_xStart(split_quad_maxSplitX+1);
						tmpSeg2.set_xEnd(prev_vect[s].get_xEnd());
						tmpSeg2.set_yStart(split_quad_maxSplitY);
						tmpSeg2.set_yEnd(prev_vect[s].get_yEnd());
						tmpSeg2.set_mean(global_best_mean2);
						
						tmp.push_back(tmpSeg2);

						tmpSeg3.set_xStart(split_quad_maxSplitX+1);
						tmpSeg3.set_xEnd(prev_vect[s].get_xEnd());
						tmpSeg3.set_yStart(prev_vect[s].get_yStart());
						tmpSeg3.set_yEnd(split_quad_maxSplitY-1);
						tmpSeg3.set_mean(global_best_mean3);

						tmp.push_back(tmpSeg3);

						tmpSeg4.set_xStart(prev_vect[s].get_xStart());
						tmpSeg4.set_xEnd(split_quad_maxSplitX);
						tmpSeg4.set_yStart(prev_vect[s].get_yStart());
						tmpSeg4.set_yEnd(split_quad_maxSplitY-1);
						tmpSeg4.set_mean(global_best_mean4);
						
						tmp.push_back(tmpSeg4);

						this->split_quad = split_Quadrant_no;

						this->firstNewQuadrant.set_xStart(prev_vect[s].get_xStart());
						this->firstNewQuadrant.set_xEnd(split_quad_maxSplitX);
						this->firstNewQuadrant.set_yStart(split_quad_maxSplitY);
						this->firstNewQuadrant.set_yEnd(prev_vect[s].get_yEnd());
						this->firstNewQuadrant.set_mean(global_best_mean1);

						this->secondNewQuadrant.set_xStart(split_quad_maxSplitX+1);
						this->secondNewQuadrant.set_xEnd(prev_vect[s].get_xEnd());
						this->secondNewQuadrant.set_yStart(split_quad_maxSplitY);
						this->secondNewQuadrant.set_yEnd(prev_vect[s].get_yEnd());
						this->secondNewQuadrant.set_mean(global_best_mean2);

						this->thirdNewQuadrant.set_xStart(split_quad_maxSplitX+1);
						this->thirdNewQuadrant.set_xEnd(prev_vect[s].get_xEnd());
						this->thirdNewQuadrant.set_yStart(prev_vect[s].get_yStart());
						this->thirdNewQuadrant.set_yEnd(split_quad_maxSplitY-1);
						this->thirdNewQuadrant.set_mean(global_best_mean3);

						this->fourthNewQuadrant.set_xStart(prev_vect[s].get_xStart());
						this->fourthNewQuadrant.set_xEnd(split_quad_maxSplitX);
						this->fourthNewQuadrant.set_yStart(prev_vect[s].get_yStart());
						this->fourthNewQuadrant.set_yEnd(split_quad_maxSplitY-1);
						this->fourthNewQuadrant.set_mean(global_best_mean4);

						this->isTwoSeg = false;
						this->isOneCell = false;

						double tmp_mean = getMax(getMax(global_best_mean1,global_best_mean2),getMax(global_best_mean3,global_best_mean4));
						if(this->avgMax < tmp_mean)
							this->avgMax = tmp_mean;

						tmp_mean = getMin(getMin(global_best_mean1,global_best_mean2),getMin(global_best_mean3,global_best_mean4));
						
						if(this->avgMin > tmp_mean)
							this->avgMin = tmp_mean;
					}
				} 
				else {
					if (split_quad_maxSplitY == prev_vect[s].get_yStart()) {
						// no split
			
						
						//lets split
						if(prev_vect[s].get_xEnd()-prev_vect[s].get_xStart()==0) 
						{
							//split in two
							if(prev_vect[s].get_yEnd()-prev_vect[s].get_yStart()!=0) // one column
							{
								float avgTemp = groupAvgs_heat[prev_vect[s].get_xEnd()][prev_vect[s].get_yStart()];
								
								float totalCells = (prev_vect[s].get_xEnd()-prev_vect[s].get_xStart()+1)* (prev_vect[s].get_yEnd()-prev_vect[s].get_yStart()+1);
								
								float newAvg = (totalCells*prev_vect[s].get_mean()-avgTemp)/(totalCells-1);
								
								tmpSeg1.set_xStart(prev_vect[s].get_xStart());
								tmpSeg1.set_xEnd(prev_vect[s].get_xEnd());
								tmpSeg1.set_yStart(prev_vect[s].get_yStart()+1);
								tmpSeg1.set_yEnd(prev_vect[s].get_yEnd());
								tmpSeg1.set_mean(newAvg);

								tmp.push_back(tmpSeg1);

								tmpSeg2.set_xStart(prev_vect[s].get_xStart());
								tmpSeg2.set_xEnd(prev_vect[s].get_xEnd());
								tmpSeg2.set_yStart(prev_vect[s].get_yStart());
								tmpSeg2.set_yEnd(prev_vect[s].get_yStart());
								tmpSeg2.set_mean(avgTemp);

								tmp.push_back(tmpSeg2);

								this->split_quad = split_Quadrant_no;

								this->firstNewQuadrant.set_xStart(prev_vect[s].get_xStart());
								this->firstNewQuadrant.set_xEnd(prev_vect[s].get_xEnd());
								this->firstNewQuadrant.set_yStart(prev_vect[s].get_yStart()+1);
								this->firstNewQuadrant.set_yEnd(prev_vect[s].get_yEnd());
								this->firstNewQuadrant.set_mean(newAvg);

								this->secondNewQuadrant.set_xStart(prev_vect[s].get_xStart());
								this->secondNewQuadrant.set_xEnd(prev_vect[s].get_xEnd());
								this->secondNewQuadrant.set_yStart(prev_vect[s].get_yStart());
								this->secondNewQuadrant.set_yEnd(prev_vect[s].get_yStart());
								this->secondNewQuadrant.set_mean(avgTemp);

								this->isTwoSeg = true;

								double tmp_mean = newAvg;
								if(avgTemp > tmp_mean)
									tmp_mean = avgTemp;
								if(this->avgMax < tmp_mean)
									this->avgMax = tmp_mean;
								
								tmp_mean = newAvg;
								if(avgTemp < tmp_mean)
									tmp_mean = avgTemp;

								if(this->avgMin > tmp_mean)
									this->avgMin = tmp_mean;


								
								
								
							}
							else //one cell
							{
								tmpSeg1.set_xStart(prev_vect[s].get_xStart());
								tmpSeg1.set_xEnd(prev_vect[s].get_xEnd());
								tmpSeg1.set_yStart(prev_vect[s].get_yStart());
								tmpSeg1.set_yEnd(prev_vect[s].get_yEnd());
								tmpSeg1.set_mean(prev_vect[s].get_mean());

								tmp.push_back(tmpSeg1);

								this->isTwoSeg =  false;
								this->isOneCell = true;
								
								this->split_quad = split_Quadrant_no;


							}
							
						}
						else
						{
							//split in two
							if(prev_vect[s].get_yEnd()-prev_vect[s].get_yStart()==0) //one row
							{
								
								float avgTemp = groupAvgs_heat[prev_vect[s].get_xEnd()][prev_vect[s].get_yStart()];
								
								float totalCells = (prev_vect[s].get_xEnd()-prev_vect[s].get_xStart()+1)* (prev_vect[s].get_yEnd()-prev_vect[s].get_yStart()+1);
								
								float newAvg = (totalCells*prev_vect[s].get_mean()-avgTemp)/(totalCells-1);
								
								
								tmpSeg1.set_xStart(prev_vect[s].get_xStart());
								tmpSeg1.set_xEnd(prev_vect[s].get_xEnd()-1);
								tmpSeg1.set_yStart(prev_vect[s].get_yStart());
								tmpSeg1.set_yEnd(prev_vect[s].get_yEnd());
								tmpSeg1.set_mean(newAvg);

								tmp.push_back(tmpSeg1);

								tmpSeg2.set_xStart(prev_vect[s].get_xEnd());
								tmpSeg2.set_xEnd(prev_vect[s].get_xEnd());
								tmpSeg2.set_yStart(prev_vect[s].get_yStart());
								tmpSeg2.set_yEnd(prev_vect[s].get_yEnd());
								tmpSeg2.set_mean(avgTemp);

								tmp.push_back(tmpSeg2);

								this->split_quad = split_Quadrant_no;

								this->firstNewQuadrant.set_xStart(prev_vect[s].get_xStart());
								this->firstNewQuadrant.set_xEnd(prev_vect[s].get_xEnd()-1);
								this->firstNewQuadrant.set_yStart(prev_vect[s].get_yStart());
								this->firstNewQuadrant.set_yEnd(prev_vect[s].get_yEnd());
								this->firstNewQuadrant.set_mean(newAvg);

								this->secondNewQuadrant.set_xStart(prev_vect[s].get_xEnd());
								this->secondNewQuadrant.set_xEnd(prev_vect[s].get_xEnd());
								this->secondNewQuadrant.set_yStart(prev_vect[s].get_yStart());
								this->secondNewQuadrant.set_yEnd(prev_vect[s].get_yEnd());
								this->secondNewQuadrant.set_mean(avgTemp);

								this->isTwoSeg = true;
								this->isOneCell = false;

								double tmp_mean = newAvg;
								if(avgTemp > tmp_mean)
									tmp_mean = avgTemp;
								if(this->avgMax < tmp_mean)
									this->avgMax = tmp_mean;
								
								tmp_mean = newAvg;
								if(avgTemp < tmp_mean)
									tmp_mean = avgTemp;

								if(this->avgMin > tmp_mean)
									this->avgMin = tmp_mean;
								
							}
							else //total cell--rectangle. split in 4
							{
								float quad3 = groupAvgs_heat[prev_vect[s].get_xEnd()][prev_vect[s].get_yStart()]; //quad 3
																	
								float quad1=0,quad2=0,quad4=0;
								
								//quad 2
								
								for(int q=prev_vect[s].get_yStart()+1;q<=prev_vect[s].get_yEnd();q++)
								{
									quad2+= groupAvgs_heat[prev_vect[s].get_xEnd()][q];
									
								}
								quad2 = quad2/(prev_vect[s].get_yEnd()-(prev_vect[s].get_yStart()+1)+1);
								//quad 4
								
								for(int p=prev_vect[s].get_xStart();p<prev_vect[s].get_xEnd();p++)
								{
									quad4+= groupAvgs_heat[p][prev_vect[s].get_yStart()];
									
								}
								quad4 = quad4/(prev_vect[s].get_xEnd()-1-prev_vect[s].get_xStart()+1); 
								
	
								
								quad1= prev_vect[s].get_mean()-(quad2+quad3+quad4);
								
								tmpSeg1.set_xStart(prev_vect[s].get_xStart());
								tmpSeg1.set_xEnd(prev_vect[s].get_xEnd()-1);
								tmpSeg1.set_yStart(prev_vect[s].get_yStart()+1);
								tmpSeg1.set_yEnd(prev_vect[s].get_yEnd());
								tmpSeg1.set_mean(quad1);

								tmp.push_back(tmpSeg1);

								tmpSeg2.set_xStart(prev_vect[s].get_xEnd());
								tmpSeg2.set_xEnd(prev_vect[s].get_xEnd());
								tmpSeg2.set_yStart(prev_vect[s].get_yStart()+1);
								tmpSeg2.set_yEnd(prev_vect[s].get_yEnd());
								tmpSeg2.set_mean(quad2);
								
								tmp.push_back(tmpSeg2);

								tmpSeg3.set_xStart(prev_vect[s].get_xEnd());
								tmpSeg3.set_xEnd(prev_vect[s].get_xEnd());
								tmpSeg3.set_yStart(prev_vect[s].get_yStart());
								tmpSeg3.set_yEnd(prev_vect[s].get_yStart());
								tmpSeg3.set_mean(quad3);

								tmp.push_back(tmpSeg3);

								tmpSeg4.set_xStart(prev_vect[s].get_xStart());
								tmpSeg4.set_xEnd(prev_vect[s].get_xEnd()-1);
								tmpSeg4.set_yStart(prev_vect[s].get_yStart());
								tmpSeg4.set_yEnd(prev_vect[s].get_yStart());
								tmpSeg4.set_mean(quad4);
								
								tmp.push_back(tmpSeg4);

								this->split_quad = split_Quadrant_no;

								this->firstNewQuadrant.set_xStart(prev_vect[s].get_xStart());
								this->firstNewQuadrant.set_xEnd(prev_vect[s].get_xEnd()-1);
								this->firstNewQuadrant.set_yStart(prev_vect[s].get_yStart()+1);
								this->firstNewQuadrant.set_yEnd(prev_vect[s].get_yEnd());
								this->firstNewQuadrant.set_mean(quad1);

								this->secondNewQuadrant.set_xStart(prev_vect[s].get_xEnd());
								this->secondNewQuadrant.set_xEnd(prev_vect[s].get_xEnd());
								this->secondNewQuadrant.set_yStart(prev_vect[s].get_yStart()+1);
								this->secondNewQuadrant.set_yEnd(prev_vect[s].get_yEnd());
								this->secondNewQuadrant.set_mean(quad2);
								
								this->thirdNewQuadrant.set_xStart(prev_vect[s].get_xEnd());
								this->thirdNewQuadrant.set_xEnd(prev_vect[s].get_xEnd());
								this->thirdNewQuadrant.set_yStart(prev_vect[s].get_yStart());
								this->thirdNewQuadrant.set_yEnd(prev_vect[s].get_yStart());
								this->thirdNewQuadrant.set_mean(quad3);

								this->fourthNewQuadrant.set_xStart(prev_vect[s].get_xStart());
								this->fourthNewQuadrant.set_xEnd(prev_vect[s].get_xEnd()-1);
								this->fourthNewQuadrant.set_yStart(prev_vect[s].get_yStart());
								this->fourthNewQuadrant.set_yEnd(prev_vect[s].get_yStart());
								this->fourthNewQuadrant.set_mean(quad4);
								
								this->isTwoSeg = false;
								this->isOneCell = false;
								
								double tmp_mean = getMax(getMax(quad1,quad2),getMax(quad3,quad4));
								if(this->avgMax < tmp_mean)
									this->avgMax = tmp_mean;

								tmp_mean = getMin(getMin(quad1,quad2),getMin(quad3,quad4));
								
								if(this->avgMin > tmp_mean)
									this->avgMin = tmp_mean;
							}
							
						}
					} else {
						// split in two quad 1 and quad 4
						
						
						tmpSeg1.set_xStart(prev_vect[s].get_xStart());
						tmpSeg1.set_xEnd(prev_vect[s].get_xEnd());
						tmpSeg1.set_yStart(split_quad_maxSplitY);
						tmpSeg1.set_yEnd(prev_vect[s].get_yEnd());
						tmpSeg1.set_mean(global_best_mean1);

						tmp.push_back(tmpSeg1);

						tmpSeg2.set_xStart(prev_vect[s].get_xStart());
						tmpSeg2.set_xEnd(prev_vect[s].get_xEnd());
						tmpSeg2.set_yStart(prev_vect[s].get_yStart());
						tmpSeg2.set_yEnd(split_quad_maxSplitY-1);
						tmpSeg2.set_mean(global_best_mean4);
						
						tmp.push_back(tmpSeg2);

						this->split_quad = split_Quadrant_no;

						this->firstNewQuadrant.set_xStart(prev_vect[s].get_xStart());
						this->firstNewQuadrant.set_xEnd(prev_vect[s].get_xEnd());
						this->firstNewQuadrant.set_yStart(split_quad_maxSplitY);
						this->firstNewQuadrant.set_yEnd(prev_vect[s].get_yEnd());
						this->firstNewQuadrant.set_mean(global_best_mean1);

						this->secondNewQuadrant.set_xStart(prev_vect[s].get_xStart());
						this->secondNewQuadrant.set_xEnd(prev_vect[s].get_xEnd());
						this->secondNewQuadrant.set_yStart(prev_vect[s].get_yStart());
						this->secondNewQuadrant.set_yEnd(split_quad_maxSplitY-1);
						this->secondNewQuadrant.set_mean(global_best_mean4);

						this->isTwoSeg = true;
						this->isOneCell = false;

						double tmp_mean = global_best_mean1;
						if(global_best_mean4 > tmp_mean)
							tmp_mean = global_best_mean4;
						if(this->avgMax < tmp_mean)
							this->avgMax = tmp_mean;
						
						tmp_mean = global_best_mean1;
						if(global_best_mean4 < tmp_mean)
							tmp_mean = global_best_mean4;

						if(this->avgMin > tmp_mean)
							this->avgMin = tmp_mean;
						
					}

				}

				

			} else {
				tmp.push_back(prev_vect[s]); 	
				
			}
		
		

			


		}

	
		//cout << "split done" << endl;
		k_Best_heat[i] = tmp;

		
		vector<Quadrant>().swap(tmp);
		vector<Quadrant>().swap(prev_vect);

	}
			


}

void Heat::genHeatBase(int iteration,SDB& db11,Schema& schema11)
{
		// required variables
	cout << "in gen charts: " << iteration << endl;
	
	
	swi_heat.isTrend = false;
	
    std::ostringstream temp;


	int i = iteration;
	string query_fname = "";
	string qt = "./incvisage-cpp/needletail-code-base/scripts/";

	if(this->queryType==1)
		query_fname = qt+"qh1";
	
	temp.str("");
	temp.clear();
	temp << query_fname;
	char* q = new char[temp.str().length() + 1];
	std::strcpy(q, temp.str().c_str());

	YAML::Node aqry = YAML::LoadFile(q);

	this->avgMin = 10000000;
  	this->avgMax = -10000000;
		//---- load the Quadrant structure from previous iteation
	if(this->firstIteration)
	{
		cout << "in first iteration" << endl;
		for(int recX = 0; recX < this->cardX ; recX++)
		{
			for(int recY = 0; recY < this->cardY ; recY++)
			{
				record_id_heat[recX][recY] = 0; 
			}
			
		}
	}


		int startX = 0; //xMinF
		int endX = this->cardX-1; //xMaxF

		int startY = 0; //xMinF
		int endY = this->cardY-1; //xMaxF

		for(int gX = startX;gX <= endX;gX++)
		{
			for(int gY = startY;gY <= endY;gY++)
			{
				if(record_id_heat[gX][gY]==-1)
					continue;

				conv_rate_heat = 1.0;
				int conv_samples = 1;

				YAML::Node& qry = aqry;
				
				if(this->queryType==1)
				{
					qry["q"]["__nodes__"][this->dimXAttr] = this->groupXMap[gX];
					qry["q"]["__nodes__"][this->dimYAttr] = this->groupYMap[gY];
				}

				Query query(qry, schema11);
				query.set_num_samples(conv_samples);
				
				query.set_conv_rate(conv_rate_heat);

				ResultSet results(query.get_targets().size());

				
				record_id_heat[gX][gY] = swi_heat.run_query(query, results, sample_type_heat,record_id_heat[gX][gY],firstIteration,this->groupXMap[gX],this->groupYMap[gY]);
				if(swi_heat.isRecordEmpty==true)
				{
					record_id_heat[gX][gY] = -1;
					continue;
				}
				
				
				std::vector<double> avgs = results.get_avgs();
				groupAvgs_heat[gX][gY] = (groupAvgs_heat[gX][gY]*groupNums_heat[gX][gY] + avgs[0]*conv_samples)/(groupNums_heat[gX][gY]+conv_samples);
				groupNums_heat[gX][gY] += conv_samples;

				string key = to_string(this->groupXMap[gX])+","+to_string(this->groupYMap[gY]);
				
				groupAvgMap[key] = groupAvgs_heat[gX][gY];

				if(this->avgMax < groupAvgs_heat[gX][gY])
					this->avgMax = groupAvgs_heat[gX][gY];
			

				if(this->avgMin > groupAvgs_heat[gX][gY])
					this->avgMin = groupAvgs_heat[gX][gY];

			}
		}

		
		
		this->firstIteration = false;

		isContinue = toContinue();

			
}

int Heat::toContinue()
{
	for(int recX = 0; recX < this->cardX ; recX++)
	{
		for(int recY = 0; recY < this->cardY ; recY++)
		{
			if(record_id_heat[recX][recY] != -1)
				return 1; 
		}
		
	}

	return 0;

}

bool Heat::validCells(int gX,int gY)
{	
	if(record_id_heat[gX][gY]==-1)
		return false;
	return true;
}
