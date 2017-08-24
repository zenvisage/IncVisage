#include <cassert>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <utility>
#include <exception>
#include "config.h"
#include "swift_map.h"
#include "utils.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#if USE_PAIRING_HEAP
#include <boost/heap/pairing_heap.hpp>
#endif

using namespace std;

const size_t SwiftMap::npos = static_cast<size_t>(-1);

SingleBitmap::SingleBitmap(uint64_t num_records, bool set)
    : SwiftMap(num_records)
{
    b = new bitmap(num_records);
    if (set) {
        b->set();
    }
}

SingleBitmap::~SingleBitmap()
{
    delete b;
    samples.clear();
    order=0;
}

size_t SingleBitmap::sequential_sample()
{
    if (last_sampled != npos) {
        last_sampled = b->find_next(last_sampled);
    }else{
	    last_sampled = b->find_first();
    }
    return last_sampled;
}

size_t SingleBitmap::random_sample()
{
 if(samples.size() < total_samples_wanted){
  vector<uint64_t>  allsamples;
  uint64_t lastsmpl= b->find_first();
  while (lastsmpl !=  npos){
     allsamples.push_back(lastsmpl);
     lastsmpl=b->find_next(lastsmpl);
   }
 std::random_shuffle (allsamples.begin(), allsamples.end() );
  // print out content:
  for (std::vector<uint64_t>::iterator it=allsamples.begin(); samples.size() < total_samples_wanted; ++it)
      samples.push_back(*it);

//  std::sort(samples.begin(), samples.end(), id_compare);
 }
 return samples[order++];
}







/*
size_t SingleBitmap::find_first()
{
    return b->find_first();
}

size_t SingleBitmap::find_next(size_t pos)
{
    return b->find_next(pos);
}
*/

void SingleBitmap::set(uint64_t row)
{
    (*b)[row] = true;
}

void SingleBitmap::and_combine(SwiftMap* m)
{
    *b &= *static_cast<SingleBitmap*>(m)->b;
}

void SingleBitmap::or_combine(SwiftMap* m)
{
    *b |= *static_cast<SingleBitmap*>(m)->b;
}

void SingleBitmap::read(istream& in)
{
    uint64_t num_blocks;
    in >> num_blocks;
    in.get();
    vector<block_type_t> v(num_blocks);
    in.read((char*) &v[0], num_blocks * sizeof(block_type_t));
    boost::from_block_range(v.begin(), v.end(), *b);
}

void SingleBitmap::write(std::ostream& out)
{
    out << b->num_blocks() << ' ';
    vector<block_type_t> v;
    boost::to_block_range(*b, back_inserter(v));
    out.write((const char*) &v[0], b->num_blocks() * sizeof(block_type_t));
}

bool SingleBitmap::get(size_t pos)
{
    return (*b)[pos];
}

string SingleBitmap::to_string()
{
    string s;
    boost::to_string(*b, s);
    reverse(s.begin(), s.end());
    return s;
}

template<>
SwiftMap::bitmap* LazyBitmap<SwiftMap::bitmap>::new_bitmap(bool set)
{
    bitmap* b = new bitmap(SPLIT_SIZE);
    if (set) {
        b->set();
    }
    return b;
}

template<>
SwiftMap::sbitmap* LazyBitmap<SwiftMap::sbitmap>::new_bitmap(bool set)
{
    sbitmap* b = new sbitmap();
    if (set) {
        b->set();
    }
    return b;
}

template<>
std::string LazyBitmap<SwiftMap::bitmap>::to_string()
{
    std::string s;
    for (size_t i = 0; i < v.size(); i++) {
        std::string ss;
        boost::to_string(*eval_bitmap(i), ss);
        reverse(ss.begin(), ss.end());
        s += ss;
    }
    return s;
}

template<>
std::string LazyBitmap<SwiftMap::sbitmap>::to_string()
{
    std::string s;
    for (size_t i = 0; i < v.size(); i++) {
        s += v[i]->to_string();
    }
    return s;
}

template<>
void PoolLazyBitmap<SwiftMap::bitmap>::fill_pool()
{
    cout << "in fill pull" <<endl;
    for (uint64_t i = 0; i < POOL_SIZE; i++) {
        cout << "in fill pull---" << pool.size() <<endl;
        pool.push_back(new bitmap(SPLIT_SIZE));
    }
}

template<>
void PoolLazyBitmap<SwiftMap::sbitmap>::fill_pool()
{
    for (uint64_t i = 0; i < POOL_SIZE; i++) {
        pool.push_back(new sbitmap());
    }
}


NoIndex::NoIndex(uint64_t num_records, uint64_t records_per_buf, bool set)
    : SwiftMap(num_records), records_per_buf(records_per_buf),
     num_splits(UPDIV(num_records, records_per_buf)),last_sampled(0)
{ }

NoIndex::~NoIndex()
{ }

size_t NoIndex::sequential_sample()
{
	last_sampled = (last_sampled + 1) % num_splits;
     return last_sampled * records_per_buf;

}

size_t NoIndex::pseudorandom_sample()
{
	 UNIMPLEMENTED("FIXME");
}

void NoIndex::and_combine(SwiftMap* m)
{
    and_maps.push_back(static_cast<NoIndex*>(m));
}

void NoIndex::or_combine(SwiftMap* m)
{
    or_maps.push_back(static_cast<NoIndex*>(m));
}

void NoIndex::read(std::istream& in)
{
	 UNIMPLEMENTED("NoIndex::read");
}
void NoIndex::write(std::ostream& out)
{
	 UNIMPLEMENTED("NoIndex::write");
}








bool NoIndex::get(size_t pos)
{
    UNIMPLEMENTED("NoIndex::get");
}

std::string NoIndex::to_string()
{
	UNIMPLEMENTED("NoIndex::get");
}

size_t NoIndex::random_sample()
{
    UNIMPLEMENTED("NoIndex::random_sample");
}

void NoIndex::set(uint64_t row)
{
	// UNIMPLEMENTED("NoIndex::set);
}

void NoIndex::post_set()
{
 //   UNIMPLEMENTED("NoIndex::postset");
}

EWAHBitmap::EWAHBitmap(uint64_t num_records, bool set)
    : SwiftMap(num_records), order(0)
{
    b = new ewahmap();
    if (set) {
        b->padWithZeroes(num_records);
        b->inplace_logicalnot();
    }
    isSamplesEmpty=false;
}

EWAHBitmap::~EWAHBitmap()
{
   // cout << "ewah destructor" << endl;
    delete b;
    std::vector<size_t>().swap(samples);
}

void EWAHBitmap::set(uint64_t row)
{
    b->set(row);
}

void EWAHBitmap::write(std::ostream& out)
{
    b->write(out);
}

void EWAHBitmap::and_combine(SwiftMap* m)
{
    and_maps.push_back(static_cast<EWAHBitmap*>(m));
}

void EWAHBitmap::or_combine(SwiftMap* m)
{
    or_maps.push_back(static_cast<EWAHBitmap*>(m));
}

