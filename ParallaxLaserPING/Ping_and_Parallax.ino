#include <SoftwareSerial.h>

const int RX1 = 2; 
const int TX1 = 3;
const int RX2 = 4;
const int TX2 = 5;

SoftwareSerial pingSerial(RX1, TX1);
SoftwareSerial paraSerial(RX2, TX2);

char buff[4] = {0x80, 0x06, 0x03, 0x77};
unsigned char data[11] = {0};

void setup() {
  Serial.begin(9600);
  pingSerial.begin(9600);
  paraSerial.begin(9600);

  paraSerial.print(buff);
  delay(50);
}

void loop() {

  static char valueString[8];
  static int count = 0;

  int pingAvail = pingSerial.available();
  Serial.print("ping avail: ");
  Serial.println(pingAvail);

  while (pingSerial.available()) {
  char dataValue = pingSerial.read();
  if (dataValue == '\r') {
    valueString[count] = '\0';
    int value = atoi(valueString);
    Serial.print("LaserPING: ");
    Serial.print(valueString);
    Serial.println(" mm");
    count = 0;
    } else {
    valueString[count] = dataValue;
    count++;
    }
  }


  if (paraSerial.available() >= 11) {
    for (int i = 0; i < 11; i++) {
      data[i] = paraSerial.read();
    }

    unsigned char Check = 0;
    for (int i = 0; i < 10; i++) {
      Check = Check + data[i];
    }
    Check = ~Check + 1;

    if (data[10] == Check) {
      if (data[3] == 'E' && data[4] == 'R' && data[5] == 'R') {
        Serial.println("Out of range");
      } else {
        float distance = 0;
        distance = (data[3] - '0') * 100 + (data[4] - '0') * 10 + (data[5] - '0') +
                   (data[7] - '0') * 0.1 + (data[8] - '0') * 0.01 + (data[9] - '0') * 0.001;
        Serial.print("Parallax = ");
        Serial.print(distance, 3);
        Serial.println(" m");
      }
    } else {
      Serial.println("Invalid Data!");
    }
  }
  delay(20);
}