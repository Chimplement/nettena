#ifndef OPTIONS_H
# define OPTIONS_H

#include <stddef.h>
#include <stdbool.h>
#include <netinet/in.h>

typedef struct options_s {
    size_t content_line_limit;
    bool addr_colors;
    bool show_all;
    sockaddr_storage src_addr;
    sockaddr_storage dst_addr;
} options_t;

#endif