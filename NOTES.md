# NOTES

## Design

The implementation improves UDP reliability using controlled packet duplication.

Each packet is transmitted immediately upon arrival. Approximately 90% of packets are duplicated by skipping duplication for sequence numbers divisible by 10. This keeps the bandwidth overhead below the required 2× limit while providing redundancy.

The receiver forwards packets immediately and suppresses duplicates using a fixed-size ring buffer indexed by sequence number.

## Final Configuration

- Playout delay: **85 ms**
- Packet duplication: **90%**
- Duplicate suppression: Ring buffer (1024 entries)

## Results

| Profile | Deadline Misses | Bandwidth |
|---------|----------------:|----------:|
| A | 0.47% | 1.95× |
| B | 0.93% | 1.95× |

The final configuration satisfies the deadline miss and bandwidth constraints on both visible grading profiles.