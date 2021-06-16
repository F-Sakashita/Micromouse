#include "MainTask.hpp"
#include "main.h"
#include "MessageQueueType.h"
#include "SystickTimer.h"


DebugQueue& MainTask::rDebugQueue = DebugQueue::GetInstance();
TrajControlTask& MainTask::rTrajTask = TrajControlTask::GetInstance();
VelControlTask& MainTask::rVelTask = VelControlTask::GetInstance();
WallDetectTask& MainTask::rWallTask = WallDetectTask::GetInstance();
MessageQueue<OdometoryMsg_t>& MainTask::rPosMsgQueue = MainTask::rTrajTask.GetPosMsgQueueInst();
static MainTask& g_rMainTask = MainTask::GetInstance();

/* Private member functions */
MainTask::MainTask()
{
    enNowMode = EN_MODE_FIRST;
    enNowState = EN_TOP_STATE_FIRST;
    enOldState = EN_TOP_STATE_FIRST;
}

void MainTask::InputUpdate()
{
    //スイッチのUpdate
    switch (enNowState)
    {
    case EN_TOP_STATE_WAIT_MODE_SELECT:
        //モード選択スイッチの状態をアップデート
        ModeSelectSW.Update();
        if(EN_MODE_FREE != enNowMode){
            StartSW.Update();
        }
        break;
    case EN_TOP_STATE_PREPARE:
    case EN_TOP_STATE_CALIBRATING:
    case EN_TOP_STATE_RUNNING:
        //動作中はスタートスイッチの状態をアップデート
        StartSW.Update();
        break;
    default:
        break;
    }
}

void MainTask::OutputUpdate()
{
    //LED点灯
    TickLed.Update();
    for(uint8_t ucCount = 0; ucCount < 3; ucCount ++){
    	DbgLed[ucCount].Update();
    }
}

void MainTask::SetTopState(EN_TOP_STATE enState)
{
    rVelTask.SetTopState(enState);
	rTrajTask.SetTopState(enState);
    rWallTask.SetTopState(enState);
}

void MainTask::SetLedMode(EN_MODE enMode, EN_TOP_STATE enState)
{
    if(EN_TOP_STATE_WAIT_MODE_SELECT == enState){
        //モード選択待ち
        switch(enMode){
        case EN_MODE_FREE:
            //モータ停止モード
            DbgLed[0].Off();
            DbgLed[1].Off();
            DbgLed[2].Off();
            break;
        case EN_MODE_ENKAI:
            //宴会芸
            DbgLed[0].On();
            DbgLed[1].Off();
            DbgLed[2].Off();
            break;
        case EN_MODE_ANG_SIN:
            //目標角度正弦波
            DbgLed[0].On();
            DbgLed[1].On();
            DbgLed[2].Off();
            break;
        default:
            break;
        }
    }else if(EN_TOP_STATE_PREPARE == enState){
        //準備中
        switch(enMode){
        case EN_MODE_ENKAI:
            //宴会芸
            DbgLed[0].SetPeriod(1000);
            DbgLed[1].Off();
            DbgLed[2].Off();
            break;
        case EN_MODE_ANG_SIN:
            //目標角度正弦波
            DbgLed[0].SetPeriod(1000);
            DbgLed[1].SetPeriod(1000);
            DbgLed[2].Off();
            break;
        default:
            break;
        }
    }else if(EN_TOP_STATE_CALIBRATING == enState){
        //キャリブレーション中
        switch(enMode){
        case EN_MODE_ENKAI:
            //宴会芸
            DbgLed[0].SetPeriod(250);
            DbgLed[1].Off();
            DbgLed[2].Off();
            break;
        case EN_MODE_ANG_SIN:
            //目標角度正弦波
            DbgLed[0].SetPeriod(500);
            DbgLed[1].SetPeriod(500);
            DbgLed[2].Off();
            break;
        default:
            break;
        }
    }else if(EN_TOP_STATE_RUNNING == enState){
        //動作中
        switch(enMode){
        case EN_MODE_ENKAI:
            //宴会芸
            DbgLed[0].SetPeriod(50);
            DbgLed[1].Off();
            DbgLed[2].Off();
            break;
        case EN_MODE_ANG_SIN:
            //目標角度正弦波
            DbgLed[0].SetPeriod(50);
            DbgLed[1].SetPeriod(50);
            DbgLed[2].Off();
            break;
        default:
            break;
        }
    }else if(EN_TOP_STATE_BATT_LOW == enState){
        //バッテリー低下
        DbgLed[0].SetPeriod(1000, 0.05f);
        DbgLed[1].SetPeriod(1000, 0.05f);
        DbgLed[2].SetPeriod(1000, 0.05f);
    }else{
        DbgLed[0].Off();
        DbgLed[1].Off();
        DbgLed[2].Off();
    }
}

