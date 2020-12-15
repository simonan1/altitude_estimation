#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include "geometry_msgs/Vector3Stamped.h"
#include <tf/transform_datatypes.h>

ros::Publisher rot_publisher ;

void imuRecievedCallback(const sensor_msgs::Imu::ConstPtr& msg)
{
  // ROS_INFO("I heard: [%lf]", msg->orientation.x);
  //convert to yaw, pitch, roll
  double yaw, pitch, roll;
  tf::Quaternion q(msg->orientation.x,msg->orientation.y,msg->orientation.z,msg->orientation.w);
  tf::Matrix3x3 m(q);
  m.getRPY(roll,pitch,yaw);
  geometry_msgs::Vector3Stamped rpy;
  rpy.vector.x = roll;
  rpy.vector.y = pitch;
  rpy.vector.z = yaw;
  rpy.header.stamp = msg->header.stamp;
  rpy.header.seq = msg->header.seq;
  rot_publisher.publish(rpy);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "convert_rot_ang_node");

  
  ros::NodeHandle n;

  
  ros::Subscriber sub = n.subscribe("/mavros/imu/data", 1000, imuRecievedCallback);
  rot_publisher = n.advertise<geometry_msgs::Vector3Stamped>("/rotation_angles", 1000);
  ros::spin();

  return 0;
}