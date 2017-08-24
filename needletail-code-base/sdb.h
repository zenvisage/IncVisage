#ifndef _SDB_H_
#define _SDB_H_

#include <vector>
#include <string>
#include <cinttypes>
#include <cstdio>

#include <boost/unordered_map.hpp>

#include "csv.h"
#include "config.h"
#include "db_utils.h"
#include "schema.h"

class SDB {
public:
    SDB(const char* fname, Schema& schema);

    ~SDB();

    // Returns a pointer to the value stored in a uint8_t* buffer. The returned
    // buffer is only valid until the next call to get_val.
    inline const uint8_t* get_val(uint64_t record_idx, Attr& attr)
    {
        const uint8_t* buf = get_record(record_idx);
        return &buf[attr.get_offset()];
    }


    inline void get_vals(uint64_t record_idx, const std::vector<const Attr*>& attrs,
            std::vector<const uint8_t*>& vals)
    {
        const uint8_t* buf = get_record(record_idx);
        for (size_t i = 0; i < attrs.size(); i++) {
            vals.push_back(&buf[attrs[i]->get_offset()]);
        }
    }

    const uint8_t* get_record(uint64_t record_idx);

    const uint64_t records_per_buf() const
    {
        return buf_size / schema.get_record_size();
    }

    uint64_t block_fetch_count=0;
    void clear();

protected:
    FILE* f;
    uint8_t* buf;
    uint64_t buf_idx;   // idx of the current buffer we're at
    uint32_t buf_size;

    Schema& schema;
};

#if 0
class SDB {
public:
    typedef boost::unordered_map<const std::string,Attr> attrmap;
    /* TODO - Offsets should be part of Attr class */
    typedef boost::unordered_map<const std::string,uint64_t> offsetmap;

    SDB(const char* fname);
    ~SDB();

    /* FIXME - Replace attrs with Schema */
    void init(const std::vector<Attr>& attrs);
    void init(const std::vector<Attr>& attrs, CSV& csv);

    /* TODO: Need a better interface than just returning the fname. */
    const char* get_fname() const { return fname; }
    const attrmap& get_attrs() const { return attrs; }
    const std::vector<Attr>& get_ordered_attrs() const { return ordered_attrs; }
    uint64_t get_num_attrs() const { return attrs.size(); }
    //const offsetmap& get_attr_offsets() const { return attr_offsets; }
    uint64_t get_attr_offset(const std::string& name) const
    { return attr_offsets.at(name); }
    uint64_t get_record_size() const { return record_size; }
    uint64_t get_num_records() const { return num_records; }

    /* Returns a pointer to the value stored in a uint8_t buffer. Because of
     * possible concurrent calls to sdb, client_id can be used to identify which
     * file descriptor and buffer to use.
     * WARNING - The returned buffer is only valid until the next call of
     * get_val
     * WARNING - You cannot call get_val/get_record concurrently with same
     * client_id
     */
    inline const uint8_t* get_val(uint64_t record_idx, const std::string& attr,
            uint32_t client_id = NUM_THREADS)
    {
        return get_val(record_idx, attr_offsets[attr], client_id);
    }

    /* Same as above, except we are given the offset. This helps to optimize for
     * speed because we don't have to do the attr offset lookup. */
    inline const uint8_t* get_val(uint64_t record_idx, uint64_t offset, uint32_t
            client_id = NUM_THREADS)
    {
        const uint8_t* buf = get_record(record_idx, client_id);
        return &buf[offset];
    }

    /* TODO - Is this faster than calling get_val a bunch of times? Because
     * we need to initialize the vector and whatnot. */
    inline void get_vals(uint64_t record_idx, std::vector<const uint8_t*>&
            vals, std::vector<uint64_t>& offsets, uint32_t clientd_id =
            NUM_THREADS)
    {
        const uint8_t* buf = get_record(record_idx, clientd_id);
        for (size_t i = 0; i < offsets.size(); i++) {
            vals.push_back(&buf[offsets[i]]);
        }
    }

    /* Same as get_val except we return the entired record
     * WARNING - The returned buffer is only valid until the next call of
     * get_val
     * WARNING - You cannot call get_val/get_record concurrently with same
     * client_id
     */
    const uint8_t* get_record(uint64_t record_idx, uint32_t client_id =
            NUM_THREADS);

protected:
    /* Initializtion function to prepare for parallel reading from sdb */
    void init_read();

    inline const uint8_t* __get_record(uint64_t record_idx, FILE* file, uint8_t*
            buf, uint8_t* tmp, uint64_t& idx);

    const char* fname;

    attrmap attrs;
    std::vector<Attr> ordered_attrs;
    uint64_t record_size;
    uint64_t num_records;
    offsetmap attr_offsets;

    boost::unordered_map<uint32_t,FILE*> file_map;
    boost::unordered_map<uint32_t,uint8_t*> buf_map;
    boost::unordered_map<uint32_t,uint64_t> idx_map;        // Refers to last read buf index
    boost::unordered_map<uint32_t,uint8_t*> tmp_map;

    /* For use when client_id = NUM_THREADS, and we're not using the
     * multi-threaded interface. By not having to do a map lookup, this speeds
     * things up. */
    FILE* single_file;
    uint8_t* single_buf;
    uint64_t single_idx;
    uint8_t* single_tmp;
    uint32_t buf_size;
};
#endif

#endif /* _SDB_H_ */
