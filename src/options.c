#define _DEFAULT_SOURCE
#include <arpa/inet.h>
#include <stdlib.h>
#include <getopt.h>

#include "options.h"

options_t parse_options(int argc, char* argv[]) {
    options_t options = {
        .content_line_limit = -1,
        .addr_colors = true,
        .only_from_src = false,
        .only_from_dst = false,
    };
    char c;

    while ((c = getopt(argc, argv, "n:hcs:d:?")) != -1) {
        switch (c)
        {
            case 'n':
                // check if int
                options.content_line_limit = atoi(optarg);
                break;
            case 'h':
                options.content_line_limit = 0;
                break;
            case 'c':
                options.addr_colors = false;
                break;
            case 's':
                options.only_from_src = true;
                if (inet_pton(AF_INET, optarg, &options.src_addr) == 1) {
                    options.src_family = AF_INET;
                } else {
                    if (inet_pton(AF_INET6, optarg, &options.src_addr) == 1) {
                        options.src_family = AF_INET6;
                    } else {
                        //handle error
                    }
                }
                break;
            case 'd':
                options.only_from_dst = true;
                if (inet_pton(AF_INET, optarg, &options.dst_addr) == 1) {
                    options.dst_family = AF_INET;
                } else {
                    if (inet_pton(AF_INET6, optarg, &options.dst_addr) == 1) {
                        options.dst_family = AF_INET6;
                    } else {
                        //handle error
                    }
                }
                break;
            case '?':
                // print help
                break;
            default:
                break;
        }
    }
    return (options);
}