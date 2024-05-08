#ifndef OPTIONS_H
# define OPTIONS_H

#include <stddef.h>
#include <stdbool.h>
#include <sys/socket.h>

typedef struct options_s {
    ssize_t content_line_limit;
    bool addr_colors;
    bool only_from_src;
    struct sockaddr_storage src_addr;
    bool only_from_dst;
    struct sockaddr_storage dst_addr;
} options_t;

options_t parse_options(int argc, char* argv[]);

#endif