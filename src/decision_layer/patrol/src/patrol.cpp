#include "patrol.h"

FSM(Patrol){
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
            FSM_CALL_TASK(patrol_begin) 
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