/* Public member functions */
bool MainTask::Initialize(const MainTask_OsFunc_t *pOsFunc)
{
    bool bResult = true;

    bResult &= TickLed.Initialize(TICK_LED_GPIO_Port, TICK_LED_Pin, 1000);
    bResult &= DbgLed[0].Initialize(DBG_LED0_GPIO_Port, DBG_LED0_Pin, 1000);
    bResult &= DbgLed[1].Initialize(DBG_LED1_GPIO_Port, DBG_LED1_Pin, 1000);
    bResult &= DbgLed[2].Initialize(DBG_LED2_GPIO_Port, DBG_LED2_Pin, 1000);
    bResult &= ModeSelectSW.Initialize(SW0_GPIO_Port, SW0_Pin, true);
    bResult &= StartSW.Initialize(SW1_GPIO_Port, SW1_Pin, true);
    bResult &= PosCmdMsgQueue.Initialize(pOsFunc->PosCmdQueueId);

    if(!bResult){
        return false;
    }

    TickLed.ForceOn();
    for(uint8_t ucCount = 0; ucCount < 3; ucCount ++){
        SystickTimer_DelayMS(50);
        DbgLed[ucCount].ForceOn();
    }
   
    bInitialized = true;
    return true;
}

void MainTask::Update()
{
    uint32_t uiTick = osKernelGetTickCount();
	osDelayUntil(uiTick + MAIN_TASK_SAMPLING_PERIOD_MS);
    
    if(     !bInitialized 
        ||  !rDebugQueue.IsInitialized()
        ||  !rPosMsgQueue.IsInitialized()){
        return;
    }

    InputUpdate();

    TickLed.SetPeriod(1000);

    static uint32_t uiStartTimeMs = SystickTimer_GetTimeMS();

    if(rVelTask.IsBatteryError()){
        enNowState = EN_TOP_STATE_BATT_LOW;
    }

    static Posture_t stResetPosture;
    static PosControlCmdMsg_t stPosCmdMsg;

    //状態遷移
    switch(enNowState){
    case EN_TOP_STATE_WAIT_MODE_SELECT:
        //停止中
        //押した回数に応じてモードを決定
        enNowMode = static_cast<EN_MODE>(ModeSelectSW.IsPushCount() % static_cast<uint32_t>(EN_MODE_LAST));
        
        if(!StartSW.IsReleaseEdge()){
            SetLedMode(enNowMode, enNowState);    
        }else{
            //動作開始
            switch(enNowMode){
            case EN_MODE_ENKAI:
            case EN_MODE_ANG_SIN:
                enOldState = EN_TOP_STATE_WAIT_MODE_SELECT;
                enNowState = EN_TOP_STATE_PREPARE;
                uiStartTimeMs = SystickTimer_GetTimeMS();
                break;
            case EN_MODE_FREE:
            default:
                //遷移させない
                break;
            }
        }
        break;
    case EN_TOP_STATE_PREPARE:
        if(EN_TOP_STATE_WAIT_MODE_SELECT == enOldState){
            SetLedMode(enNowMode, enNowState);
            if(SystickTimer_IsTimeElapsed(uiStartTimeMs, 3000)){
                enOldState = EN_TOP_STATE_PREPARE;
                enNowState = EN_TOP_STATE_CALIBRATING;
                uiStartTimeMs = SystickTimer_GetTimeMS();
            }else if(StartSW.IsReleaseEdge()){
                //スタートスイッチを押下したらモード選択に戻る
                enOldState = EN_TOP_STATE_PREPARE;
                enNowState = EN_TOP_STATE_WAIT_MODE_SELECT;
            }
        }
        break;
    case EN_TOP_STATE_CALIBRATING:
        if(EN_TOP_STATE_PREPARE == enOldState){
            //キャリブレーション中
            SetLedMode(enNowMode, enNowState);
            
            stResetPosture.fRad = 0.0f;
            stResetPosture.fValueX = 0.0f;
            stResetPosture.fValueY = 0.0f;
            //rTrajTask.ResetPosture(stResetPosture);
            //キャリブレーション完了
            if(rVelTask.IsCalibCompleted()){ //&& rTrajTask.IsResetPosCompleted()){
                enOldState = EN_TOP_STATE_CALIBRATING;
                enNowState = EN_TOP_STATE_RUNNING;
                uiStartTimeMs = SystickTimer_GetTimeMS();
            }else if(StartSW.IsReleaseEdge()){
                //スタートスイッチを押下したらモード選択に戻る
                enOldState = EN_TOP_STATE_CALIBRATING;
                enNowState = EN_TOP_STATE_WAIT_MODE_SELECT;
            }else{
            }
        }
        break;
    case EN_TOP_STATE_RUNNING:
        if(enOldState == EN_TOP_STATE_CALIBRATING){
            SetLedMode(enNowMode, enNowState);
            //動作中
            
            switch(enNowMode){
            case EN_MODE_ENKAI:
                stPosCmdMsg.stTargetPos.fValueX = 0.0f;
                stPosCmdMsg.stTargetPos.fValueY = 0.0f;
                stPosCmdMsg.stTargetPos.fRad = 0.0f;
                break;
            case EN_MODE_ANG_SIN:
                stPosCmdMsg.stTargetPos.fValueX = 0.0f;
                stPosCmdMsg.stTargetPos.fValueY = 0.0f;
                stPosCmdMsg.stTargetPos.fRad = Calc_ConvDegToRad(180.0f) * arm_sin_f32(0.5f * 2.0f * M_PI * static_cast<float>(SystickTimer_GetTimeMS() - uiStartTimeMs));
                break;
            }

            if(EN_MODE_FREE != enNowMode){
                if(!PosCmdMsgQueue.IsFull()){
                    PosCmdMsgQueue.Push(&stPosCmdMsg,0);
                }
            }

            if(StartSW.IsReleaseEdge()){
                //スタートスイッチを押下したらモード選択に戻る
                enOldState = EN_TOP_STATE_RUNNING;
                enNowState = EN_TOP_STATE_WAIT_MODE_SELECT;
            }
        }
        break;
    case EN_TOP_STATE_BATT_LOW:
        SetLedMode(enNowMode, enNowState);
        break;
    default:
        break;
    }

    //MainTaskの状態遷移を他タスクに渡す
    SetTopState(enNowState);

    OutputUpdate();
}