size_t EWAHBitmap::random_sample()
{
    if (sampled_so_far == 0) {
        samples.clear();
        if (and_maps.size() > 0 && or_maps.size() > 0) {
            panic("EWAHBitmap should not have both non-zero and_maps and or_maps");
        }

        if (and_maps.size() > 0) {
            for (size_t i = 0; i < and_maps.size(); i++) {
                *b = b->logicaland(*and_maps[i]->b);
            }
        } else if (or_maps.size() > 0) {
            const ewahmap* inputs[or_maps.size()+1];
            for (size_t i = 0; i < or_maps.size(); i++) {
                inputs[i] = or_maps[i]->b;
            }
            inputs[or_maps.size()] = b;
            *b = fast_logicalor(or_maps.size(), inputs);
        }

        samples = b->toArray();
        std::random_shuffle(samples.begin(), samples.end());

    }

    size_t idx = samples[sampled_so_far++];
    return idx;
}


size_t EWAHBitmap::sequential_sample()
{

    if (sampled_so_far == 0) {
        samples.clear();
        if (and_maps.size() > 0 && or_maps.size() > 0) {
            panic("EWAHBitmap should not have both non-zero and_maps and or_maps");
        }

        if (and_maps.size() > 0) {
         //   cout << "and" << endl;
            for (size_t i = 0; i < and_maps.size(); i++) {
                *b = b->logicaland(*and_maps[i]->b);
            }
        } else if (or_maps.size() > 0) {
        //    cout << "or" << endl;
            const ewahmap* inputs[or_maps.size()+1];
            for (size_t i = 0; i < or_maps.size(); i++) {
                inputs[i] = or_maps[i]->b;
            }
            inputs[or_maps.size()] = b;
            *b = fast_logicalor(or_maps.size(), inputs);
        }

        samples = b->toArray();
    //  cout << "samples array assinged" << endl;

    }

    if(samples.size() == 0)
    {   
        isSamplesEmpty =  true;
       // cout << "samples size 0" << endl;
        return 0;
    }
    isSamplesEmpty =  false;
    size_t idx = samples[sampled_so_far++];
    return idx;


    
}


LossyBitmap::LossyBitmap(uint64_t num_records, uint64_t records_per_buf, bool set)
    : SwiftMap(num_records), records_per_buf(records_per_buf)
{
    uint64_t num_buf = num_records/records_per_buf + 1;
    b = new bitmap(num_buf);
    if (set) {
        b->set();
    }
}

LossyBitmap::~LossyBitmap()
{
    delete b;
}

void LossyBitmap::set(uint64_t row)
{
    uint64_t buf_idx = row / records_per_buf;
    (*b)[buf_idx] = true;
}

void LossyBitmap::write(std::ostream& out)
{
    out << b->num_blocks() << ' ';
    vector<block_type_t> v;
    boost::to_block_range(*b, back_inserter(v));
    out.write((const char*) &v[0], b->num_blocks() * sizeof(block_type_t));
}



void LossyBitmap::and_combine(SwiftMap* m)
{
    *b &= *static_cast<LossyBitmap*>(m)->b;
}

void LossyBitmap::or_combine(SwiftMap* m)
{
    *b |= *static_cast<LossyBitmap*>(m)->b;
}

size_t LossyBitmap::sequential_sample()
{
    // Here, last_sampled is the last sampled block

    if (last_sampled != npos) {
        last_sampled = b->find_next(last_sampled);
    }else{
            last_sampled = b->find_first();
    }

/**
   if (sampled_so_far == 0){
	last_sampled = b->find_first();	
   }else{
	last_sampled = b->find_next(last_sampled);

   }
**/
 
    return last_sampled * records_per_buf;
}

DensityMap::DensityMap(uint64_t num_records, uint64_t records_per_buf, bool set)
    : SwiftMap(num_records), records_per_buf(records_per_buf),
    num_splits(UPDIV(num_records, (records_per_buf*GRANULARITY))), ordered_done(false),
    v(num_splits), nonzero(num_splits*GRANULARITY), valid(num_splits), set_default(set),
    last_sampled(0)
{ 
    isSamplesEmpty=false;
}

DensityMap::~DensityMap()
{
    ordered.clear();
    temp_ordered.clear();
    old_seen.clear();
    and_maps.clear();
    or_maps.clear();
    v.clear();
    sampled.clear();
    ordered_done=false;
    isSamplesEmpty=false;

/**
    vector<Density>().swap(ordered);
    vector<Density>().swap(temp_ordered);
    unordered_set<uint64_t>().swap(old_seen);
    vector<DensityMap*>().swap(and_maps);
    vector<DensityMap*>().swap(or_maps);
    vector<Density>().swap(v);
    unordered_set<uint64_t>().swap(sampled);
    ordered_done = false;
**/
}

#if 0
size_t DensityMap::sequential_sample()
{


    Density d;
    if (!ordered_done) {

	d = linear_pass_fixed_blocks(last_sampled/granularity);
	last_sampled = (last_sampled + 1);
        if (d.idx != npos) {
        	if( nonzero[d.idx*granularity+((last_sampled-1)%granularity)]){
        	return (d.idx*granularity+(last_sampled-1)%granularity)* records_per_buf;
        	}
        	else
        	 return sequential_sample();

        }
    }

    d = linear_pass_fixed_blocks(last_sampled/granularity);

    last_sampled = (last_sampled + 1) % (granularity*num_splits);
    if (d.idx != npos) {
    	if( nonzero[d.idx*granularity+((last_sampled-1)%granularity)]){
    		return (d.idx*granularity+(last_sampled-1)%granularity)* records_per_buf;
	}
        else
        	return sequential_sample();
    } else {
        return npos;
   }
}
#endif

size_t DensityMap::sequential_sample(){
    Density d;
    if (!ordered_done) {
        d = linear_pass_fixed_blocks(last_sampled/granularity);
        last_sampled = (last_sampled + 1);
        if (d.idx != npos) {
                return (d.idx*granularity + (last_sampled-1)%granularity)* records_per_buf;
             }
     }

    d = linear_pass_fixed_blocks(last_sampled/granularity);
    last_sampled = (last_sampled + 1) % (granularity*num_splits);
    if (d.idx != npos) {
                     return (d.idx*granularity+(last_sampled-1)%granularity)* records_per_buf;

        }
     else {
        return npos;
    }

}


