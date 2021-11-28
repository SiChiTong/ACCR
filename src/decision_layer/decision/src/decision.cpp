#include "decision.h"
#include <cmath>

FSM(Decision)
{
    FSM_STATES
    {
        Charging_,
        Cleaning_,
        Retrieving_,
        ReturnDock_,
        Standby_,
        Malfunction_
    }
    FSM_START(Standby_);
    FSM_BGN
    {
        FSM_STATE(Charging_)
        {
            FSM_CALL_TASK(bucketLifting)
            FSM_TRANSITIONS
            {
                FSM_ON_EVENT(RETURNDOCK, FSM_NEXT(ReturnDock_))
                FSM_ON_EVENT(CLEAN, FSM_NEXT(Cleaning_))
                // FSM_ON_EVENT(RETRIEVE, FSM_NEXT(Retrieving_))
            }
        }
        FSM_STATE(ReturnDock_)
        {
            FSM_CALL_TASK(bucketLifting)
            FSM_CALL_FSM(ReturnDock)
            FSM_TRANSITIONS
            {
                FSM_ON_EVENT(CHARGE, FSM_NEXT(Charging_)) // only set one possible transition in normal work condition
                // FSM_ON_EVENT(CLEAN, FSM_NEXT(Cleaning_)) 
                // FSM_ON_EVENT(RETRIEVE, FSM_NEXT(Retrieving_))
                FSM_ON_EVENT(STANDBY, FSM_NEXT(Standby_))                
                FSM_ON_EVENT(MALFUNCTION, FSM_NEXT(Malfunction_))
                // FSM_ON_EVENT(RETURNDOCK, FSM_NEXT(ReturnDock_))
            }
        }
        FSM_STATE(Retrieving_)
        {
            FSM_CALL_TASK(bucketLifting)
            FSM_CALL_FSM(Retrieving)
            FSM_ON_STATE_EXIT_BGN
            {
                FSM_CALL_TASK(bucketEmpty)
            }
            FSM_ON_STATE_EXIT_END
            FSM_TRANSITIONS
            {
                FSM_ON_EVENT(RETURNDOCK, FSM_NEXT(ReturnDock_))
                FSM_ON_EVENT(CLEAN, FSM_NEXT(Cleaning_))
                FSM_ON_EVENT(MALFUNCTION, FSM_NEXT(Malfunction_))
                // FSM_ON_EVENT(RETRIEVE, FSM_NEXT(Retrieving_))
            }
        }
        FSM_STATE(Cleaning_)
        {
            FSM_CALL_TASK(bucketCollecting)
            FSM_CALL_FSM(Cleaning)
            FSM_TRANSITIONS
            {
                FSM_ON_EVENT(RETURNDOCK, FSM_NEXT(Retrieving_)) // no matter what the user instruction, empty the bucket first
                FSM_ON_EVENT(RETRIEVE, FSM_NEXT(Retrieving_))
                FSM_ON_EVENT(STANDBY, FSM_NEXT(Standby_))
                FSM_ON_EVENT(MALFUNCTION, FSM_NEXT(Malfunction_))
                // FSM_ON_EVENT(CLEAN, FSM_NEXT(Cleaning_))
            }
        }
        FSM_STATE(Standby_)
        {
            FSM_CALL_TASK(savePose) // define later, save current pose to a local script
            FSM_ON_STATE_EXIT_BGN
            {
                FSM_CALL_TASK(readLastPose) // define later, reading saved pose info
            }
            FSM_ON_STATE_EXIT_END
            FSM_TRANSITIONS
            {
                FSM_ON_EVENT(RETURNDOCK, FSM_NEXT(ReturnDock_))
                FSM_ON_EVENT(CLEAN, FSM_NEXT(Cleaning_)) // try pub event in terminal
                FSM_ON_EVENT(RETRIEVE, FSM_NEXT(Retrieving_))
                FSM_ON_EVENT(CHARGE, FSM_NEXT(Charging_))
                // FSM_ON_EVENT(STANDBY, FSM_NEXT(Standby_))
            }
        }
        FSM_STATE(Malfunction_)
        {
            FSM_CALL_TASK(sendError) // send error message
            FSM_TRANSITIONS
            {
                FSM_ON_EVENT(STANDBY, FSM_NEXT(Standby_))
            }
        }
    }
    FSM_END
}

