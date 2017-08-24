#ifndef _SWIFT_INDEX_H_
#define _SWIFT_INDEX_H_

#include <cstring>
#include <string>
#include <vector>
#include <cstdio>
#include <utility>
#include <pthread.h>

#include <boost/unordered_map.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/functional/hash.hpp>

#include "binner.h"
#include "config.h"
#include "db_utils.h"
#include "query.h"
#include "schema.h"
#include "sdb.h"
#include "swift_map.h"

/*#if USE_EWAH
#include <ewah.h>
#endif
*/
/* FIXME - Consider using the Type object's equal_to method instead of this.
 * May be slower. Have to double check. */
struct attr_val_hasher {
    attr_val_hasher(type_constant_t type_constant)
        : type_constant(type_constant), int_hasher(), float_hasher(),
        string_hasher()
    { }

    attr_val_hasher()
    { UNIMPLEMENTED("Don't call this ever"); }

    std::size_t operator()(const uint8_t* attr_val) const
    {
        if (type_constant == INT_TYPE) {
            return int_hasher(*(int*) attr_val);
        } else if (type_constant == FLOAT_TYPE) {
            return float_hasher(*(float*) attr_val);
        } else if (type_constant == STRING_TYPE) {
            /* FIXME - This will be slow. Make my own hasher for char* */
            return string_hasher(std::string((char*) attr_val));
        } else {
            panic("No known hash for given type.");
        }
    }
    
    type_constant_t type_constant;
    boost::hash<int> int_hasher;
    boost::hash<float> float_hasher;
    boost::hash<std::string> string_hasher;
};

/* FIXME - Consider using the Type object's equal_to method instead of this.
 * May be slower. Have to double check. */
struct attr_val_equal {
    attr_val_equal(type_constant_t type_constant)
        : type_constant(type_constant)
    { }

    attr_val_equal()
    { UNIMPLEMENTED("Don't call this ever"); }

    bool operator()(const uint8_t* x, const uint8_t* y) const
    {
        if (type_constant == INT_TYPE) {
            return *(int*) x == *(int*) y;
        } else if (type_constant == FLOAT_TYPE) {
            return *(float*) x == *(float*) y;
        } else if (type_constant == STRING_TYPE) {
            return !strcmp((const char*) x, (const char*) y);
        } else {
            panic("Unsupported type for equal.");
        }
    }

    type_constant_t type_constant;
};

class SwiftIndex {
public:

    /* TODO - We should experiment with different map implementations */
#if 0
#if USE_EWAH
    typedef uint32_t bitmap_block_type_t;
    typedef EWAHBoolArray<bitmap_block_type_t> bitmap;
#else
    typedef unsigned long bitmap_block_type_t;
    typedef boost::dynamic_bitset<bitmap_block_type_t> bitmap;
#endif
#endif
    //typedef boost::unordered_map<const uint8_t*,bitmap*, attr_val_hasher, attr_val_equal> l2map; // 2nd level bitmap for the Swift Index
    //typedef boost::unordered_map<Binner::key_t,bitmap*> l2map; // 2nd level bitmap for the Swift Index
    typedef boost::unordered_map<Binner::key_t,SwiftMap*> l2map; // 2nd level bitmap for the Swift Index
    typedef boost::unordered_map<const std::string,l2map> l1map; // 1st level bitmap for the Swift Index

    typedef boost::unordered_map<const std::string,pthread_mutex_t> lockmap;


    /* FIXME - Assumes int attr vals. */
    typedef boost::unordered_map<const std::string,uint8_t*> constraintmap;
    typedef std::vector<std::pair<std::string,uint8_t*> > constraintlist;

    SwiftIndex(SDB& sdb, Schema& schema, Binner& binner,
            SwiftMapFactory::map_type_t bmap_type, const char* fname = NULL);
    /* FIXME - Have a destructor */

    ~SwiftIndex();

    void save(const char* fname);

    /* FIXME - Here we assume that queries mean take the average of the targets. */
    void run_query(const Query& query, ResultSet& results,
            SwiftMap::sample_type_t sample_type);

    uint64_t run_query(const Query& query, ResultSet& results,
            SwiftMap::sample_type_t sample_type, uint64_t record_id,bool fistIter);

