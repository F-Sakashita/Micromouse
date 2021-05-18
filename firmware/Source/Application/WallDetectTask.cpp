#include "WallDetectTask.h"
#include "DebugQueue.h"
#include "FreeRTOS.h"
#include "WallSensor.h"
#include "TaskConfig.h"

bool WallDetectTask_Initialize()
{
    bool bResult = false;
    bResult = WallSensor_Initialize();
    return bResult;
}

void WallDetectTask_Update(void)
{
    uint32_t uiTick = osKernelGetTickCount();
	osDelayUntil(uiTick + WALL_DETECT_TASK_SAMPLING_PERIOD_MS);

    WallSensor_Update();

    uint16_t usWallAdcValue[EN_WALLSENSOR_POS_LAST]={0u};
    for(uint8_t ucCount = EN_WALLSENSOR_POS_FIRST; ucCount < EN_WALLSENSOR_POS_LAST; ucCount ++){
        usWallAdcValue[ucCount] = WallSensor_GetValue((EN_WALLSENSOR_POS)ucCount);
    }
    /*
    if(!DebugQueue_IsFull()){
        DebugQueue_SetPrintf(0,0,"LS,%d,LF,%d,RF,%d,RS,%d",
                                usWallAdcValue[EN_WALLSENSOR_POS_LS],
                                usWallAdcValue[EN_WALLSENSOR_POS_LF],
                                usWallAdcValue[EN_WALLSENSOR_POS_RF],
                                usWallAdcValue[EN_WALLSENSOR_POS_RS]
                                );
    }*/
}