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

#include <boost/unordered_map.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

//#include <gperftools/profiler.h>


#include "csv.h"
//#include "intel_sample.h"
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
//#define NUM_ROWS (20000000)

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


void incVisage(int argc, char *argv[])
{
	// required variables
	const char usage[] =
			"USAGE: %s [-i swi] [-o swi] [-q queryfile] [-r rnd] [-f] [-b impl] [-n rows] [-d density] [-t distribution] [-p datasetcount] sdb schema\n";
	//bool import_csv = false;
	//const char* csv_fname = NULL;
	const char* query_fname = NULL;
	bool old_swi = false;
	bool new_swi = false;
	bool use_query = false;
	const char* swi_fname = "swi_file";
	const char* bmap_impl = NULL;
	uint64_t num_rows = 10000000;
	uint64_t dataset_count = DATASET_COUNT;
	const char* randomness = NULL;
	double density = 0.10;
        double clustering_rate=0.10;
	string distribution = "uniform";
	string indextype = "density";
	double conv_rate = 1.0;
	int trials =10;
	string dataset_name = "synthetic";
	
	char c;
	while ((c = getopt(argc, argv, "i:o:r:q:b:n:d:t:c:x:m:p")) != -1) {
		switch (c) {
		//case 'c':
		//    import_csv = true;
		//    csv_fname = optarg;
		//    break;
		case 'i':
			old_swi = true;
			swi_fname = optarg;
			break;
		case 'o':
			new_swi = true;
			swi_fname = optarg;
			break;
		case 'r':
			randomness = optarg;
			break;
		case 'q':
			use_query = true;
			query_fname = optarg;
			break;
		case 'b':
			bmap_impl = optarg;
			break;
		case 'n':
			num_rows = string2uint64_t(string(optarg));
			break;
		case 'd':
//			clustering_rate = string2double(string(optarg));
			density = string2double(string(optarg));
			break;
		case 't':
			distribution = (string(optarg));
			break;
       		case 'p':
			dataset_count = string2uint64_t(string(optarg));
			break;
		case 'x':
			conv_rate = string2double(string(optarg));
			break;
		case 'c':
                        clustering_rate = string2double(string(optarg));
                        break;
		case 'm':
                        dataset_name = (string(optarg));
                        break;
		}
	}

	if (old_swi && new_swi) {
		fprintf(stderr, usage, argv[0]);
		exit(2);
	}

	if ((optind + 2) != argc) {
		fprintf(stderr, usage, argv[0]);
		exit(2);
	}

	SwiftMapFactory::map_type_t bmap_type = SwiftMapFactory::POOL_LAZY_BITMAP;

        if (bmap_impl) {
		string bmap_impl_s(bmap_impl);
		indextype=bmap_impl_s;
		if (bmap_impl_s == "single") {
			bmap_type = SwiftMapFactory::SINGLE_BITMAP;
		} else if (bmap_impl_s == "lazy") {
			bmap_type = SwiftMapFactory::LAZY_BITMAP;
		} else if (bmap_impl_s == "lazy_s") {
			bmap_type = SwiftMapFactory::STATIC_LAZY_BITMAP;
		} else if (bmap_impl_s == "pool_lazy") {
			bmap_type = SwiftMapFactory::POOL_LAZY_BITMAP;
		} else if (bmap_impl_s == "pool_lazy_s") {
			bmap_type = SwiftMapFactory::POOL_STATIC_LAZY_BITMAP;
		} else if (bmap_impl_s == "density") {
			bmap_type = SwiftMapFactory::DENSITY_MAP;
		} else if (bmap_impl_s == "noindex") {
			bmap_type = SwiftMapFactory::NO_INDEX;
		} else if (bmap_impl_s == "ewah") {
      bmap_type = SwiftMapFactory::EWAH_BITMAP;
    } else if (bmap_impl_s == "lossy"){
      bmap_type = SwiftMapFactory::LOSSY_BITMAP;
  }
	}

	SwiftMap::sample_type_t sample_type = SwiftMap::SEQUENTIAL;


	vector<SwiftMap::sample_type_t> list_sample_types = { SwiftMap::TWOPRONG  ,SwiftMap::SEQUENTIAL, SwiftMap::PSEUDORANDOM};

	const char* sdb_fname = argv[optind];
	const char* schema_fname = argv[optind + 1];

cout << "creating dummy" << endl;
	
	Schema schema_dummy("/home/srahman7/needletail-code-base/data/schema_9", 100000000);

	SDB sdb_dummy("/home/srahman7/needletail-code-base/data/sdb_9", schema_dummy);
    sdb_dummy.clear();
    cout << "clear dummy" << endl;
    Binner binner_dummy;
    SwiftIndex swi_dummy(sdb_dummy, schema_dummy, binner_dummy, bmap_type,
                                              old_swi ? swi_fname : NULL);

    cout << "done dummy" << endl;

    std::ostringstream temp;

    temp << "/home/srahman7/needletail-code-base/results/results_IV_" << dataset_name << ".csv" << ends;

    ofstream results_file;
	results_file.open(temp.str(),std::ofstream::out | std::ofstream::app);

	results_file <<"Trial"<<","<< "Wall Clock Time (ms)" << "," << "CPU Time (ms)" <<","<< "IO Time (ms)" << endl;

	pid_t pid = fork();
	if (pid == 0) {
		//load data

		Schema schema(schema_fname, 0);

		SDB sdb(sdb_fname, schema);

		sdb.clear();
		Binner binner;

                    
		SwiftIndex swi(sdb, schema, binner, bmap_type,
				old_swi ? swi_fname : NULL);
	
		if (new_swi) {
			swi.save(swi_fname);
		}

		cout << "swift index done i" << endl;
		
		std::string data_set = sdb_fname;
		std::string t11 ("sdb_2");
		std::string t13 ("sdb_4");
		int card = 0;
		//get dimension cardinailty and min-max---- set manually
		if(data_set.find(t11) != std::string::npos)
		{
			cout << "t11" << endl;
			card = 365;
		}
		else if(data_set.find(t13) != std::string::npos)
		{
			cout << "t13" << endl;
			card = 365;
		}
		else
		{
			card = 366;
		}


		for(int t = 1; t <= trials; t++ )
		{	
			double cpu_time = 0;
			double io_time = 0;
			double wc_time = 0;

			bool firstIteration = true;
			// initialize segment data structure
			vector<Segment> k_Best[card];
			double groupAvgs[card]; 
			int groupNums[card];

			int opt_start = 25000;
			float opt_alpha = 1.02;

			int nonSingleGroups = card;
			double avg_map[366][366]={0};
			cout << "trial: " << t << endl;

			

			uint64_t row = 0;
            while (row < 100000000){
                    sdb_dummy.get_record(row);
                    row += sdb_dummy.records_per_buf();
            }

            vector<Segment> tmp0;
			Segment tmpSeg10(0, 365, 10);
			tmp0.push_back(tmpSeg10);
			k_Best[0] = tmp0;

			sdb_dummy.clear();
			uint64_t record_id[card];
			for(int rec = 0; rec < card ; rec++)
			{
				record_id[rec] = 0;
				
			}
			
			//loop from min to max
			
			for(int i=0; i< card;i++)
			{
				//cout << "iteration: " << i << endl;
				double cpu_time_iter = 0;
				double io_time_iter = 0;
				double wc_time_iter = 0;

				double cpu_time_s = 0;
				double io_time_s = 0;
				double wc_time_s = 0;

				double cpu_time_i = 0;
				double io_time_i = 0;
				double wc_time_i = 0;

				float iter_samples = floor((opt_start*1.0)/pow(opt_alpha,i));
				//cout << "at first iteration: " << i << endl;
				double segment_avg = 0;
				vector<Segment> prev_vect;
					//---- load the segment structure from previous iteation
				if(firstIteration)
					prev_vect = k_Best[0];
				else
					prev_vect = k_Best[i-1];

					double global_seg_mean = 0;
					int split_segment_no = -1;
					int split_segment_maxSplit = -1;
					double global_max = -1000000000;
					double global_best_mean1 = 0, global_best_mean2 = 0;

					for(int s=0;s<prev_vect.size();s++)
					{
						
						int start = prev_vect[s].get_start();
						int end = prev_vect[s].get_end();

						if(end - start < 1)
							continue;
						//---- ask for N samples from a non-single segment group for some target/measure and get the avg 
						

						for(int g = start,am=0;g <= end;g++,am++)
						{
							conv_rate = 1.0;
							
							int conv_samples = floor(iter_samples/nonSingleGroups);

							temp.str("");
							temp.clear();
							temp << string(query_fname) << (g+1);
							char* q = new char[temp.str().length() + 1];
							std::strcpy(q, temp.str().c_str());

							Query query(q, schema);
							query.set_num_samples(conv_samples);
							
							query.set_conv_rate(conv_rate);

							ResultSet results(query.get_targets().size());

							temp.str("");
							temp.clear();
							temp << "swi_query" << i << s << g <<t;
							char* swi_query_new = new char[temp.str().length()
									+ 1];
							std::strcpy(swi_query_new, temp.str().c_str());
							std::clock_t c_start_i = std::clock();
								
							clear_mem_time();
							clear_io_timer();
							INIT_TIMER(swi_query_new);
							START_TIMER(swi_query_new);
							start_mem_time();
							//ProfilerStart("cpu.out");
							//std::clock_t c_start = std::clock();
							//swi.run_query(query, results, sample_type);
							record_id[g] = swi.run_query(query, results, sample_type,record_id[g],firstIteration);
							std::vector<double> avgs = results.get_avgs();
							groupAvgs[g-1] = (groupAvgs[g-1]*groupNums[g-1] + avgs[0]*conv_samples)/(groupNums[g-1]+conv_samples);
							groupNums[g-1] += conv_samples;

							if(firstIteration)
								segment_avg += avgs[0];
							else
							{
								avg_map[am][am] = groupAvgs[g-1];
								for(int am_it = 0; am_it <am; am_it++)
								{
									avg_map[am_it][am] = avg_map[am_it][am-1]+groupAvgs[g-1];
								}
							}
								
							std::clock_t c_end_i = std::clock();
							end_mem_time();
							//ProfilerStop();
							END_TIMER(swi_query_new);
							
							double wc_vect = (double) GET_TIMER(swi_query_new);
							double cpu_vect = (c_end_i - c_start_i);
							double io_vect = get_io_timer();

							
								wc_time_i += wc_vect;
								cpu_time_i += cpu_vect;

								io_time_i += io_vect;

								//cout << "Wall Clock S:" << g << "--" << wc_vect  << endl ;
							//	cout << "CPU S:" << g << "--" << cpu_vect  << endl ;
							//	cout << "IO S:" << g << "--" << io_vect  << endl ;

							
							
						}
					
						double alphaT = 0;
						double alphaU = 0;
						double seg_mean = 0;
						double best_mean1 = 0;
						double best_mean2 = 0;
						double tmp1, tmp2;
						double max = -1000000000;
						int maxSplit = -1;

						temp.str("");
						temp.clear();
						temp << "split" << i << s << t;
						char* split_query_new = new char[temp.str().length()
								+ 1];

						std::strcpy(split_query_new, temp.str().c_str());
						
						std::clock_t c_start_s = std::clock();
									
						clear_mem_time();
						clear_io_timer();
						INIT_TIMER(split_query_new);
						START_TIMER(split_query_new);
						start_mem_time();
						if(firstIteration)
						{

							segment_avg /= card;

							vector<Segment> tmp;
							Segment tmpSeg(1,card,segment_avg); 
							tmp.push_back(tmpSeg); 
							k_Best[i] = tmp;

							firstIteration = false;
						}
						else
						{
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

						std::clock_t c_end_s = std::clock();
						end_mem_time();
						//ProfilerStop();
						END_TIMER(split_query_new);
						

						double wc_vect = (double) GET_TIMER(
							split_query_new);
						double cpu_vect = (c_end_s - c_start_s);
						double io_vect = get_io_timer();
						wc_time_s += wc_vect;
						cpu_time_s += cpu_vect;

						io_time_s += io_vect;

						//cout << "Wall Clock DP:" << s << "--"<< wc_vect  << endl ;
						//cout << "CPU DP:" << s << "--"<< cpu_vect  << endl ;
						//cout << "IO DP:" << s << "--"<< io_vect  << endl ;

					}	

					temp.str("");
					temp.clear();
					temp << "split" << i  << t;
					char* split_query_new = new char[temp.str().length()
							+ 1];

					std::strcpy(split_query_new, temp.str().c_str());

					std::clock_t c_start_s = std::clock();
									
					clear_mem_time();
					clear_io_timer();
					INIT_TIMER(split_query_new);
					START_TIMER(split_query_new);
					start_mem_time();
						
						//---- create new segment structure and push
					vector<Segment> tmp;
					Segment tmpSeg(0, 0, 0);
					nonSingleGroups = 0;
					for (int s = 0; s < prev_vect.size(); s++) {
						if (s == split_segment_no) {
							if (split_segment_maxSplit == prev_vect[s].get_end()) {

								tmpSeg.set_start(prev_vect[s].get_start());
								tmpSeg.set_end(split_segment_maxSplit - 1);
								tmpSeg.set_mean(global_best_mean1);
								tmp.push_back(tmpSeg); 

								if(tmpSeg.get_end() - tmpSeg.get_start() > 0)
									nonSingleGroups += tmpSeg.get_end() - tmpSeg.get_start()+1;

								tmpSeg.set_start(split_segment_maxSplit);
								tmpSeg.set_end(prev_vect[s].get_end());
								tmpSeg.set_mean(global_best_mean2);
								tmp.push_back(tmpSeg); 
	
								if(tmpSeg.get_end() - tmpSeg.get_start() > 0)
									nonSingleGroups += tmpSeg.get_end() - tmpSeg.get_start()+1;
								
							} else {
								tmpSeg.set_start(prev_vect[s].get_start());
								tmpSeg.set_end(split_segment_maxSplit);
								tmpSeg.set_mean(global_best_mean1);
								tmp.push_back(tmpSeg); 

								if(tmpSeg.get_end() - tmpSeg.get_start() > 0)
									nonSingleGroups += tmpSeg.get_end() - tmpSeg.get_start()+1;

								tmpSeg.set_start(split_segment_maxSplit+1);
								tmpSeg.set_end(prev_vect[s].get_end());
								tmpSeg.set_mean(global_best_mean2);
								tmp.push_back(tmpSeg); 
	
								if(tmpSeg.get_end() - tmpSeg.get_start() > 0)
									nonSingleGroups += tmpSeg.get_end() - tmpSeg.get_start()+1;
								
							}

						} else {
							tmp.push_back(prev_vect[s]); 

							if(prev_vect[s].get_end() - prev_vect[s].get_start() > 0)
								nonSingleGroups += prev_vect[s].get_end() - prev_vect[s].get_start()+1;

							
							
						}
					
					

						


					}


						
					k_Best[i] = tmp;

					std::clock_t c_end_s = std::clock();
					end_mem_time();
					//ProfilerStop();
					END_TIMER(split_query_new);

					double wc_vect = (double) GET_TIMER(
							split_query_new);
					double cpu_vect = (c_end_s - c_start_s);
					double io_vect = get_io_timer();
					wc_time_s += wc_vect;
					cpu_time_s += cpu_vect;

					io_time_s += io_vect;

					//cout << "Wall Clock V:" << i << "--"<< wc_vect  << endl ;
				//cout << "CPU V:" << i << "--"<< cpu_vect  << endl ;
					//cout << "IO V:" << i << "--"<< io_vect  << endl ;

						
				vector<Segment>().swap(tmp);
					vector<Segment>().swap(prev_vect);

				wc_time_iter = (wc_time_i+wc_time_s);
				io_time_iter = (io_time_i+io_time_s);
				cpu_time_iter = (cpu_time_i+cpu_time_s);
				//cout << "Wall Clock:" << wc_time_i << "," << wc_time_s << "," << wc_time_iter << endl ;
				//cout << "CPU:" << cpu_time_i << "," << cpu_time_s << "," << cpu_time_iter << endl ;
				//cout << "IO:" << io_time_i << "," << io_time_s << "," << io_time_iter << endl ;

				wc_time += wc_time_iter;
				io_time += io_time_iter;
				cpu_time += cpu_time_iter;

				
				
			}

			
			swi.clear();
			sdb.clear();
			if(t > 0)
				results_file <<t<<"," << wc_time << "," << cpu_time <<","<< io_time << endl;
			
			
			
		}


		results_file.close();
			

		exit( EXIT_SUCCESS);
	}else if (pid > 0) {
			waitpid(pid, NULL, 0);
		}
	
	
	
}


void incVisageSamplingRate(int argc, char *argv[])
{
	// required variables
	const char usage[] =
			"USAGE: %s [-i swi] [-o swi] [-q queryfile] [-r rnd] [-f] [-b impl] [-n rows] [-d density] [-t distribution] [-p datasetcount] sdb schema\n";
	//bool import_csv = false;
	//const char* csv_fname = NULL;
	const char* query_fname = NULL;
	bool old_swi = false;
	bool new_swi = false;
	bool use_query = false;
	const char* swi_fname = "swi_file";
	const char* bmap_impl = NULL;
	uint64_t num_rows = 10000000;
	uint64_t dataset_count = DATASET_COUNT;
	const char* randomness = NULL;
	double density = 0.10;
        double clustering_rate=0.10;
	string distribution = "uniform";
	string indextype = "density";
	double conv_rate = 1.0;
	int trials =5;
	string dataset_name = "synthetic";
	
	char c;
	while ((c = getopt(argc, argv, "i:o:r:q:b:n:d:t:c:x:m:p")) != -1) {
		switch (c) {
		//case 'c':
		//    import_csv = true;
		//    csv_fname = optarg;
		//    break;
		case 'i':
			old_swi = true;
			swi_fname = optarg;
			break;
		case 'o':
			new_swi = true;
			swi_fname = optarg;
			break;
		case 'r':
			randomness = optarg;
			break;
		case 'q':
			use_query = true;
			query_fname = optarg;
			break;
		case 'b':
			bmap_impl = optarg;
			break;
		case 'n':
			num_rows = string2uint64_t(string(optarg));
			break;
		case 'd':
//			clustering_rate = string2double(string(optarg));
			density = string2double(string(optarg));
			break;
		case 't':
			distribution = (string(optarg));
			break;
       		case 'p':
			dataset_count = string2uint64_t(string(optarg));
			break;
		case 'x':
			conv_rate = string2double(string(optarg));
			break;
		case 'c':
                        clustering_rate = string2double(string(optarg));
                        break;
		case 'm':
                        dataset_name = (string(optarg));
                        break;
		}
	}

	if (old_swi && new_swi) {
		fprintf(stderr, usage, argv[0]);
		exit(2);
	}

	if ((optind + 2) != argc) {
		fprintf(stderr, usage, argv[0]);
		exit(2);
	}

	SwiftMapFactory::map_type_t bmap_type = SwiftMapFactory::POOL_LAZY_BITMAP;

        if (bmap_impl) {
		string bmap_impl_s(bmap_impl);
		indextype=bmap_impl_s;
		if (bmap_impl_s == "single") {
			bmap_type = SwiftMapFactory::SINGLE_BITMAP;
		} else if (bmap_impl_s == "lazy") {
			bmap_type = SwiftMapFactory::LAZY_BITMAP;
		} else if (bmap_impl_s == "lazy_s") {
			bmap_type = SwiftMapFactory::STATIC_LAZY_BITMAP;
		} else if (bmap_impl_s == "pool_lazy") {
			bmap_type = SwiftMapFactory::POOL_LAZY_BITMAP;
		} else if (bmap_impl_s == "pool_lazy_s") {
			bmap_type = SwiftMapFactory::POOL_STATIC_LAZY_BITMAP;
		} else if (bmap_impl_s == "density") {
			bmap_type = SwiftMapFactory::DENSITY_MAP;
		} else if (bmap_impl_s == "noindex") {
			bmap_type = SwiftMapFactory::NO_INDEX;
		} else if (bmap_impl_s == "ewah") {
      bmap_type = SwiftMapFactory::EWAH_BITMAP;
    } else if (bmap_impl_s == "lossy"){
      bmap_type = SwiftMapFactory::LOSSY_BITMAP;
  }
	}

	SwiftMap::sample_type_t sample_type = SwiftMap::SEQUENTIAL;


	vector<SwiftMap::sample_type_t> list_sample_types = { SwiftMap::TWOPRONG  ,SwiftMap::SEQUENTIAL, SwiftMap::PSEUDORANDOM};

	const char* sdb_fname = argv[optind];
	const char* schema_fname = argv[optind + 1];

cout << "creating dummy" << endl;
	
	Schema schema_dummy("/home/srahman7/needletail-code-base/data/schema_9", 100000000);

	SDB sdb_dummy("/home/srahman7/needletail-code-base/data/sdb_9", schema_dummy);
    sdb_dummy.clear();
    cout << "clear dummy" << endl;
    Binner binner_dummy;
    SwiftIndex swi_dummy(sdb_dummy, schema_dummy, binner_dummy, bmap_type,
                                              old_swi ? swi_fname : NULL);

    cout << "done dummy" << endl;

    
	pid_t pid = fork();
	if (pid == 0) {
		//load data

		Schema schema(schema_fname, 0);

		SDB sdb(sdb_fname, schema);

		sdb.clear();
		Binner binner;

                    
		SwiftIndex swi(sdb, schema, binner, bmap_type,
				old_swi ? swi_fname : NULL);
	
		if (new_swi) {
			swi.save(swi_fname);
		}

		cout << "swift index done i" << endl;
		
		std::string data_set = sdb_fname;
		std::string t11 ("sdb_2");
		std::string t13 ("sdb_4");
		int card = 0;
		//get dimension cardinailty and min-max---- set manually
		if(data_set.find(t11) != std::string::npos)
		{
			cout << "t11" << endl;
			card = 365;
		}
		else if(data_set.find(t13) != std::string::npos)
		{
			cout << "t13" << endl;
			card = 365;
		}
		else
		{
			card = 366;
		}


	//	int samples_samp[5] = {5000,10000,15000,25000};
	//	int iterations_cnt[4] = {10,25,50,100}; //1

		//for(int i_iter = 0; i_iter<4;i_iter++)
		{
		//	for(int i_samp=0;i_samp<4;i_samp++)
			{
				std::ostringstream temp;
				int opt_start = 25000;
				float opt_alpha = 1.02;
			    temp << "/home/srahman7/needletail-code-base/results/results_sampling_"<<opt_start<<"_"<< dataset_name << ".csv" << ends;

			    ofstream results_file;
				results_file.open(temp.str(),std::ofstream::out | std::ofstream::app);

				results_file <<"Trial"<<"," << "Itr" << ","<< "Wall Clock Time (ms)" << "," << "CPU Time (ms)" <<","<< "IO Time (ms)" << endl;

				for(int t = 1; t <= trials; t++ )
				{	
					double cpu_time = 0;
					double io_time = 0;
					double wc_time = 0;

					bool firstIteration = true;
					// initialize segment data structure
					vector<Segment> k_Best[card];
					double groupAvgs[card]; 
					int groupNums[card];

					 // 5000,10000,15000,25000 samples_samp[i_samp];
					

					int nonSingleGroups = card;
					double avg_map[366][366]={0};
					cout << "trial: " << t << endl;

					

					uint64_t row = 0;
		            while (row < 100000000){
		                    sdb_dummy.get_record(row);
		                    row += sdb_dummy.records_per_buf();
		            }

		            vector<Segment> tmp0;
					Segment tmpSeg10(0, 365, 10);
					tmp0.push_back(tmpSeg10);
					k_Best[0] = tmp0;

					sdb_dummy.clear();
					uint64_t record_id[card];
					for(int rec = 0; rec < card ; rec++)
					{
						record_id[rec] = 0;
						
					}
					
					//loop from min to max
					
					for(int i=0; i< card;i++)
					{
						//cout << "iteration: " << i << endl;
						double cpu_time_iter = 0;
						double io_time_iter = 0;
						double wc_time_iter = 0;

						double cpu_time_s = 0;
						double io_time_s = 0;
						double wc_time_s = 0;

						double cpu_time_i = 0;
						double io_time_i = 0;
						double wc_time_i = 0;

						float iter_samples = floor((opt_start*1.0)/pow(opt_alpha,i));
						//cout << "at first iteration: " << i << endl;
						double segment_avg = 0;
						vector<Segment> prev_vect;
							//---- load the segment structure from previous iteation
						if(firstIteration)
							prev_vect = k_Best[0];
						else
							prev_vect = k_Best[i-1];

							double global_seg_mean = 0;
							int split_segment_no = -1;
							int split_segment_maxSplit = -1;
							double global_max = -1000000000;
							double global_best_mean1 = 0, global_best_mean2 = 0;

							for(int s=0;s<prev_vect.size();s++)
							{
								
								int start = prev_vect[s].get_start();
								int end = prev_vect[s].get_end();

								if(end - start < 1)
									continue;
								//---- ask for N samples from a non-single segment group for some target/measure and get the avg 
								

								for(int g = start,am=0;g <= end;g++,am++)
								{
									conv_rate = 1.0;
									
									int conv_samples = floor(iter_samples/nonSingleGroups);

									temp.str("");
									temp.clear();
									temp << string(query_fname) << (g+1);
									char* q = new char[temp.str().length() + 1];
									std::strcpy(q, temp.str().c_str());

									Query query(q, schema);
									query.set_num_samples(conv_samples);
									
									query.set_conv_rate(conv_rate);

									ResultSet results(query.get_targets().size());

									temp.str("");
									temp.clear();
									temp << "swi_query" << i << s << g <<t;
									char* swi_query_new = new char[temp.str().length()
											+ 1];
									std::strcpy(swi_query_new, temp.str().c_str());
									std::clock_t c_start_i = std::clock();
										
									clear_mem_time();
									clear_io_timer();
									INIT_TIMER(swi_query_new);
									START_TIMER(swi_query_new);
									start_mem_time();
									//ProfilerStart("cpu.out");
									//std::clock_t c_start = std::clock();
									//swi.run_query(query, results, sample_type);
									record_id[g] = swi.run_query(query, results, sample_type,record_id[g],firstIteration);
									std::vector<double> avgs = results.get_avgs();
									groupAvgs[g-1] = (groupAvgs[g-1]*groupNums[g-1] + avgs[0]*conv_samples)/(groupNums[g-1]+conv_samples);
									groupNums[g-1] += conv_samples;

									if(firstIteration)
										segment_avg += avgs[0];
									else
									{
										avg_map[am][am] = groupAvgs[g-1];
										for(int am_it = 0; am_it <am; am_it++)
										{
											avg_map[am_it][am] = avg_map[am_it][am-1]+groupAvgs[g-1];
										}
									}
										
									std::clock_t c_end_i = std::clock();
									end_mem_time();
									//ProfilerStop();
									END_TIMER(swi_query_new);
									
									double wc_vect = (double) GET_TIMER(swi_query_new);
									double cpu_vect = (c_end_i - c_start_i);
									double io_vect = get_io_timer();

									
										wc_time_i += wc_vect;
										cpu_time_i += cpu_vect;

										io_time_i += io_vect;

										//cout << "Wall Clock S:" << g << "--" << wc_vect  << endl ;
									//	cout << "CPU S:" << g << "--" << cpu_vect  << endl ;
									//	cout << "IO S:" << g << "--" << io_vect  << endl ;

									
									
								}
							
								double alphaT = 0;
								double alphaU = 0;
								double seg_mean = 0;
								double best_mean1 = 0;
								double best_mean2 = 0;
								double tmp1, tmp2;
								double max = -1000000000;
								int maxSplit = -1;

								temp.str("");
								temp.clear();
								temp << "split" << i << s << t;
								char* split_query_new = new char[temp.str().length()
										+ 1];

								std::strcpy(split_query_new, temp.str().c_str());
								
								std::clock_t c_start_s = std::clock();
											
								clear_mem_time();
								clear_io_timer();
								INIT_TIMER(split_query_new);
								START_TIMER(split_query_new);
								start_mem_time();
								if(firstIteration)
								{

									segment_avg /= card;

									vector<Segment> tmp;
									Segment tmpSeg(1,card,segment_avg); 
									tmp.push_back(tmpSeg); 
									k_Best[i] = tmp;

									firstIteration = false;
								}
								else
								{
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

								std::clock_t c_end_s = std::clock();
								end_mem_time();
								//ProfilerStop();
								END_TIMER(split_query_new);
								

								double wc_vect = (double) GET_TIMER(
									split_query_new);
								double cpu_vect = (c_end_s - c_start_s);
								double io_vect = get_io_timer();
								wc_time_s += wc_vect;
								cpu_time_s += cpu_vect;

								io_time_s += io_vect;

								//cout << "Wall Clock DP:" << s << "--"<< wc_vect  << endl ;
								//cout << "CPU DP:" << s << "--"<< cpu_vect  << endl ;
								//cout << "IO DP:" << s << "--"<< io_vect  << endl ;

							}	

							temp.str("");
							temp.clear();
							temp << "split" << i  << t;
							char* split_query_new = new char[temp.str().length()
									+ 1];

							std::strcpy(split_query_new, temp.str().c_str());

							std::clock_t c_start_s = std::clock();
											
							clear_mem_time();
							clear_io_timer();
							INIT_TIMER(split_query_new);
							START_TIMER(split_query_new);
							start_mem_time();
								
								//---- create new segment structure and push
							vector<Segment> tmp;
							Segment tmpSeg(0, 0, 0);

							nonSingleGroups = 0;
							for (int s = 0; s < prev_vect.size(); s++) {
								if (s == split_segment_no) {
									if (split_segment_maxSplit == prev_vect[s].get_end()) {

										tmpSeg.set_start(prev_vect[s].get_start());
										tmpSeg.set_end(split_segment_maxSplit - 1);
										tmpSeg.set_mean(global_best_mean1);
										tmp.push_back(tmpSeg); 

										if(tmpSeg.get_end() - tmpSeg.get_start() > 0)
											nonSingleGroups += tmpSeg.get_end() - tmpSeg.get_start()+1;

										tmpSeg.set_start(split_segment_maxSplit);
										tmpSeg.set_end(prev_vect[s].get_end());
										tmpSeg.set_mean(global_best_mean2);
										tmp.push_back(tmpSeg); 
			
										if(tmpSeg.get_end() - tmpSeg.get_start() > 0)
											nonSingleGroups += tmpSeg.get_end() - tmpSeg.get_start()+1;
										
									} else {
										tmpSeg.set_start(prev_vect[s].get_start());
										tmpSeg.set_end(split_segment_maxSplit);
										tmpSeg.set_mean(global_best_mean1);
										tmp.push_back(tmpSeg); 

										if(tmpSeg.get_end() - tmpSeg.get_start() > 0)
											nonSingleGroups += tmpSeg.get_end() - tmpSeg.get_start()+1;

										tmpSeg.set_start(split_segment_maxSplit+1);
										tmpSeg.set_end(prev_vect[s].get_end());
										tmpSeg.set_mean(global_best_mean2);
										tmp.push_back(tmpSeg); 
			
										if(tmpSeg.get_end() - tmpSeg.get_start() > 0)
											nonSingleGroups += tmpSeg.get_end() - tmpSeg.get_start()+1;
										
									}

								} else {
									tmp.push_back(prev_vect[s]); 

									if(prev_vect[s].get_end() - prev_vect[s].get_start() > 0)
										nonSingleGroups += prev_vect[s].get_end() - prev_vect[s].get_start()+1;

									
									
								}
							
							

								


							}


								
							k_Best[i] = tmp;

							std::clock_t c_end_s = std::clock();
							end_mem_time();
							//ProfilerStop();
							END_TIMER(split_query_new);

							double wc_vect = (double) GET_TIMER(
									split_query_new);
							double cpu_vect = (c_end_s - c_start_s);
							double io_vect = get_io_timer();
							wc_time_s += wc_vect;
							cpu_time_s += cpu_vect;

							io_time_s += io_vect;

							//cout << "Wall Clock V:" << i << "--"<< wc_vect  << endl ;
						//cout << "CPU V:" << i << "--"<< cpu_vect  << endl ;
							//cout << "IO V:" << i << "--"<< io_vect  << endl ;

								
						vector<Segment>().swap(tmp);
							vector<Segment>().swap(prev_vect);

						wc_time_iter = (wc_time_i+wc_time_s);
						io_time_iter = (io_time_i+io_time_s);
						cpu_time_iter = (cpu_time_i+cpu_time_s);
						//cout << "Wall Clock:" << wc_time_i << "," << wc_time_s << "," << wc_time_iter << endl ;
						//cout << "CPU:" << cpu_time_i << "," << cpu_time_s << "," << cpu_time_iter << endl ;
						//cout << "IO:" << io_time_i << "," << io_time_s << "," << io_time_iter << endl ;

						wc_time += wc_time_iter;
						io_time += io_time_iter;
						cpu_time += cpu_time_iter;

						if(i == 5 || i == 9 || i == 24 || i==49 || i==99)
						{
							results_file <<t<<"," << (i+1) << "," << wc_time << "," << cpu_time <<","<< io_time << endl;
						}

						
						
					}

					
					swi.clear();
					sdb.clear();
					if(t > 0)
						results_file <<t<<"," << (card+1) << "," << wc_time << "," << cpu_time <<","<< io_time << endl;
					
					
					
					
				}


				results_file.close();
			}

		}

		
			

		exit( EXIT_SUCCESS);
	}else if (pid > 0) {
			waitpid(pid, NULL, 0);
		}
	
	
	
}


int main(int argc, char *argv[])
{
	//cout << "incvisage" << endl;
	//incVisage(argc,argv);

	//cout << "incvisage samples" << endl;
	//incVisageSamplingRate(argc,argv);

	//cout << "dp" << endl;
	//dpSamplingRate(argc,argv);

	//cout << "range scan test" << endl;
	//rangeSeqScan(argc,argv);
	


	cout << "file scan test" << endl;
	std::ostringstream temp;
	int dset[3] = {1,3,5};
	int len = (sizeof(dset)/sizeof(*dset));
	for(int i = 0; i < len; i++ )
	{
		temp.str("");
		temp.clear();
		temp << "/home/srahman7/needletail-code-base/data/sdb_" << dset[i];
		char* dataset = new char[temp.str().length() + 1];
		std::strcpy(dataset, temp.str().c_str());

		temp.str("");
		temp.clear();
		temp << "/home/srahman7/needletail-code-base/data/schema_" << dset[i];
		char* schema = new char[temp.str().length() + 1];
		std::strcpy(schema, temp.str().c_str());

		temp.str("");
		temp.clear();
		temp << "/home/srahman7/needletail-code-base/scripts/q";
		char* query = new char[temp.str().length() + 1];
		std::strcpy(query, temp.str().c_str());
		fileScan(dataset,schema,query);	
	}
	
	//testSequential_read(sdb_fname,schema_fname);
	//testGroupRead(argc,argv);
	
	return 0;
}
