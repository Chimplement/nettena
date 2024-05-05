#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>

#define PACKET_LEN 8192

int get_ip_version(void* packet) {
    return ((*((uint8_t*) packet) & 0xF0) >> 4);
}

void log_ip4_packet(void* packet) {
    struct iphdr* ip4_hdr = (struct iphdr*)packet;

    char src_address_str[INET_ADDRSTRLEN];
    char dst_address_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ip4_hdr->saddr, src_address_str, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &ip4_hdr->daddr, dst_address_str, INET_ADDRSTRLEN);

    printf("\x1b[38;5;%im%s\x1b[39m -> \x1b[38;5;%im%s\x1b[39m\n",
        htonl(ip4_hdr->saddr) & 0xFF,
        src_address_str,
        htonl(ip4_hdr->daddr) & 0xFF,
        dst_address_str
    );
}

void log_ip6_packet(void* packet) {
    struct ip6_hdr* ip6_hdr = (struct ip6_hdr*)packet;
    
    char src_address_str[INET6_ADDRSTRLEN];
    char dst_address_str[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &ip6_hdr->ip6_src, src_address_str, INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, &ip6_hdr->ip6_dst, dst_address_str, INET6_ADDRSTRLEN);

    printf("\x1b[38;5;%im%s\x1b[39m -> \x1b[38;5;%im%s\x1b[39m\n",
        ip6_hdr->ip6_src.__in6_u.__u6_addr8[15],
        src_address_str,
        ip6_hdr->ip6_dst.__in6_u.__u6_addr8[15],
        dst_address_str
    );
}

int main() {
    int sock = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ALL));
	if (sock == -1)
	{
		return (1);
	}

    ssize_t bytes_received;
    uint8_t packet[PACKET_LEN];
    
    while (1)
    {
        bytes_received = recv(sock, packet, PACKET_LEN, 0);
        if (bytes_received == -1) {
            break;
        }
        
        switch (get_ip_version(packet))
        {
            case 4:
                log_ip4_packet(packet);
                break;
            case 6:
                log_ip6_packet(packet);
                break;
            default:
                break;
        }
    }
    return (0);
}