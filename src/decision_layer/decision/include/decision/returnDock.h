#ifndef RETURNDOCK_H
#define RETURNDOCK_H

#include "decision.h"
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>

using namespace decision_making;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

// goal_points_t dockPose;
// goal_points_t default_docePose;
move_base_msgs::MoveBaseGoal goal_dock;
volatile bool charging = true; // charging status sent from low level

MoveBaseClient* mbcDock; 

// FSM_HEADER(ReturnDock)

TaskResult returnDock_begin(std::string, const FSMCallContext& c, EventQueue& e);
TaskResult moveToDock(std::string, const FSMCallContext& c, EventQueue& e);
void returnDock_init();


#endif // RETURNDOCK_H