size_t DensityMap::two_prong_sample(){
    Density d;
    if (!ordered_done) {
        d = two_prong(last_sampled/granularity);
        last_sampled = (last_sampled + 1);
        if (d.idx != npos) {
                return (d.idx*granularity + (last_sampled-1)%granularity)* records_per_buf;
             }
     }

    d = two_prong(last_sampled/granularity);
    last_sampled = (last_sampled + 1) % (granularity*num_splits);
    if (d.idx != npos) {
                     return (d.idx*granularity+(last_sampled-1)%granularity)* records_per_buf;

        }
     else {
        return npos;
    }

}
#if 0
size_t DensityMap::two_prong_sample()
{

    Density d;
    if (!ordered_done) {

        d = two_prong(last_sampled/granularity);
        last_sampled = (last_sampled + 1);
        if (d.idx != npos) {
                if( nonzero[d.idx*granularity+((last_sampled-1)%granularity)]){
                return (d.idx*granularity+(last_sampled-1)%granularity)* records_per_buf;
                }
                else
                 return two_prong_sample();

        }
    }

    d = two_prong(last_sampled/granularity);

    last_sampled = (last_sampled + 1) % (granularity*num_splits);
    if (d.idx != npos) {
        if( nonzero[d.idx*granularity+((last_sampled-1)%granularity)]){
                return (d.idx*granularity+(last_sampled-1)%granularity)* records_per_buf;
        }
        else
                return two_prong_sample();
    } else {
        return npos;
   }
}
#endif

#if 0
// For Forward Optimal
size_t DensityMap::sequential_sample()
{
    Density d;
    if (!ordered_done) {
        d = vanilla_forward_optimal(last_sampled/granularity);
        last_sampled = (last_sampled + 1);
        if (d.idx != npos) {
        	return (d.idx*granularity + (last_sampled-1)%granularity)* records_per_buf;
             }
     }

    d = vanilla_forward_optimal(last_sampled/granularity);
    last_sampled = (last_sampled + 1) % (granularity*num_splits);
    if (d.idx != npos) {
    		     return (d.idx*granularity+(last_sampled-1)%granularity)* records_per_buf;

    	}
     else {
        return npos;
    }
}
#endif

size_t DensityMap::pseudorandom_sample()
{

    Density d;
    if (!ordered_done) {
        d = ordered_get(last_sampled/granularity);
        last_sampled = (last_sampled + 1);
        if (d.idx != npos) {
            isSamplesEmpty =  false;
              return (d.idx*granularity + (last_sampled-1)%granularity)* records_per_buf;
             }
           /*  else // sunny's addition for non-existent bitmap
             {
                isSamplesEmpty =  true;
                return npos;
             }*/
     }

    d = ordered_get(last_sampled/granularity);

    last_sampled = (last_sampled + 1) % (granularity*num_splits);
    if (d.idx != npos) {
                    isSamplesEmpty =  false;
                     return (d.idx*granularity+(last_sampled-1)%granularity)* records_per_buf;

        }
     else {
        isSamplesEmpty =  true;
        return npos;
    }
}






//bool DensityMap::next_top(Density* d, unordered_set<uint64_t>& skipset)
//{
//    if (and_maps.empty() && or_maps.empty()) {
//        // We are a base map (not combining)
//        for (size_t i = 0; i < ordered.size(); i++) {
//            if (skipset.find(ordered[i].idx) == skipset.end()) {
//                d->idx = ordered[i].idx;
//                d->freq = ordered[i].freq;
//                return true;
//            }
//        }
//        return false
//    } else {
//        for (size_t i = 0; i < and_maps.size(); i++) {
//            for (size_t i
//        }
//    }
//}

// TODO - Might not make sense to throw away all the past calculated densities
// for 1024 * 512 dataset
#if 0
uint64_t DensityMap::calc_cost(uint64_t start, uint64_t end){
        float x = log2(granularity * (end - start));

        if ( x < 10)
                return -48.5* pow(x,2) + 1195*x+ 3000;

        return 10661;
}
#endif

std::vector<uint64_t> DensityMap::find_valid_blocks(){
	std::vector<uint64_t> valid_blocks;
    uint64_t est_valid_records = 0;

        std::vector<DensityMap*> temp_maps=and_maps;
        if(!or_maps.empty())
                temp_maps=or_maps;

        if (!temp_maps.empty()){
                for (uint64_t i = 0; i<temp_maps[0]->num_splits; i++) {
	                Density aggr_j;
	                if(!and_maps.empty())
        	                aggr_j = Density{i, 1};
                	else
                        	aggr_j = Density{i, 0};
			        for (uint64_t j = 0; j < temp_maps.size(); j++){
				            Density d = temp_maps[j]->index_get(i);

                                if(!and_maps.empty())
                                        aggr_j.freq *= d.freq;
                                else
                                        aggr_j.freq += d.freq;
                    }


                    if (!or_maps.empty()){
                        aggr_j.freq = std::min(aggr_j.freq, 1.0);
			        }

			         if( aggr_j.freq > 0){
				        valid_blocks.push_back(aggr_j.idx);
                        est_valid_records += aggr_j.freq * (records_per_buf * granularity);
			         }
		      }
	   }
    set_est_valid_records(est_valid_records);

	return valid_blocks;
}

uint64_t DensityMap::calc_cost(uint64_t start, uint64_t end){
//        float x = log2(granularity * (end - start));
	float x = granularity * (end - start);
	if (x < 512)
		return -0.016 * pow(x, 2) + 16.692 * x +157.512;
	return 4098.2;

}


