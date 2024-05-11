#ifndef OPTIONS_H
# define OPTIONS_H

#include <stddef.h>
#include <stdbool.h>
#include <sys/socket.h>

typedef struct options_s {
    ssize_t content_line_limit;
    bool addr_colors;
    bool only_from_src;
    sa_family_t src_family;
    uint8_t src_addr[sizeof(struct in6_addr)];
    bool only_from_dst;
    sa_family_t dst_family;
    uint8_t dst_addr[sizeof(struct in6_addr)];
} options_t;

options_t parse_options(int argc, char* argv[]);

#endif