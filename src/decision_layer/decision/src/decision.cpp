#include "decision.h"

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
                FSM_ON_CONDITION(workMode == RETURNDOCK, FSM_NEXT(ReturnDock_))
                FSM_ON_CONDITION(workMode == CLEAN, FSM_NEXT(Cleaning_))
                // FSM_ON_CONDITION(workMode == RETRIEVE, FSM_NEXT(Retrieving_))
            }
        }
        FSM_STATE(ReturnDock_)
        {
            FSM_CALL_TASK(bucketLifting)
            FSM_CALL_FSM(ReturnDock)
            FSM_TRANSITIONS
            {
                FSM_ON_CONDITION(workMode == CHARGE, FSM_NEXT(Charging_)) // only set one possible transition in normal work condition
                // FSM_ON_CONDITION(workMode == CLEAN, FSM_NEXT(Cleaning_)) 
                // FSM_ON_CONDITION(workMode == RETRIEVE, FSM_NEXT(Retrieving_))
                FSM_ON_CONDITION(workMode == STANDBY, FSM_NEXT(Standby_))                
                FSM_ON_CONDITION(workMode == MALFUNCTION, FSM_NEXT(Malfunction_))

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
                FSM_ON_CONDITION(workMode == RETURNDOCK, FSM_NEXT(ReturnDock_))
                FSM_ON_CONDITION(workMode == CLEAN, FSM_NEXT(Cleaning_))
                FSM_ON_CONDITION(workMode == MALFUNCTION, FSM_NEXT(Malfunction_))
            }
        }
        FSM_STATE(Cleaning_)
        {
            FSM_CALL_TASK(bucketCollecting)
            FSM_CALL_FSM(Cleaning)
            FSM_TRANSITIONS
            {
                FSM_ON_CONDITION(workMode == RETURNDOCK, FSM_NEXT(Retrieving_)) // no matter what the user instruction, empty the bucket first
                FSM_ON_CONDITION(workMode == RETRIEVE, FSM_NEXT(Retrieving_))
                FSM_ON_CONDITION(workMode == STANDBY, FSM_NEXT(Standby_))
                FSM_ON_CONDITION(workMode == MALFUNCTION, FSM_NEXT(Malfunction_))
            }
        }
        FSM_STATE(Standby_)
        {
            // FSM_CALL_TASK(savePose) // define later, save current pose to a local script
            FSM_ON_STATE_EXIT_BGN
            {
                // FSM_CALL_TASK(readLastPose) // define later, reading saved pose info
            }
            FSM_ON_STATE_EXIT_END
            FSM_TRANSITIONS
            {
                FSM_ON_CONDITION(workMode == RETURNDOCK, FSM_NEXT(ReturnDock_))
                FSM_ON_CONDITION(workMode == CLEAN, FSM_NEXT(Cleaning_))
                FSM_ON_CONDITION(workMode == RETRIEVE, FSM_NEXT(Retrieving_))
                FSM_ON_CONDITION(workMode == CHARGE, FSM_NEXT(Charging_))
            }
        }
        FSM_STATE(Malfunction_)
        {
            FSM_CALL_TASK(sendError) // send error message
            FSM_TRANSITIONS
            {
                FSM_ON_CONDITION(workMode == STANDBY, FSM_NEXT(Standby_))
            }
        }
    }
    FSM_END
}

void decision_init()
{
    ros::NodeHandle decision_nh;
    // keybd_sub = decision_nh.subscribe<>();// wait for the app
    // goals_sub = decision_nh.subscribe<>();// wait for the app
    bucket_pub = decision_nh.advertise<decision::bucket_pose>("bucket_msgs", 10);

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
    ros::Duration(5.0).sleep();

    return TaskResult::SUCCESS();
}

TaskResult bucketCollecting(std::string, const FSMCallContext& c, EventQueue& e)
{
    bucket_msgs.bucket_joint_angle.data = BUCKET_COLLECTING;
    bucket_msgs.bucket_joint_speed.data = BUCKET_SPEED;
    return TaskResult::SUCCESS();
}

TaskResult bucketLifting(std::string, const FSMCallContext& c, EventQueue& e)
{
    bucket_msgs.bucket_joint_angle.data = BUCKET_LIFTING;
    bucket_msgs.bucket_joint_speed.data = BUCKET_SPEED;
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
    return TaskResult::SUCCESS();
}

TaskResult readLastPose(std::string, const FSMCallContext& c, EventQueue& e)
{
    // Define functionality later...
    return TaskResult::SUCCESS();
}

// void goalCB()
// {}

// void keybdCB()
// {}