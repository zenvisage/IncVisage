#ifndef _CSV_READER_H_
#define _CSV_READER_H_

#include <vector>
#include <string>
#include <cinttypes>

#include "db_utils.h"

/* TODO: Figure out how much we're going to expand on this interface. */
class CSV {
public:
    CSV(const char* fname);

    uint64_t get_num_records() { return num_records; }
    const char* get_fname() { return fname; }

protected:
    const char* fname;
    uint64_t num_records;
};

#endif /* _CSV_READER_H_ */
