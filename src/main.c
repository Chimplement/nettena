#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdint.h>

#define PACKET_LEN 8192

int main() {
    int sock = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ALL));
	if (sock == -1)
	{
		return (1);
	}

    ssize_t bytes_received;
    uint8_t buffer[PACKET_LEN];

    struct iphdr* iphdr = (struct iphdr*)buffer;

    while (1)
    {
        bytes_received = recv(sock, buffer, PACKET_LEN, 0);
        if (bytes_received == -1) {
            break;
        }
        
        printf("%i, %s -> %s\n",
            iphdr->version,
            inet_ntoa((struct in_addr){
                iphdr->saddr
            }),
            inet_ntoa((struct in_addr){
                iphdr->daddr
            })
        );
    }
    return (0);
}