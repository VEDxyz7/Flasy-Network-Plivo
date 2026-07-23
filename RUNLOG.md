# RUNLOG

## Experiment 1
Protocol: Direct forwarding
Delay: 80 ms

Result:
- High deadline miss rate.
- No redundancy.

---

## Experiment 2
Protocol: XOR Forward Error Correction

Delay: 80 ms

Results:
- Profile A: 0.20%
- Profile B: 6.60%
- Bandwidth: 1.54×

Observation:
Good bandwidth efficiency but recovery packets often arrived after the playback deadline on Profile B.

---

## Experiment 3
Protocol: Controlled Packet Duplication

Delay: 80 ms

Results:
- Profile A: 0.47%
- Profile B: 3.87%
- Bandwidth: 1.93×

Observation:
Duplication reduced misses significantly compared to XOR while remaining below the bandwidth limit.

---

## Experiment 4 (Final Submission)

Protocol: Controlled Packet Duplication (90%)

Delay: **85 ms**

Results:

| Profile | Misses | Bandwidth | Status |
|---------|--------|-----------|--------|
| A | 0.47% | 1.95× | VALID |
| B | 0.93% | 1.95× | VALID |

Observation:

Increasing the playout delay from 80 ms to 85 ms provided sufficient timing margin while maintaining bandwidth below the required 2× limit. This configuration was selected for the final submission.