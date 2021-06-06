#include "MainTask.h"
#include "Odometory.hpp"
#include "TaskConfig.h"
#include "Calculation.h"
#include "Button.hpp"
#include "Blink.hpp"
#include "main.h"
#include "DebugQueue.hpp"
#include "MessageQueueType.h"
#include "SystickTimer.h"
#include "TrajControlTask.h"
#include "VelControlTask.h"


MessageQueue<PosControlCmdMsg_t> g_PosCmdMsgQueue;
MessageQueue<bool> g_MotionStartMsgQueue;

static Button g_Sw0(SW0_GPIO_Port, SW0_Pin);
static Blink g_TickLed;
static bool g_bInitialized = false;
static DebugQueue& g_rDebugQueue = DebugQueue::GetInstance();
static bool g_bEnable = false;
static const MainTask_OsFunc_t*	g_pOsFunc;

static void MainTask_SetOtherTaskEnable(bool bEnable)
{
    if(bEnable){
        VelControlTask_Enable();
        TrajControlTask_Enable();
        g_bEnable = true;
    }else{
        VelControlTask_Disable();
        TrajControlTask_Disable();
        g_bEnable= false;
    }
}


bool MainTask_Initialize(const MainTask_OsFunc_t *pOsFunc)
{
    bool bResult = true;

    bResult &= g_TickLed.Initialize(TICK_LED_GPIO_Port, TICK_LED_Pin, 1000);
    bResult &= g_PosCmdMsgQueue.Initialize(pOsFunc->PosCmdQueueId);
    
    if(!bResult){
        return false;
    }

    g_pOsFunc = pOsFunc;
    g_Sw0.SetPushReverse();
    g_TickLed.ForceOn();
   
    g_bInitialized = true;
    return true;
}

void MainTask_Update()
{
    uint32_t uiTick = osKernelGetTickCount();
	osDelayUntil(uiTick + MAIN_TASK_SAMPLING_PERIOD_MS);
    
    if(     !g_bInitialized 
        ||  !g_rDebugQueue.IsInitialized()){
        return;
    }

    g_Sw0.Update();

    uint32_t uiSwCount = g_Sw0.IsPushCount();
    static bool bMotionStartFlag = false;
    static uint32_t uiTimerMs = SystickTimer_GetTimeMS();
    //動作開始
    if(1u == uiSwCount % 2){
        if(g_Sw0.IsReleaseEdge() && !bMotionStartFlag){
            bMotionStartFlag = true;
            uiTimerMs = SystickTimer_GetTimeMS();
        }
    }else{

        MainTask_SetOtherTaskEnable(false);
    }

    //動作開始フラグが有効
    if(bMotionStartFlag){
        //5秒後に動作開始
        if(SystickTimer_IsTimeElapsed(uiTimerMs, 5000)){
            MainTask_SetOtherTaskEnable(true);
            bMotionStartFlag = false;
        }
    }

    static uint32_t uiConsoleOutputTimeMs = SystickTimer_GetTimeMS();

    if(g_bEnable){
    	g_TickLed.SetPeriod(250);
        uiConsoleOutputTimeMs = SystickTimer_GetTimeMS();
    }else if(bMotionStartFlag && 1u == uiSwCount % 2){
    	g_TickLed.SetPeriod(500);

        uiConsoleOutputTimeMs = SystickTimer_GetTimeMS();

        #ifdef ENABLE_MAIN_TASK_DEBUG_CONSOLE
        if(!g_rDebugQueue.IsFull()){
            g_rDebugQueue.Printf(0,"Waiting for Start, Progress: %3.1f [%%]", static_cast<float>(SystickTimer_GetTimeMS() - uiTimerMs) / 50.0f);
        }
        #endif
    }else{
    	g_TickLed.SetPeriod(1000);
    
        if(SystickTimer_IsTimeElapsed(uiConsoleOutputTimeMs, 1000)){
            if(!g_rDebugQueue.IsFull()){
                g_rDebugQueue.Printf(0,"Waiting for Start switch pushed ...");
            }
            uiConsoleOutputTimeMs = SystickTimer_GetTimeMS();
        }
    }

    #ifdef ENABLE_MAIN_TASK_DEBUG_CONSOLE
    if(!g_rDebugQueue.IsFull()){
            g_rDebugQueue.Printf(0,"sw0:%d",uiSwCount);
    }
    #endif

    g_TickLed.Update();
}
