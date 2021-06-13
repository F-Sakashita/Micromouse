#if 0
#include "VelControlTask.h"
#include "TaskConfig.h"

#include "SystickTimer.h"
#include "main.h"

//外部参照可能なメッセージキューインスタンス
MessageQueue<OdometoryMsg_t> g_VelMsgQueue;

static Odometory &g_rOdometory = Odometory::GetInstance();
static bool g_bInitialized = false;
static DCMotor &g_rLeftMotor = DCMotor::GetInstance(DCMotor::EN_MOTOR_LEFT);
static DCMotor &g_rRightMotor = DCMotor::GetInstance(DCMotor::EN_MOTOR_RIGHT);
static DebugQueue &g_rDebugQueue = DebugQueue::GetInstance();
static PidController g_WheelVelPidCon[DCMotor::EN_MOTOR_LAST];
static BatteryMonitor &g_rBattMoni = BatteryMonitor::GetInstance();
static const VelControlTask_OsFunc_t *g_pOsFunc;
static bool g_bEnable = false;

bool VelControlTask_Initialize(const VelControlTask_OsFunc_t *pOsFunc)
{
    bool bResult = true;
    bResult &= g_rOdometory.InitializeVel(VEL_CONTROL_TASK_SAMPLING_PERIOD_MS);
    bResult &= g_VelMsgQueue.Initialize(pOsFunc->OdometoryVelQueueId);
    bResult &= g_rLeftMotor.Initialize(0.8f);
    bResult &= g_rRightMotor.Initialize(0.8f);
    bResult &= g_WheelVelPidCon[DCMotor::EN_MOTOR_RIGHT].Initialize(VEL_CONTROL_TASK_SAMPLING_PERIOD_MS, 1000.0f);
    bResult &= g_WheelVelPidCon[DCMotor::EN_MOTOR_LEFT].Initialize(VEL_CONTROL_TASK_SAMPLING_PERIOD_MS, 1000.0f);
    bResult &= g_rBattMoni.Initialize(8.2f);

    if(!bResult){
        return false;
    }

    g_pOsFunc = pOsFunc;
    g_rBattMoni.SetOffset(-0.112f);
    g_WheelVelPidCon[DCMotor::EN_MOTOR_RIGHT].SetAllGain(0.01f,  1.2f, 0.0000f);
    g_WheelVelPidCon[DCMotor::EN_MOTOR_LEFT].SetAllGain(0.01f, 1.2f, 0.0000f);
    g_WheelVelPidCon[DCMotor::EN_MOTOR_RIGHT].SetOutputLimit(-7.0f, 7.0f);
    g_WheelVelPidCon[DCMotor::EN_MOTOR_LEFT].SetOutputLimit(-7.0f, 7.0f);
    g_bEnable = false;

    g_bInitialized = true;
    return true;
}

