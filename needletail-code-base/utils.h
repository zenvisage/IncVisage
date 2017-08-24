#ifndef _UTILS_H_
#define _UTILS_H_

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <cinttypes>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <boost/functional/hash.hpp>

#include "config.h"

#define UNIMPLEMENTED(name) \
    panic("unimplemented method: " #name)

#define panic(...) _panic(__FILE__, __LINE__, __VA_ARGS__)

void _panic(const char*, int, const char*, ...) __attribute__((noreturn));

#if DEBUG
#define debug(...) _debug(__FILE__, __LINE__, __VA_ARGS__)
#else
#define debug(...) _no_debug(__FILE__, __LINE__, __VA_ARGS__)
#endif

void _debug(const char *file, int line, const char *fmt, ...);
void _no_debug(const char *file, int line, const char *fmt, ...);

void record(FILE *f, const char *fmt, ...);

#if 0
static inline uint64_t read_tscp(void)
{
    uint64_t tsc;
    asm volatile("rdtscp" : "=A" (tsc));
    return tsc;
}

static inline uint64_t read_tsc(void)
{
    uint64_t tsc;
    asm volatile("rdtsc" : "=A" (tsc));
    return tsc;
}

static inline uint64_t read_tsc_serialized(void)
{
    asm volatile("mfence");
    return read_tsc();
}


extern uint64_t __cpu_clock_speed;

/* NOTE: Must call this if you want to use get_current_time */
void init_clock_speed();

/* Returns current time in microseconds. */
static inline uint64_t get_current_time()
{
    return read_tsc_serialized()  / (__cpu_clock_speed / 1000 / 1000);
}
#endif

/* Returns the current time in us */
static inline uint64_t get_current_time()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return 1000000 * tv.tv_sec + tv.tv_usec;
}

extern uint64_t PREV_TIME, NEXT_TIME;

#if TIMER
#define EXTERN_TIMER(timer) extern int64_t __TIMER_ ## timer
#define INIT_TIMER(timer) int64_t __TIMER_ ## timer = 0
#if DEBUG
//#define START_TIMER(timer) do { debug("Starting timer: " #timer "\n"); __TIMER_ ## timer -= get_current_time(); } while (0)
#define START_TIMER(timer) do { __TIMER_ ## timer -= get_current_time(); } while (0)
#else /* DEBUG */
#define START_TIMER(timer) __TIMER_ ## timer -= get_current_time()
#endif /* DEBUG */
#if DEBUG
//#define END_TIMER(timer) do { __TIMER_ ## timer += get_current_time(); debug("Ending timer: " #timer "\n"); } while (0)
#define END_TIMER(timer) do { __TIMER_ ## timer += get_current_time();} while (0)
#else /* DEBUG */
#define END_TIMER(timer) __TIMER_ ## timer += get_current_time()
#endif /* DEBUG */
#define RESET_TIMER(timer) __TIMER_ ## timer = 0
#define GET_TIMER(timer) (__TIMER_ ## timer)
#else
#define EXTERN_TIMER(timer)
#define INIT_TIMER(timer)
#define START_TIMER(timer)
#define END_TIMER(timer)
#define RESET_TIMER(timer)
#define GET_TIMER(timer) ((int64_t) -1)
#endif

// UPDIV := ceil(a / b)
#define UPDIV(a, b) ((a) / (b) + ((a) % (b) ? 1: 0))

static inline int string2int(const std::string& s)
{
    int i;
    std::stringstream ss(s);
    ss >> i;
    return i;
}

static inline float string2float(const std::string& s)
{
    float i;
    std::stringstream ss(s);
    ss >> i;
    return i;
}

static inline double string2double(const std::string& s)
{
    double i;
    std::stringstream ss(s);
    ss >> i;
    return i;
}

static inline uint64_t string2uint64_t(const std::string& s)
{
    uint64_t i;
    std::stringstream ss(s);
    ss >> i;
    return i;
}

template <typename Container>
struct  container_hash {
    std::size_t operator()(Container const& c) const {
        return boost::hash_range(c.begin(), c.end());
    }
};

static inline FILE* direct_fopen(const char* path, const char* mode)
{
#if DIRECT_IO
    assert(strlen(mode) == 1 && (mode[0] == 'r' || mode[0] == 'w'));
    int flags = mode[0] == 'r' ? O_RDONLY : O_WRONLY;
    int fd = open(path, flags | O_DIRECT | O_SYNC);
    assert(fd >= 0);
    return fdopen(fd, mode);
#else
    return fopen(path, mode);
#endif
}

EXTERN_TIMER(fread);
EXTERN_TIMER(fseeko);

EXTERN_TIMER(mem_time);




static inline void clear_mem_time()
{
    RESET_TIMER(mem_time);
}

static inline uint64_t get_mem_timer()
{
    return GET_TIMER(mem_time);
}

static inline void start_mem_time()
{
#if MEMORY_TIMER
     START_TIMER(mem_time);
#endif
}

static inline void end_mem_time()
{
#if MEMORY_TIMER
     END_TIMER(mem_time);
#endif
}







const uint64_t IO_TIMER_RATE = 1;

static inline void clear_io_timer()
{
    RESET_TIMER(fread);
    RESET_TIMER(fseeko);
}

static inline uint64_t get_io_timer()
{
    return (GET_TIMER(fseeko) + GET_TIMER(fread)) * IO_TIMER_RATE;
}

static inline size_t timed_fread(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
end_mem_time();
#if IO_TIMER
    static uint32_t io_counter = 0;
    if (io_counter % IO_TIMER_RATE == 0) {
        START_TIMER(fread);
    }
#endif
    size_t ret = fread(ptr, size, nmemb, stream);

#if IO_TIMER
    if (io_counter % IO_TIMER_RATE == 0) {
        END_TIMER(fread);
        io_counter = 1;
    } else {
        io_counter++;
    }
#endif
  start_mem_time();
    return ret;
}

static inline int timed_fseeko(FILE* stream, off_t offset, int whence)
{
	end_mem_time();
#if IO_TIMER
    static uint32_t io_counter = 0;
    if (io_counter % IO_TIMER_RATE == 0) {
        START_TIMER(fseeko);
    }
#endif
    size_t ret = fseeko(stream, offset, whence);
#if IO_TIMER
    if (io_counter % IO_TIMER_RATE == 0) {
        END_TIMER(fseeko);
        io_counter = 1;
    } else {
        io_counter++;
    }
#endif
    start_mem_time();
    return ret;
}


static inline void clear_all_time()
{
	clear_mem_time();
	clear_io_timer();

}

#endif /* _UTILS_H_ */

