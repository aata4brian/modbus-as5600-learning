"""Synthetic Modbus RTU slave 2; requires PyModbus 2.5.3 and a serial adapter."""

import argparse
import threading
import time

from pymodbus.datastore import (
    ModbusSequentialDataBlock,
    ModbusServerContext,
    ModbusSlaveContext,
)
from pymodbus.server.sync import StartSerialServer
from pymodbus.transaction import ModbusRtuFramer


def register_values(counter):
    """All values must remain representable by a 16-bit holding register."""
    return [(counter + offset) & 0xFFFF for offset in (0, 10, 20, 30, 0)]


def update_loop(store):
    counter = 0
    while True:
        counter = (counter + 1) & 0xFFFF
        values = register_values(counter)
        store.setValues(3, 0, values)
        print("Laptop Data:", values)
        time.sleep(0.5)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--port", required=True, help="Serial device, e.g. COM23 or /dev/ttyUSB0")
    args = parser.parse_args()
    store = ModbusSlaveContext(
        hr=ModbusSequentialDataBlock(0, [0] * 5), zero_mode=True
    )
    context = ModbusServerContext(slaves={2: store}, single=False)
    threading.Thread(target=update_loop, args=(store,), daemon=True).start()
    print("LAPTOP MODBUS SLAVE READY")
    StartSerialServer(
        context, framer=ModbusRtuFramer, port=args.port,
        baudrate=9600, bytesize=8, parity="N", stopbits=1, timeout=1,
    )


if __name__ == "__main__":
    main()
