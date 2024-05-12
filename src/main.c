#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "ansi.h"
#include "options.h"
#include "hexdump.h"

#define PACKET_LEN 8192

int get_ip_version(void* packet) {
    return ((*((uint8_t*) packet) & 0xF0) >> 4);
}

void log_ip4_packet(void* packet, options_t options) {
    struct iphdr* ip4_hdr = (struct iphdr*)packet;

    if (options.only_from_src || options.only_from_dst) {
        bool has_correct_src = (
            options.only_from_src &&
            options.src_family == AF_INET &&
            *(uint32_t*)&options.src_addr == ip4_hdr->saddr
        );
        bool has_correct_dst = (
            options.only_from_dst &&
            options.dst_family == AF_INET &&
            *(uint32_t*)&options.dst_addr == ip4_hdr->daddr
        );
        if (!has_correct_src && !has_correct_dst) {
            return ;
        }
    }

    char src_address_str[INET_ADDRSTRLEN];
    char dst_address_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ip4_hdr->saddr, src_address_str, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &ip4_hdr->daddr, dst_address_str, INET_ADDRSTRLEN);

    printf("src: \x1b[38;5;%im%-15s"DEFAULT_COLOR" dst: \x1b[38;5;%im%-15s"DEFAULT_COLOR"\n",
        htonl(ip4_hdr->saddr) & 0xFF,
        src_address_str,
        htonl(ip4_hdr->daddr) & 0xFF,
        dst_address_str
    );
    if (options.content_line_limit == -1)
        hexdump(packet, PACKET_LEN, PACKET_LEN / DUMP_WIDTH);
    else
        hexdump(packet, PACKET_LEN, options.content_line_limit);
}

void log_ip6_packet(void* packet, options_t options) {
    struct ip6_hdr* ip6_hdr = (struct ip6_hdr*)packet;
    
    if (options.only_from_src || options.only_from_dst) {
        bool has_correct_src = (
            options.only_from_src &&
            options.src_family == AF_INET6 &&
            !memcmp(&options.dst_addr, &ip6_hdr->ip6_dst, sizeof(struct in6_addr))
        );
        bool has_correct_dst = (
            options.only_from_dst &&
            options.dst_family == AF_INET6 &&
            !memcmp(&options.dst_addr, &ip6_hdr->ip6_dst, sizeof(struct in6_addr))
        );
        if (!has_correct_src && !has_correct_dst) {
            return ;
        }
    }

    char src_address_str[INET6_ADDRSTRLEN];
    char dst_address_str[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &ip6_hdr->ip6_src, src_address_str, INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, &ip6_hdr->ip6_dst, dst_address_str, INET6_ADDRSTRLEN);

    printf("src: \x1b[38;5;%im%-39s"DEFAULT_COLOR": \x1b[38;5;%im%-39s"DEFAULT_COLOR"\n",
        ip6_hdr->ip6_src.__in6_u.__u6_addr8[15],
        src_address_str,
        ip6_hdr->ip6_dst.__in6_u.__u6_addr8[15],
        dst_address_str
    );
    if (options.content_line_limit == -1)
        hexdump(packet, PACKET_LEN, PACKET_LEN / DUMP_WIDTH);
    else
        hexdump(packet, PACKET_LEN, options.content_line_limit);
}

int main(int argc, char* argv[]) {
    options_t options = parse_options(argc, argv);

    int sock = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ALL));
	if (sock == -1)
	{
		exit_error(strerror(errno));
	}

    ssize_t bytes_received;
    uint8_t packet[PACKET_LEN];
    
    while (1)
    {
        memset(packet, 0, PACKET_LEN);
        bytes_received = recv(sock, packet, PACKET_LEN, 0);
        if (bytes_received == -1) {
            break;
        }
        
        switch (get_ip_version(packet))
        {
            case 4:
                log_ip4_packet(packet, options);
                break;
            case 6:
                log_ip6_packet(packet, options);
                break;
            default:
                break;
        }
    }
    return (0);
}