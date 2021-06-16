#include "TrajControlTask.hpp"
#include "TaskList.h"
#include "VelControlTask.hpp"
#include "MainTask.hpp"
#include "TaskConfig.h"
#include "MessageQueueList.hpp"

//初期化が必要なインスタンスの取得
Odometory& TrajControlTask::rOdometory = Odometory::GetInstance();

//別タスクで初期化を行うインスタンスの取得
DebugQueue& TrajControlTask::rDebugQueue = DebugQueue::GetInstance();
MessageQueue<OdometoryMsg_t>& TrajControlTask::rVelMsgQueue = VelControlTask::GetInstance().GetVelMsgQueueInst();
MessageQueue<PosControlCmdMsg_t>& TrajControlTask::rPosCmdMsgQueue = MainTask::GetInstance().GetPosCmdMsgQueueInst();

//本クラスのインスタンスの取得
static TrajControlTask& g_rTrajControlTask = TrajControlTask::GetInstance();

/* Private member functions */
TrajControlTask::TrajControlTask()
{
    enNowTopState = EN_TOP_STATE_FIRST;
    bResetPosCompleted = false;
    bStartResetPosFlag = false;
}

void TrajControlTask::InputUpdate()
{
    static OdometoryMsg_t stVelQueue = {
        0u,
        {0.0f, 0.0f, 0.0f},
    };
    switch (enNowTopState)
    {
    case EN_TOP_STATE_CALIBRATING:
    case EN_TOP_STATE_RUNNING:
        //ロボットの速度情報をVelControlTaskからQueueより取得
        //オドメトリによる速度メッセージキューからPop
        if(!rVelMsgQueue.IsEmpty()){
            rVelMsgQueue.Pop(&stVelQueue, 0);
        }

        //ロボットの位置を更新
        rOdometory.UpdatePosition(stVelQueue.stData);

        break;
    
    default:
        break;
    }
}

void TrajControlTask::OutputUpdate()
{

}

/* Public member functions */
bool TrajControlTask::Initialize(const TrajControlTask_OsFunc_t *pOsFunc)
{
    bool bResult = true;
    Posture_t stInitPosture = {
        0.0f,0.0f,0.0f
    };
    bResult &= rOdometory.InitializePos(TRAJ_CONTROL_TASK_SAMPLING_PERIOD_MS, stInitPosture);
    bResult &= PosMsgQueue.Initialize(pOsFunc->OdometoryPosQueueId);
    bResult &= VelCmdMsgQueue.Initialize(pOsFunc->VelCmdQueueId);
    bResult &= AnglePidCon.Initialize(TRAJ_CONTROL_TASK_SAMPLING_PERIOD_MS, 1000.0f, PidController::EN_MODE_NORMAL, PidController::EN_PID_TYPE_POSITION);
    this->pOsFunc = (TrajControlTask_OsFunc_t *)pOsFunc;
    if(!bResult){
        return false;
    }
    
    AnglePidCon.SetAllGain(40.0f, 0.0f, 2.0f);
    bInitialized = true;
    return true;
}

void TrajControlTask::Update()
{
    uint32_t uiTick = osKernelGetTickCount();
	osDelayUntil(uiTick + TRAJ_CONTROL_TASK_SAMPLING_PERIOD_MS);
	
    //未初期化(MsgQueueも含む)なら即Return
    if(     !bInitialized 
        ||  !rVelMsgQueue.IsInitialized()
        ||  !rPosCmdMsgQueue.IsInitialized()
        ||  !rDebugQueue.IsInitialized()){
        return;
    }

    InputUpdate();

    //受信するメッセージ
    static PosControlCmdMsg_t stPosCmdMsg;
    
    //発信するメッセージ
    OdometoryMsg_t stPosQueue;
    VelControlCmdMsg_t stVelCmdMsg;

    switch (enNowTopState)
    {
    case EN_TOP_STATE_CALIBRATING:
        //osMutexAcquire(pOsFunc->ResetPosMutexId, 0);
        if(bStartResetPosFlag){
            //位置のリセットフラグが立っている時，オドメトリのキャルブレが完了してる場合はリセットする
            if(rOdometory.IsEnableUpdate()){
                rOdometory.ResetPosture(stRequestResetPosture);
                bStartResetPosFlag = false;
                bResetPosCompleted = true;
            }
        }
        //osMutexRelease(pOsFunc->ResetPosMutexId);
        break;
    case EN_TOP_STATE_RUNNING:
        //位置情報をメッセージキューにPush
        stPosQueue.stData = rOdometory.GetPosture();
        stPosQueue.uiTimestamp = uiTick;
        if(!PosMsgQueue.IsFull()){
            PosMsgQueue.Push(&stPosQueue, 0u);
        }

        //MainTaskからの目標値を取得
        if(!rPosCmdMsgQueue.IsEmpty()){
            rPosCmdMsgQueue.Pop(&stPosCmdMsg, 0);
        }

        //軌道追従制御
        stVelCmdMsg.fStraightVelCmd = 0.0f;
        stVelCmdMsg.fAngVelCmd = AnglePidCon.GetOutput(stPosCmdMsg.stTargetPos.fRad, rOdometory.GetAngle());

        //速度制御タスクに速度指令をPush
        if(!VelCmdMsgQueue.IsFull()){
            VelCmdMsgQueue.Push(&stVelCmdMsg, 0);
        }            

        //Debug Consoleに出力
        #ifdef ENABLE_TRAJ_CONTROL_TASK_DEBUG_CONSOLE
        if(!rDebugQueue.IsFull()){
            rDebugQueue.Printf(0,"Cmd,%6.4f,Ang,%6.4f,CmdV,%6.4f",
            						stPosCmdMsg.stTargetPos.fRad,
                                    rOdometory.GetAngle(),
                                    stVelCmdMsg.fAngVelCmd
                                    );
        }
        #endif

        break;
    default:
        break;
    }

    OutputUpdate();
}

