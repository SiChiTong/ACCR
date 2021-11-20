#include "returnDock.h"

#define ANG_TOL 0.01
#define DIST_TOL 0.0001

FSM(ReturnDock)
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
                FSM_ON_EVENT("/return", FSM_NEXT(Moving))
            }
        }
        FSM_STATE(Moving)
        {
            FSM_CALL_TASK(moveToDock)
            FSM_TRANSITIONS
            {
                FSM_ON_EVENT("/stop", FSM_NEXT(Stop))
            }
        }
    }
    FSM_END
}

TaskResult returnDock_begin(std::string, const FSMCallContext& c, EventQueue& e)
{
    ROS_INFO("FSM task: returnDock_begin");
    double dist = (dockPose.x - default_docePose.x)*(dockPose.x - default_docePose.x) + 
        (dockPose.y - default_docePose.y)*(dockPose.y - default_docePose.y);
    double ang_diff = dockPose.w - default_docePose.w;
    if(dist < DIST_TOL && ang_diff < ANG_TOL)
    {
        e.riseEvent("/return");
    }else if(dist < DIST_TOL && ang_diff >= ANG_TOL)
    {
        ROS_INFO("Orientation of current dock pose is off from the default dock pose, returning to current dock pose");
        e.riseEvent("/return");
    }else if(dist >= DIST_TOL && ang_diff < ANG_TOL)
    {
        ROS_INFO("Position of current dock pose is off from the default dock pose, returning to current dock pose");
        e.riseEvent("/return");
    }else
    {
        ROS_INFO("Position and orientation of current dock pose is off from the default dock pose, returning to current dock pose");
        e.riseEvent("/return");
    }
    return TaskResult::SUCCESS();
}

TaskResult moveToDock(std::string, const FSMCallContext& c, EventQueue& e)
{
    ROS_INFO("FSM task: moveToDock");
    goal_dock.target_pose.header.stamp = ros::Time::now();
    goal_dock.target_pose.header.frame_id = "map";
    goal_dock.target_pose.pose.position.x = dockPose.x;
    goal_dock.target_pose.pose.position.y = dockPose.y;
    goal_dock.target_pose.pose.orientation.w = dockPose.w;

    mbcDock->sendGoal(goal_dock);
    ROS_INFO("Dock pose sent");

    mbcDock->waitForResult(ros::Duration(200.0));
    if(mbcDock->getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        e.riseEvent("/stop"); // Higher level conditions take care of transition in top layer FSM.
    else
    {
        // Choose one of these two condition/event, whichever works
        e.riseEvent("/stop"); // transit to Stop state in THIS FSM
        workMode = MALFUNCTION; // transit to top layer state Malfunction
    }
    return TaskResult::SUCCESS();
}

void returnDock_init()
{
    ros::NodeHandle returnDock_nh;
    mbcDock = new MoveBaseClient(returnDock_nh, "move_base", true);

    LocalTasks::registrate("returnDock_begin", returnDock_begin);
    LocalTasks::registrate("moveToDock", moveToDock);
}