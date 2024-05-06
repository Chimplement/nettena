#include <stdint.h>
#include <ctype.h>
#include <stdio.h>

#include "hexdump.h"

#define DUMP_WIDTH 16

void hexdump(void* start, size_t length) {
    for (size_t i = 0; i < length;) {
        printf("%#016lx ", i);
        size_t j;
        for (j = 0; j < DUMP_WIDTH && i < length; j++) {
            uint8_t byte = ((uint8_t*)start)[i];
            printf("%02x", byte);
            if (i & 1) printf(" ");
            i++;
        }
        for (; j < DUMP_WIDTH; j++) {
            printf("  ");
            if (i & 1) printf(" ");
            i++;
        }
        printf(" ");
        i -= DUMP_WIDTH;
        for (j = 0; j < DUMP_WIDTH && i < length; j++) {
            uint8_t byte = ((uint8_t*)start)[i];
            if (isprint(byte) && byte)
                printf("%c", byte);
            else
                printf(".");
            i++;
        }
        printf("\n");
    }
}