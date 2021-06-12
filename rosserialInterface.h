/**************************************
 * Title    : rosserialInterface.h
 * Author   : Ian Gagnon
 * Created  : 2021-05-05
 *************************************/

#ifndef ROSSERIAL_INTERFACE_H
#define ROSSERIAL_INTERFACE_H

#include <ros.h>
#include <std_msgs/Float32MultiArray.h>

#define N_DATA_TO_PUBLISH 6
#define N_DATA_TO_RECEIVE 6


class ROSserialInterface
/**
 * @brief Serial interface to communicate with ROS
 * Creates 1 publisher and 1 subscriber
 */
{
public:
  // CTOR & DTOR
  ROSserialInterface();
  ~ROSserialInterface();

  // Methods
  void setup();
  void callback(const std_msgs::Float32MultiArray& msg);
  bool newCommandReceived();
  float* getCommand();
  void publish(float* arrayToPublish);
  void main();

private:
  // ROS objects
  ros::NodeHandle nh_;
  ros::Subscriber<std_msgs::Float32MultiArray, ROSserialInterface> sub_;
  ros::Publisher pub_;

  // Attributes
  std_msgs::Float32MultiArray msg_;
  float steps_[N_DATA_TO_RECEIVE]= {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  bool newCommandReceived_ = false;
};
#endif
