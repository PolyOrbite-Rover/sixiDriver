#include "sensorManager.h"
#include <SPI.h>

unsigned long UPDATE_RATE_MS = 1000;
unsigned long reportDelay = 0;

SensorManager manager;
void setup() {
  Serial.begin(57600);
  manager.setup();
}


void loop() {
  if (millis() > reportDelay) {
    manager.updateAll();
    reportDelay = millis() + UPDATE_RATE_MS;
    for (int i=0; i<6; i++){
      Serial.print(manager.sensors[i].angle, 3);
      Serial.print('\t');
    }
    Serial.println();
  }
}
