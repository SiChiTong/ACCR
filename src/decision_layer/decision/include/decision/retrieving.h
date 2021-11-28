#ifndef RETRIEVE_H
#define RETRIEVE_H

#include "decision.h"
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>

using namespace decision_making;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

// goal_points_t dumpPose;
// goal_points_t default_dumpPose;
move_base_msgs::MoveBaseGoal goal_dump;

MoveBaseClient* mbcDump; 

// FSM_HEADER(Retrieving)

TaskResult retrieve_begin(std::string, const FSMCallContext& c, EventQueue& e);
TaskResult moveToDump(std::string, const FSMCallContext& c, EventQueue& e);
void retrieving_init();


#endif // RETRIEVE_H