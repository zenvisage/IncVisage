#ifndef _SWIFT_MAP_H_
#define _SWIFT_MAP_H_

#include <bitset>
#include <cinttypes>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <queue>
#include <random>

#include <boost/dynamic_bitset.hpp>

#include <ewah.h>

#include "query.h"
#include "utils.h"


class SwiftMapFactory;
class DensityMap;

class SwiftMap {
    friend class SwiftMapFactory;

public:
    enum sample_type_t {
        SEQUENTIAL, // Completely sequential
        // TODO - We have many levels of pseudorandomness, including not oring
        // with highly selective bitmaps or not doing bin edge checking, or
        // sequential within one page read
        PSEUDORANDOM,
		SCAN,
        RANDOM, // Completely random
	   TWOPRONG,
    };
    bool isSamplesEmpty;

    virtual size_t next_sample(sample_type_t sample_type)
    {
        if (sample_type == SEQUENTIAL) {
            return sequential_sample();
        } else if (sample_type == PSEUDORANDOM) {
            return pseudorandom_sample();
        } else if (sample_type == TWOPRONG){
		return two_prong_sample();
	}else {
            // (sample_type == RANDOM)
            return random_sample();
        }
    }

    virtual void pre_set() { }
    virtual void set(uint64_t row) = 0;
    virtual void post_set() { }

    // Equiavalent to this &= m
    virtual void and_combine(SwiftMap* m) = 0;
    // Equiavalent to this |= m
    virtual void or_combine(SwiftMap* m) = 0;

    virtual void read(std::istream& in) = 0;
    virtual void write(std::ostream& out) = 0;

    virtual bool is_approx() = 0;

    virtual std::vector<uint64_t> find_valid_blocks() = 0;

    // block ids where |valid records| > 0
    virtual std::vector<uint64_t> get_valid_blocks(){
	return valid_blocks;
    }

    // NOTE - These methods should only be used for debugging purposes as
    // they're not the fastest
    virtual bool get(size_t pos) = 0;
    virtual std::string to_string() = 0;

/**
    virtual void clear(){
	if (map_type == NO_INDEX || map_type == DENSITY_MAP)
    		last_sampled = 0;
	else
		last_sampled = npos;
	std::vector<uint64_t>().swap(valid_blocks);
    }
**/
    virtual void clear(){
	last_sampled = npos;
        std::vector<uint64_t>().swap(valid_blocks);


	}
    static const size_t npos;

    virtual void set_total_samples(uint64_t samples){
    	total_samples_wanted=samples;
    }

    virtual void set_sampled_so_far(uint64_t samples){
    	sampled_so_far=samples;
    }

    virtual size_t get_sampled_so_far(){
       	return sampled_so_far;
    }

    virtual void set_est_valid_records(uint64_t est_valid_records){
        this -> est_valid_records = est_valid_records;
    }

    virtual uint64_t get_est_valid_records(){
        return this -> est_valid_records;
    }





protected:
    SwiftMap(uint64_t num_records)
        : num_records(num_records), last_sampled(npos)
    { }
    virtual ~SwiftMap() { }

    virtual size_t sequential_sample() = 0;
    virtual size_t pseudorandom_sample() = 0;
    virtual size_t random_sample() = 0;     // NOTE - This is with replacement
    virtual size_t two_prong_sample() = 0;

    typedef unsigned long block_type_t;
    typedef boost::dynamic_bitset<block_type_t> bitmap;
    typedef std::bitset<SPLIT_SIZE> sbitmap;
    typedef EWAHBoolArray<block_type_t> ewahmap;

    std::vector<uint64_t> valid_blocks;

    uint64_t num_records;
    uint64_t est_valid_records;

    size_t last_sampled;
    size_t total_samples_wanted;
    size_t sampled_so_far;




};

class SingleBitmap : public SwiftMap {
    friend class SwiftMapFactory;

public:


    virtual void set(uint64_t row);

