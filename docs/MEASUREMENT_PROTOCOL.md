# Bench measurement protocol

No timing or reliability result has been measured in this portfolio update. Preserve raw observations before filling this table.

| Metric | Result | Method |
| --- | --- | --- |
| Transaction latency p50/p95/p99 | measurement pending | Request start to validated complete response |
| Successful polling frequency | measurement pending | Successful responses / elapsed seconds, per node |
| Response failure rate | measurement pending | Timeouts + CRC errors + exceptions / attempted requests |
| Stale-data rate | measurement pending | Successful responses with unchanged heartbeat beyond expected update window |
| Longest continuous run | measurement pending | Timed log without restarts |
| Maximum tested nodes | measurement pending | Add real nodes one at a time and repeat the same protocol |

1. Record board models, library/core versions, baud/parity/stop bits, cable length, termination, bias, power supply and connected node IDs.
2. Log at least 1,000 transactions per condition with monotonic timestamps, sequence number, node ID, register count, response code, CRC validity, heartbeat and status.
3. Separate normal operation, a disconnected node, a disconnected sensor and reconnect recovery. Avoid motor movement during communication fault tests.
4. Export raw CSV before computing summaries. Distinguish retries from new transactions. Report sample counts and percentile method.
5. Use a logic analyzer to compare software timestamps with on-wire timing and check turn-around gaps.
6. Repeat after any UART, cable, timeout or CAN change. Report the same workload; do not compare unmatched tests.

Suggested CSV columns: `run_id,sequence,node_id,request_start_ns,response_end_ns,result,heartbeat,sensor_status,retries`.

For robot integration, require command expiry, freshness checks and a defined safe actuator state before commanding motors. Those features are requirements, not current implementation claims.
