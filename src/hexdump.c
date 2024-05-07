#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>

#include "hexdump.h"

bool is_null_line(void* line_start, size_t length_left) {
    for (size_t i = 0; i < length_left && i < DUMP_WIDTH; i++) {
        uint8_t byte = ((uint8_t*)line_start)[i];
        if (byte != 0)
            return (false);
    }
    return (true);
}

void dump_line(void* line_start, size_t length_left) {
    size_t i;
    for (i = 0; i < DUMP_WIDTH && i < length_left; i++) {
        uint8_t byte = ((uint8_t*)line_start)[i];
        printf("%02x", byte);
        if (i & 1) printf(" ");
    }
    for (; i < DUMP_WIDTH; i++) {
        printf("  ");
        if (i & 1) printf(" ");
    }
    printf(" ");
    for (i = 0; i < DUMP_WIDTH && i < length_left; i++) {
        uint8_t byte = ((uint8_t*)line_start)[i];
        if (isprint(byte) && byte)
            printf("%c", byte);
        else
            printf(".");
    }
}

void hexdump(void* start, size_t length, size_t max_rows) {
    bool last_was_null_line = false;
    size_t row_count = 0;
    for (size_t i = 0; i < length && row_count < max_rows; i += DUMP_WIDTH) {
        if (is_null_line((uint8_t*)start + i, length - i)) {
            if (!last_was_null_line)
            {
                printf("*\n");
                last_was_null_line = true;
                row_count += 1;
            }
            continue;
        }
        printf("%#016lx ", i);
        dump_line((uint8_t*)start + i, length - i);
        printf("\n");
        last_was_null_line = false;
        row_count += 1;
    }
}