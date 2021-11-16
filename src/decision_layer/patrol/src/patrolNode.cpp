#include "patrol.h"
#include <geometry_msgs/PoseStamped.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <nav_msgs/Odometry.h>

#define POINT_NUM 3

typedef struct {
    double x;
    double y;
} goal_points_t;

// using namespace move_base;
goal_points_t goal_points[3];

nav_msgs::Odometry* odom_ptr;
actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("moveBaseAction", true);
int i = 0;

void odomCB(const nav_msgs::Odometry::ConstPtr& odom) {
    odom_ptr->header = odom->header;
    odom_ptr->child_frame_id = odom->child_frame_id;
    odom_ptr->pose = odom->pose;
    odom_ptr->twist = odom->twist;
}

decision_making::TaskResult patrol_begin(std::string, 
        const decision_making::FSMCallContext& c, decision_making::EventQueue& e) {
    if(i < POINT_NUM) e.riseEvent("/start");
    
    return decision_making::TaskResult::SUCCESS();
}

decision_making::TaskResult nextGoal(std::string, 
        const decision_making::FSMCallContext& c, decision_making::EventQueue& e) {
    if(i < POINT_NUM-1) e.riseEvent("/more_goal");
    else e.riseEvent("/no_goal");

    return decision_making::TaskResult::SUCCESS();
}

decision_making::TaskResult moveToGoal(std::string, 
        const decision_making::FSMCallContext& c, decision_making::EventQueue& e) {
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.header.frame_id = odom_ptr->header.frame_id;
    goal.target_pose.pose.position.x = goal_points[i].x;
    goal.target_pose.pose.position.y = goal_points[i].y;
    goal.target_pose.pose.position.z = odom_ptr->pose.pose.position.z;
    goal.target_pose.pose.orientation = odom_ptr->pose.pose.orientation;
    // goal.pose.orientation.x = 
    // goal.pose.orientation.y = 
    // goal.pose.orientation.z = 
    // goal.pose.orientation.w = 
    ac.sendGoal(goal);
    i++;

    bool finished = ac.waitForResult(ros::Duration(20.0));
    if(finished){
        e.riseEvent("/arrived");
    } 
    else{
        e.riseEvent("/timeOut");
    } 
    return decision_making::TaskResult::SUCCESS();
}

int main(int argc, char **argv) {
    goal_points[0].x = 3;
    goal_points[0].y = 6;
    goal_points[1].x = 3;
    goal_points[1].y = 8;
    goal_points[2].x = 5;
    goal_points[2].y = 9;
    
    ros::init(argc, argv, "Patrol");
    ros::NodeHandle nh;
    ros::Subscriber odom_sub;
    
    
    odom_sub = nh.subscribe<nav_msgs::Odometry>("odom", 5, odomCB);
    ros_decision_making_init(argc, argv);

    ROS_INFO("Preparing patrol...");

    RosEventQueue eventQueue;

    LocalTasks::registrate("moveToGoal", moveToGoal);
    LocalTasks::registrate("nextGoal", nextGoal);
    LocalTasks::registrate("patrol_begin", patrol_begin);

    ros::AsyncSpinner spinner(1);
    spinner.start();

    FsmPatrol(NULL, &eventQueue);

}
