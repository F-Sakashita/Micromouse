#include "TrajControlTask.h"
#include "TaskConfig.h"
#include "Odometory.hpp"
#include "Blink.hpp"
#include "main.h"
#include "DebugQueue.hpp"
#include "MessageQueueList.hpp"
#include "PidController.hpp"
#include "SystickTimer.h"

//外部参照可能なメッセージキューインスタンス
MessageQueue<OdometoryMsg_t> g_PosMsgQueue;
MessageQueue<VelControlCmdMsg_t> g_VelCmdMsgQueue;

static Odometory &g_rOdometory = Odometory::GetInstance();
static bool g_bInitialized = false;
static Blink g_DebugLed1;
static DebugQueue &g_rDebugQueue = DebugQueue::GetInstance();
static PidController g_AnglePidCon;
static bool g_bEnable = false;
static const TrajControlTask_OsFunc_t *g_pOsFunc;

bool TrajControlTask_Initialize(const TrajControlTask_OsFunc_t *pOsFunc)
{
    bool bResult = true;
    Posture_t stInitPos = {0.0f, 0.0f, 0.0f};
    bResult &= g_DebugLed1.Initialize(DBG_LED1_GPIO_Port, DBG_LED1_Pin, 1000);
    bResult &= g_rOdometory.InitializePos(TRAJ_CONTROL_TASK_SAMPLING_PERIOD_MS, stInitPos);
    bResult &= g_PosMsgQueue.Initialize(pOsFunc->OdometoryPosQueueId);
    bResult &= g_VelCmdMsgQueue.Initialize(pOsFunc->VelCmdQueueId);
    bResult &= g_AnglePidCon.Initialize(TRAJ_CONTROL_TASK_SAMPLING_PERIOD_MS, 1000.0f, PidController::EN_MODE_NORMAL, PidController::EN_PID_TYPE_POSITION);

    if(!bResult){
        return false;
    }

    g_pOsFunc = pOsFunc;
    g_AnglePidCon.SetAllGain(15.0f, 0.0f, 0.5f);
    g_DebugLed1.ForceOn();
    g_bEnable = false;
    g_bInitialized = true;
    return true;
}

void TrajControlTask_Update()
{
    uint32_t uiTick = osKernelGetTickCount();
	osDelayUntil(uiTick + TRAJ_CONTROL_TASK_SAMPLING_PERIOD_MS);
	
    //未初期化(MsgQueueも含む)なら即Return
    if(     !g_bInitialized 
        ||  !g_VelMsgQueue.IsInitialized()
        ||  !g_rDebugQueue.IsInitialized()){
        return;
    }

    if(g_bEnable){
        //OdometoryのUpdateが有効（ジャイロ補正が終わっている時）
        if(g_rOdometory.IsEnableUpdate()){
            //ロボットの速度情報をVelControlTaskからQueueより取得
            static OdometoryMsg_t stVelQueue = {
                0u,
                {0.0f, 0.0f, 0.0f},
            };

            //オドメトリによる速度メッセージキューからPop
            if(!g_VelMsgQueue.IsEmpty()){
                g_VelMsgQueue.Pop(&stVelQueue, 0);
            }

            //ロボットの位置を更新
            g_rOdometory.UpdatePosition(stVelQueue.stData);
            
            //位置情報をメッセージキューにPush
            OdometoryMsg_t stPosQueue;
            stPosQueue.stData = g_rOdometory.GetPosture();
            stPosQueue.uiTimestamp = uiTick;
            if(!g_PosMsgQueue.IsFull()){
                g_PosMsgQueue.Push(&stPosQueue, 0u);
            }

            
            //速度制御タスクに速度指令をPush
            VelControlCmdMsg_t stVelCmdMsg;
            stVelCmdMsg.uiTimestamp = uiTick;
            stVelCmdMsg.fStraightVelCmd = 0.0f;
            stVelCmdMsg.fAngVelCmd = 0.0f;

            //軌道追従制御

            static uint32_t uiTimerMs = SystickTimer_GetTimeMS();
            static uint8_t ucCount = 0u;
            if(SystickTimer_IsTimeElapsed(uiTimerMs, 5000)){
                uiTimerMs = SystickTimer_GetTimeMS();
                //ucCount ++;
                if(ucCount >= 4){
                    ucCount = 0;
                }
            }

            float fTargetAngle = 0.0f;
            switch(ucCount){
                case 0:     fTargetAngle = 0.0f;    break;
                case 1:     fTargetAngle = 90.0f;   break;
                case 2:     fTargetAngle = 0.0f;    break;
                case 3:     fTargetAngle = -90.0f;  break;
                default:    break;
            }
            fTargetAngle = Calc_ConvDegToRad(fTargetAngle);
            
            
            stVelCmdMsg.fAngVelCmd = g_AnglePidCon.GetOutput(fTargetAngle, g_rOdometory.GetAngle());
        

            if(!g_VelCmdMsgQueue.IsFull()){
                g_VelCmdMsgQueue.Push(&stVelCmdMsg, 0);
            }
            

            //Debug Consoleに出力
            #ifdef ENABLE_TRAJ_CONTROL_TASK_DEBUG_CONSOLE
            if(!g_rDebugQueue.IsFull()){
                g_rDebugQueue.Printf(0,"Cmd,%6.4f,Ang,%6.4f,CmdV,%6.4f,AngV,%6.4f", 
                                        fTargetAngle,
                                        g_rOdometory.GetAngle(),
                                        stVelCmdMsg.fAngVelCmd,
                                        g_rOdometory.GetAngleVel()
                                        );
            }
            #endif

            g_DebugLed1.SetPeriod(250u);   
        }else{
            g_DebugLed1.SetPeriod(500u);
        }
    }else{
        g_DebugLed1.SetPeriod(1000u);
    }

    g_DebugLed1.Update();
}

void TrajControlTask_Enable()
{
    g_bEnable = true;
}
void TrajControlTask_Disable()
{
    g_bEnable = false;
}