void VelControlTask_Update()
{
    uint32_t uiTick = osKernelGetTickCount();
	osDelayUntil(uiTick + VEL_CONTROL_TASK_SAMPLING_PERIOD_MS);
	
    if(     !g_bInitialized
        ||  !g_VelCmdMsgQueue.IsInitialized()
        ||  !g_rDebugQueue.IsInitialized()){
        return;
    }

#if 1
    g_rBattMoni.Update();

    float fBattVoltage = g_rBattMoni.GetVoltage();
    static bool bBattMoniDispOneshot = false;
    if(!bBattMoniDispOneshot){
        if(!g_rDebugQueue.IsFull()){
            g_rDebugQueue.Printf(0,"Current Battery Voltage, %f", fBattVoltage);
        }
        bBattMoniDispOneshot = true;
    }
    if(fBattVoltage <= 0.0f){
        return;
    }
    //バッテリー電圧が閾値を下回ったら停止する
    if(g_rBattMoni.IsError()){
        g_bEnable = false;
        if(!g_rDebugQueue.IsFull()){
            g_rDebugQueue.Printf(0,"Current Battery Voltage, %f", fBattVoltage);
        }
    }

    OdometoryMsg_t stVelMsg = {
        0u,
        {0.0f, 0.0f, 0.0f},
    };
#endif

    if(g_bEnable){
        //オドメトリの速度データを更新
        g_rOdometory.UpdateVelocity();

        if(g_rOdometory.IsEnableUpdate()){
            //ジャイロ補正完了

            //速度データをメッセージキューにPush
            stVelMsg.stData = g_rOdometory.GetVelocity();
            stVelMsg.uiTimestamp = uiTick;
            if(!g_VelMsgQueue.IsFull()){
                g_VelMsgQueue.Push(&stVelMsg, 0);
            }

            //目標速度をメッセージキューからPop
            static VelControlCmdMsg_t stVelCmdMsg;
            if(!g_VelCmdMsgQueue.IsEmpty()){
                g_VelCmdMsgQueue.Pop(&stVelCmdMsg, 0);
            }

            //目標速度を各車輪の速度に変換する
            /*
                Vr = V + Tr/2 * Omega
                Vl = V - Tr/2 * Omega
            */
            float fWheelVelCmd[DCMotor::EN_MOTOR_LAST];
            fWheelVelCmd[DCMotor::EN_MOTOR_RIGHT] = stVelCmdMsg.fStraightVelCmd + ROBOT_PARAM_TREAD_MM / 2.0f * stVelCmdMsg.fAngVelCmd;
            fWheelVelCmd[DCMotor::EN_MOTOR_LEFT] = stVelCmdMsg.fStraightVelCmd - ROBOT_PARAM_TREAD_MM / 2.0f * stVelCmdMsg.fAngVelCmd;

            //現在の速度情報を取得
            float fNowWheelVel[DCMotor::EN_MOTOR_LAST];
            //Vr,l = rad/sec * diameter / 2
            fNowWheelVel[DCMotor::EN_MOTOR_RIGHT] = ROBOT_PARAM_WHEEL_DIAM_MM * g_rOdometory.rRightEnc.GetRPS() / 2.0f;
            fNowWheelVel[DCMotor::EN_MOTOR_LEFT] = ROBOT_PARAM_WHEEL_DIAM_MM * g_rOdometory.rLeftEnc.GetRPS() / 2.0f;

            //ホイール速度フィードバック制御
            float fMotorVoltage[DCMotor::EN_MOTOR_LAST];
            float fMotorDuty[DCMotor::EN_MOTOR_LAST];
            for(uint8_t ucMotor = DCMotor::EN_MOTOR_FIRST; ucMotor < DCMotor::EN_MOTOR_LAST; ucMotor++){
                fMotorVoltage[ucMotor] = g_WheelVelPidCon[ucMotor].GetOutput(fWheelVelCmd[ucMotor], fNowWheelVel[ucMotor]);

                //バッテリー電圧に応じた割合としてPWMDutyを変更する
                fMotorDuty[ucMotor] = fMotorVoltage[ucMotor] / fBattVoltage;
            }

            //モータPWMデューティ比設定
            g_rLeftMotor.SetDuty(fMotorDuty[DCMotor::EN_MOTOR_LEFT]);
            g_rRightMotor.SetDuty(fMotorDuty[DCMotor::EN_MOTOR_RIGHT]);

            //デバッグ出力
            #ifdef ENABLE_VEL_CONTROL_TASK_DEBUG_CONSOLE
            if(!g_rDebugQueue.IsFull()){
                #if 0
                g_rDebugQueue.Printf(0,"L,%6.3f,%6.3f,R,%6.3f,%6.3f",
                                        fWheelVelCmd[DCMotor::EN_MOTOR_LEFT],
                                        fNowWheelVel[DCMotor::EN_MOTOR_LEFT],
                                        fWheelVelCmd[DCMotor::EN_MOTOR_RIGHT],
                                        fNowWheelVel[DCMotor::EN_MOTOR_RIGHT]
                                        );
                #else
                g_rDebugQueue.Printf(0,"Cmd,%6.3f,Enc,%6.3f,Gyro,%6.3f", 
                                    stVelCmdMsg.fAngVelCmd,
                                    (-fNowWheelVel[DCMotor::EN_MOTOR_LEFT] + fNowWheelVel[DCMotor::EN_MOTOR_RIGHT]) / (ROBOT_PARAM_TREAD_MM),
                                    g_rOdometory.GetAngleVel()
                                    );
                #endif
                #if 0
                static bool bToggle = false;
                if(bToggle) g_DebugLed2.ForceOn();
                else        g_DebugLed2.ForceOff();
                bToggle = !bToggle;
                #endif
            }
            #endif
            
        }else{
            //ジャイロ補正中
            g_rLeftMotor.Stop();
            g_rRightMotor.Stop(); 
        }
    }else{
        //動作停止中
        g_rLeftMotor.Stop();
        g_rRightMotor.Stop(); 
    }
    
    #ifndef ENABLE_MOTOR_OUTPUT
    g_rLeftMotor.Stop();
    g_rRightMotor.Stop();
    #endif
    
    //モータ出力
    g_rLeftMotor.Update();
    g_rRightMotor.Update();
}

void VelControlTask_Enable()
{
    g_bEnable = true;
}
void VelControlTask_Disable()
{
    g_bEnable = false;
}

void VelControlTask_RestartOdometoryCalib()
{
    g_rOdometory.RestartCalibration();
}
#endif

#include "VelControlTask.hpp"

static DCMotor& VelControlTask::rLeftMotor = DCMotor::GetInstance(DCMotor::EN_MOTOR_LEFT);
static DCMotor& VelControlTask::rRightMotor = DCMotor::GetInstance(DCMotor::EN_MOTOR_RIGHT);
static Odometory& VelControlTask::rOdometory = Odometory::GetInstance();
static DebugQueue& VelControlTask::rDebugQueue = DebugQueue::GetInstance();
static BatteryMonitor& VelControlTask::rBattMoni = BatteryMonitor::GetInstance();

