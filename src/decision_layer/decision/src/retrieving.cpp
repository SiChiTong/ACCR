#include "retrieving.h"

#define ANG_TOL 0.01
#define DIST_TOL 0.0001

FSM(Retrieving)
{
    FSM_STATES
    {
        Stop,
        Moving
    }
    FSM_START(Stop);
    FSM_BGN
    {
        FSM_STATE(Stop)
        {
            FSM_CALL_TASK(returnDock_begin)
            FSM_TRANSITIONS
            {
                FSM_ON_EVENT("/dump", FSM_NEXT(Moving))
            }
        }
        FSM_STATE(Moving)
        {
            FSM_CALL_TASK(moveToDock)
            FSM_TRANSITIONS
            {
                FSM_ON_EVENT("/hold", FSM_NEXT(Stop))
            }
        }
    }
    FSM_END
}

TaskResult retrieve_begin(std::string, const FSMCallContext& c, EventQueue& e)
{
    ROS_INFO("FSM task: retrieve_begin");
    goal_dump.target_pose.header.stamp = ros::Time::now();
    goal_dump.target_pose.header.frame_id = "map";
    goal_dump.target_pose.pose.position.x = dumpPose.x;
    goal_dump.target_pose.pose.position.y = dumpPose.y;
    goal_dump.target_pose.pose.orientation.w = dumpPose.w;
    return TaskResult::SUCCESS();
}

TaskResult moveToDump(std::string, const FSMCallContext& c, EventQueue& e)
{
    ROS_INFO("FSM task: moveToDump");

    mbcDump->sendGoal(goal_dump);
    ROS_INFO("Dump pose sent");

    mbcDump->waitForResult(ros::Duration(200.0));
    if(mbcDump->getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        e.riseEvent("/hold"); // Higher level conditions take care of transition in top layer FSM.
        if(workMode == RETRIEVE) workMode = CLEAN;
    else
    {
        // Choose one of these two condition/event, whichever works
        e.riseEvent("/hold"); // transit to Stop state in THIS FSM
        workMode = MALFUNCTION; // transit to top layer state Malfunction
    }
    return TaskResult::SUCCESS();
}

void retrieving_init()
{
    ros::NodeHandle retrieve_nh;
    mbcDump = new MoveBaseClient(retrieve_nh, "move_base", true);

    LocalTasks::registrate("retrieve_begin", retrieve_begin);
    LocalTasks::registrate("moveToDump", moveToDump);
}