    virtual void and_combine(SwiftMap* m);
    virtual void or_combine(SwiftMap* m);

    virtual void read(std::istream& in);
    virtual void write(std::ostream& out);

    virtual bool is_approx() { return false; }

    virtual bool get(size_t pos);
    virtual std::string to_string();

protected:
    // NOTE - Setting 'set' to true will take more time to intialize
    SingleBitmap(uint64_t num_records, bool set);
    virtual ~SingleBitmap();

    virtual size_t sequential_sample();
    virtual size_t pseudorandom_sample()
    {
        UNIMPLEMENTED("LazyBitmap::pseudorandom_sample");
    }
    virtual size_t two_prong_sample()
    {
        UNIMPLEMENTED("LazyBitmap::two_prong_sample");
    }

    virtual std::vector<uint64_t> find_valid_blocks(){
	UNIMPLEMENTED("LazyBitmap::find_valid_blocks");
	}

 





    virtual size_t random_sample();
    std::unordered_set<uint64_t> seen;
    std::vector<uint64_t> samples;
    uint64_t  order=0;

    bitmap* b;

     static bool id_compare(int lhs, const int rhs)
       {
           return lhs < rhs;
      }



};

// FIXME - We can make the size of LazyBitmap smaller by not pre-allocating
// all-0 bitmaps; in effect getting some form of RLE
template<typename T>
class LazyBitmap : public SwiftMap {
    friend class SwiftMapFactory;

public:
    virtual void set(uint64_t row);

    virtual void and_combine(SwiftMap* m);
    virtual void or_combine(SwiftMap* m);

    virtual void read(std::istream& in);
    virtual void write(std::ostream& out);

    virtual bool is_approx() { return false; }


    virtual bool get(size_t pos);
    // NOTE - This will evaluate the entire bitmap
    virtual std::string to_string();

protected:
    LazyBitmap(uint64_t num_records, bool set);
    virtual ~LazyBitmap();

    virtual size_t sequential_sample();
    virtual size_t pseudorandom_sample()
    {
        UNIMPLEMENTED("LazyBitmap::pseudorandom_sample");
    }
    virtual size_t random_sample();
    virtual size_t two_prong_sample()
    {
        UNIMPLEMENTED("LazyBitmap::two_prong_sample");
    }


   virtual std::vector<uint64_t> find_valid_blocks(){
	UNIMPLEMENTED("LazyBitmap::find_valid_blocks");
	}





    inline size_t find_first();
    inline size_t find_next(size_t pos);

    inline size_t _find_first(size_t i);
    inline size_t _find_next(size_t i, size_t pos);

    virtual T* new_bitmap(bool set);
    virtual void delete_bitmap(T* b);

    // This method is safe to call even when the bitmap has already been
    // evaluated. It will just return right away in that case.
    virtual T* eval_bitmap(size_t bitmap_idx);

    std::vector<T*> v;
    bool set_default;

    std::default_random_engine rng_generator;
    std::uniform_int_distribution<uint64_t> rng_distrib;

    std::vector<LazyBitmap*> and_maps;
    std::vector<LazyBitmap*> or_maps;
};


// PoolLazyBitmap is a variation of LazyBitmap which uses a preallocated pool
// of buffers for its bitmaps; NOTE - NOT THREAD SAFE
template<typename T>
class PoolLazyBitmap : public LazyBitmap<T> {
    friend class SwiftMapFactory;
    friend class DensityMap;

protected:
    PoolLazyBitmap(uint64_t num_records, bool set)
        : LazyBitmap<T>(num_records, set)
    { }

    virtual T* new_bitmap(bool set);
    virtual void delete_bitmap(T* b);

    static void fill_pool();

    static std::list<T*> pool;
};

// The indices returned by DensityMap may not
class DensityMap : public SwiftMap {
    friend class SwiftMapFactory;

public:
    virtual void set(uint64_t row);
    virtual void post_set();

