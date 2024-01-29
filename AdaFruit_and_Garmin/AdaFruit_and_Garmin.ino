#include <Arduino.h>
#include <Wire.h>
#include <vl53l4cd_class.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <LIDARLite_v4LED.h>

VL53L4CD AdaFruit(&Wire, 0x29);
LIDARLite_v4LED Garmin;

uint8_t NewAdaFruitReady = 0;
VL53L4CD_Result_t AdaFruitResults;
uint8_t AdaFruitStatus;
char report[64];

void setup() {
  // initialize serial comms and i2c bus
  Wire.begin();
  Serial.begin(115200);
  delay(1000);

  // initialize sensors
    // if (AdaFruit.begin() != 0) {
    //   Serial.println("AdaFruit failed..");
    //   while(1);
    // }
    // Serial.println("AdaFruit online!");
  AdaFruit.begin();
  AdaFruit.VL53L4CD_Off();
  AdaFruit.InitSensor();
  AdaFruit.VL53L4CD_SetRangeTiming(200, 0);
  AdaFruit.VL53L4CD_StartRanging();

  if (Garmin.begin() == false) {
    Serial.println("Garmin failed..");
    while(1);
  }
  Serial.println("Garmin online!");
}

void loop() {
  // AdaFruit 
  do {
    AdaFruitStatus = AdaFruit.VL53L4CD_CheckForDataReady(&NewAdaFruitReady);
  } while (!NewAdaFruitReady);

  if ((!AdaFruitStatus) && (NewAdaFruitReady != 0)) {
  AdaFruit.VL53L4CD_ClearInterrupt();
  AdaFruit.VL53L4CD_GetResult(&AdaFruitResults);
  snprintf(report, sizeof(report), "AdaFruit = %5u mm, ", AdaFruitResults.distance_mm);
  Serial.print(report);
  }

  // Garmin
  float GarminDistance;
  GarminDistance = Garmin.getDistance();
  Serial.print("Garmin = ");
  Serial.print(GarminDistance * 10);
  Serial.println(" mm");

  delay(20);
}
