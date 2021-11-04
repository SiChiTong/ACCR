#include "ros/ros.h"
#include "decision/bucket_pose.h"

// define the bucket states in milli radian
#define Empty 785 // 45 degree
#define Collecting 524 // 30 degree
#define Lifting 0 // 0 degree





int main(int argc, char **argv)
{
  ros::init(argc,argv,"talker");
  ros::NodeHandle n;
  
  ros::Publisher pub = n.advertise<decision::bucket_pose>("bucket_msg",10);
  
  ros::Rate loop_rate(10);
  
  while (ros::ok())
  {
    decision::bucket_pose msg;
    

    msg.bucket_joint_angle.data = Empty;
    
    pub.publish(msg);
    loop_rate.sleep();

  }

  return 0;


}
