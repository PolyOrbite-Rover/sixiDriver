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

#ifndef SIXI_H
#define SIXI_H

#include <AccelStepper.h>
#include <Servo.h> 
#include "sixi_mega.h"
#include "rosserialInterface.h"
#include "sensorManager.h"

#define NUM_STEPPERS 6
#define NUM_SERVO 1

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
  void attachTool();
  void moveTool(int positionTool);
  void runAll();
  void main();

private:
  // Objects
  SensorManager sensorManager_;
  ROSserialInterface rosserialInterface_;
  AccelStepper* pAccelStepperObjectArray_[NUM_STEPPERS];
  Servo servoTool_; 

  // Attributes
  float positions_[NUM_SENSORS];
  unsigned long lastTime_ = 0;
  unsigned long publicationPeriodMs_;
  bool verbose_;

  // Constants
  const int SERVO_PIN_= TOOL_PIN;

  const float STEPS_PER_DEGREE_[NUM_STEPPERS] = { MOTOR_0_STEPS_PER_DEGREE,
                                                  MOTOR_1_STEPS_PER_DEGREE,
                                                  MOTOR_2_STEPS_PER_DEGREE,
                                                  MOTOR_3_STEPS_PER_DEGREE,
                                                  MOTOR_4_STEPS_PER_DEGREE,
                                                  MOTOR_5_STEPS_PER_DEGREE};

  const int STEP_PINS_[NUM_STEPPERS] = {MOTOR_0_STEP_PIN,
                                        MOTOR_1_STEP_PIN,
                                        MOTOR_2_STEP_PIN,
                                        MOTOR_3_STEP_PIN,
                                        MOTOR_4_STEP_PIN,
                                        MOTOR_5_STEP_PIN};

  const int DIR_PINS_[NUM_STEPPERS] = { MOTOR_0_DIR_PIN,
                                        MOTOR_1_DIR_PIN,
                                        MOTOR_2_DIR_PIN,
                                        MOTOR_3_DIR_PIN,
                                        MOTOR_4_DIR_PIN,
                                        MOTOR_5_DIR_PIN};

  const int ENABLE_PINS_[NUM_STEPPERS] = {MOTOR_0_ENABLE_PIN,
                                          MOTOR_1_ENABLE_PIN,
                                          MOTOR_2_ENABLE_PIN,
                                          MOTOR_3_ENABLE_PIN,
                                          MOTOR_4_ENABLE_PIN,
                                          MOTOR_5_ENABLE_PIN};

  const float DEFAULT_SPEEDS_[NUM_STEPPERS] = {700.0,700.0, 700.0, 700.0, 700.0, 700.0};
  const float DEFAULT_ACCELS_[NUM_STEPPERS] = {300.0, 300.0, 300.0, 300.0, 300.0, 300.0};

  float HOME_POSITION_[NUM_STEPPERS] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

};
#endif
