#pragma once
//------------------------------------------------------------------------------
// Makelangelo - firmware for various robot kinematic models
// dan@marginallycelver.com 2013-12-26
// Please see http://www.github.com/MarginallyClever/makelangeloFirmware for more information.
//------------------------------------------------------------------------------

// Modified by Ian Gagnon (2021-04-21)

#define NUM_SENSORS 6

const float SENSORS_OFFSETS[NUM_SENSORS] = {-24.1, -70.4, -65.1, 0.0, 67.8, 90.4};

// use in for(ALL_SENSORS(i)) { //i will be rising
#  define ALL_SENSORS(NN) \
    int NN = 0;           \
    NN < NUM_SENSORS;     \
    ++NN

// sensor bits, flags, and masks
#define BOTTOM_14_MASK       (0x3FFF)
#define SENSOR_TOTAL_BITS    (16)
#define SENSOR_DATA_BITS     (15)
#define SENSOR_ANGLE_BITS    (14)
#define SENSOR_ANGLE_PER_BIT (360.0 / (float)((long)1 << SENSOR_ANGLE_BITS))  // 0.02197265625

// ---------------------------------------------------
// set bit on
#ifndef SBI
#define SBI(NN, BB) (NN |= (1 << BB))
#endif

// set bit off
#ifndef CBI
#define CBI(NN, BB) (NN &= ~(1 << BB))
#endif

#define SET_BIT_ON(NN, BB)  SBI(NN, BB)


#include "sixi_mega.h"

#define CRITICAL_SECTION_START noInterrupts();
#define CRITICAL_SECTION_END   interrupts();

// ---------------------------------------------------

#include <Arduino.h>  // for uint8_t


class SensorAS5147 {
 public:
  uint8_t pin_CSEL;
  uint8_t pin_CLK;
  uint8_t pin_MISO;
  uint8_t pin_MOSI;
  float angle;      // current reading after adjustment
  float angleHome;  // sensor raw angle value at home position.  reading - this = 0.

  void start();

  /**
   * See https://ams.com/documents/20143/36005/AS5147_DS000307_2-00.pdf
   * @param result where to store the returned value.  may be changed even if method fails.
   * @return 0 on fail, 1 on success.
   */
  bool getRawValue(uint16_t &result);
};

class SensorManager {
 public:
  SensorAS5147 sensors[NUM_SENSORS];
  uint8_t positionErrorFlags;

  void updateAll();
  void setup();

  float WRAP_DEGREES(float n);

  /**
   * @param rawValue 16 bit value from as4157 sensor, including parity and EF bit
   * @return degrees calculated from bottom 14 bits.
   */
  inline float extractAngleFromRawValue(uint16_t rawValue) {
    return (float)(rawValue & BOTTOM_14_MASK) * 360.0 / (float)(1 << SENSOR_ANGLE_BITS);
  }
};