    virtual void and_combine(SwiftMap* m);
    virtual void or_combine(SwiftMap* m);

    virtual void read(std::istream& in);
    virtual void write(std::ostream& out);


    // NOTE - For approx maps, we can just return the begnning idx of the first
    // DENSITY_MAP block of data as the idx for find_first. Then
    // FIXME - Maybe after density map indicates whether a map is likely to
    // have a value or not a bitmap should be constructed based on the density
    // map
    virtual bool is_approx() { return true; }

    virtual bool get(size_t pos);
    virtual std::string to_string();

   /* virtual void set_last_sampled_so_far(uint64_t samples){
        last_sampled=samples;
    }

    virtual size_t get_last_sampled_so_far(){
        return last_sampled;
    }*/

protected:
    DensityMap(uint64_t num_records, uint64_t records_per_buf, bool set);
    ~DensityMap();

    // This will return the index to the first record in the page
    virtual size_t sequential_sample();
    virtual size_t pseudorandom_sample();
    virtual size_t random_sample();
    virtual size_t two_prong_sample();
    virtual std::vector<uint64_t> find_valid_blocks();

  


    struct Density {
        //bool init;
        //bool pushed;
        uint64_t idx;
        double freq;
    };

    // Get the 'order'th element in density map. Returns false if cannot find.
    virtual Density ordered_get(uint64_t order);
    // Do random access to get the density corresponding to 'idx'.
    virtual Density index_get(uint64_t idx);

    Density linear_pass_fixed_blocks(uint64_t order);
    Density two_prong(uint64_t order);

    Density vanilla_forward_optimal(uint64_t order);
//    vector<Density> build_dp(uint64_t start_index, uint64_t num_blocks);
    uint64_t calc_cost(uint64_t start, uint64_t end);
 //   Density seg_forward_optimal(uint64_t order);


    static bool density_compare(const Density& lhs, const Density& rhs)
    {
        return lhs.freq > rhs.freq;
    }

    static bool id_compare(const Density& lhs, const Density& rhs)
       {
           return lhs.idx < rhs.idx;
      }


    struct DensityCompare {
        bool operator()(const Density& lhs, const Density& rhs) const
        {
            return lhs.freq > rhs.freq;
        }
    };

    uint64_t records_per_buf;
    uint64_t num_splits;
    std::vector<Density> ordered;
    std::vector<Density> temp_ordered;
    std::unordered_set<uint64_t> old_seen;
    Density curr_max_density;
    uint64_t curr_max_pos;
    uint64_t curr_pos=0;

    bool ordered_done;
    std::vector<Density> v;  // TODO - This may have to be a hash map to save on space
    bitmap nonzero;
    bitmap valid; // whether the density map at that index has been initialized
    bool set_default;
    uint64_t granularity=GRANULARITY;
    std::vector<DensityMap*> and_maps;
    std::vector<DensityMap*> or_maps;
    float delta=DELTA;
    std::unordered_set<uint64_t> sampled;
    uint64_t last_sampled;


};



class NoIndex : public SwiftMap {
    friend class SwiftMapFactory;

public:
    virtual void set(uint64_t row);
    virtual void post_set();


    virtual void and_combine(SwiftMap* m);
    virtual void or_combine(SwiftMap* m);

    virtual void read(std::istream& in);
    virtual void write(std::ostream& out);



    // NOTE - For approx maps, we can just return the begnning idx of the first
    // DENSITY_MAP block of data as the idx for find_first. Then
    // FIXME - Maybe after density map indicates whether a map is likely to
    // have a value or not a bitmap should be constructed based on the density
    // map
    virtual bool is_approx() { return true; }

    virtual bool get(size_t pos);
    virtual std::string to_string();

protected:
     NoIndex(uint64_t num_records, uint64_t records_per_buf,bool set);
    ~NoIndex();

    // This will return the index to the first record in the page
    virtual size_t sequential_sample();
    virtual size_t pseudorandom_sample();
    virtual size_t random_sample();
    virtual size_t two_prong_sample()
    {
        UNIMPLEMENTED("NoIndex::two_prong_sample");
    }

