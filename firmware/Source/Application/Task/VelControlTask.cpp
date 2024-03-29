#include "VelControlTask.hpp"
#include "TrajControlTask.hpp"
#include "MessageQueueList.hpp"
#include "SystickTimer.h"

//初期化が必要なインスタンスの取得
DCMotor& VelControlTask::rLeftMotor = DCMotor::GetInstance(DCMotor::EN_MOTOR_LEFT);
DCMotor& VelControlTask::rRightMotor = DCMotor::GetInstance(DCMotor::EN_MOTOR_RIGHT);
Odometory& VelControlTask::rOdometory = Odometory::GetInstance();
BatteryMonitor& VelControlTask::rBattMoni = BatteryMonitor::GetInstance();
//MessageQueue<OdometoryMsg_t> VelControlTask::VelMsgQueue;

//別タスクで初期化を行うインスタンスの取得
DebugQueue& VelControlTask::rDebugQueue = DebugQueue::GetInstance();
MessageQueue<VelControlCmdMsg_t>& VelControlTask::rVelCmdMsgQueue = TrajControlTask::GetInstance().GetVelCmdMsgQueueInst();

//本クラスのインスタンスの取得
static VelControlTask &g_rVelControlTask = VelControlTask::GetInstance();


/* Private Member Functions */
VelControlTask::VelControlTask()
{
    bCalibCompleted = false;
    enNowTopState = EN_TOP_STATE_FIRST;
    bIsBatteryError = false; 
    bFirstCalibShot = false;
}

void VelControlTask::InputUpdate()
{
    switch (enNowTopState)
    {
    case EN_TOP_STATE_CALIBRATING:
    case EN_TOP_STATE_RUNNING:
        //キャリブレーション中または動作中のみオドメトリの速度情報を更新
        rOdometory.UpdateVelocity();
        break;
    default:
        //何もしない
        break;
    }
    rBattMoni.Update();
}

void VelControlTask::OutputUpdate()
{
    //モータ出力有効化のマクロが未定義の場合は強制的にモータを停止
    #ifndef ENABLE_MOTOR_OUTPUT
    rLeftMotor.Stop();
    rRightMotor.Stop();
    #endif
    
    //モータ出力
    rLeftMotor.Update();
    rRightMotor.Update();
}

/* Public Member Function */
bool VelControlTask::Initialize(const VelControlTask_OsFunc_t *pOsFunc)
{
    bool bResult = true;
    bResult &= rOdometory.InitializeVel(VEL_CONTROL_TASK_SAMPLING_PERIOD_MS);
    bResult &= rLeftMotor.Initialize(0.8f);
    bResult &= rRightMotor.Initialize(0.8f);
    bResult &= WheelVelPidCon[DCMotor::EN_MOTOR_RIGHT].Initialize(VEL_CONTROL_TASK_SAMPLING_PERIOD_MS, 1000.0f);
    bResult &= WheelVelPidCon[DCMotor::EN_MOTOR_LEFT].Initialize(VEL_CONTROL_TASK_SAMPLING_PERIOD_MS, 1000.0f);
    bResult &= rBattMoni.Initialize(8.2f);
    bResult &= VelMsgQueue.Initialize(pOsFunc->OdometoryVelQueueId);
    stOsFunc = *pOsFunc;
    if(!bResult){
        return false;
    }
    rBattMoni.SetOffset(-0.112f);
    WheelVelPidCon[DCMotor::EN_MOTOR_RIGHT].SetAllGain(0.01f,  1.2f, 0.0000f);
    WheelVelPidCon[DCMotor::EN_MOTOR_LEFT].SetAllGain(0.01f, 1.2f, 0.0000f);
    WheelVelPidCon[DCMotor::EN_MOTOR_RIGHT].SetOutputLimit(-5.0f, 5.0f);
    WheelVelPidCon[DCMotor::EN_MOTOR_LEFT].SetOutputLimit(-5.0f, 5.0f);
    bFirstCalibShot = true;
    bInitialized = true;
    return true;
}

