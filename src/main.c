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

    printf("%s -> %s\n",
        src_address_str,
        dst_address_str
    );
}

void log_ip6_packet(void* packet) {
    struct ip6_hdr* ip6_hdr = (struct ip6_hdr*)packet;
    
    char src_address_str[INET6_ADDRSTRLEN];
    char dst_address_str[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &ip6_hdr->ip6_src, src_address_str, INET6_ADDRSTRLEN);
    inet_ntop(AF_INET6, &ip6_hdr->ip6_dst, dst_address_str, INET6_ADDRSTRLEN);

    printf("%s -> %s\n",
        src_address_str,
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