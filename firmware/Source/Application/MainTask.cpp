#include "MainTask.h"
#include "SystickTimer.h"
#include "RotaryEncoder.hpp"
#include "DebugConsole.h"
#include "Blink.hpp"
#include "Button.hpp"
#include "DCMotor.hpp"
#include "IMU.hpp"
#include "BatteryMonitor.hpp"
#include "Calculation.h"
#include "main.h"
#include <stdio.h>

static const uint32_t g_uiSamplingTimeMs = 2u;
static RotaryEncoder &rLeftEnc = RotaryEncoder::GetInstance(RotaryEncoder::EN_ENC_LEFT);
static RotaryEncoder &rRightEnc = RotaryEncoder::GetInstance(RotaryEncoder::EN_ENC_RIGHT);
static Blink TickLed(TICK_LED_GPIO_Port, TICK_LED_Pin);
static Button Sw[2] = {
    Button(SW0_GPIO_Port, SW0_Pin),
    Button(SW1_GPIO_Port, SW1_Pin),
};
static DCMotor &rLeftMotor = DCMotor::GetInstance(DCMotor::EN_MOTOR_LEFT);
static DCMotor &rRightMotor = DCMotor::GetInstance(DCMotor::EN_MOTOR_RIGHT);
static BatteryMonitor BatMoni;
static IMU &rImu = IMU::GetInstance();

void MainTask_Setup()
{
    SystickTimer_EnableInterrupt();
    SystickTimer_SetSamplingTime(g_uiSamplingTimeMs);

    DebugConsole_Setup();

    rLeftEnc.Initialize(g_uiSamplingTimeMs, false, false);
    rRightEnc.Initialize(g_uiSamplingTimeMs, true, false);
    rLeftMotor.Initialize(0.5f);
    rRightMotor.Initialize(0.5f);

    Sw[0].SetPushReverse();
    Sw[1].SetPushReverse();
    Sw[0].SetEdgeFilter(10,10);
    Sw[1].SetEdgeFilter(10,10);
    //Encoder_Setup(EN_ENCODER_0,0xFFFFFFFF);
    //Encoder_Setup(EN_ENCODER_1,0xFFFFFFFF);
    TickLed.SetPeriod(500, 0.5);
    BatMoni.Initialize(8.0f);
    BatMoni.SetOffset(-0.09f);

    rImu.Initialize(g_uiSamplingTimeMs);

    DebugConsole_Printf("Initializing was completed,\n");
    SystickTimer_DelayMS(3000);
}

void MainTask_Update()
{
    if(SystickTimer_IsSamplingTimeElapsed()){
        rLeftEnc.Update();
        rRightEnc.Update();
        Sw[0].Update();
        Sw[1].Update();
        BatMoni.Update();
        rImu.Update();

        //Encoder_Update(EN_ENCODER_0);
        //Encoder_Update(EN_ENCODER_1);
        //DebugConsole_Printf("Left,%d, Right,%d\n", (int32_t)rLeftEnc.GetCount(), (int32_t)rRightEnc.GetCount());
        static uint64_t ullStartTimeMs[DCMotor::EN_MOTOR_LAST] = {0.0f};
        float fMotorReqDuty[DCMotor::EN_MOTOR_LAST] = {0.0f};
        Button *pSw;
        for(uint8_t ucCount=DCMotor::EN_MOTOR_FIRST; ucCount<DCMotor::EN_MOTOR_LAST; ucCount++){
            if(DCMotor::EN_MOTOR_LEFT == ucCount){
                pSw = &Sw[0];
            }else{
                pSw = &Sw[1];
            }

            if(1u == pSw->IsPushCount() % 2){
                float fTimer = static_cast<float>(SystickTimer_GetTimeMS() - ullStartTimeMs[ucCount]) / (1000.0f);
                fMotorReqDuty[ucCount] = arm_sin_f32(fTimer * M_PI / 4);
            }else{
                ullStartTimeMs[ucCount] = SystickTimer_GetTimeMS();
                fMotorReqDuty[ucCount] = 0.0f;
            }
            
        }
        rLeftMotor.SetDuty(fMotorReqDuty[DCMotor::EN_MOTOR_LEFT]);
        rRightMotor.SetDuty(fMotorReqDuty[DCMotor::EN_MOTOR_RIGHT]);

        //DebugConsole_Printf("Left,%f,%d, Right,%f,%d\n", rLeftEnc.GetDegree(), rLeftEnc.IsEnableUpdateAngle(), rRightEnc.GetDegree(), rRightEnc.IsEnableUpdateAngle());
        //DebugConsole_Printf("Cnt,%d,DPS,%f, RPS,%f\n",  (int32_t)rRightEnc.GetCount(), rRightEnc.GetDPS(), rRightEnc.GetRPS());
        /*
        DebugConsole_Printf("Left,NowDuty,%f,Rps,%f,Right,NowDuty,%f,Rps,%f,Bat,%f, GyroZ,%f, %d\n",
                            rLeftMotor.IsNowDuty(),
                            rLeftEnc.GetRPS(),
                            rRightMotor.IsNowDuty(),
                            rRightEnc.GetRPS(),
                            BatMoni.GetVoltage(),
                            rImu.GetGyroDeg().fValueZ,
                            rImu.IsGyroOffsetCompleted()
                            );
                            */
        DebugConsole_Printf("%f, %d\n",rImu.GetGyroDeg().fValueZ,
                            rImu.IsGyroOffsetCompleted());

        TickLed.Update();
        rLeftMotor.Update();
        rRightMotor.Update();
    }
}
