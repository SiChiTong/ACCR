#include "cleaning.h"
// #include <boost/bind.hpp>
// define the bucket states in milli radian
// #define BUCKET_EMPTY 1047 // 60 degree
// #define BUCKET_COLLECTING 524 // 30 degree
// #define BUCKET_LIFTING   0 // 0 degree
// #define BUCKET_SPEED 768 // 44 degree/s

FSM(Cleaning)
{
    FSM_STATES
    {
        Stop,
        Reached,
        Moving,
        Error
        // Malfunction
    }
    FSM_START(Stop);
    FSM_BGN
    {
        FSM_STATE(Stop)
        {
            FSM_CALL_TASK(cleaning_begin) 
            FSM_TRANSITIONS
            {
                FSM_ON_EVENT("/start", FSM_NEXT(Moving)) 
            }
        }
        FSM_STATE(Moving)
        {
            FSM_CALL_TASK(moveToGoal)
            FSM_TRANSITIONS
            {
                FSM_ON_EVENT("/arrived", FSM_NEXT(Reached)) 
                FSM_ON_EVENT("/timeOut", FSM_NEXT(Error))// return to top layer state
            }
        }
        FSM_STATE(Reached)
        {
            FSM_CALL_TASK(nextGoal) 
            FSM_TRANSITIONS
            {
                FSM_ON_EVENT("/more_goal", FSM_NEXT(Moving)) 
                FSM_ON_EVENT("/no_goal", FSM_NEXT(Stop)) 
            }
        }
        FSM_STATE(Error)
        {
            FSM_CALL_TASK(reportWarning)
            FSM_TRANSITIONS
        }
    }
    FSM_END
}

TaskResult cleaning_begin(std::string, const FSMCallContext& c, EventQueue& e)
{
    ROS_INFO("FSM task: cleaning_begin");
    if(i < pointNum-1) e.riseEvent("/start");
    
    return TaskResult::SUCCESS();
}

TaskResult nextGoal(std::string, const FSMCallContext& c, EventQueue& e)
{
    ROS_INFO("FSM task: nextGoal");
    if(i < pointNum-1)
    {
        i++;
        ROS_INFO("i = %d", i);
        // if(goal_points[i].clear == 0)
        // {
        //     bucket_msgs.bucket_joint_angle.data = BUCKET_EMPTY;
        //     bucket_msgs.bucket_joint_speed.data = BUCKET_SPEED;
        // }else if(goal_points[i].clear == 1)
        // {
        //     bucket_msgs.bucket_joint_angle.data = BUCKET_COLLECTING;
        //     bucket_msgs.bucket_joint_speed.data = BUCKET_SPEED;
        // }else if(goal_points[i].clear == 2)
        // {
        //     bucket_msgs.bucket_joint_angle.data = BUCKET_LIFTING;
        //     bucket_msgs.bucket_joint_speed.data = BUCKET_SPEED;
        // }
        // bucket_pub.publish(bucket_msgs);
        e.riseEvent("/more_goal");
    }
    else 
    {
        e.riseEvent("/no_goal");
        if(workMode == CLEAN) workMode = RETRIEVE;
    }
    return TaskResult::SUCCESS();
}

TaskResult moveToGoal(std::string, const FSMCallContext& c, EventQueue& e)
{
    ROS_INFO("FSM task: moveToGoal");
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.pose.position.x = goal_points[i].x;
    goal.target_pose.pose.position.y = goal_points[i].y;
    goal.target_pose.pose.orientation.w = 1.0;

    mbc->sendGoal(goal);
    ROS_INFO("goal sent");

    mbc->waitForResult(ros::Duration(100.0));
    if(mbc->getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        e.riseEvent("/arrived");
    else
    {
        // Choose one of these two condition/event, whichever works
        e.riseEvent("/timeOut"); // transit to error state in THIS FSM
        workMode = MALFUNCTION; // transit to top layer state Malfunction
    }
    return TaskResult::SUCCESS();
}

// TaskResult reportWarning(std::string, const FSMCallContext& c, EventQueue& e)
// {
//     ROS_WARN("Robot jammed, please check on site");
//     return TaskResult::SUCCESS();
// }

void cleaning_init()
{
    ros::NodeHandle cleaning_nh;
    mbc = new MoveBaseClient(cleaning_nh, "move_base", true);
    // bucket_pub = nh.advertise<cleaning::bucket_pose>("bucket_msgs",10);

    LocalTasks::registrate("moveToGoal", moveToGoal);
    LocalTasks::registrate("nextGoal", nextGoal);
    LocalTasks::registrate("cleaning_begin", cleaning_begin);

    // ros::AsyncSpinner spinner(1); // needed in all fsm init?
    // spinner.start();

    // FsmCleaning(NULL, &eventQueue);

}