bool MainTask_Initialize(const MainTask_OsFunc_t *pOsFunc)
{
   return g_rMainTask.Initialize(pOsFunc);
}

void MainTask_Update()
{
    g_rMainTask.Update();
}


#if 0

MessageQueue<PosControlCmdMsg_t> g_PosCmdMsgQueue;
MessageQueue<bool> g_MotionStartMsgQueue;

static Button g_ModeSelectSW;
static Button g_StartSW;
static Blink g_TickLed;
static Blink g_DbgLed[3];
static bool g_bInitialized = false;
static DebugQueue& g_rDebugQueue = DebugQueue::GetInstance();
static bool g_bEnable = false;
static const MainTask_OsFunc_t*	g_pOsFunc;

typedef enum{
    EN_MODE_FIRST = 0,
    EN_MODE_FREE = 0,
    EN_MODE_ENKAI,
    EN_MODE_ANG_SIN,
    EN_MODE_LAST,
}EN_MODE;


bool MainTask_Initialize(const MainTask_OsFunc_t *pOsFunc)
{
    bool bResult = true;

    bResult &= g_TickLed.Initialize(TICK_LED_GPIO_Port, TICK_LED_Pin, 1000);
    bResult &= g_DbgLed[0].Initialize(DBG_LED0_GPIO_Port, DBG_LED0_Pin, 1000);
    bResult &= g_DbgLed[1].Initialize(DBG_LED1_GPIO_Port, DBG_LED1_Pin, 1000);
    bResult &= g_DbgLed[2].Initialize(DBG_LED2_GPIO_Port, DBG_LED2_Pin, 1000);
    bResult &= g_ModeSelectSW.Initialize(SW0_GPIO_Port, SW0_Pin, true);
    bResult &= g_StartSW.Initialize(SW1_GPIO_Port, SW1_Pin, true);
    bResult &= g_PosCmdMsgQueue.Initialize(pOsFunc->PosCmdQueueId);

    if(!bResult){
        return false;
    }

    g_pOsFunc = pOsFunc;

    g_TickLed.ForceOn();
    for(uint8_t ucCount = 0; ucCount < 3; ucCount ++){
        SystickTimer_DelayMS(50);
        g_DbgLed[ucCount].ForceOn();
    }
   
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

    g_TickLed.SetPeriod(1000);

    static EN_MODE enMode = EN_MODE_FREE;
    static EN_TOP_STATE enNowState = EN_TOP_STATE_WAIT_MODE_SELECT;
    static EN_TOP_STATE enOldState = EN_TOP_STATE_WAIT_MODE_SELECT;
    static uint32_t uiStartTimeMs = SystickTimer_GetTimeMS();

    //スイッチのUpdate
    switch (enNowState)
    {
    case EN_TOP_STATE_WAIT_MODE_SELECT:
        //モード選択スイッチの状態をアップデート
        g_ModeSelectSW.Update();
        if(EN_MODE_FREE != enMode){
            g_StartSW.Update();
        }
        break;
    case EN_TOP_STATE_PREPARE:
    case EN_TOP_STATE_CALIBRATING:
    case EN_TOP_STATE_RUNNING:
        //動作中はスタートスイッチの状態をアップデート
        g_StartSW.Update();
        break;
    default:
        break;
    }

    //状態遷移
    switch(enNowState){
    case EN_TOP_STATE_WAIT_MODE_SELECT:
        //停止中
        //押した回数に応じてモードを決定
        enMode = static_cast<EN_MODE>(g_ModeSelectSW.IsPushCount() % static_cast<uint32_t>(EN_MODE_LAST));
        
        if(!g_StartSW.IsReleaseEdge()){
            switch(enMode){
            case EN_MODE_FREE:
                //モータ停止モード
                g_DbgLed[0].Off();
                g_DbgLed[1].Off();
                g_DbgLed[2].Off();
                break;
            case EN_MODE_ENKAI:
                //宴会芸
                g_DbgLed[0].On();
                g_DbgLed[1].Off();
                g_DbgLed[2].Off();
                break;
            case EN_MODE_ANG_SIN:
                //目標角度正弦波
                g_DbgLed[0].On();
                g_DbgLed[1].On();
                g_DbgLed[2].Off();
                break;
            default:
                break;
            }
        }else{
            //動作開始
            switch(enMode){
            case EN_MODE_ENKAI:
            case EN_MODE_ANG_SIN:
                enOldState = EN_TOP_STATE_WAIT_MODE_SELECT;
                enNowState = EN_TOP_STATE_PREPARE;
                uiStartTimeMs = SystickTimer_GetTimeMS();
                break;
            case EN_MODE_FREE:
            default:
                //遷移させない
                break;
            }
        }
        break;
    case EN_TOP_STATE_PREPARE:
        if(EN_TOP_STATE_WAIT_MODE_SELECT == enOldState){
            switch(enMode){
            case EN_MODE_ENKAI:
                //宴会芸
                g_DbgLed[0].SetPeriod(1000);
                g_DbgLed[1].Off();
                g_DbgLed[2].Off();
                break;
            case EN_MODE_ANG_SIN:
                //目標角度正弦波
                g_DbgLed[0].SetPeriod(1000);
                g_DbgLed[1].SetPeriod(1000);
                g_DbgLed[2].Off();
                break;
            }
            if(SystickTimer_IsTimeElapsed(uiStartTimeMs, 5000)){
                enOldState = EN_TOP_STATE_PREPARE;
                enNowState = EN_TOP_STATE_CALIBRATING;
                uiStartTimeMs = SystickTimer_GetTimeMS();
            }else if(g_StartSW.IsReleaseEdge()){
                //スタートスイッチを押下したらモード選択に戻る
                enOldState = EN_TOP_STATE_PREPARE;
                enNowState = EN_TOP_STATE_WAIT_MODE_SELECT;
            }
        }
        break;
    case EN_TOP_STATE_CALIBRATING:
        if(EN_TOP_STATE_PREPARE == enOldState){
            //キャリブレーション中
            switch(enMode){
            case EN_MODE_ENKAI:
                //宴会芸
                g_DbgLed[0].SetPeriod(250);
                g_DbgLed[1].Off();
                g_DbgLed[2].Off();
                break;
            case EN_MODE_ANG_SIN:
                //目標角度正弦波
                g_DbgLed[0].SetPeriod(500);
                g_DbgLed[1].SetPeriod(500);
                g_DbgLed[2].Off();
                break;
            }
            //キャリブレーション開始を別タスクに通知
            

            //キャリブレーション完了
            if(SystickTimer_IsTimeElapsed(uiStartTimeMs, 5000)){
                enOldState = EN_TOP_STATE_CALIBRATING;
                enNowState = EN_TOP_STATE_RUNNING;
            }else if(g_StartSW.IsReleaseEdge()){
                //スタートスイッチを押下したらモード選択に戻る
                enOldState = EN_TOP_STATE_CALIBRATING;
                enNowState = EN_TOP_STATE_WAIT_MODE_SELECT;
            }
        }
        break;
    case EN_TOP_STATE_RUNNING:
        if(enOldState == EN_TOP_STATE_CALIBRATING){
            //動作中
            switch(enMode){
            case EN_MODE_ENKAI:
                //宴会芸
                g_DbgLed[0].SetPeriod(50);
                g_DbgLed[1].Off();
                g_DbgLed[2].Off();
                break;
            case EN_MODE_ANG_SIN:
                //目標角度正弦波
                g_DbgLed[0].SetPeriod(50);
                g_DbgLed[1].SetPeriod(50);
                g_DbgLed[2].Off();
                break;
            default:
            	break;
            }

            //動作中
            if(g_StartSW.IsReleaseEdge()){
                //スタートスイッチを押下したらモード選択に戻る
                enOldState = EN_TOP_STATE_RUNNING;
                enNowState = EN_TOP_STATE_WAIT_MODE_SELECT;
            }
        }
        break;
    default:
        break;
    }


#if 0
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
#endif

    #ifdef ENABLE_MAIN_TASK_DEBUG_CONSOLE
    if(!g_rDebugQueue.IsFull()){
            g_rDebugQueue.Printf(0,"sw0:%d",uiSwCount);
    }
    #endif

    //LED点灯
    g_TickLed.Update();
    for(uint8_t ucCount = 0; ucCount < 3; ucCount ++){
        g_DbgLed[ucCount].Update();
    }
}

#endif
