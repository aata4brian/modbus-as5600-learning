#include <SoftwareSerial.h>
#include <ModbusRtu.h>

// =====================================
// MODBUS PIN
// =====================================
#define MODBUS_RX_PIN 10
#define MODBUS_TX_PIN 11
#define RS485_EN_PIN  4

#define MODBUS_BAUDRATE 9600

// =====================================
// SLAVE ID
// =====================================
#define SLAVE_ENCODER_ID 1
#define SLAVE_LAPTOP_ID  2

// =====================================
// SOFTWARE SERIAL
// =====================================
SoftwareSerial modbusPort(
  MODBUS_RX_PIN,
  MODBUS_TX_PIN
);

// =====================================
// MODBUS MASTER
// =====================================
Modbus modbusMaster(
  0,
  modbusPort,
  RS485_EN_PIN
);

modbus_t modbusRequest;

// =====================================
// SLAVE 1 DATA
// =====================================
// 0 = angle
// 1 = heartbeat
// 2 = status
uint16_t encoderNodeData[3] =
{
  0,
  0,
  0
};

// =====================================
// SLAVE 2 DATA
// =====================================
// 0 = data0
// 1 = data1
// 2 = data2
// 3 = data3
// 4 = heartbeat
uint16_t laptopNodeData[5] =
{
  0,
  0,
  0,
  0,
  0
};

unsigned long lastRequestTime = 0;

uint8_t state = 0;

// =====================================
// REQUEST SLAVE 1
// =====================================
void requestEncoderNode() {

  modbusRequest.u8id =
    SLAVE_ENCODER_ID;

  modbusRequest.u8fct = 3;

  modbusRequest.u16RegAdd = 0;

  modbusRequest.u16CoilsNo = 3;

  modbusRequest.au16reg =
    encoderNodeData;

  modbusMaster.query(
    modbusRequest
  );

  delay(10);
}

// =====================================
// REQUEST SLAVE 2
// =====================================
void requestLaptopNode() {

  modbusRequest.u8id =
    SLAVE_LAPTOP_ID;

  modbusRequest.u8fct = 3;

  modbusRequest.u16RegAdd = 0;

  modbusRequest.u16CoilsNo = 5;

  modbusRequest.au16reg =
    laptopNodeData;

  modbusMaster.query(
    modbusRequest
  );

  delay(10);
}

// =====================================
// PRINT SLAVE 1
// =====================================
void printEncoderNode() {

  Serial.print(
    "ENCODER ANGLE: "
  );

  Serial.print(
    encoderNodeData[0]
  );

  Serial.print(
    " | HB: "
  );

  Serial.print(
    encoderNodeData[1]
  );

  Serial.print(
    " | STATUS: "
  );

  Serial.println(
    encoderNodeData[2]
    ? "OK"
    : "ERR"
  );
}

// =====================================
// PRINT SLAVE 2
// =====================================
void printLaptopNode() {

  Serial.print(
    "LAPTOP ARRAY: "
  );

  Serial.print("[ ");

  Serial.print(
    laptopNodeData[0]
  );

  Serial.print(", ");

  Serial.print(
    laptopNodeData[1]
  );

  Serial.print(", ");

  Serial.print(
    laptopNodeData[2]
  );

  Serial.print(", ");

  Serial.print(
    laptopNodeData[3]
  );

  Serial.print(" ]");

  Serial.print(
    " | HB: "
  );

  Serial.println(
    laptopNodeData[4]
  );
}

void setup() {

  Serial.begin(115200);

  modbusPort.begin(
    MODBUS_BAUDRATE
  );

  modbusPort.listen();

  modbusMaster.start();

  modbusMaster.setTimeOut(
    300
  );

  Serial.println(
    "MASTER READY"
  );
}

void loop() {

  modbusMaster.poll();

  switch (state) {

    // ================================
    // REQUEST SLAVE 1
    // ================================
    case 0:

      if (
        millis()
        - lastRequestTime
        >= 500
      ) {

        lastRequestTime =
          millis();

        requestEncoderNode();

        state = 1;
      }

      break;

    // ================================
    // WAIT SLAVE 1
    // ================================
    case 1:

      if (
        modbusMaster.getState()
        == COM_IDLE
      ) {

        printEncoderNode();

        requestLaptopNode();

        state = 2;
      }

      break;

    // ================================
    // WAIT SLAVE 2
    // ================================
    case 2:

      if (
        modbusMaster.getState()
        == COM_IDLE
      ) {

        printLaptopNode();

        Serial.println(
          "----------------------"
        );

        state = 0;
      }

      break;
  }
}
