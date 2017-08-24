#include <algorithm>
#include <cassert>
#include <sstream>
#include <string>
#include <fstream>
#include <cstdio>
#include <cstdlib>

#include "config.h"
#include "sdb.h"
#include "utils.h"

using namespace std;

SDB::SDB(const char* fname, Schema& schema)
    : buf_idx(0), buf_size(DISK_READ_SIZE), schema(schema)
{
    f = direct_fopen(fname, "r");
    posix_memalign((void**) &buf, DISK_BLOCK_SIZE, buf_size);
//    assert(fread(buf, 1, buf_size, f) > 0);
//    buf_idx = -1;
    clear();
}

SDB::~SDB()
{
    free(buf);
    fclose(f);
}

void SDB::clear()
{
//	 assert(fread(buf, 1, buf_size, f) > 0);
	 buf_idx = -2;
	 block_fetch_count=0;
//	 sync();
//	 std::ofstream ofs("/proc/sys/vm/drop_caches");
//	 ofs << "3" << std::endl;-
    	 system("sudo sync && echo 3 | sudo tee /proc/sys/vm/drop_caches");
}


const uint8_t* SDB::get_record(uint64_t record_idx)
{
    // TODO - Error check to make sure record_idx is not out of bounds

    uint64_t record_size = schema.get_record_size();
    uint64_t records_per_buf = buf_size / record_size;
    uint64_t new_idx = record_idx / records_per_buf;

   if (new_idx != buf_idx) {
        if (new_idx != (buf_idx + 1)) {
	    
            timed_fseeko(f, new_idx * buf_size, SEEK_SET);
        }
        assert(timed_fread(buf, 1, buf_size, f) > 0);
        buf_idx = new_idx;
	block_fetch_count+=1;
    }
    return &buf[(record_idx % records_per_buf) * record_size];
}






#if 0
SDB::SDB(const char* fname)
    : fname(fname), attrs(), ordered_attrs(), record_size(0), num_records(0),
    attr_offsets(), file_map(), buf_map(), idx_map(), tmp_map(), single_file(),
    single_buf(), single_idx(0), single_tmp(), buf_size(DISK_READ_SIZE)
{
}

SDB::~SDB()
{
    for (boost::unordered_map<uint32_t,FILE*>::iterator it = file_map.begin();
            it != file_map.end();
            it++) {
        if (it->second) {
            fclose(it->second);
        }
    }

    for (boost::unordered_map<uint32_t,uint8_t*>::iterator it = buf_map.begin();
            it != buf_map.end();
            it++) {
        if (it->second) {
            free(it->second);
        }
    }

    fclose(single_file);
    free(single_buf);
    free(single_tmp);
}

void SDB::init(const vector<Attr>& attrs)
{
    for (size_t i = 0; i < attrs.size(); i++) {
        ordered_attrs.push_back(attrs[i]);
        this->attrs[attrs[i].get_name()] = ordered_attrs.back();
    }

    FILE* sdb_f = fopen(fname, "r");
    fseeko(sdb_f, 0, SEEK_END);
    record_size = 0;
    for (unsigned int i = 0; i < attrs.size(); i++) {
        attr_offsets[attrs[i].get_name()] = record_size;
        record_size += attrs[i].get_type()->get_size();
    }
    num_records = ftello(sdb_f) / record_size;
    fclose(sdb_f);

    init_read();
}

void SDB::init(const vector<Attr>& attrs, CSV& csv)
{
    for (size_t i = 0; i < attrs.size(); i++) {
        ordered_attrs.push_back(attrs[i]);
        this->attrs[attrs[i].get_name()] = ordered_attrs.back();
    }

    FILE* sdb_f = direct_fopen(fname, "w");
    ifstream csv_f(csv.get_fname(), ios::in);
    string line;
    string token;
    stringstream ss;
    unsigned int i;
    uint8_t buf[MAX_TYPE_SIZE];

    // For header line
    //getline(csv_f, line);

    num_records = 0;
    while (getline(csv_f, line)) {
        ss.str(line);
        i = 0;
        while (getline(ss, token, ',')) {
            memset(buf, 0, MAX_TYPE_SIZE);
            attrs[i].get_type()->parse(buf, token);
            /* TODO - Figure out what to do if there are varying sizes for
             * different attributes. */
            fwrite(buf, attrs[i].get_type()->get_size(), 1, sdb_f);
            i++;
        }
        ss.clear();
        num_records++;
    }
    csv_f.close();
    fclose(sdb_f);

    assert(num_records == csv.get_num_records());

    record_size = 0;
    for (unsigned int i = 0; i < attrs.size(); i++) {
        attr_offsets[attrs[i].get_name()] = record_size;
        record_size += attrs[i].get_type()->get_size();
    }

    init_read();
}

void SDB::init_read()
{
    for (uint32_t client_id = 0; client_id < NUM_THREADS; client_id++) {
        file_map[client_id] = direct_fopen(fname, "r");
        /* TODO - Experiment with bigger buffer sizes */
        posix_memalign((void**) &buf_map[client_id], DISK_BLOCK_SIZE, buf_size);
        //buf_map[client_id] = (uint8_t*) malloc(buf_size);
        assert(fread(buf_map[client_id], 1, buf_size, file_map[client_id]) > 0);
        idx_map[client_id] = 0;
    }

    single_file = direct_fopen(fname, "r");
    posix_memalign((void**) &single_buf, DISK_BLOCK_SIZE, buf_size);
    assert(fread(single_buf, 1, buf_size, single_file) > 0);
    single_idx = 0;
    single_tmp = (uint8_t*) malloc(record_size);
}

/* TODO - Try experimenting with SEEK_SET vs SEEK_CUR */
/* TODO - Try experimenting with fopen/fseek vs mmap */
const uint8_t* SDB::get_record(uint64_t record_idx, uint32_t client_id)
{
    if (client_id == NUM_THREADS) {
        return __get_record(record_idx, single_file, single_buf, single_tmp,
                single_idx);
    } else {
        return __get_record(record_idx, file_map[client_id], buf_map[client_id],
                tmp_map[client_id], idx_map[client_id]);
    }
}

inline const uint8_t* SDB::__get_record(uint64_t record_idx, FILE* file, uint8_t* buf,
        uint8_t* tmp, uint64_t& idx)
{
    uint64_t record_begin = record_idx * record_size % buf_size;
    uint64_t new_idx = record_idx * record_size / buf_size;

    if (new_idx != idx) {
        if (new_idx != (idx + 1)) {
            timed_fseeko(file, new_idx * buf_size, SEEK_SET);
        }
        assert(timed_fread(buf, 1, buf_size, file) > 0);
        idx = new_idx;

        if (record_begin + record_size > buf_size) {
            uint64_t extra_offset = buf_size - record_begin;
            memcpy(tmp, &buf[record_begin], extra_offset);
            assert(timed_fread(buf, 1, buf_size, file) > 0);
            memcpy(&tmp[extra_offset], buf, record_size - extra_offset);
            idx = new_idx;
            return tmp;
        }
    }
    return &buf[record_begin];
}
#endif
