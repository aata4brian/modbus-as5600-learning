from pymodbus.server.sync import StartSerialServer
from pymodbus.datastore import ModbusSlaveContext
from pymodbus.datastore import ModbusServerContext
from pymodbus.datastore import ModbusSequentialDataBlock
from pymodbus.transaction import ModbusRtuFramer

import threading
import time

PORT = "COM23"

SLAVE_ID = 2

# ==========================================
# HOLDING REGISTER
# ==========================================
# 0 = data0
# 1 = data1
# 2 = data2
# 3 = data3
# 4 = heartbeat
store = ModbusSlaveContext(

    hr=ModbusSequentialDataBlock(
        0,
        [0, 0, 0, 0, 0]
    ),

    zero_mode=True
)

context = ModbusServerContext(

    slaves={
        SLAVE_ID: store
    },

    single=False
)

# ==========================================
# UPDATE DATA LOOP
# ==========================================
def updateLoop():

    counter = 0

    while True:

        counter += 1

        data0 = counter
        data1 = counter + 10
        data2 = counter + 20
        data3 = counter + 30

        heartbeat = counter

        store.setValues(

            3,
            0,

            [
                data0,
                data1,
                data2,
                data3,
                heartbeat
            ]
        )

        print(
            "Laptop Data:",
            [
                data0,
                data1,
                data2,
                data3,
                heartbeat
            ]
        )

        time.sleep(0.5)

# ==========================================
# THREAD
# ==========================================
threading.Thread(

    target=updateLoop,

    daemon=True

).start()

print(
    "LAPTOP MODBUS SLAVE READY"
)

# ==========================================
# START MODBUS SERVER
# ==========================================
StartSerialServer(

    context,

    framer=ModbusRtuFramer,

    port=PORT,

    baudrate=9600,

    bytesize=8,

    parity="N",

    stopbits=1,

    timeout=1
)
