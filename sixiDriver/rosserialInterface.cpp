/**************************************
 * Title    : rosserialInterface.cpp
 * Author   : Ian Gagnon
 * Created  : 2021-05-05
 *************************************/

#include "rosserialInterface.h"


ROSserialInterface::ROSserialInterface(): sub_("arduino_motors", &ROSserialInterface::callback, this), pub_("arduino_sensors", &msg_) {
}


ROSserialInterface::~ROSserialInterface() {
}


void ROSserialInterface::setup() {
  nh_.initNode();
  float data[N_DATA_TO_PUBLISH];
  msg_.data = data;
  msg_.data_length = N_DATA_TO_PUBLISH;
  nh_.subscribe(sub_);
  nh_.advertise(pub_);
}


void ROSserialInterface::callback(const std_msgs::Float32MultiArray& msg) {
  for (int i=0; i<N_DATA_TO_RECEIVE; i++)
    steps_[i] = msg.data[i];
  newCommandReceived_ = true;
}


bool ROSserialInterface::newCommandReceived() {
  return newCommandReceived_;
}


float* ROSserialInterface::getCommand() {
  return steps_;
}


void ROSserialInterface::publish(float* arrayToPublish) {
  for (int i=0; i<N_DATA_TO_PUBLISH; i++){
    msg_.data[i] = arrayToPublish[i];
  }
  pub_.publish(&msg_);
}


void ROSserialInterface::main() {
  nh_.spinOnce();
}