    virtual std::vector<uint64_t> find_valid_blocks(){
	UNIMPLEMENTED("NoIndex::find_valid_blocks");
	}

  




    uint64_t records_per_buf;
    uint64_t num_splits;
    std::vector<NoIndex*> and_maps;
    std::vector<NoIndex*> or_maps;
    uint64_t last_sampled;

};


class EWAHBitmap : public SwiftMap {
    friend class SwiftMapFactory;

    public:
    virtual void set(uint64_t row);

    virtual void and_combine(SwiftMap* m);
    virtual void or_combine(SwiftMap* m);

    virtual void read(std::istream& in)
    {
        UNIMPLEMENTED("EWAHBitmap::read");
    }
    virtual void write(std::ostream& out);

    virtual bool is_approx() { return false; }

    virtual std::vector<uint64_t> find_valid_blocks()
    {
        UNIMPLEMENTED("EWAHBitmap::find_valid_blocks");
    }

  
    virtual bool get(size_t pos)
    {
        UNIMPLEMENTED("EWAHBitmap::get");
    }

    virtual std::string to_string()
    {
        UNIMPLEMENTED("EWAHBitmap::to_string");
    }

    virtual void clear(){
        //std::cout << "ewah clear" << std::endl;
        delete b;
        std::vector<size_t>().swap(samples);

    }

    protected:
    EWAHBitmap(uint64_t num_records, bool set);
    virtual ~EWAHBitmap();

    virtual size_t sequential_sample();
    virtual size_t pseudorandom_sample()
    {
        UNIMPLEMENTED("EWAHBitmap::pseudorandom_sample");
    }
    virtual size_t random_sample();
    virtual size_t two_prong_sample()
    {
        UNIMPLEMENTED("EWAHBitmap::two_prong_sample");
    }

    ewahmap* b;
    std::vector<size_t> samples;
    uint64_t order;

    std::vector<EWAHBitmap*> and_maps;
    std::vector<EWAHBitmap*> or_maps;
};


class LossyBitmap : public SwiftMap {
    friend class SwiftMapFactory;

    public:
    virtual void set(uint64_t row);

    virtual void and_combine(SwiftMap* m);
    virtual void or_combine(SwiftMap* m);
  

    virtual void read(std::istream& in)
    {
        UNIMPLEMENTED("LossyBitmap::read");
    }
    virtual void write(std::ostream& out);

    virtual bool is_approx() { return true; }

    virtual std::vector<uint64_t> find_valid_blocks()
    {
        UNIMPLEMENTED("LossyBitmap::find_valid_blocks");
    }

    virtual bool get(size_t pos)
    {
        UNIMPLEMENTED("LossyBitmap::get");
    }

    virtual std::string to_string()
    {
        UNIMPLEMENTED("LossyBitmap::to_string");
    }

    virtual void clear(){
	last_sampled = npos;

    }

    protected:
    LossyBitmap(uint64_t num_records, uint64_t records_per_buf, bool set);
    virtual ~LossyBitmap();

    virtual size_t sequential_sample();

    virtual size_t pseudorandom_sample()
    {
        UNIMPLEMENTED("LossyBitmap::pseudorandom_sample");
    }

    virtual size_t random_sample(){
        UNIMPLEMENTED("LossyBitmap::pseudorandom_sample");
    }

    virtual size_t two_prong_sample()
    {
        UNIMPLEMENTED("LossyBitmap::two_prong_sample");
    }

     bitmap* b; //size of the bitmap equals to the number of blocks.
     std::vector<size_t> samples;
    uint64_t order;

    std::vector<LossyBitmap*> and_maps;
    std::vector<LossyBitmap*> or_maps;
    uint64_t records_per_buf;
};





