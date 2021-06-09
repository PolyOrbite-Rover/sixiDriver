// PolyOrbite: SixiDriver runner
// Authors: Ian Gagnon
// Date: 2021-05-20

#include "sixiDriver.h"

SixiDriver driver(1000, false);

void setup()
{
  Serial.begin(57600);
  driver.start();
  Serial.println("Started!");
  driver.moveToHome();
}

void loop()
{
  driver.main();
}
