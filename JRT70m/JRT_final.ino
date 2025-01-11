#include <SoftwareSerial.h>
// #include <RTC.h>
// #include <Serial.h>

SoftwareSerial uart(6, 7);
const int BUFFER_SIZE = 12;
uint8_t reply[BUFFER_SIZE];
unsigned long time1 = millis();

void setup() {
  pinMode(A5, OUTPUT);
  digitalWrite(A5, HIGH);

  Serial.begin(19200);
  uart.begin(19200);

  uint8_t autoBaud[] = {0x55};
  uart.write(autoBaud, sizeof(autoBaud));
  delay(1000);

  startLaser();
  delay(1000);
  readStatus();
  delay(1000);
  startAutoMeasure();
//   startSlowMeasure();
//   startFastMeasure();

}

void loop() {
//  delay(400);
  int incomingByte = uart.read();
//  uint8_t reply[13];

  if (incomingByte == 0xAA ) {
      int len = uart.readBytes(reply, BUFFER_SIZE);
    }

//    delay(10);
//    for (int i = 0; i<12; i++){
//      Serial.print(" ");
//    }

    if (reply[2] == 0x22) {
      uint32_t distance = (reply[5] << 24) | (reply[6] << 16) | (reply[7] << 8)| reply[8];
      if (millis() > time1 + 500UL){
        if (distance < 2000) { 
          Serial.print("JRT: ");
          Serial.print(distance);
          Serial.println(" mm");
        }
        time1 = millis();
      }
//
////      uint16_t signalQuality = (reply[10] << 8) | reply[11];
////      Serial.print("Signal Quality: ");
////      Serial.println(signalQuality);
//    }
  }
}

void readStatus() {
  uint8_t readStatus[] = {0xAA, 0x80, 0x00, 0x00, 0x80};
  uart.write(readStatus, sizeof(readStatus));
  delay(100);
  int incomingByte = uart.read();
  if (incomingByte == 0xAA ) {
    uint8_t reply[9];
    for (int i = 0; i < 9; i++) {
      reply[i] = uart.read();
    }
  uint8_t status = reply[7];
  Serial.print("Status: 0x");
  Serial.println(status, HEX);
  }
  delay(1000);
}

void startLaser() {
  uint8_t startLaser[] = {0xAA, 0x00, 0x01, 0xBE, 0x00, 0x01, 0x00, 0x01, 0xC1};
  uart.write(startLaser, sizeof(startLaser));
  delay(100);

  // int availableBytes = uart.available();
  // Serial.print("Available bytes: ");
  // Serial.println(availableBytes);

  if (uart.available() >= 9) {
    uint8_t reply[9];
    for (int i = 0; i < 9; i++) {
      reply[i] = uart.read();
    }
  uint8_t payload = reply[7];
  Serial.print("Payload: 0x");
  Serial.println(payload, HEX); 

  }
}

void startAutoMeasure() {
  uint8_t startAutoMeasure[] = {0xAA, 0x00, 0x00, 0x20, 0x00, 0x01, 0x00, 0x04, 0x25}; 
  uart.write(startAutoMeasure, sizeof(startAutoMeasure));
  delay(100);
}

void startSlowMeasure() {
  uint8_t startSlowMeasure[] = {0xAA, 0x00, 0x00, 0x20, 0x00, 0x01, 0x00, 0x05, 0x26};
  uart.write(startSlowMeasure, sizeof(startSlowMeasure));
  delay(100);
}

void startFastMeasure() {
  uint8_t startFastMeasure[] = {0xAA, 0x00, 0x00, 0x20, 0x00, 0x01, 0x00, 0x06, 0x27};
  uart.write(startFastMeasure, sizeof(startFastMeasure));
  delay(100);
}

void readDistance() {

  int incomingByte = uart.read();
//  Serial.print(incomingByte, HEX);
//  Serial.print(" ");
//  
  uint8_t reply[13];

  if (incomingByte == 0xAA ) {
      int len = uart.readBytes(reply, BUFFER_SIZE);
    }
    for (int i = 0; i<12; i++){
      Serial.print(reply[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    
    if (reply[2] == 0x22) {
      uint32_t distance = (reply[5] << 24) | (reply[6] << 16) | (reply[7] << 8)| reply[8];
      if (distance < 2000) { 
        Serial.print("JRT: ");
        Serial.print(distance);
        Serial.println(" mm");
      }
//
////      uint16_t signalQuality = (reply[10] << 8) | reply[11];
////      Serial.print("Signal Quality: ");
////      Serial.println(signalQuality);
//    }
  }

  // uint32_t distance = (reply[7] << 24) | (reply[8] << 16) | (reply[9] << 8)| reply[10];
  // Serial.print(distance);
  // Serial.println(distance, HEX);

  }
// }