// NOTE - This class is NOT thread-safe AT ALL
class SwiftMapFactory {
public:
    enum map_type_t {
        SINGLE_BITMAP,
        LAZY_BITMAP,
        STATIC_LAZY_BITMAP,
        POOL_LAZY_BITMAP,
        POOL_STATIC_LAZY_BITMAP,
        DENSITY_MAP,
        NO_INDEX,
        EWAH_BITMAP,
        LOSSY_BITMAP
    };

    SwiftMapFactory(map_type_t map_type, uint64_t records_per_buf);

    // 'tmp' is set, the SwiftMap will be garbage collected by the
    // 'destroy_tmp' method. Otherwise, the SwiftMap must be destroyed properly
    // with the 'destroy_map' method.
    // TODO - We assume that a temporary map means a lazy map and a
    // non-temporary map means a non-lazy map. Verify that this is a correct
    // thing to assume.
    SwiftMap* create_map(uint64_t num_records, bool set, bool tmp);

    void destroy_tmp();
    void destroy_swiftmap(SwiftMap *m);

protected:
    map_type_t map_type;
    std::vector<SwiftMap*> tmp_maps;
    uint64_t records_per_buf;
};




// ******************************************************************
// ******************************************************************
// ******************************************************************
//
//
// Implemenation
//
//
// ******************************************************************
// ******************************************************************
// ******************************************************************


template<typename T>
LazyBitmap<T>::LazyBitmap(uint64_t num_records, bool set)
    : SwiftMap(num_records), set_default(set), rng_distrib(0, num_records-1)
{
    for (uint64_t i = 0; i < UPDIV(num_records, SPLIT_SIZE); i++) {
        v.push_back(NULL);
    }
}

template<typename T>
LazyBitmap<T>::~LazyBitmap()
{
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i] != NULL) {
            delete_bitmap(v[i]);
        }
    }
}

template<typename T>
void LazyBitmap<T>::delete_bitmap(T* b)
{
    delete b;
}

template<typename T>
size_t LazyBitmap<T>::sequential_sample()
{
    if (last_sampled != npos) {
        last_sampled = find_next(last_sampled);
    }
    last_sampled = find_first();
    return last_sampled;
}

template<typename T>
size_t LazyBitmap<T>::random_sample()
{
    uint64_t idx = rng_distrib(rng_generator);
    return find_next(idx);
}

template<>
inline size_t LazyBitmap<SwiftMap::bitmap>::_find_first(size_t i)
{
    SwiftMap::bitmap* b = eval_bitmap(i);
    if (b == NULL) {
        return npos;
    } else {
        return b->find_first();
    }
}

template<>
inline size_t LazyBitmap<SwiftMap::sbitmap>::_find_first(size_t i)
{
    SwiftMap::sbitmap* b = eval_bitmap(i);
    if (b == NULL) {
        return npos;
    } else {
        return b->_Find_first();
    }
}

template<typename T>
inline size_t LazyBitmap<T>::find_first()
{
    for (size_t i = 0; i < v.size(); i++) {
        size_t pos = _find_first(i);
        if (pos != npos) {
            return i * SPLIT_SIZE + pos;
        }
    }
    return npos;
}

template<>
inline size_t LazyBitmap<SwiftMap::bitmap>::_find_next(size_t i, size_t pos)
{
    return eval_bitmap(i)->find_next(pos);
}

template<>
inline size_t LazyBitmap<SwiftMap::sbitmap>::_find_next(size_t i, size_t pos)
{
    return eval_bitmap(i)->_Find_next(pos);
}

template<typename T>
inline size_t LazyBitmap<T>::find_next(size_t pos)
{
    size_t bitmap_idx = pos / SPLIT_SIZE;
    size_t new_pos = _find_next(bitmap_idx, pos % SPLIT_SIZE);
    if (new_pos != npos) {
        return bitmap_idx * SPLIT_SIZE + new_pos;
    }

    for (size_t i = bitmap_idx+1; i < v.size(); i++) {
        new_pos = _find_first(i);
        if (new_pos != npos) {
            return i * SPLIT_SIZE + new_pos;
        }
    }

    return npos;
}

