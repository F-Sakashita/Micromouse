#include "WallDetectTask.h"
#include "FreeRTOS.h"
#include "WallSensor.h"
#include "TaskConfig.h"
#include "Blink.hpp"
#include "Button.hpp"
#include "DebugQueue.hpp"
#include "main.h"

static Blink g_Led0;
static Button g_Sw1(SW1_GPIO_Port, SW1_Pin);
static DebugQueue &g_rDebugQueue = DebugQueue::GetInstance();
static bool g_bInitialized = false;

bool WallDetectTask_Initialize(const WallDetectTask_OsFunc_t *pOsFunc)
{
    bool bResult = true;
    bResult &= g_Led0.Initialize(DBG_LED0_GPIO_Port, DBG_LED0_Pin, 500);
    bResult &= WallSensor_Initialize();
    if(!bResult){
    	return false;
    }

    g_Sw1.SetPushReverse();
    g_Led0.ForceOn();
    g_bInitialized = true;
    return bResult;
}

void WallDetectTask_Update(void)
{
    uint32_t uiTick = osKernelGetTickCount();
	osDelayUntil(uiTick + WALL_DETECT_TASK_SAMPLING_PERIOD_MS);
    
	if(!g_bInitialized){
		return;
	}

    g_Sw1.Update();
    WallSensor_Update();

    uint16_t usWallAdcValue[EN_WALLSENSOR_POS_LAST]={0u};
    for(uint8_t ucCount = EN_WALLSENSOR_POS_FIRST; ucCount < EN_WALLSENSOR_POS_LAST; ucCount ++){
        usWallAdcValue[ucCount] = WallSensor_GetValue((EN_WALLSENSOR_POS)ucCount);
    }
    
    #ifdef ENABLE_WALL_DETECT_TASK_DEBUG_CONSOLE
    if(g_rDebugQueue.IsInitialized() && !g_rDebugQueue.IsFull()){
        if(0 == g_Sw1.IsPushCount() % 2){
            
            g_rDebugQueue.Printf(0,"LS,%d,LF,%d,RF,%d,RS,%d",
                                usWallAdcValue[EN_WALLSENSOR_POS_LS],
                                usWallAdcValue[EN_WALLSENSOR_POS_LF],
                                usWallAdcValue[EN_WALLSENSOR_POS_RF],
                                usWallAdcValue[EN_WALLSENSOR_POS_RS]
                                );
             /*                   
            g_rDebugQueue.Printf(0,"RF,On,%d,Off,%d,RS,On,%d,Off,%d,LS,On,%d,Off,%d,LF,On,%d,Off,%d",
                            WallSensor_GetOnValue(EN_WALLSENSOR_POS_RF), WallSensor_GetOffValue(EN_WALLSENSOR_POS_RF),
                            WallSensor_GetOnValue(EN_WALLSENSOR_POS_RS), WallSensor_GetOffValue(EN_WALLSENSOR_POS_RS),
                            WallSensor_GetOnValue(EN_WALLSENSOR_POS_LS), WallSensor_GetOffValue(EN_WALLSENSOR_POS_LS),
                            WallSensor_GetOnValue(EN_WALLSENSOR_POS_LF), WallSensor_GetOffValue(EN_WALLSENSOR_POS_LF));
            */
            //g_Led1.SetPeriod(250, 0.5f);
        }else{
            //g_Led1.SetPeriod(1000, 0.5f);
        }
    }
    #endif

    g_Led0.Update();
}
