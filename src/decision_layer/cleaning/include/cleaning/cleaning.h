#ifndef CLEANING_H
#define CLEANING_H

#include <ros/ros.h>
#include <decision_making/FSM.h>
#include <decision_making/ROSTask.h>
#include <decision_making/DecisionMaking.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include "cleaning/bucket_pose.h"
#define POINT_NUM 3

using namespace decision_making;

typedef struct
{
    double x; // x coordinate in frame 'map'
    double y; // y coordinate in frame 'map'
    int clear; // 0 EMPTY, 1 COLLECTING, 2 LIFTING
} goal_points_t;
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

goal_points_t goal_points[POINT_NUM];
move_base_msgs::MoveBaseGoal goal;
cleaning::bucket_pose bucket_msgs;

ros::Publisher bucket_pub; // publish bucket messages
MoveBaseClient* mbc;

int i = 0;

FSM_HEADER(Cleaning)

TaskResult cleaning_begin(std::string, const FSMCallContext& c, EventQueue& e);
TaskResult nextGoal(std::string, const FSMCallContext& c, EventQueue& e);
TaskResult moveToGoal(std::string, const FSMCallContext& c, EventQueue& e);


#endif // CLEANING_H
