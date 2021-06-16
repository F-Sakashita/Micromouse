#include "WallDetectTask.hpp"
#include "WallSensor.h"
#include "main.h"

//初期化が必要なインスタンスの取得

//別タスクで初期化を行うインスタンスの取得
DebugQueue& WallDetectTask::rDebugQueue = DebugQueue::GetInstance();

//本クラスのインスタンスの取得
static WallDetectTask &g_rWallDeteckTask = WallDetectTask::GetInstance();

/* Private member functions */
WallDetectTask::WallDetectTask()
{
    enNowTopState = EN_TOP_STATE_FIRST;
}

/* Public member functions */
bool WallDetectTask::Initialize(const WallDetectTask_OsFunc_t *pOsFunc)
{
    bool bResult = true;
    //bResult &= g_Led0.Initialize(DBG_LED0_GPIO_Port, DBG_LED0_Pin, 500);
    bResult &= WallSensor_Initialize();
    if(!bResult){
    	return false;
    }

    bInitialized = true;
    return bResult;
}
void WallDetectTask::Update()
{
    uint32_t uiTick = osKernelGetTickCount();
	osDelayUntil(uiTick + WALL_DETECT_TASK_SAMPLING_PERIOD_MS);
    
	if(     !bInitialized
        ||  !rDebugQueue.IsInitialized()){
		return;
	}

    WallSensor_Update();

    uint16_t usWallAdcValue[EN_WALLSENSOR_POS_LAST]={0u};
    for(uint8_t ucCount = EN_WALLSENSOR_POS_FIRST; ucCount < EN_WALLSENSOR_POS_LAST; ucCount ++){
        usWallAdcValue[ucCount] = WallSensor_GetValue((EN_WALLSENSOR_POS)ucCount);
    }
    
    #ifdef ENABLE_WALL_DETECT_TASK_DEBUG_CONSOLE
    if(!rDebugQueue.IsFull()){
    	rDebugQueue.Printf(0,"LS,%d,LF,%d,RF,%d,RS,%d",
                                usWallAdcValue[EN_WALLSENSOR_POS_LS],
                                usWallAdcValue[EN_WALLSENSOR_POS_LF],
                                usWallAdcValue[EN_WALLSENSOR_POS_RF],
                                usWallAdcValue[EN_WALLSENSOR_POS_RS]
                                );
    }
    #endif
}

bool WallDetectTask_Initialize(const WallDetectTask_OsFunc_t *pOsFunc)
{
    return g_rWallDeteckTask.Initialize(pOsFunc);
}

void WallDetectTask_Update(void)
{
    g_rWallDeteckTask.Update();
}
