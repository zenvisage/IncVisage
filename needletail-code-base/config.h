#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <cstdint>

#define DEBUG (1)

#define TIMER (1)

#define DIRECT_IO (1)

#ifndef IO_TIMER
#define IO_TIMER (1)
#endif

#ifndef MEMORY_TIMER
#define MEMORY_TIMER (1)
#endif


#ifndef USE_EWAH
#define USE_EWAH (0)
#endif

#ifndef USE_PAIRING_HEAP
#define USE_PAIRING_HEAP (1)
#endif


const uint32_t NUM_THREADS = 60;

const size_t DISK_BLOCK_SIZE =512;

const size_t DISK_READ_SIZE = 1024 * 256;

const uint64_t SPLIT_SIZE = DISK_READ_SIZE;

const uint64_t DENSITY_SIZE = DISK_READ_SIZE;

const uint64_t POOL_SIZE = 10;

const uint64_t GRANULARITY = 1;

const float DELTA=1.0;

const float block_read_time = 8;

const float alloted_time = 1000;

#endif /* _CONFIG_H_ */
