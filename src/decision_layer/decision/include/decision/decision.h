#ifndef DECISION_H
#define DECISION_H

#include <ros/ros.h>
#include <decision_making/FSM.h>
#include <decision_making/ROSTask.h>
#include <decision_making/DecisionMaking.h>
#include "decision/Bucket_pose.h"
#include "decision/Instruction.h"

#define BUCKET_EMPTY 1047 // 60 degree
#define BUCKET_COLLECTING 524 // 30 degree
#define BUCKET_LIFTING   0 // 0 degree
#define BUCKET_SPEED 768 // 44 degree/s
#define TIMEOUT 30.0

#define STANDBY "/Standby"
#define CLEAN "/Clean"
#define RETRIEVE "/Retrieve"
#define RETURNDOCK "/ReturnDock"
#define CHARGE "/Charge"
#define MALFUNCTION "/Malfunction"

enum modes
{
    STANDBY1,
    CLEAN1, 
    RETRIEVE1,
    RETURNDOCK1,
    CHARGE1,
    MALFUNCTION1
}; // 11.28 left here to prevent compiler complain

using namespace decision_making;

typedef struct
{
    double x; // x coordinate in frame 'map'
    double y; // y coordinate in frame 'map'
    double w; // 4th term of rotation quaternion
    // int clear; // 0 EMPTY, 1 COLLECTING, 2 LIFTING
} goal_points_t;

const int pointNum = 1; // could be any number, may not even needed
goal_points_t goal_point;
extern volatile modes workMode; // 11.28 left here to prevent compiler complain
// extern int mode_num;

ros::Subscriber instruction_sub;

ros::Publisher bucket_pub;
decision::Bucket_pose bucket_msgs;

FSM_HEADER(Decision)
FSM_HEADER(ReturnDock)
FSM_HEADER(Cleaning)
FSM_HEADER(Retrieving)

TaskResult bucketEmpty(std::string, const FSMCallContext& c, EventQueue& e);
TaskResult bucketCollecting(std::string, const FSMCallContext& c, EventQueue& e);
TaskResult bucketLifting(std::string, const FSMCallContext& c, EventQueue& e);
TaskResult sendError(std::string, const FSMCallContext& c, EventQueue& e);
TaskResult savePose(std::string, const FSMCallContext& c, EventQueue& e);
TaskResult readLastPose(std::string, const FSMCallContext& c, EventQueue& e);
void decision_init();
void instructCB(const decision::Instruction::ConstPtr& instruct_msg);
// void keybdCB();


#endif // DECISION_H