#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>

#define PACKET_SIZE 164

int main(void) {
    // --- SOCKET SETUP ---
    int sock_in = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_in < 0) { perror("Failed input socket"); exit(1); }
    
    int sock_out = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_out < 0) { perror("Failed output socket"); exit(1); }
    
    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(47010);
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(sock_in, (struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) {
        perror("Failed bind"); exit(1);
    }

    struct sockaddr_in relay_addr;
    memset(&relay_addr, 0, sizeof(relay_addr));
    relay_addr.sin_family = AF_INET;
    relay_addr.sin_port = htons(47001);
    relay_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // --- SENDER LOGIC ---
    uint8_t buf[PACKET_SIZE];

    while (1) {
        ssize_t n = recvfrom(sock_in, buf, sizeof(buf), 0, NULL, NULL);
        if (n != PACKET_SIZE) continue;

        // Extract sequence number
        uint32_t net_seq;
        memcpy(&net_seq, buf, 4);
        uint32_t host_seq = ntohl(net_seq);

        // 1. Send the primary packet immediately
        sendto(sock_out, buf, PACKET_SIZE, 0, (struct sockaddr*)&relay_addr, sizeof(relay_addr));

        // 2. Throttled Fractional Duplication (90% of packets)
        // Skip duplication on multiples of 10 to guarantee < 1.95x bandwidth overhead
        if ((host_seq % 10) != 0) {
            sendto(sock_out, buf, PACKET_SIZE, 0, (struct sockaddr*)&relay_addr, sizeof(relay_addr));
        }
    }

    return 0;
}