void VelControlTask::Update()
{
    static OdometoryMsg_t stVelMsg;
    static VelControlCmdMsg_t stVelCmdMsg;
    float fWheelVelCmd[DCMotor::EN_MOTOR_LAST] = {0.0f};
    float fMotorVoltage[DCMotor::EN_MOTOR_LAST];
    float fMotorDuty[DCMotor::EN_MOTOR_LAST];
    float fNowWheelVel[DCMotor::EN_MOTOR_LAST];

    uint32_t uiTick = osKernelGetTickCount();
	osDelayUntil(uiTick + VEL_CONTROL_TASK_SAMPLING_PERIOD_MS);

    if(     !bInitialized
        ||  !rDebugQueue.IsInitialized()
        ||  !rVelCmdMsgQueue.IsInitialized()){
    	return;
    }

    //入力系の更新
    InputUpdate();

    float fBattVoltage = rBattMoni.GetVoltage();
    static bool bOneShort = true;
    if(bOneShort){
        rDebugQueue.Printf(0,"Current Battery Voltage, %3.3f [V]",fBattVoltage);
        bOneShort = false;
    }


    switch (enNowTopState)
    {
    case EN_TOP_STATE_WAIT_MODE_SELECT:
    case EN_TOP_STATE_PREPARE:
        bStartCalibFlag = false;
        bCalibCompleted = false;
        rLeftMotor.Stop();
        rRightMotor.Stop();
        break;
    case EN_TOP_STATE_CALIBRATING: //キャリブレーション中
        //キャリブレーションを開始
        if(!bStartCalibFlag && !bFirstCalibShot){
            bStartCalibFlag = true;
            rOdometory.RestartCalibration();    
        }
        //キャリブレーションが完了しているか確認
        bCalibCompleted = rOdometory.IsEnableUpdate();

        rLeftMotor.Stop();
        rRightMotor.Stop();
        break;
    case EN_TOP_STATE_RUNNING:      //動作中
        bFirstCalibShot = false;
        bStartCalibFlag = false;
        if(rBattMoni.IsError() || !bCalibCompleted){
            //バッテリー電圧低下，またはキャリブレーションが未完了で遷移した場合モータ停止
            rLeftMotor.Stop();
            rRightMotor.Stop();
            bIsBatteryError = true;
        }else{
            //速度データをメッセージキューにPush
            stVelMsg.stData = rOdometory.GetVelocity();
            stVelMsg.uiTimestamp = uiTick;
            if(!VelMsgQueue.IsFull()){
                VelMsgQueue.Push(&stVelMsg, 0);
            }

            //目標速度をメッセージキューからPop
            if(!rVelCmdMsgQueue.IsEmpty()){
                rVelCmdMsgQueue.Pop(&stVelCmdMsg, 0);
            }
            
            //目標速度を各車輪の速度に変換する
            /*
                Vr = V + Tr/2 * Omega
                Vl = V - Tr/2 * Omega
            */
            fWheelVelCmd[DCMotor::EN_MOTOR_RIGHT] = stVelCmdMsg.fStraightVelCmd + ROBOT_PARAM_TREAD_MM / 2.0f * stVelCmdMsg.fAngVelCmd;
            fWheelVelCmd[DCMotor::EN_MOTOR_LEFT] = stVelCmdMsg.fStraightVelCmd - ROBOT_PARAM_TREAD_MM / 2.0f * stVelCmdMsg.fAngVelCmd;

            //現在の速度情報を取得
            //Vr,l = rad/sec * diameter / 2
            fNowWheelVel[DCMotor::EN_MOTOR_RIGHT] = ROBOT_PARAM_WHEEL_DIAM_MM * rOdometory.rRightEnc.GetRPS() / 2.0f;
            fNowWheelVel[DCMotor::EN_MOTOR_LEFT] = ROBOT_PARAM_WHEEL_DIAM_MM * rOdometory.rLeftEnc.GetRPS() / 2.0f;

            //ホイール速度フィードバック制御
            for(uint8_t ucMotor = DCMotor::EN_MOTOR_FIRST; ucMotor < DCMotor::EN_MOTOR_LAST; ucMotor++){
                fMotorVoltage[ucMotor] = WheelVelPidCon[ucMotor].GetOutput(fWheelVelCmd[ucMotor], fNowWheelVel[ucMotor]);

                //バッテリー電圧に応じた割合としてPWMDutyを変更する
                fMotorDuty[ucMotor] = fMotorVoltage[ucMotor] / fBattVoltage;
            }

            //モータPWMデューティ比設定
            rLeftMotor.SetDuty(fMotorDuty[DCMotor::EN_MOTOR_LEFT]);
            rRightMotor.SetDuty(fMotorDuty[DCMotor::EN_MOTOR_RIGHT]);
        }
        break;
    case EN_TOP_STATE_BATT_LOW: //バッテリー電圧低下
    default:
        rLeftMotor.Stop();
        rRightMotor.Stop();
        break;
    }

    static uint32_t uiTimerMs = SystickTimer_GetTimeMS();
    if(bIsBatteryError && SystickTimer_IsTimeElapsed(uiTimerMs, 1000u)){
        uiTimerMs = SystickTimer_GetTimeMS();
        if(!rDebugQueue.IsFull()){
            rDebugQueue.Printf(0,"Battery Low Error, %6.3f [V]", fBattVoltage);
        }

    }

    //デバッグ出力
    #ifdef ENABLE_VEL_CONTROL_TASK_DEBUG_CONSOLE
    if(!rDebugQueue.IsFull()){
        #if 0
        rDebugQueue.Printf(0,"Cmd,%6.3f,Enc,%6.3f,Gyro,%6.3f", 
                            stVelCmdMsg.fAngVelCmd,
                            (-fNowWheelVel[DCMotor::EN_MOTOR_LEFT] + fNowWheelVel[DCMotor::EN_MOTOR_RIGHT]) / (ROBOT_PARAM_TREAD_MM),
                            rOdometory.GetAngleVel()
                            );

        #endif
        //rDebugQueue.Printf(0,"L,%6.3f,R,%6.3f",fMotorDuty[DCMotor::EN_MOTOR_LEFT], fMotorDuty[DCMotor::EN_MOTOR_RIGHT]);
        //rDebugQueue.Printf(0,"Calib,%d,%d,%d",enNowTopState, bStartCalibFlag, bCalibCompleted);
    }
    #endif

    //出力系の更新
    OutputUpdate();
}

/* RTOSでコールされる関数の定義 */
bool VelControlTask_Initialize(const VelControlTask_OsFunc_t *pOsFunc)
{
    return g_rVelControlTask.Initialize(pOsFunc);
}
void VelControlTask_Update()
{
    g_rVelControlTask.Update();
}

