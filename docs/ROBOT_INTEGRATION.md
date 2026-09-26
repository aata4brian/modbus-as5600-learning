# Robot integration and CAN roadmap

The current source is an RS485/Modbus experiment with an AS5600 and a simulated laptop data source. It does not implement an ESP32 CAN network, motor controller stack, RTOS tasks or odometry.

Next integration steps:

1. Check communication result, sensor status and heartbeat before consuming a value; report stale data explicitly.
2. Unwrap encoder angle across the 4095→0 boundary with a physically justified maximum angular speed and sampling interval. A missed half-turn cannot be recovered unambiguously from one sample.
3. Calibrate wheel radius, track width, gearing and encoder direction before differential-drive pose updates.
4. Add sequence numbers, command expiry and a stop behavior that does not depend on the host remaining connected.
5. For CAN, select a controller/transceiver matched to voltage and bitrate. Define message IDs, priority, units, signedness, timestamps, heartbeat and bus-off recovery.
6. Benchmark RS485 and CAN with the same payload, node count and workload before claiming an improvement.

A separate `robot-communication-stack` repository is deferred until there is a distinct implementation. Splitting this existing source into a second portfolio repo would overstate the amount of completed work.