template<typename T>
void LazyBitmap<T>::set(uint64_t row)
{
    uint64_t bmap_idx = row / SPLIT_SIZE;
    if (v[bmap_idx] == NULL) {
        v[bmap_idx] = new_bitmap(set_default);
    }
    (*v[row / SPLIT_SIZE])[row % SPLIT_SIZE] = true;
}

template<typename T>
void LazyBitmap<T>::and_combine(SwiftMap* m)
{
    and_maps.push_back(static_cast<LazyBitmap*>(m));
}

template<typename T>
void LazyBitmap<T>::or_combine(SwiftMap* m)
{
    or_maps.push_back(static_cast<LazyBitmap*>(m));
}

template<typename T>
void LazyBitmap<T>::read(std::istream& in)
{
    /* TODO - This might be faster for dynamic_bitset; double check
    uint64_t num_blocks;
    in >> num_blocks;
    in.get();
    uint64_t num_splits;
    in >> num_splits;
    in.get();
    for (uint64_t i = 0; i < num_splits; i++) {
        vector<block_type_t> w(num_blocks);
        in.read((char*) &w[0], num_blocks * sizeof(block_type_t));
        boost::from_block_range(w.begin(), w.end(), *v[i]);
    }
    */
    uint64_t num_splits;
    in >> num_splits;
    in.get();
    for (uint64_t i = 0; i < num_splits; i++) {
        bool valid;
        in >> valid;
        if (valid) {
            v[i] = new_bitmap(set_default);
            in >> *v[i];
        } else {
            v[i] = NULL;
        }

    }
}

template<typename T>
void LazyBitmap<T>::write(std::ostream& out)
{
    /* TODO - This might be faster for dynamic_bitset; double check
    out << v[0]->num_blocks() << ' ' << v.size() << ' ';
    for (size_t i = 0; i < v.size(); i++) {
        vector<block_type_t> w;
        boost::to_block_range(*v[i], back_inserter(w));
        out.write((const char*) &w[0], v[i]->num_blocks() * sizeof(block_type_t));
    }
    */
    out << v.size() << ' ';
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i] == NULL) {
            out << false;
        } else {
            out << true;
            out << *v[i];
        }
    }
}

template<typename T>
bool LazyBitmap<T>::get(size_t pos)
{
    return (*eval_bitmap(pos / SPLIT_SIZE))[pos % SPLIT_SIZE];
}

template<typename T>
T* LazyBitmap<T>::eval_bitmap(size_t bitmap_idx)
{
    if (v[bitmap_idx] != NULL) {
        return v[bitmap_idx];
    }

    if (and_maps.size() == 0 && or_maps.size() == 0) {
        return NULL;
    }

    T* b = new_bitmap(set_default);

    for (size_t i = 0; i < and_maps.size(); i++) {
        // TODO - Go from most selective first and do a AND checks in the
        // middle to make sure we don't need to further to do more ANDing
        *b &= *and_maps[i]->eval_bitmap(bitmap_idx);
    }

    for (size_t i = 0; i < or_maps.size(); i++) {
        *b |= *or_maps[i]->eval_bitmap(bitmap_idx);
    }

    v[bitmap_idx] = b;
    return b;
}

template<typename T>
T* PoolLazyBitmap<T>::new_bitmap(bool set)
{
    if (pool.empty()) {
        fill_pool();
    }

    T* b = pool.front();
    pool.pop_front();
    // TODO - We can optimize this flag by saying whther b is dirty from last
    // use
    if (set) {
        b->set();
    } else {
        b->reset();
    }
    return b;
}

template<typename T>
void PoolLazyBitmap<T>::delete_bitmap(T* b)
{
    pool.push_front(b);
}

template<typename T>
std::list<T*> PoolLazyBitmap<T>::pool;


#endif /* _SWIFT_MAP_H_ */

