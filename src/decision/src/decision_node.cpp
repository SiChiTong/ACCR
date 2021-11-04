#include "ros/ros.h"
#include "decision/bucket_pose.h"

// define the bucket states in milli radian
#define BUCKET_EMPTY 1047 // 60 degree
#define BUCKET_COLLECTING 524 // 30 degree
#define BUCKET_LIFTING   0 // 0 degree
#define BUCKET_SPEED 768 // 44 degree/s





int main(int argc, char **argv)
{
  ros::init(argc,argv,"talker");
  ros::NodeHandle n;
  
  ros::Publisher pub = n.advertise<decision::bucket_pose>("bucket_msg",10);

  ros::Rate loop_rate(10);
  
  while (ros::ok())
  {
    decision::bucket_pose msg;
    

    msg.bucket_joint_angle.data = BUCKET_EMPTY;
    msg.bucket_joint_speed.data = BUCKET_SPEED;
    pub.publish(msg);
    ros::Duration(5).sleep();
    

    msg.bucket_joint_angle.data = BUCKET_COLLECTING;
    msg.bucket_joint_speed.data = BUCKET_SPEED;
    pub.publish(msg);
    ros::Duration(5).sleep();

    msg.bucket_joint_angle.data = BUCKET_LIFTING;
    msg.bucket_joint_speed.data = BUCKET_SPEED;
    pub.publish(msg);
    ros::Duration(5).sleep();


    
    pub.publish(msg);
    //loop_rate.sleep();

  }

  return 0;


}