void decision_init()
{
    ros::NodeHandle decision_nh;
    bucket_pub = decision_nh.advertise<decision::Bucket_pose>("bucket_msgs", 10);
    instruction_sub = decision_nh.subscribe<decision::Instruction>("instruct", 10, instructCB);

    LocalTasks::registrate("bucketEmpty", bucketEmpty);
    LocalTasks::registrate("bucketCollecting", bucketCollecting);
    LocalTasks::registrate("bucketLifting", bucketLifting);
    LocalTasks::registrate("sendError", sendError);
    LocalTasks::registrate("readLastPose", readLastPose);
    LocalTasks::registrate("savePose", savePose);
}

TaskResult bucketEmpty(std::string, const FSMCallContext& c, EventQueue& e)
{
    bucket_msgs.bucket_joint_angle.data = BUCKET_EMPTY;
    bucket_msgs.bucket_joint_speed.data = BUCKET_SPEED;
    ROS_INFO("bucket empty");
    ros::Duration(5.0).sleep();

    return TaskResult::SUCCESS();
}

TaskResult bucketCollecting(std::string, const FSMCallContext& c, EventQueue& e)
{
    bucket_msgs.bucket_joint_angle.data = BUCKET_COLLECTING;
    bucket_msgs.bucket_joint_speed.data = BUCKET_SPEED;
    ROS_INFO("bucket in cleaning position");
    return TaskResult::SUCCESS();
}

TaskResult bucketLifting(std::string, const FSMCallContext& c, EventQueue& e)
{
    bucket_msgs.bucket_joint_angle.data = BUCKET_LIFTING;
    bucket_msgs.bucket_joint_speed.data = BUCKET_SPEED;
    ROS_INFO("bucket lifted");
    return TaskResult::SUCCESS();
}

TaskResult sendError(std::string, const FSMCallContext& c, EventQueue& e)
{
    ROS_WARN("Some error occured!");
    return TaskResult::SUCCESS();
}

TaskResult savePose(std::string, const FSMCallContext& c, EventQueue& e)
{
    // Define functionality later...
    ROS_INFO("standby");
    return TaskResult::SUCCESS();
}

TaskResult readLastPose(std::string, const FSMCallContext& c, EventQueue& e)
{
    // Define functionality later...
    ROS_INFO("exit standby, workMode = %d", workMode);
    return TaskResult::SUCCESS();
}

void instructCB(const decision::Instruction::ConstPtr& instruct_msg)
{
    // mode_num = instruct_msg -> mode.data;
    RosEventQueue event;
    
    if(instruct_msg -> mode.data >= 0 && instruct_msg -> mode.data <= 5)
    {
        workMode = static_cast<modes>(instruct_msg -> mode.data);
        if(instruct_msg -> mode.data == 0) event.riseEvent("/Standby");
        else if(instruct_msg -> mode.data == 1) event.riseEvent("/Clean");
        else if(instruct_msg -> mode.data == 2) event.riseEvent("/Retrieve");
        else if(instruct_msg -> mode.data == 3) event.riseEvent("/ReturnDock");
        else if(instruct_msg -> mode.data == 4) event.riseEvent("/Charge");
        else event.riseEvent("/Malfunction");

        ROS_INFO("workMode changed to: %d", workMode);
        goal_point.x = instruct_msg -> goal.x;
        goal_point.y = instruct_msg -> goal.y;
        goal_point.w = cos((instruct_msg -> goal.theta)/2.0);
    }
    else ROS_WARN("Invalid work mode input!");
}

// void keybdCB()
// {}