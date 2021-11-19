#include "cleaning.h"

// define the bucket states in milli radian
#define BUCKET_EMPTY 1047 // 60 degree
#define BUCKET_COLLECTING 524 // 30 degree
#define BUCKET_LIFTING   0 // 0 degree
#define BUCKET_SPEED 768 // 44 degree/s

FSM(Cleaning)
{
    FSM_STATES
    {
        Stop,
        Reached,
        Moving
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
                FSM_ON_EVENT("/timeOut", FSM_NEXT(Stop))
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
    }
    FSM_END
}

TaskResult cleaning_begin(std::string, const FSMCallContext& c, EventQueue& e)
{
    ROS_INFO("FSM task: cleaning_begin");
    if(i < POINT_NUM-1) e.riseEvent("/start");
    
    return TaskResult::SUCCESS();
}

TaskResult nextGoal(std::string, const FSMCallContext& c, EventQueue& e)
{
    ROS_INFO("FSM task: nextGoal");
    if(i < POINT_NUM-1)
    {
        i++;
        ROS_INFO("i = %d", i);
        if(goal_points[i].clear == 0)
        {
            bucket_msgs.bucket_joint_angle.data = BUCKET_EMPTY;
            bucket_msgs.bucket_joint_speed.data = BUCKET_SPEED;
        }else if(goal_points[i].clear == 1)
        {
            bucket_msgs.bucket_joint_angle.data = BUCKET_COLLECTING;
            bucket_msgs.bucket_joint_speed.data = BUCKET_SPEED;
        }else if(goal_points[i].clear == 2)
        {
            bucket_msgs.bucket_joint_angle.data = BUCKET_LIFTING;
            bucket_msgs.bucket_joint_speed.data = BUCKET_SPEED;
        }
        bucket_pub.publish(bucket_msgs);
        e.riseEvent("/more_goal");
    }
    else e.riseEvent("/no_goal");

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
        e.riseEvent("/timeOut");
    
    return TaskResult::SUCCESS();
}