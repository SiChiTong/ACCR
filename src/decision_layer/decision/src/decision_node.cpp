#include "decision.h"
#include "cleaning.h"
#include "retrieving.h"
#include "returnDock.h"

int main(int argc, char **argv)
{
    goal_points[0].x = 3;
    goal_points[0].y = 1;
    goal_points[1].x = 3;
    goal_points[1].y = 1;
    goal_points[2].x = 5;
    goal_points[2].y = 1.5;
    
    ros::init(argc, argv, "Decision");
    ros_decision_making_init(argc, argv);

    ROS_INFO("Preparing cleaning...");

    RosEventQueue eventQueue;
    cleaning_init();
    retrieving_init();
    returnDock_init();
    decision_init();

    ros::AsyncSpinner spinner(1);
    spinner.start();
    // Take a closer look to class AsynSpinner, spinner.stop() etc. 

    FsmDecision(NULL, &eventQueue);

}