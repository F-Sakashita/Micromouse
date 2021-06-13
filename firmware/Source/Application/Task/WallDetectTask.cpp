#include "WallDetectTask.h"
#include "FreeRTOS.h"
#include "WallSensor.h"
#include "TaskConfig.h"
#include "Blink.hpp"
#include "Button.hpp"
#include "DebugQueue.hpp"
#include "main.h"

static DebugQueue &g_rDebugQueue = DebugQueue::GetInstance();
static bool g_bInitialized = false;

bool WallDetectTask_Initialize(const WallDetectTask_OsFunc_t *pOsFunc)
{
    bool bResult = true;
    //bResult &= g_Led0.Initialize(DBG_LED0_GPIO_Port, DBG_LED0_Pin, 500);
    bResult &= WallSensor_Initialize();
    if(!bResult){
    	return false;
    }

    g_bInitialized = true;
    return bResult;
}

void WallDetectTask_Update(void)
{
    uint32_t uiTick = osKernelGetTickCount();
	osDelayUntil(uiTick + WALL_DETECT_TASK_SAMPLING_PERIOD_MS);
    
	if(     !g_bInitialized
        ||  !g_rDebugQueue.IsInitialized()){
		return;
	}

    WallSensor_Update();

    uint16_t usWallAdcValue[EN_WALLSENSOR_POS_LAST]={0u};
    for(uint8_t ucCount = EN_WALLSENSOR_POS_FIRST; ucCount < EN_WALLSENSOR_POS_LAST; ucCount ++){
        usWallAdcValue[ucCount] = WallSensor_GetValue((EN_WALLSENSOR_POS)ucCount);
    }
    
    #ifdef ENABLE_WALL_DETECT_TASK_DEBUG_CONSOLE
    if(!g_rDebugQueue.IsFull()){
        g_rDebugQueue.Printf(0,"LS,%d,LF,%d,RF,%d,RS,%d",
                                usWallAdcValue[EN_WALLSENSOR_POS_LS],
                                usWallAdcValue[EN_WALLSENSOR_POS_LF],
                                usWallAdcValue[EN_WALLSENSOR_POS_RF],
                                usWallAdcValue[EN_WALLSENSOR_POS_RS]
                                );
    }
    #endif

    //g_Led0.Update();
}
