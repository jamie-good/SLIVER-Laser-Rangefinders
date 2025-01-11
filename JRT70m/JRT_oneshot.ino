#include <SoftwareSerial.h>

SoftwareSerial uart(0, 1);

const int BUFFER_SIZE = 12;
uint8_t reply[BUFFER_SIZE]; 

void setup() {
  pinMode(A5, OUTPUT);
  digitalWrite(A5, HIGH);

  Serial.begin(19200);
  uart.begin(19200);

  uint8_t autoBaud[] = {0x55};
  uart.write(autoBaud, sizeof(autoBaud));
  delay(1000);

  startLaser();
  delay(100);

  readStatus();
  delay(100);

}

void loop() {

  oneShot();
  readDistance(); 
  delay(1); 

}

void startLaser() {
  uint8_t startLaser[] = {0xAA, 0x00, 0x01, 0xBE, 0x00, 0x01, 0x00, 0x01, 0xC1};
  uart.write(startLaser, sizeof(startLaser));
}

void readStatus() {

  uint8_t readStatus[] = {0xAA, 0x80, 0x00, 0x00, 0x80};
  uart.write(readStatus, sizeof(readStatus));

  int incomingByte = uart.read();
  if (incomingByte == 0xAA ) {
    uint8_t reply[9];
    for (int i = 0; i < 9; i++) {
      reply[i] = uart.read();
    }

  uint8_t status = reply[6];
  Serial.print("Status: 0x");
  Serial.println(status, HEX);
  }

}

void oneShot() {
  uint8_t oneShot[] = {0xAA, 0x00, 0x20, 0x00, 0x01, 0x00, 0x02, 0x23}; 
  uart.write(oneShot, sizeof(oneShot)); 
}

void readDistance() {

  int incomingByte = uart.read();
  
  if (incomingByte == 0xAA) {
    for (int i = 0; i < 12; i++) {
      reply[i] = uart.read();
    }
  uint32_t distance = (reply[5] << 24) | (reply[6] << 16) | (reply[7] << 8)| reply[8];
  Serial.print("JRT: ");
  Serial.print(distance);
  Serial.println(" mm"); 
  }
}