DensityMap::Density DensityMap::vanilla_forward_optimal(uint64_t order){

	if (order < ordered.size()){
		return ordered[order];
	}else if (ordered_done){
		return Density{npos, 0};
	}

	std::vector<Density> new_densities;

        uint64_t samples_wanted = delta * (total_samples_wanted - sampled_so_far);




	uint64_t fixed_rows = samples_wanted;

	uint64_t sample_granularity;

        if (samples_wanted < fixed_rows){
        	sample_granularity = 1;
                fixed_rows = samples_wanted;
        }else{

		sample_granularity = samples_wanted / fixed_rows;
	}

	uint64_t cost_threshold  = 512;

// ATTENTION



	std::vector<DensityMap*> temp_maps = and_maps;
        uint64_t num_blocks = temp_maps[0] -> num_splits;

	const uint64_t MAX_COST = npos;
	const uint64_t BLOCK_FETCH = 364 * granularity;
	struct Parent {
		pair<uint64_t, uint64_t> parent_index;
		bool isFromBest; //which parent the curr records derived from
	};

	vector<vector<uint64_t>> cost_record;
	vector<vector<uint64_t>> best_cost;
	vector<vector<Parent>> record_parent;
	vector<vector<Parent>> best_parent;
	uint64_t curr_col = 0;

//	if (not ordered_done){
	if (!temp_maps.empty()) {
		temp_ordered.clear();
		for (uint64_t i = 0; i< temp_maps[0] -> num_splits; i++){
			// if this block i has been sampled, then skip this
			if (sampled.find(i) != sampled.end())
				continue;


			// i is the block number. so block index in temp_ordered is consistent
			Density aggr_j= Density{i, 1};
			for (uint64_t j = 0; j < temp_maps.size(); j++) {
                                Density d = temp_maps[j]->index_get(i);
                                aggr_j.freq *= d.freq;
               		}
			if(granularity>1){
                                for(uint64_t b=0;b<granularity;b++){
                                        uint64_t row_id=i+b;;
                                        for (uint64_t j = 0; j < temp_maps.size(); j++){
                                                if(temp_maps[j]->nonzero[row_id]){
                                                        nonzero[row_id]=true;
                                                        break;
                                                }
                                        }
                                }
                        }else{
                                nonzero[i]=true;
                        }
		       	temp_ordered.push_back(aggr_j);

			uint64_t curr_samples = aggr_j.freq * (records_per_buf * granularity);
//ATTENTION:
			if (curr_samples == 0)
				curr_samples ++;

			uint64_t curr_row = curr_samples / sample_granularity;

/*			if (curr_row % sample_granularity > sample_granularity * 0.6)
				curr_row += 1;
*/
			if (curr_row > fixed_rows)
				curr_row = fixed_rows;

			uint64_t min_cost = MAX_COST;
        	        uint64_t min_row = npos;
			uint64_t min_col = npos;
			vector<uint64_t> cost_record_col;
			vector<uint64_t> best_cost_col;
			vector<Parent> record_parent_col;
			vector<Parent> best_parent_col;

			if (curr_col == 0){ //BASE CASE
				for (uint64_t j = 0; j <= curr_row; j++){
					cost_record_col.push_back(BLOCK_FETCH);
					best_cost_col.push_back(BLOCK_FETCH);
					record_parent_col.push_back(Parent{make_pair(npos,npos), false});
					best_parent_col.push_back(Parent{make_pair(npos, npos), false});
				}
				for (uint64_t j = curr_row + 1; j <= fixed_rows; j++){
					cost_record_col.push_back(MAX_COST);
					best_cost_col.push_back(MAX_COST);
					record_parent_col.push_back(Parent{make_pair(npos,npos), false});
                                        best_parent_col.push_back(Parent{make_pair(npos, npos), false});

				}

				cost_record.push_back(cost_record_col);
				best_cost.push_back(best_cost_col);
				record_parent.push_back(record_parent_col);
				best_parent.push_back(best_parent_col);
				curr_col ++;
				continue;
			}

			//update each row of column i

			uint64_t start_col;
			if (curr_col < cost_threshold)
				start_col = 0;
			else
				start_col = curr_col - cost_threshold;

			uint64_t start_row;

			for(uint64_t j = 0; j <= curr_row; j++){
//				uint64_t temp_cost = calc_cost(0,i) + BLOCK_FETCH;
				uint64_t temp_cost = calc_cost(0,curr_col);
				cost_record_col.push_back(temp_cost);

				record_parent_col.push_back(Parent{make_pair(npos, npos), false});

				if (best_cost[curr_col-1][j] < temp_cost){
					best_cost_col.push_back(best_cost[curr_col-1][j]);
					best_parent_col.push_back(Parent{make_pair(curr_col-1, j), true});

				}else{
					best_cost_col.push_back(temp_cost);
					best_parent_col.push_back(Parent{make_pair(npos, npos), false});

				}
			}

			for(uint64_t j = curr_row + 1 ; j <= fixed_rows; j++){
				uint64_t lookup_idx = j - curr_row;

                                min_cost = MAX_COST;
                                min_col = npos;
                                min_row = npos;

				bool isFromBest = false;
				for (uint64_t n = start_col; n < curr_col; n ++){
					if (cost_record[n][lookup_idx] == MAX_COST) continue;
//					uint64_t curr_cost = cost_record[n][lookup_idx] + calc_cost(n,i) + BLOCK_FETCH;
					uint64_t curr_cost = cost_record[n][lookup_idx] + calc_cost(n,curr_col);

					if (curr_cost < min_cost){
						min_cost = curr_cost;
						min_row = lookup_idx;
						min_col = n;
						isFromBest = false;
					}
				}

/**
				for (uint64_t n = 0; n < other_threshold.size(); n++){
					if (curr_col >= other_threshold[n]){
						uint64_t lookup_col = curr_col - other_threshold[n];
		                                if (curr_cost < min_cost){
                                                	min_cost = curr_cost;
	                                                min_row = lookup_idx;
        	                                        min_col = start_col - 1;
                	                                isFromBest = true;
                        	                }


					}
					else break;
				}

**/


				if (start_col >= 1 && best_cost[start_col-1][lookup_idx] != MAX_COST){
					uint64_t curr_cost = best_cost[start_col-1][lookup_idx] + calc_cost(start_col-1, curr_col);
					if (curr_cost < min_cost){
                                                min_cost = curr_cost;
                                                min_row = lookup_idx;
                                                min_col = start_col - 1;
						isFromBest = true;
                                        }
				}

				cost_record_col.push_back(min_cost);
				record_parent_col.push_back(Parent{make_pair(min_col, min_row), isFromBest});

				if (best_cost[curr_col-1][j] < min_cost){
					best_cost_col.push_back(best_cost[curr_col-1][j]);
					best_parent_col.push_back(Parent{make_pair(curr_col-1, j), true});
				}else{
					best_cost_col.push_back(min_cost);
					best_parent_col.push_back(Parent{make_pair(min_col, min_row), isFromBest});
				}

			}

			cost_record.push_back(cost_record_col);
                        best_cost.push_back(best_cost_col);

			record_parent.push_back(record_parent_col);
			best_parent.push_back(best_parent_col);
			curr_col ++;
		}// end of iteration on blocks


		uint64_t min_cost = MAX_COST;

		uint64_t lookup_row = fixed_rows;
		if(curr_col < 1){
			ordered_done = true;
			return Density{npos, 0};

		}
		pair<uint64_t, uint64_t> lookup_idx = make_pair(curr_col - 1, lookup_row);


		Parent cell_parent;
		bool isFromBest = best_parent[lookup_idx.first][lookup_idx.second].isFromBest;

		while (lookup_idx != make_pair(npos, npos)){
	                if (isFromBest)
	                        cell_parent = best_parent[lookup_idx.first][lookup_idx.second];
        	        else
                	        cell_parent = record_parent[lookup_idx.first][lookup_idx.second];

			if (cell_parent.isFromBest)
				isFromBest = true;
			else
				isFromBest = false;

			if (cell_parent.parent_index.second != lookup_idx.second){
				new_densities.push_back(temp_ordered[lookup_idx.first]);
				sampled.insert(temp_ordered[lookup_idx.first].idx);
			}
    		        lookup_idx  = cell_parent.parent_index;
		}



		reverse(new_densities.begin(), new_densities.end());

	}// end of if (!temp_maps.empty())

	if (new_densities.size()>0){
             std::sort(new_densities.begin(), new_densities.end(), id_compare);
              for (std::vector<Density>::iterator it = new_densities.begin() ; it != new_densities.end(); ++it){
		   ordered.push_back(*it);

                 }
	      return ordered[order];
     	}else{
	     return Density{npos, 0};
   	}
}



