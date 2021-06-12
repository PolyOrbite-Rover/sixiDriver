/**************************************
 * Title    : sixiDriver.h
 * Author   : Ian Gagnon
 * Created  : 2021-04-21
 *************************************/

// Motor gearbox ratio
#define MOTOR_0_GEARBOX_RATIO 30.0
#define MOTOR_1_GEARBOX_RATIO 40.0
#define MOTOR_2_GEARBOX_RATIO 35.0
#define MOTOR_3_GEARBOX_RATIO 20.0 // To validate
#define MOTOR_4_GEARBOX_RATIO 20.0 // To validate
#define MOTOR_5_GEARBOX_RATIO 20.0 // To validate

// Controller steps per revolution (check controller settings)
#define CONTROLLER_0_STEPS_PER_REV 400.0
#define CONTROLLER_1_STEPS_PER_REV 400.0
#define CONTROLLER_2_STEPS_PER_REV 400.0
#define CONTROLLER_3_STEPS_PER_REV 400.0 // To validate
#define CONTROLLER_4_STEPS_PER_REV 400.0 // To validate
#define CONTROLLER_5_STEPS_PER_REV 400.0 // To validate

// Motor steps per degree
#define MOTOR_0_STEPS_PER_DEGREE (CONTROLLER_0_STEPS_PER_REV * MOTOR_0_GEARBOX_RATIO) / 360.0
#define MOTOR_1_STEPS_PER_DEGREE (CONTROLLER_1_STEPS_PER_REV * MOTOR_1_GEARBOX_RATIO) / 360.0
#define MOTOR_2_STEPS_PER_DEGREE (CONTROLLER_2_STEPS_PER_REV * MOTOR_2_GEARBOX_RATIO) / 360.0
#define MOTOR_3_STEPS_PER_DEGREE (CONTROLLER_3_STEPS_PER_REV * MOTOR_3_GEARBOX_RATIO) / 360.0 // To validate
#define MOTOR_4_STEPS_PER_DEGREE (CONTROLLER_4_STEPS_PER_REV * MOTOR_4_GEARBOX_RATIO) / 360.0 // To validate
#define MOTOR_5_STEPS_PER_DEGREE (CONTROLLER_5_STEPS_PER_REV * MOTOR_5_GEARBOX_RATIO) / 360.0 // To validate

// TO REMOVE
#define DM322T_MICROSTEP (2.0)
#define ELBOW_DOWNGEAR_RATIO (30.0 / 20.0)
#define NEMA17_RATIO         (DM322T_MICROSTEP * NEMA17_CYCLOID_GEARBOX_RATIO * ELBOW_DOWNGEAR_RATIO)
#define NEMA23_RATIO_ELBOW   (NEMA23_CYCLOID_GEARBOX_RATIO_ELBOW)
#define NEMA23_RATIO_ANCHOR  (NEMA23_CYCLOID_GEARBOX_RATIO_ANCHOR)
#define NEMA24_RATIO         (NEMA24_CYCLOID_GEARBOX_RATIO)

// Motors are numbered 0 (base) to 5 (hand)
#define MOTOR_0_STEPS_PER_TURN (MOTOR_STEPS_PER_TURN * NEMA23_RATIO_ANCHOR)  // anchor
#define MOTOR_1_STEPS_PER_TURN (MOTOR_STEPS_PER_TURN * NEMA24_RATIO)         // shoulder
#define MOTOR_2_STEPS_PER_TURN (MOTOR_STEPS_PER_TURN * NEMA23_RATIO_ELBOW)   // elbow
#define MOTOR_3_STEPS_PER_TURN (MOTOR_STEPS_PER_TURN * NEMA17_RATIO)         // ulna
#define MOTOR_4_STEPS_PER_TURN (MOTOR_STEPS_PER_TURN * NEMA17_RATIO)         // wrist
#define MOTOR_5_STEPS_PER_TURN (MOTOR_STEPS_PER_TURN * NEMA17_RATIO)         // hand


#ifndef SIXI_H
#define SIXI_H

#include <AccelStepper.h>
#include "sixi_mega.h"
#include "rosserialInterface.h"
#include "sensorManager.h"

#define NUM_TEST 3


class SixiDriver
/**
 * @brief Driver for Sixi 2 robotic arm based on AccelStepper library
 * Uses rosserialInterface to communicates with ROS and sensorManager to read sensors
 */
{
public:
  // CTOR & DTOR
  SixiDriver(unsigned long publicationPeriodMs, bool verbose=false);
  ~SixiDriver();

  // Methods
  void start();
  void moveToHome();
  void updateSteppersPositions();
  void updateSensors();
  void enableMotors();
  void disableMotors();
  void setMaxSpeedForAll(const float* speedArray);
  void setAccelerationForAll(const float* accelArray);
  void moveAllTo(float* positionArray);
  void runAll();
  void main();

private:
  // Objects
  SensorManager sensorManager_;
  ROSserialInterface rosserialInterface_;
  AccelStepper* pAccelStepperObjectArray_[NUM_TEST];

  // Attributes
  float positions_[NUM_SENSORS];
  unsigned long lastTime_ = 0;
  unsigned long publicationPeriodMs_;
  bool verbose_;

  // Constants
  const float STEPS_PER_DEGREE_[NUM_TEST] = { MOTOR_0_STEPS_PER_DEGREE,
                                              MOTOR_1_STEPS_PER_DEGREE,
                                              MOTOR_2_STEPS_PER_DEGREE};

  const int STEP_PINS_[NUM_TEST] = {MOTOR_0_STEP_PIN,
                                    MOTOR_1_STEP_PIN,
                                    MOTOR_2_STEP_PIN};

  const int DIR_PINS_[NUM_TEST] = { MOTOR_0_DIR_PIN,
                                    MOTOR_1_DIR_PIN,
                                    MOTOR_2_DIR_PIN};

  const int ENABLE_PINS_[NUM_TEST] = {MOTOR_0_ENABLE_PIN,
                                      MOTOR_1_ENABLE_PIN,
                                      MOTOR_2_ENABLE_PIN};

  const float DEFAULT_SPEEDS_[NUM_TEST] = {200.0, 200.0, 200.0};
  const float DEFAULT_ACCELS_[NUM_TEST] = {200.0, 200.0, 200.0};

  float HOME_POSITION_[NUM_TEST] = {0.0, 0.0, 0.0}; // {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}

};
#endif
