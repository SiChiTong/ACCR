#include "cleaning.h"

int main(int argc, char **argv)
{
    goal_points[0].x = 3;
    goal_points[0].y = 1;
    goal_points[1].x = 3;
    goal_points[1].y = 1;
    goal_points[2].x = 5;
    goal_points[2].y = 1.5;
    
    ros::init(argc, argv, "Cleaning");
    ros_decision_making_init(argc, argv);
    ros::NodeHandle nh;

    ROS_INFO("Preparing cleaning...");   

    mbc = new MoveBaseClient(nh, "move_base", true);
    bucket_pub = nh.advertise<cleaning::bucket_pose>("bucket_msgs",10);
    RosEventQueue eventQueue;

    LocalTasks::registrate("moveToGoal", moveToGoal);
    LocalTasks::registrate("nextGoal", nextGoal);
    LocalTasks::registrate("cleaning_begin", cleaning_begin);

    ros::AsyncSpinner spinner(1);
    spinner.start();

    FsmCleaning(NULL, &eventQueue);
}