DensityMap::Density DensityMap::two_prong(uint64_t order){
	if (order < ordered.size()){
		return ordered[order];
	}else if (ordered_done){
		return Density{npos, 0};
	}


	if(order==0){
		temp_ordered.clear();
	 }


	temp_ordered.clear();
	uint64_t samples_wanted = delta * (total_samples_wanted-sampled_so_far);
	//cout << "samples_wanted: " << samples_wanted << endl;

	std::vector<DensityMap*> temp_maps=and_maps;
	if(!or_maps.empty())
        	temp_maps=or_maps;

	int l = 0;
	if (!temp_maps.empty()){
        	for (uint64_t i = 0; i<temp_maps[0]->num_splits; i++) {
		Density aggr_j;
		if(!and_maps.empty())
                	aggr_j = Density{i, 1};
            	else
                	aggr_j = Density{i, 0};

//  	Density aggr_j= Density{i, 1};
			for (uint64_t j = 0; j < temp_maps.size(); j++) {
				Density d = temp_maps[j]->index_get(i);
				if(!and_maps.empty())
                        		aggr_j.freq *= d.freq;
                		else
                        		aggr_j.freq += d.freq;
			//	cout << "aggr_j: " << aggr_j.freq << endl;

				//aggr_j.freq *= d.freq;
			}


			if (!or_maps.empty())
				aggr_j.freq = std::min(aggr_j.freq, 1.0);

			if(granularity>1){
                 		for(uint64_t b=0;b<granularity;b++){
                           		uint64_t row_id=i+b;;
		                        for (uint64_t j = 0; j < temp_maps.size(); j++){
                		                if(temp_maps[j]->nonzero[row_id]){
                                		        nonzero[row_id]=true;
		                                        break;
                		                }
                           		}
                   		}
	                }else{
        	        	nonzero[i]=true;
	                }

			if (sampled.find(i) == sampled.end()){
				temp_ordered.push_back(aggr_j);
			}



		}



		if (temp_ordered.size() == 0){
			ordered_done = true;
			return Density{npos, 0};

		}



		// find smallest contiguous blocks whose estimated samples > samples_wanted

//		while (curr_end < num_blocks){
  	        uint64_t candidate_size = temp_ordered.size();

   	        uint64_t curr_samples = 0, curr_start =0, curr_end = 0;
	        uint64_t neg_value = 0;
		uint64_t min_len = candidate_size + 1, min_start=0, min_end = 0;
		uint64_t i = 0;
		while (curr_end < candidate_size){
			while (curr_samples < (samples_wanted+neg_value) && curr_end < candidate_size){
                                uint64_t num_samples =  temp_ordered[curr_end].freq *(records_per_buf * granularity);

				curr_samples += num_samples;


				//cout << "curr_end: " << curr_end << " num_samples: " << num_samples << " curr_samples: " << curr_samples << endl;


                                if (neg_value > 0){
                                        if (curr_samples >= neg_value){
                                                curr_samples -= neg_value;
                                                neg_value = 0;
                                        }else{
                                                neg_value -= curr_samples;
                                                curr_samples = 0;
                                        }
                                }
				curr_end ++;
                        }
			if (curr_end - curr_start < min_len){
                                        min_len = curr_end - curr_start;
                                        min_start = curr_start;
                                        min_end = curr_end-1;
                        }
			uint64_t num_samples = temp_ordered[curr_start].freq *(records_per_buf * granularity);
			if (curr_samples >= (num_samples+neg_value)){
				curr_samples = curr_samples -  num_samples - neg_value;
				neg_value = 0;
			}else{
				neg_value += num_samples - curr_samples;
				curr_samples = 0;
			}

				curr_start ++;
			//cout << "curr_end: " << curr_end << " num_samples: " << num_samples << " curr_samples: " << curr_samples << endl;

		}
/**
		while (curr_end < candidate_size && curr_start < candidate_size){
			while (curr_samples <= (samples_wanted+neg_value) && curr_end < candidate_size){
				curr_samples += temp_ordered[curr_end].freq *(records_per_buf * granularity);
				if (neg_value > 0){
					if (curr_samples >= neg_value){
						curr_samples -= neg_value;
						neg_value = 0;
					}else{
						neg_value -= curr_samples;
						curr_samples = 0;
					}
				}
				curr_end ++;

			}
//			while (curr_samples > (samples_wanted+neg_value) && curr_start < candidate_size && curr_start < curr_end){
			while (curr_samples > (samples_wanted + neg_value) && curr_start < candidate_size){
				if (curr_end - curr_start < min_len){
					min_len = curr_end - curr_start;
					min_start = curr_start;
					min_end = curr_end-1;
				}

				uint64_t next_block_samples = temp_ordered[curr_start].freq * ( granularity * records_per_buf);

				if (curr_samples > (next_block_samples + neg_value)){
					curr_samples = curr_samples - next_block_samples - neg_value;
					curr_start++;
				}else{
					neg_value = next_block_samples + neg_value - curr_samples;
					curr_samples = 0;
					curr_start ++;
				}
					//curr_samples = 0;
			}
		}

**/
		//cout << "min_start:" << min_start << " min_end: " << min_end << endl;

		for (uint64_t i = min_start; i <= min_end; i++){
			if (temp_ordered[i].freq != 0)
				ordered.push_back(temp_ordered[i]);
			//	cout << "insert index: " << temp_ordered[i].idx << endl;
			//	cout << "freq: " << temp_ordered[temp_ordered[i].idx].freq << endl;
			//	cout << "# samples : " << temp_ordered[temp_ordered[i].idx].freq * records_per_buf<< endl;
			sampled.insert(temp_ordered[i].idx);

		}



/**
		for (std::vector<Density>::iterator it = temp_ordered.begin()+min_start ; it <= temp_ordered.begin()+min_end; ++it){
			ordered.push_back(*it);

		}
**/

//		ordered_done=true;

	}
	if (order < ordered.size()) {
 		return ordered[order];
	}else {
	        return Density{npos, 0};
	}



}


DensityMap::Density DensityMap::linear_pass_fixed_blocks(uint64_t order)
{
    if (order < ordered.size()) {
        return ordered[order];
    }else if (ordered_done) {
	cout << "order: " << order << " return npos: " << npos << endl;
        return Density{npos, 0};
    }

   if(order==0){
         temp_ordered.clear();
   }

   std::vector<DensityMap*> temp_maps=and_maps;

   uint64_t samples_wanted = delta * (total_samples_wanted-sampled_so_far);

   if(!temp_maps.empty()){
	   for (uint64_t i = 0; i<temp_maps[0]->num_splits; i++) {
		   Density aggr_j= Density{i, 1};;
	       for (uint64_t j = 0; j < temp_maps.size(); j++) {
		       Density d = temp_maps[j]->index_get(i);
		       aggr_j.freq *= d.freq;
       		}
		if(granularity>1){
	           for(uint64_t b=0;b<granularity;b++){
        		   uint64_t row_id=i+b;;
		           for (uint64_t j = 0; j < temp_maps.size(); j++){
           			if(temp_maps[j]->nonzero[row_id]){
		           		nonzero[row_id]=true;
           				break;
           			}
           		   }
           	   }
           	}else{
	       	  nonzero[i]=true;
       		}
//	     	ordered.push_back(aggr_j);
		if (sampled.find(i) == sampled.end())
			temp_ordered.push_back(aggr_j);

	   }

	   if (temp_ordered.size() == 0)
		ordered_done = true;

//	   std::sort(ordered.begin(), ordered.end(), density_compare);
      	   std::sort(temp_ordered.begin(), temp_ordered.end(), density_compare);
	   uint64_t estimated_samples = 0;
	   for (uint64_t i = 0; i < temp_ordered.size(); i++){
		estimated_samples += temp_ordered[i].freq * (records_per_buf * granularity);
		ordered.push_back(temp_ordered[i]);
		sampled.insert(temp_ordered[i].idx);
		if (estimated_samples >= samples_wanted){
			break;
		}
	   }


   }

   std::sort(ordered.begin(), ordered.end(), id_compare);
   if (order < ordered.size()) {
         return ordered[order];
   }else {
         return Density{npos, 0};
   }

}