bool TrajControlTask_Initialize(const TrajControlTask_OsFunc_t *pOsFunc)
{
    return g_rTrajControlTask.Initialize(pOsFunc);
}

void TrajControlTask_Update()
{
    return g_rTrajControlTask.Update();
}


#if 0
#include "TrajControlTask.h"
#include "TaskConfig.h"
#include "Odometory.hpp"
#include "Blink.hpp"
#include "main.h"
#include "DebugQueue.hpp"
#include "MessageQueueList.hpp"
#include "PidController.hpp"
#include "SystickTimer.h"
#include "VelControlTask.hpp"
#include "TaskList.h"

//外部参照可能なメッセージキューインスタンス
MessageQueue<OdometoryMsg_t> g_PosMsgQueue;
MessageQueue<VelControlCmdMsg_t> g_VelCmdMsgQueue;

static Odometory &g_rOdometory = Odometory::GetInstance();
static bool g_bInitialized = false;
static DebugQueue &g_rDebugQueue = DebugQueue::GetInstance();
static PidController g_AnglePidCon;
static bool g_bEnable = false;
static const TrajControlTask_OsFunc_t *g_pOsFunc;
static MessageQueue<OdometoryMsg_t> &g_rVelMsgQueue = VelControlTask::GetInstance().GetVelMsgQueueInst();

bool TrajControlTask_Initialize(const TrajControlTask_OsFunc_t *pOsFunc)
{
    bool bResult = true;
    Posture_t stInitPos = {0.0f, 0.0f, 0.0f};
    bResult &= g_rOdometory.InitializePos(TRAJ_CONTROL_TASK_SAMPLING_PERIOD_MS, stInitPos);
    bResult &= g_PosMsgQueue.Initialize(pOsFunc->OdometoryPosQueueId);
    bResult &= g_VelCmdMsgQueue.Initialize(pOsFunc->VelCmdQueueId);
    bResult &= g_AnglePidCon.Initialize(TRAJ_CONTROL_TASK_SAMPLING_PERIOD_MS, 1000.0f, PidController::EN_MODE_NORMAL, PidController::EN_PID_TYPE_POSITION);

    if(!bResult){
        return false;
    }

    g_pOsFunc = pOsFunc;
    g_AnglePidCon.SetAllGain(40.0f, 0.0f, 2.0f);
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
        ||  !g_rVelMsgQueue.IsInitialized()
        ||  !g_PosCmdMsgQueue.IsInitialized()
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
            if(!g_rVelMsgQueue.IsEmpty()){
            	g_rVelMsgQueue.Pop(&stVelQueue, 0);
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

            //MainTaskからの目標値を取得
            static PosControlCmdMsg_t stPosCmdMsg;
            if(!g_PosCmdMsgQueue.IsEmpty()){
                g_PosCmdMsgQueue.Pop(&stPosCmdMsg, 0);
            }

            VelControlCmdMsg_t stVelCmdMsg;
            stVelCmdMsg.uiTimestamp = uiTick;
    
            //軌道追従制御
            stVelCmdMsg.fStraightVelCmd = 0.0f;         
            float fTargetAngle = stPosCmdMsg.stTargetPos.fRad;
            stVelCmdMsg.fAngVelCmd = g_AnglePidCon.GetOutput(fTargetAngle, g_rOdometory.GetAngle());

            //速度制御タスクに速度指令をPush
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
                                        stVelQueue.stData.fRad
                                        );
            }
            #endif
            
        }else{
            //ジャイロキャリブレーション中
        }
    }else{
        //タスク無効時
    }
}

void TrajControlTask_Enable()
{
    g_bEnable = true;
}
void TrajControlTask_Disable()
{
    g_bEnable = false;
}
#endif
