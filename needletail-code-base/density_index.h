#ifndef _DENSITY_INDEX_H_
#define _DENSITY_INDEX_H_

#include <boost/unordered_map.hpp>
#include <boost/heap/priority_queue.hpp>

#include "config.h"
#include "db_utils.h"
#include "sdb.h"
#include "swift_index.h"

struct Density {
    uint64_t idx;
    uint32_t count;

    Density(uint64_t idx, uint32_t count) : idx(idx), count(count) { }
};

struct DensityCompare {
    bool operator()(const Density& lhs, const Density& rhs) const
    {
        return lhs.idx < rhs.idx;
    }
};

class DensityIndex {
public:
    DensityIndex(SDB& sdb, const char* fname = NULL);
    /* TODO - Try experimenting around with other priorirty queue types.
     * Espeically, fibonacci heap and pairing heap. */
    typedef boost::heap::priority_queue<Density, boost::heap::compare<DensityCompare> > dmap;
    typedef boost::unordered_map<const uint8_t*,dmap*, attr_val_hasher, attr_val_equal> l2map;
    typedef boost::unordered_map<const std::string,l2map> l1map;
    typedef boost::unordered_map<const std::string,std::vector<uint8_t> > l2bufmap;

    typedef boost::unordered_map<const std::string,uint8_t*> constraintmap;

protected:
    void read();

    SDB& sdb;
    l1map index;
    l2bufmap l2buf;
};

#endif /* _DENSITY_INDEX_H_ */
