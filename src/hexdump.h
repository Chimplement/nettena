#ifndef HEXDUMP_H
# define HEXDUMP_H

# include <stddef.h>

#define DUMP_WIDTH 16

void hexdump(void* start, size_t length, size_t max_rows);

#endif