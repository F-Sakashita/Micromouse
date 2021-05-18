#include "ApplicationTask.h"
#include "Odometory.hpp"
#include "DebugQueue.h"
#include "TaskConfig.h"
#include "Calculation.h"

static Odometory &rOdometory = Odometory::GetInstance();
static bool bInitialized = false;

bool ApplicationTask_Initialize()
{
    bool bResult = true;
    Posture_t stInitPos = {0.0f, 0.0f, 0.0f};
    bResult &= rOdometory.Initialize(APP_TASK_SAMPLING_PERIOD_MS, stInitPos);
    if(!bResult){
        return false;
    }
    bInitialized = true;
    return true;
}

void ApplicationTask_Update()
{
    uint32_t uiTick = osKernelGetTickCount();
	osDelayUntil(uiTick + APP_TASK_SAMPLING_PERIOD_MS);
    
	
    if(!bInitialized){
        return;
    }

    rOdometory.Update();
    if(!DebugQueue_IsFull()){
        if(rOdometory.IsEnableUpdate()){
            
            DebugQueue_SetPrintf(0,0,"PosX,%6.3f,PosY,%6.3f,Deg,%6.3f",
                                        rOdometory.GetPosture().fValueX,
                                        rOdometory.GetPosture().fValueY,
                                        Calc_ConvRadToDeg(rOdometory.GetPosture().fRad)
                                        );
                                        
        }else{
            //DebugQueue_SetPrintf(0,0,"Gyro calibrating...");
        }
    }
	//DebugQueue_SetPrintf(0,0,"Gyro calibrating...");
}
