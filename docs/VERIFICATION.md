# Verification — 21 September 2026

Base revision: `737539ed493377160c440919c4a93c89e5542a23`.

Three host tests passed using Python 3.12, PyModbus 2.5.3 and pyserial 3.5: initial register layout, unsigned 16-bit rollover, and real in-memory datastore round trip. Python compilation passed and the CLI `--help` completed without opening a serial port.

The earlier script launched a thread/server during import, hardcoded COM23 and eventually exceeded uint16 register capacity. The updated example keeps hardware startup under `main`, takes an explicit `--port` and wraps all register values.

No Arduino compilation, attached-hardware transaction, timing benchmark or CAN test was performed. A compatible ModbusRtu library/core version is still required for reproducible firmware builds. Host CI intentionally does not claim firmware or hardware coverage.
