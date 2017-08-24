#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include "utils.h"

uint64_t PREV_TIME, NEXT_TIME;

void _panic(const char* file, int line, const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	// Print the panic message
        printf("********************* ERROR *********************\n");
        printf("[PANIC] %s:%d: ", file, line);
	vprintf(fmt, ap);
	printf("\n");

        assert(0);
}

void _debug(const char *file, int line, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	// Print the panic message
        printf("[debug][%s:%d]: ", file, line);
	vprintf(fmt, ap);
}

void _no_debug(const char *file, int line, const char *fmt, ...)
{
}

void record(FILE *f, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);

	va_start(ap, fmt);
	vfprintf(f, fmt, ap);
	va_end(ap);
}

#if 0
uint64_t __cpu_clock_speed;

void init_clock_speed()
{
    FILE* f = fopen("/proc/cpuinfo", "r");
    char buf[4096];
    size_t num_read = fread(buf, 1, sizeof(buf), f);
    buf[num_read - 1] = '\0';
    char* match = strstr(buf, "cpu MHz");
    assert(match);
    float cpu_speed_mhz;
    sscanf(match, "cpu MHz : %f", &cpu_speed_mhz);
    __cpu_clock_speed = 1000 * (uint64_t) (1000 * cpu_speed_mhz);
}
#endif

INIT_TIMER(fread);
INIT_TIMER(fseeko);
INIT_TIMER(mem_time);