//Uses Fagin's for AND/OR queries

//Uses Fagin's for AND/OR queries

DensityMap::Density DensityMap::ordered_get(uint64_t order)
{

    if (order < ordered.size()) {
        return ordered[order];
    } else if (ordered_done) {

	return Density{npos, 0};
    }


    unordered_set<uint64_t> new_seen;

    if(order==0){
     //handles only AND case for now
     temp_ordered.clear();
     curr_max_density=Density{npos,-1};
     curr_pos=0;
     curr_max_pos=-1;
    }

    //for holding new densities, we sort these by ids before adding them to ordered array
    std::vector<Density> new_densities;

    Density max_d = Density{npos, -1};


    bool sampling_done=false;
    uint64_t estimated_samples=0;

 // To add in util;
    uint64_t samples_wanted=total_samples_wanted-sampled_so_far;

    std::vector<DensityMap*> temp_maps=and_maps;
    if(!or_maps.empty())
    	 temp_maps=or_maps;

    if (!temp_maps.empty()) {
        for (uint64_t i = curr_pos; ; i++) {
            new_seen.clear();
         	Density aggr_j;
            if(!and_maps.empty())
            	aggr_j = Density{npos, 1};
            else
            	aggr_j = Density{npos, 0};

            bool all_done = true;
            for (uint64_t j = 0; j < temp_maps.size(); j++) {
                Density d = temp_maps[j]->ordered_get(i);
                if (d.idx != npos && sampled.find(d.idx) == sampled.end() ) {
                    new_seen.insert(d.idx);
                }



                if(!and_maps.empty())
                	aggr_j.freq *= d.freq;
                else
                	aggr_j.freq += d.freq;
            }


            for (unordered_set<uint64_t>::const_iterator it = new_seen.begin();
                    it != new_seen.end(); it++) {
                Density aggr_d;
                if (valid[*it]) {
                    aggr_d = v[*it];
                } else {
                    aggr_d = Density{*it, 1};
                    for (uint64_t j = 0; j < temp_maps.size(); j++) {
                        Density d = temp_maps[j]->index_get(aggr_d.idx);
                        //aggr_d.freq *= d.freq;
                        if(!and_maps.empty())
                        	aggr_d.freq *= d.freq;
                        else
                           	aggr_d.freq += d.freq;
                    }
                    valid[aggr_d.idx] = true;
                    v[aggr_d.idx] = aggr_d;
                    if(granularity>1){
                    for(uint64_t b=0;b<granularity;b++){
                    uint64_t row_id=aggr_d.idx+b;;
                    for (uint64_t j = 0; j < temp_maps.size(); j++){
                    	if(temp_maps[j]->nonzero[row_id])
                    	{
                    		nonzero[row_id]=true;
                    		break;

                    	}
                    }
                    }
                    }
                  else{
                	  nonzero[aggr_d.idx]=true;
                }
                }

                temp_ordered.push_back(aggr_d);
                sampled.insert(aggr_d.idx);
                 if(aggr_d.freq > curr_max_density.freq){
                  		 curr_max_density=aggr_d;
                   		 curr_max_pos=temp_ordered.size()-1;
                 }


                while(not sampling_done and curr_max_density.freq >= aggr_j.freq) {

	            	 	 new_densities.push_back(curr_max_density);



			     temp_ordered.erase(temp_ordered.begin()+(curr_max_pos));


			    // in case for or_case, where the addition may exceed to 1
				double max_freq = std::min(1.0, curr_max_density.freq);
		   		estimated_samples+=max_freq*(granularity*records_per_buf);
		            // estimated_samples+=curr_max_density.freq*(granularity*records_per_buf);

                	    // update estimated_samples.
         			  curr_max_density=Density{npos,-1};
                	      if (estimated_samples < delta*samples_wanted){
                	    	  int c=0;
                	    	   for (std::vector<Density>::iterator it = temp_ordered.begin() ; it != temp_ordered.end(); ++it)
                	    	                  	{
                	    	                  		if(it->freq>curr_max_density.freq){
                	    	                  			curr_max_density=*it;
                	    	                 	        curr_max_pos=c;
                	    	                  	}
                	    	                  		c++;
                	    	                  	}
                			}
                			else{
                				sampling_done=true;
                          }
                  }//end of while

            }//end fo for loop in new seen


            if (aggr_j.freq == 0.0) {
                ordered_done = true;
		sampling_done=true;

            }

            if (sampling_done){
            	curr_pos=i;
            	break;
            }
        }
    }else {
        UNIMPLEMENTED("FINISH ME");
    }


    if (new_densities.size()>0){
         std::sort(new_densities.begin(), new_densities.end(), id_compare);
         for (std::vector<Density>::iterator it = new_densities.begin() ; it != new_densities.end(); ++it)
          	 {    ordered.push_back(*it);

         	 }
      return ordered[order];
     }
     else{

     return Density{npos, 0};
   }

}


/*
DensityMap::Density DensityMap::ordered_get(uint64_t order)
{
    if (order < ordered.size()) {
        return ordered[order];
    } else if (ordered_done) {
        return Density{npos, 0};
    }
    unordered_set<uint64_t> old_seen;
    unordered_set<uint64_t> new_seen;
    Density max_d = Density{npos, -1};
    std::vector<DensityMap*> temp_maps=and_maps;
    if(!or_maps.empty())
		temp_maps=or_maps;

    if (!temp_maps.empty()) {
        for (uint64_t i = 0; ; i++) {
            new_seen.clear();
         	Density aggr_j;
            if(!and_maps.empty())
            	aggr_j = Density{npos, 1};
            else
            	aggr_j = Density{npos, 0};

            bool all_done = true;
            for (uint64_t j = 0; j < temp_maps.size(); j++) {
                Density d = temp_maps[j]->ordered_get(i);
                if (d.idx != npos && sampled.find(d.idx) == sampled.end() &&
                        old_seen.find(d.idx) == old_seen.find(d.idx)) {
                    new_seen.insert(d.idx);
                }
                if(!and_maps.empty())
                	aggr_j.freq *= d.freq;
                else
                	aggr_j.freq += d.freq;

            }

            for (unordered_set<uint64_t>::const_iterator it = new_seen.begin();
                    it != new_seen.end(); it++) {
                Density aggr_d;
                if (valid[*it]) {
                    aggr_d = v[*it];
                } else {
                    aggr_d = Density{*it, 1};
                    for (uint64_t j = 0; j < temp_maps.size(); j++) {
                        Density d = temp_maps[j]->index_get(aggr_d.idx);
                        //aggr_d.freq *= d.freq;
                        if(!and_maps.empty())
                        	aggr_d.freq *= d.freq;
                        else
                           	aggr_d.freq += d.freq;
                    }
                    valid[aggr_d.idx] = true;
                    v[aggr_d.idx] = aggr_d;
                }
                old_seen.insert(aggr_d.idx);

                if (aggr_d.freq > max_d.freq) {
                    max_d = aggr_d;
                }
            }

            if (max_d.freq > aggr_j.freq) {
                ordered.push_back(max_d);
                sampled.insert(max_d.idx);
                return max_d;
            }

            if (aggr_j.freq == 0) {
                ordered_done = true;
                return Density{npos, 0};
            }
        }
    } else {
        UNIMPLEMENTED("FINISH ME");
    }
}

*/



