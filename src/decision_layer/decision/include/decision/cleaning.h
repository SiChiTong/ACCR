#ifndef CLEANING_H
#define CLEANING_H

// #include <ros/ros.h>
// #include <decision_making/FSM.h>
// #include <decision_making/ROSTask.h>
// #include <decision_making/DecisionMaking.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include "decision.h"

// #define POINT_NUM 3

using namespace decision_making;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

goal_points_t goal_points[pointNum]; // actual points might come from cleaning path planning algorithm, wait for future upgrade
move_base_msgs::MoveBaseGoal goal;

MoveBaseClient* mbc; 

int i = 0;

// FSM_HEADER(Cleaning)

TaskResult cleaning_begin(std::string, const FSMCallContext& c, EventQueue& e);
TaskResult nextGoal(std::string, const FSMCallContext& c, EventQueue& e);
TaskResult moveToGoal(std::string, const FSMCallContext& c, EventQueue& e);
// TaskResult reportWarning(std::string, const FSMCallContext& c, EventQueue& e);
void cleaning_init();


#endif // CLEANING_H
