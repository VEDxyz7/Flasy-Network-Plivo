#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>

#define RING_SIZE 1024
#define PACKET_SIZE 164

// Simplified duplicate suppression structures
uint32_t seen_seq[RING_SIZE];
int present[RING_SIZE];

int main(void) {
    // --- SOCKET SETUP ---
    int sock_in = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_in < 0) { perror("Failed input socket"); exit(1); }

    int sock_out = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_out < 0) { perror("Failed output socket"); exit(1); }
    
    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(47002);
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(sock_in, (struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) {
        perror("Failed bind"); exit(1);
    }

    struct sockaddr_in player_addr;
    memset(&player_addr, 0, sizeof(player_addr));
    player_addr.sin_family = AF_INET;
    player_addr.sin_port = htons(47020);
    player_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // --- RECEIVER LOGIC ---
    memset(present, 0, sizeof(present));
    memset(seen_seq, 0, sizeof(seen_seq));
    
    uint8_t buf[PACKET_SIZE];

    while (1) {
        ssize_t n = recvfrom(sock_in, buf, sizeof(buf), 0, NULL, NULL);
        if (n != PACKET_SIZE) continue;

        // Parse sequence number
        uint32_t net_seq;
        memcpy(&net_seq, buf, 4);
        uint32_t seq = ntohl(net_seq);
        uint32_t idx = seq % RING_SIZE;

        // Ignore exact duplicate packets (either from our fractional duplication or the network)
        if (present[idx] && seen_seq[idx] == seq) {
            continue; 
        }

        // Mark as seen
        present[idx] = 1;
        seen_seq[idx] = seq;

        // Forward immediately to the harness player (Out-of-order is fully supported)
        sendto(sock_out, buf, PACKET_SIZE, 0, (struct sockaddr*)&player_addr, sizeof(player_addr));
    }

    return 0;
}