/*
DensityMap::Density DensityMap::ordered_get(uint64_t order)
{

    if (order < ordered.size()) {
        return ordered[order];
    } else if (ordered_done) {
        return Density{npos, 0};
    }

    int k=1;
    unordered_set<uint64_t> old_seen;
    unordered_set<uint64_t> new_seen;
    Density max_d = Density{npos, -1};
    std::vector<DensityMap*> temp_maps=and_maps;
    if(!or_maps.empty())
    	 temp_maps=or_maps;
    std::vector<Density> top_k_densities;
    Density kth_density = Density{npos,2};
    int kthpos=-1;
    top_k_densities.clear();

    if (!temp_maps.empty()) {
        for (uint64_t i = 0; ; i++) {
            new_seen.clear();
         	Density aggr_j;
            if(!and_maps.empty())
            	aggr_j = Density{npos, 1};
            else
            	aggr_j = Density{npos, 0};

            bool all_done = true;
            for (uint64_t j = 0; j < temp_maps.size(); j++) {
                Density d = temp_maps[j]->ordered_get(i);
                if (d.idx != npos && sampled.find(d.idx) == sampled.end() ) {
                    new_seen.insert(d.idx);
                }
                if(!and_maps.empty())
                	aggr_j.freq *= d.freq;
                else
                	aggr_j.freq += d.freq;
            }

            for (unordered_set<uint64_t>::const_iterator it = new_seen.begin();
                    it != new_seen.end(); it++) {
                Density aggr_d;
                if (valid[*it]) {
                    aggr_d = v[*it];
                } else {
                    aggr_d = Density{*it, 1};
                    for (uint64_t j = 0; j < temp_maps.size(); j++) {
                        Density d = temp_maps[j]->index_get(aggr_d.idx);
                        //aggr_d.freq *= d.freq;
                        if(!and_maps.empty())
                        	aggr_d.freq *= d.freq;
                        else
                           	aggr_d.freq += d.freq;
                    }
                    valid[aggr_d.idx] = true;
                    v[aggr_d.idx] = aggr_d;
                    if(granularity>1){
	                    for(uint64_t b=0;b<granularity;b++){
        		        uint64_t row_id=aggr_d.idx+b;;
                    		for (uint64_t j = 0; j < temp_maps.size(); j++){
		                    	if(temp_maps[j]->nonzero[row_id]){
		                    		nonzero[row_id]=true;
                		    		break;

                    			}
		                    }
        	            }
                    }else{
                	  nonzero[aggr_d.idx]=true;
                   }
                }

                old_seen.insert(aggr_d.idx);

//                if (aggr_d.freq > kth_density.freq) {
//                   	 temp_ordered.push_back(max_d);
 //                 }

                if(top_k_densities.size() <  k){
                   	top_k_densities.push_back(aggr_d);
                	 if(aggr_d.freq < kth_density.freq){
                		 kth_density=aggr_d;
                		 kthpos=top_k_densities.size()-1;
                	 }
                }
                else  if(aggr_d.freq>kth_density.freq){
                	top_k_densities.erase(top_k_densities.begin()+(kthpos));
                	top_k_densities.push_back(aggr_d);
                	kthpos=top_k_densities.size()-1;
                	kth_density=aggr_d;
                	int c=0;
                	for (std::vector<Density>::iterator it = top_k_densities.begin() ; it != top_k_densities.end(); ++it)
                	{
                		if(it->freq<kth_density.freq){
               		    kth_density=*it;
               	        kthpos=c;
                	}
                		c++;
                	}

                }
            }

            bool done=false;
            if (top_k_densities.size() > 0 and kth_density.freq > aggr_j.freq) {
            	done=true;
            }
            if (aggr_j.freq == 0) {
                ordered_done = true;
                done=true;
            }
             if(done){
            	if (top_k_densities.size()>0){
            	                std::sort(top_k_densities.begin(), top_k_densities.end(), id_compare);
            	                for (std::vector<Density>::iterator it = top_k_densities.begin() ; it != top_k_densities.end(); ++it)
            	                  	 {    ordered.push_back(*it);
            	                   	      sampled.insert(it->idx);
            	                	 }
            	                  return ordered[order];
            	                }
            	else{
              		return Density{npos, 0};
            	}

            }
        }
    } else {
        UNIMPLEMENTED("FINISH ME");
    }
}

*/



DensityMap::Density DensityMap::index_get(uint64_t idx)
{
    if (valid[idx]) {
        return v[idx];
    }

    if (and_maps.empty() && or_maps.empty()) {
        valid[idx] = true;
        v[idx].idx = idx;
        return v[idx];
    }

    if (!and_maps.empty()) {
        UNIMPLEMENTED("FINISH ME");
    } else {
        UNIMPLEMENTED("FINISH ME");
    }
}






#if 0
bool DensityMap::ordered_get(Density& d, uint64_t order)
{
    if (order < ordered.size()) {
        d.idx = ordered[order].idx;
        d.freq = ordered[order].freq;
        return true;
    } else if (ordered_done) {
        return false;Sampling
    }

    // Considerations
#if USE_PAIRING_HEAP
    boost::heap::pairing_heap<Density, boost::heap::compare<DensityCompare> > q;
#else
    priority_queue<Density, vector<Density>, DensityCompare> q;
#endif
    vector<DensityMap*>* combine_maps;
    bool and_op;
    if (!and_maps.empty()) {
        combine_maps = &and_maps;
        and_op = true;
    } else {
        combine_maps = &or_maps;
        and_op = false;
    }

    DensityMap* m = (*combine_maps)[0];
    for (size_t i = 0; ; i++) {
        Density d0;
        bool ok = m->ordered_get(d0, i);
        // FIXME - This might have to be from the longest ordered queue
        if (!ok) {
            break;
        } else if (sampled.find(d0.idx) != sampled.end()) {
            continue;
        }

        double thresh_freq = d0.freq;
        for (size_t j = 1; j < combine_maps->size(); j++) {
            Density dj;
            ok = (*combine_maps)[j]->ordered_get(dj, i);
            if (!ok) {
                // FIXME - This probably should depend on whether it's AND or OR
                goto cleanup;
            }
            if (and_op) {
                thresh_freq *= dj.freq;
            } else {
                thresh_freq += dj.freq;
            }

            (*combine_maps)[j]->idx_get(dj, d0.idx);
            if (and_op) {
                if (dj.freq == 0) {
                    d0.freq = 0;
                    break;
                } else {
                    d0.freq *= dj.freq;
                }
            } else {
                d0.freq += dj.freq;
            }
        }

        if (d0.freq == 0) {
            continue;
        }

        q.push(d0);
        if (q.top().freq >= thresh_freq) {
            break;
        }
    }

cleanup:
    if (q.empty()) {
        ordered_done = true;
        return false;
    } else {
        d.idx = q.top().idx;
        d.freq = q.top().freq;
        sampled.insert(d.idx);
        ordered.push_back(d);
        return true;
    }
}
#endif


