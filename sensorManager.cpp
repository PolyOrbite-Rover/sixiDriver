//------------------------------------------------------------------------------
// Makelangelo - firmware for various robot kinematic models
// dan@marginallycelver.com 2013-12-26
// Please see http://www.github.com/MarginallyClever/makelangeloFirmware for more information.
//------------------------------------------------------------------------------

// Modified by Ian Gagnon (2021-04-21)

#include "sensorManager.h"

SensorManager sensorManager;


void SensorAS5147::start() {
  pinMode(pin_CSEL, OUTPUT);
  pinMode(pin_CLK, OUTPUT);
  pinMode(pin_MISO, INPUT);
  pinMode(pin_MOSI, OUTPUT);

  digitalWrite(pin_CSEL, HIGH);
  digitalWrite(pin_MOSI, HIGH);
}


/**
 * @param result where to store the returned value.  may be changed even if method fails.
 * @return 0 on fail, 1 on success.
// @see https://ams.com/documents/20143/36005/AS5147_DS000307_2-00.pdf
 */
boolean SensorAS5147::getRawValue(uint16_t &result) {
  uint8_t input, parity = 0;

  result = 0;

  // Send the request for the angle value (command 0xFFFF) at the same time as receiving an angle.
  // This is done by leaving MOSI high all the time.

  // Collect the 16 bits of data from the sensor
  digitalWrite(pin_CSEL, LOW);  // csel

  for (int i = 0; i < SENSOR_TOTAL_BITS; ++i) {
    digitalWrite(pin_CLK, HIGH);  // clk
    // this is here to give a little more time to the clock going high.
    // only needed if the arduino is *very* fast.  I'm feeling generous.
    result <<= 1;
    digitalWrite(pin_CLK, LOW);  // clk

    input = digitalRead(pin_MISO);  // miso
    // Serial.print(input, DEC);
    result |= input;
    parity ^= (i > 0) & input;
  }

  digitalWrite(pin_CSEL, HIGH);  // csel

  // check the parity bit
  return (parity != (result >> SENSOR_DATA_BITS));
}


void SensorManager::setup() {

#  define SSP(label, NN, JJ) sensors[NN].pin_##label = PIN_SENSOR_##label##_##NN;
#  define SSP2(NN) \
    if (NUM_SENSORS > NN) { SSP(CSEL, NN, 0) SSP(CLK, NN, 1) SSP(MISO, NN, 2) SSP(MOSI, NN, 3) }

  //#define SSP(label,NN)    pins[i++] = PIN_SENSOR_##label##_##NN;
  //#define SSP2(NN)         if(NUM_SENSORS>NN) {  SSP(CSEL,NN)  SSP(CLK,NN)  SSP(MISO,NN)  SSP(MOSI,NN)  }
  SSP2(0)
  SSP2(1)
  SSP2(2)
  SSP2(3)
  SSP2(4)
  SSP2(5)

  for (ALL_SENSORS(i)) { sensors[i].start(); }

  // slow the servo on pin D13 down to 61.04Hz
  // see https://arduinoinfo.mywikis.net/wiki/Arduino-PWM-Frequency
  // TCCR0B = (TCCR0B & B11111000) | B00000101;

  // the first few reads will return junk so we force a couple empties here.
  updateAll();
  updateAll();
}


void SensorManager::updateAll() {
  uint16_t rawValue;
  float v;
  for (ALL_SENSORS(i)) {
    if (sensors[i].getRawValue(rawValue)) continue;
    v = extractAngleFromRawValue(rawValue);
    if (i != 2) v = -v;
    v -= SENSORS_OFFSETS[i];
    v = WRAP_DEGREES(v);
    sensors[i].angle = v;
  }
}


float SensorManager::WRAP_DEGREES(float n) {
  n = fmod(n, 360);
  n += 360;
  n = fmod(n, 360);
  if (n > 180) n -= 360;
  return n;
}