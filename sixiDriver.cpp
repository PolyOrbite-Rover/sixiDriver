/**************************************
 * Title    : sixiDriver.cpp
 * Author   : Ian Gagnon
 * Created  : 2021-04-21
 *************************************/

#include "sixiDriver.h"


SixiDriver::SixiDriver(unsigned long publicationPeriodMs, bool verbose): verbose_(verbose), publicationPeriodMs_(publicationPeriodMs) {
  for (int i=0;  i<NUM_STEPPERS; i++) {
    pAccelStepperObjectArray_[i] = new AccelStepper(AccelStepper::DRIVER, STEP_PINS_[i], DIR_PINS_[i]);
    pinMode(ENABLE_PINS_[i], OUTPUT);
  }
}


SixiDriver::~SixiDriver() {
  for (int i=0;  i<NUM_STEPPERS; i++)
    delete pAccelStepperObjectArray_[i];
}


void SixiDriver::start() {
  enableMotors();
  sensorManager_.setup();
  rosserialInterface_.setup();
  attachTool();
}


void SixiDriver::moveToHome() {
  updateSteppersPositions();
  setMaxSpeedForAll(DEFAULT_SPEEDS_);
  setAccelerationForAll(DEFAULT_ACCELS_);
  moveAllTo(HOME_POSITION_);
}


void SixiDriver::updateSensors() {
  sensorManager_.updateAll();
  for (int i=0; i<NUM_STEPPERS; i++)
    positions_[i] = sensorManager_.sensors[i].angle;
}


void SixiDriver::updateSteppersPositions() {
  sensorManager_.updateAll();

  for (int i=0; i<NUM_STEPPERS; i++){
    positions_[i] = sensorManager_.sensors[i].angle;
    long pos = round(STEPS_PER_DEGREE_[i] * positions_[i]);
    pAccelStepperObjectArray_[i]->setCurrentPosition(pos);
  }

  if (verbose_){
    Serial.println("-------- SENSORS POSITIONS (DEGREES) --------");
    for (int i=0; i<NUM_SENSORS; i++){
      Serial.print(sensorManager_.sensors[i].angle); Serial.print("\t");
    }
    Serial.println();
  }
}


void SixiDriver::attachTool() {
  if (verbose_) Serial.println("Attaching servo!"); 
  servoTool_.attach(SERVO_PIN_);
}


void SixiDriver::enableMotors() {
  for (int i=0; i<NUM_STEPPERS; i++)
    digitalWrite(ENABLE_PINS_[i], LOW);
}


void SixiDriver::disableMotors() {
  for (int i=0; i<NUM_STEPPERS; i++)
    digitalWrite(ENABLE_PINS_[i], HIGH);
}


void SixiDriver::setMaxSpeedForAll(const float* speedArray) {
  for (int i=0; i<NUM_STEPPERS; i++)
    pAccelStepperObjectArray_[i]->setMaxSpeed(speedArray[i]);
}


void SixiDriver::setAccelerationForAll(const float* accelArray) {
  for (int i=0; i<NUM_STEPPERS; i++)
    pAccelStepperObjectArray_[i]->setAcceleration(accelArray[i]);
}


void SixiDriver::moveAllTo(float* positionArray) {
  if (verbose_) Serial.println("-------- MOVING (STEPS) --------");
  for (int i=0; i<NUM_STEPPERS; i++) {
    long pos = round(STEPS_PER_DEGREE_[i] * positionArray[i]);
    pAccelStepperObjectArray_[i]->moveTo(pos);
    if (verbose_){
      Serial.print(pos); Serial.print("\t");
    }
  }
  if (verbose_) {
    Serial.println();
  }
}

void SixiDriver::moveTool(int positionTool) {
  if (verbose_) Serial.println("-------- MOVING TOOL (DEG) --------");
  servoTool_.write(positionTool);
  if (verbose_) {
    Serial.print(positionTool); Serial.println();
  }
}

void SixiDriver::runAll() {
  for (int i=0; i<NUM_STEPPERS; i++) {
    if (pAccelStepperObjectArray_[i]->distanceToGo() != 0)
      pAccelStepperObjectArray_[i]->run();
  }
}


void SixiDriver::main() {
  if (millis() - lastTime_ >= publicationPeriodMs_){
    lastTime_ = millis();
    updateSensors();
    rosserialInterface_.publish(positions_);
  }
  
  if (rosserialInterface_.newCommandReceived()){
    float* desiredPosition = rosserialInterface_.getCommand();
    moveAllTo(desiredPosition);
  }

  rosserialInterface_.main();
  runAll();
}
