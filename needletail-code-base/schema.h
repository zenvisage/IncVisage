#ifndef _SCHEMA_H_
#define _SCHEMA_H_

#include <vector>

#include <boost/unordered_map.hpp>

#include "db_utils.h"

class Schema {
public:
    Schema(const char* fname, uint64_t num_rows = 0);

    const std::vector<Attr>& get_attrs_v() const { return attrs_v; }
    const boost::unordered_map<const std::string, Attr>& get_attrs_m() const
    { return attrs_m; }
    uint64_t get_record_size() const { return record_size; }
    uint64_t get_num_rows() const { return num_records; }
    uint64_t get_num_pages() const { return num_pages; }
protected:
    std::vector<Attr> attrs_v;
    boost::unordered_map<const std::string, Attr> attrs_m;
    uint64_t record_size;
    uint64_t num_records;
    uint64_t records_per_page;
    uint64_t num_pages;
};

#endif /* _SCHEMA_H_ */
