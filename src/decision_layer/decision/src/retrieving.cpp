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
            FSM_CALL_TASK(retrieve_begin)
            FSM_TRANSITIONS
            {
                FSM_ON_EVENT("/dump", FSM_NEXT(Moving))
            }
        }
        FSM_STATE(Moving)
        {
            FSM_CALL_TASK(moveToDump)
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
    goal_dump.target_pose.pose.position.x = goal_point.x;
    goal_dump.target_pose.pose.position.y = goal_point.y;
    goal_dump.target_pose.pose.orientation.w = goal_point.w;

    e.riseEvent("/dump");
    return TaskResult::SUCCESS();
}

TaskResult moveToDump(std::string, const FSMCallContext& c, EventQueue& e)
{
    ROS_INFO("FSM task: moveToDump");

    mbcDump->sendGoal(goal_dump);
    ROS_INFO("Dump pose sent");

    mbcDump->waitForResult(ros::Duration(TIMEOUT));
    if(mbcDump->getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        if(workMode == RETURNDOCK1) e.riseEvent("/ReturnDock");
        else e.riseEvent("/Clean");
    else
    {
        // Do not need to return to default state manually 
        e.riseEvent(MALFUNCTION);
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