    uint64_t run_query(const Query& query, ResultSet& results,
            SwiftMap::sample_type_t sample_type, uint64_t record_id,bool fistIter,int dimX);
    uint64_t run_query(const Query& query, ResultSet& results,
            SwiftMap::sample_type_t sample_type, uint64_t record_id,bool fistIter,int dimX,int dimY);

    void clear();

    bool isRecordEmpty;
    bool isTrend;

    std::map<std::string,std::map<uint64_t,uint64_t>> perGroupValCount; 
protected:

    void init();
    void init(const char* fname);

    /* This function is for when the query is more complex than a single
     * singleton. It takes care of performing the AND/OR combinations in the
     * SwiftIndex on to the given bitmap 'b'. The 'parent' is passed in to see
     * if the AND or OR operator should be used to combine. */
    /* TODO - Allow this function to signal back whether the combined bitmap
     * has any valid values in it so that it can return early if there is an
     * empty bitmap. */
    // NOTE - Only call this on non-singleton nodes. All updates will be
    // performed on the passed-in base SwiftMap.
    void combine_maps(const QueryNode& node, SwiftMap* base);
    //void combine_maps(const QueryNode& node, const QueryNode& parent, SwiftMap* m);

    /* Get the mean of the results for the given 'query' on the constrained
     * bitmap 'b' and put the results in 'results'. */
    /* TODO - Rename this to ordered_sample_mean. */
    void sample_mean(SwiftMap* m, const Query& query, ResultSet& results,
            SwiftMap::sample_type_t sample_type);

    uint64_t sample_mean(SwiftMap* m, const Query& query, ResultSet&
        results, SwiftMap::sample_type_t sample_type,uint64_t record_id,bool fistIter);

    uint64_t sample_mean(SwiftMap* m, const Query& query, ResultSet&
        results, SwiftMap::sample_type_t sample_type,uint64_t record_id,bool fistIter,int filler);

    bool verify_record(const QueryNode& node, const uint8_t* record);

    void approx_sample_mean(SwiftMap* m, const Query& query, ResultSet&
            results, SwiftMap::sample_type_t sample_type);

    void full_scan(SwiftMap* m, const Query& query, ResultSet&
            results, SwiftMap::sample_type_t sample_type);


    /* Helper function for ordered_sample_mean which gets the ordered sample
     * mean on the constrained result set. */
    //double constrained_ordered_sample(const std::string& attr, bitmap&
    //        result_set, size_t& idx, uint64_t num_samples);
#if 0
    inline double constrained_ordered_sample_mean(const Type* target_type, uint64_t
            target_offset, bitmap* result_set, uint64_t num_samples);

    inline void multi_constrained_ordered_sample_mean(
            const std::vector<const Type*>& target_types,
            const std::vector<uint64_t>& target_offsets,
            std::vector<double>& means,
            bitmap* result_set, uint64_t num_samples);
#endif

    void read();


    SDB& sdb;
    Schema& schema;
    Binner& binner;
    l1map index;
    lockmap index_lock;
    boost::mt19937 rng;
    SwiftMapFactory map_factory;
   // SwiftMapFactory map_factory_base;
    std::map<std::string,SwiftMap*> mapSwiftMap;


#if 0
    /* FIXME - Following is hack for intel-sample experiment, DELETE. */
    FILE* sdb_f;
    size_t last_seen;
#endif

    ///* Record we sampled from last. NOTE - Used by sample_mean. */
    //size_t last_sampled_idx;

    ///* NOTE - This is for ordered_sample_mean */
    //size_t next_sample_idx;

    /* FIXME - Assumes attr vals are ints. */
    /* The index for ordered_sample_mean to use. */
    //boost::unordered_map<constraintmap,size_t,container_hash<constraintmap> > ordered_idx;
    //boost::unordered_map<constraintmap,uint64_t,container_hash<constraintmap> > ordered_idx;

    /* This was taken from boost::unordered::detail */
    static const size_t default_bucket_count = 11;

    /* Null position. (Returned by find_next if there is no set bit.) */
    static const size_t npos = static_cast<size_t>(-1);

};

#endif /* _SWIFT_INDEX_H_ */

