#include <SoftwareSerial.h>
#define SOFT_SERIAL_RX_PIN    3
#define SOFT_SERIAL_TX_PIN    2
#define MAX485_TX_RX_CONTROL     7

#define RS485Transmit    HIGH
#define RS485Receive     LOW


SoftwareSerial softSerial(SOFT_SERIAL_RX_PIN, SOFT_SERIAL_TX_PIN);

void communication_init() {
  softSerial.begin(9600);
  pinMode(MAX485_TX_RX_CONTROL, OUTPUT);
  digitalWrite ( MAX485_TX_RX_CONTROL, RS485Receive);
}

void communication() {
  if (softSerial.available() ) {
    byte tmp = softSerial.read();
    switch (tmp) {
      case 'w':
        softSerial.write(thisNode.weight);
        break;
      case 'h':
        softSerial.write(thisNode.heartBeat);
        break;
      case 's':
        softSerial.write(thisNode.SpO2);
        break;
      case 'a':
        softSerial.write(thisNode.nodeStatus);
        break;
      default:

        break;
    }
  }
}


//void rs_485_send(float dataToSend) {
//  softSerial.write(dataToSend);
//}
//
//void rs_485_send_byte(byte dataToSend) {
//  softSerial.write(dataToSend);
//  delay(1);
//}

//void rs485_send(byte byteToSend) {
//  digitalWrite(MAX485_TX_RX_CONTROL, RS485Transmit);  // cho phép truyền
//  softSerial.write(byteToSend); // gửi dữ liệu ngược lại
//  delay(10);
//  digitalWrite(MAX485_TX_RX_CONTROL, RS485Receive);  // Không cho phép truyền
//}


void rs485_send(byte byteToSend) {
//  digitalWrite(MAX485_TX_RX_CONTROL, RS485Transmit);  // cho phép truyền
  softSerial.write(byteToSend); // gửi dữ liệu ngược lại
//  delay(10);
//  digitalWrite(MAX485_TX_RX_CONTROL, RS485Receive);  // Không cho phép truyền
}