static VelControlTask &g_rVelControlTask = VelControlTask::GetInstance();


/* Private Member Functions */
VelControlTask::VelControlTask()
{
    bCalibCompleted = false;
    enNowTopState = EN_TOP_STATE_FIRST;
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
    if(!bResult){
        return false;
    }
    rBattMoni.SetOffset(-0.112f);
    WheelVelPidCon[DCMotor::EN_MOTOR_RIGHT].SetAllGain(0.01f,  1.2f, 0.0000f);
    WheelVelPidCon[DCMotor::EN_MOTOR_LEFT].SetAllGain(0.01f, 1.2f, 0.0000f);
    WheelVelPidCon[DCMotor::EN_MOTOR_RIGHT].SetOutputLimit(-7.0f, 7.0f);
    WheelVelPidCon[DCMotor::EN_MOTOR_LEFT].SetOutputLimit(-7.0f, 7.0f);
    bInitialized = true;
    return true;
}

void VelControlTask::Update()
{
    static VelControlCmdMsg_t stVelCmdMsg;
    float fWheelVelCmd[DCMotor::EN_MOTOR_LAST] = {0.0f};
    float fMotorVoltage[DCMotor::EN_MOTOR_LAST];
    float fMotorDuty[DCMotor::EN_MOTOR_LAST];

    //入力系の更新
    InputUpdate();

    switch (enNowTopState)
    {
    case EN_TOP_STATE_CALIBRATING: //キャリブレーション中
        if(bStartCalibFlag){
            rOdometory.RestartCalibration();
            bStartCalibFlag = false;
        }
        bCalibCompleted = rOdometory.IsEnableUpdate();
        rLeftMotor.Stop();
        rRightMotor.Stop();
        break;
    case EN_TOP_STATE_RUNNING:      //動作中
        if(rBattMoni.IsError() || !bCalibCompleted){
            //バッテリー電圧低下，またはキャリブレーションが未完了で遷移した場合モータ停止
            rLeftMotor.Stop();
            rRightMotor.Stop();
        }else{
            //速度データをメッセージキューにPush
            stVelMsg.stData = g_rOdometory.GetVelocity();
            stVelMsg.uiTimestamp = uiTick;
            if(!g_VelMsgQueue.IsFull()){
                g_VelMsgQueue.Push(&stVelMsg, 0);
            }

            //目標速度をメッセージキューからPop
            if(!g_VelCmdMsgQueue.IsEmpty()){
                g_VelCmdMsgQueue.Pop(&stVelCmdMsg, 0);
            }
                    //目標速度を各車輪の速度に変換する
            /*
                Vr = V + Tr/2 * Omega
                Vl = V - Tr/2 * Omega
            */
            fWheelVelCmd[DCMotor::EN_MOTOR_RIGHT] = stVelCmdMsg.fStraightVelCmd + ROBOT_PARAM_TREAD_MM / 2.0f * stVelCmdMsg.fAngVelCmd;
            fWheelVelCmd[DCMotor::EN_MOTOR_LEFT] = stVelCmdMsg.fStraightVelCmd - ROBOT_PARAM_TREAD_MM / 2.0f * stVelCmdMsg.fAngVelCmd;

            //現在の速度情報を取得
            float fNowWheelVel[DCMotor::EN_MOTOR_LAST];
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

            //デバッグ出力
            #ifdef ENABLE_VEL_CONTROL_TASK_DEBUG_CONSOLE
            if(!rDebugQueue.IsFull()){
                #if 0
                rDebugQueue.Printf(0,"L,%6.3f,%6.3f,R,%6.3f,%6.3f",
                                        fWheelVelCmd[DCMotor::EN_MOTOR_LEFT],
                                        fNowWheelVel[DCMotor::EN_MOTOR_LEFT],
                                        fWheelVelCmd[DCMotor::EN_MOTOR_RIGHT],
                                        fNowWheelVel[DCMotor::EN_MOTOR_RIGHT]
                                        );
                #else
                rDebugQueue.Printf(0,"Cmd,%6.3f,Enc,%6.3f,Gyro,%6.3f", 
                                    stVelCmdMsg.fAngVelCmd,
                                    (-fNowWheelVel[DCMotor::EN_MOTOR_LEFT] + fNowWheelVel[DCMotor::EN_MOTOR_RIGHT]) / (ROBOT_PARAM_TREAD_MM),
                                    rOdometory.GetAngleVel()
                                    );
                #endif
            }
            #endif
        }
    case EN_TOP_STATE_BATT_LOW: //バッテリー電圧低下
    default:
        rLeftMotor.Stop();
        rRightMotor.Stop();
        break;
    }

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