#if 0
void DensityMap::idx_get(Density& d, uint64_t idx)
{
    d.idx = idx;

    if (valid[idx]) {
        d.freq = v[idx].freq;
        return;
    }

    if (and_maps.empty() && or_maps.empty()) {
        // This is a base map
        d.freq = 0;
        valid[idx] = true;
        return;
    }

    vector<DensityMap*>* combine_maps;
    bool and_op;
    if (!and_maps.empty()) {
        combine_maps = &and_maps;
        and_op = true;
        d.freq = 1;
    } else {
        combine_maps = &or_maps;
        and_op = false;
        d.freq = 0;
    }

    Density di;
    for (size_t i = 0; i < combine_maps->size(); i++) {
        (*combine_maps)[i]->idx_get(di, idx);

        if (and_op) {
            if (di.freq == 0) {
                d.freq = 0;
                break;
            } else {
                d.freq *= di.freq;
            }
        } else {
            d.freq += di.freq;
        }
    }
    v[idx].idx = idx;
    v[idx].freq = d.freq;
    valid[idx] = true;
}
#endif

//bool DensityMap::pop_top(Density* d)
//{
//    if (and_maps.empty() && or_maps.empty()) {
//        // We are a base map (not combining)
//        if (pqueue.empty()) {
//            return false;
//        } else {
//            Density& top_density = pqueue.top();
//            d->idx = top_density.idx;
//            d->freq = top_density.freq;
//            pqueue.pop();
//            return true;
//        }
//    } else {
//        for (size_t i = 0; i < and_map.size(); i++) {
//            pop_top();
//        }
//    }
//}

size_t DensityMap::random_sample()
{
    UNIMPLEMENTED("DensityMap::random_sample");
}

void DensityMap::set(uint64_t row)
{
    uint64_t row_idx = row / (records_per_buf);
    uint64_t granular_id= row_idx/granularity;
    v[granular_id].idx = granular_id;
    v[granular_id].freq += (double) 1 /(granularity*records_per_buf);
    nonzero[row_idx] = true;
    valid[granular_id] = true;
}

void DensityMap::post_set()
{
    /* TODO - Might be ways to optimize this. */
    for (size_t i = valid.find_first(); i != npos; i =
    		valid.find_next(i)) {
        ordered.push_back(v[i]);
    }
    std::sort(ordered.begin(), ordered.end(), density_compare);
    ordered_done = true;
}


void DensityMap::and_combine(SwiftMap* m)
{
    and_maps.push_back(static_cast<DensityMap*>(m));
}

void DensityMap::or_combine(SwiftMap* m)
{
    or_maps.push_back(static_cast<DensityMap*>(m));
}

void DensityMap::read(std::istream& in)
{
    size_t ordered_size;
    in >> ordered_size;
    for (size_t i = 0; i < ordered_size; i++) {
        in >> ordered[i].idx;
        in >> ordered[i].freq;
        uint64_t idx = ordered[i].idx;
        nonzero[idx] = true;
        valid[idx] = true;
        v[idx].idx = ordered[i].idx;
        v[idx].freq = ordered[i].freq;
    }
}

void DensityMap::write(std::ostream& out)
{
    out << ordered.size();
    for (size_t i = 0; i < ordered.size(); i++) {
        out << ordered[i].idx;
        out << ordered[i].freq;
    }
}

bool DensityMap::get(size_t pos)
{
    UNIMPLEMENTED("DensityMap::get");
}

std::string DensityMap::to_string()
{
    string s;
    for (uint64_t i = 0; i < num_splits; i++) {
        stringstream ss;
        ss << (uint64_t) round(ordered[i].freq * records_per_buf);
        s += ss.str();
    }
    return s;
}

SwiftMapFactory::SwiftMapFactory(map_type_t map_type, uint64_t records_per_buf)
    : map_type(map_type), records_per_buf(records_per_buf)
{

    if (map_type == POOL_LAZY_BITMAP) {
        PoolLazyBitmap<SwiftMap::bitmap>::fill_pool();
    } else if (map_type == POOL_STATIC_LAZY_BITMAP) {
        PoolLazyBitmap<SwiftMap::sbitmap>::fill_pool();
    } else if (map_type == DENSITY_MAP) {
        cout << "in map Density map" <<endl;
        PoolLazyBitmap<SwiftMap::bitmap>::fill_pool();
        cout << "in map Density map after pool lazy" <<endl;
    }
}

SwiftMap* SwiftMapFactory::create_map(uint64_t num_records, bool set, bool tmp)
{
    SwiftMap* m;
    if (map_type == SINGLE_BITMAP) {
        m = new SingleBitmap(num_records, set);
    } else if (map_type == LAZY_BITMAP) {
        m = new LazyBitmap<SwiftMap::bitmap>(num_records, set);
    } else if (map_type == STATIC_LAZY_BITMAP) {
        m = new LazyBitmap<SwiftMap::sbitmap>(num_records, set);
    } else if (map_type == POOL_LAZY_BITMAP) {
        m = new PoolLazyBitmap<SwiftMap::bitmap>(num_records, set);
    } else if (map_type == POOL_STATIC_LAZY_BITMAP) {
        m = new PoolLazyBitmap<SwiftMap::sbitmap>(num_records, set);
    } else if (map_type == DENSITY_MAP) {
        m = new DensityMap(num_records, records_per_buf, set);
    } else if (map_type == NO_INDEX) {
        m = new NoIndex(num_records, records_per_buf, set);
    } else if (map_type == EWAH_BITMAP) {
        m = new EWAHBitmap(num_records, set);
    } else if (map_type == LOSSY_BITMAP){
        m = new LossyBitmap(num_records, records_per_buf, set);
    } else {
        panic("Unsupported map_type %d", map_type);
    }

    if (tmp) {
        tmp_maps.push_back(m);
    }

    return m;
}

void SwiftMapFactory::destroy_tmp()
{
    for (size_t i = 0; i < tmp_maps.size(); i++) {
        delete tmp_maps[i];
    }
    tmp_maps.clear();
}

void SwiftMapFactory::destroy_swiftmap(SwiftMap *m)
{
    delete m;
}



