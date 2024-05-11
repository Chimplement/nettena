#define _DEFAULT_SOURCE
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <getopt.h>

#include "ansi.h"

#include "options.h"

void exit_error(char* error)
{
	fprintf(stderr, BOLD RED"Error:"RESET_BOLD WHITE" %s\n", error);
	exit(1);
}

void help(char* program_name)
{
    printf("%s ["UNDERLINE"OPTIONS"RESET_UNDERLINE"...]\n", program_name);
    printf("\tListen to all the network traffic arriving at this computer and display their content.\n");
    printf("\n");
    printf(BOLD"OPTIONS"RESET_BOLD"\n");
    printf("-n "UNDERLINE"LINE_LIMIT"RESET_UNDERLINE"\n");
    printf("\tLimit the maximum amount of lines the packet hexdump will show to "UNDERLINE"LINE_LIMIT"RESET_UNDERLINE".\n");
    printf("\n");
    printf("-h\n");
    printf("\tHide the packet hexdump, same as `-n 0`.\n");
    printf("\n");
    printf("-c\n");
    printf("\tDisable address colors.\n");
    printf("\n");
    printf("-s "UNDERLINE"SOURCE_ADDRESS"RESET_UNDERLINE"\n");
    printf("\tOnly display packets coming from "UNDERLINE"SOURCE_ADDRESS"RESET_UNDERLINE".\n");
    printf("\tWhen combined with `-d` only the source or destination address has to match.\n");
    printf("\n");
    printf("-d "UNDERLINE"DESTINATION_ADDRESS"RESET_UNDERLINE"\n");
    printf("\tOnly display packets coming from "UNDERLINE"DESTINATION_ADDRESS"RESET_UNDERLINE".\n");
    printf("\tWhen combined with `-s` only the source or destination address has to match.\n");
    printf("\n");
    printf("-?\n");
    printf("\tShows this menu.\n");
    printf("\n");
}

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
                for (size_t i = 0; optarg[i] != '\0'; i++) {
                    if (!isdigit(optarg[i])) {
                        exit_error("Invalid hexdump line limit");
                    }
                }
                options.content_line_limit = atol(optarg);
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
                        exit_error("Invalid source address");
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
                        exit_error("Invalid destination address");
                    }
                }
                break;
            case '?':
                help(argv[0]);
                exit(0);
                break;
            default:
                break;
        }
    }
    return (options);
}