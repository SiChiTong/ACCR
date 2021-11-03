#include "ros/ros.h"
#include "decision/bucket_pose.h"





int main(int argc, char **argv)
{
  ros::init(argc,argv,"talker");
  ros::NodeHandle n;
  
  ros::Publisher pub = n.advertise<decision::bucket_pose>("bucket_msg",10);
  
  ros::Rate loop_rate(10);
  
  while (ros::ok())
  {
    decision::bucket_pose msg;
    
    // 30 -> down, 0 -> up
    msg.bucket_joint_angle.data = 30;
    
    pub.publish(msg);
    loop_rate.sleep();

  }

  return 0;


}
