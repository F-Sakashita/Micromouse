#include "MainTask.h"
#include "SystickTimer.h"
#include "RotaryEncoder.hpp"
#include "DebugConsole.h"
#include "Blink.hpp"
#include "main.h"
#include <stdio.h>

static const uint32_t g_uiSamplingTimeMs = 2u;
static RotaryEncoder &rLeftEnc = RotaryEncoder::GetInstance(RotaryEncoder::EN_ENC_LEFT);
static RotaryEncoder &rRightEnc = RotaryEncoder::GetInstance(RotaryEncoder::EN_ENC_RIGHT);
static Blink TickLed(TICK_LED_GPIO_Port, TICK_LED_Pin);

void MainTask_Setup()
{
    SystickTimer_EnableInterrupt();
    SystickTimer_SetSamplingTime(g_uiSamplingTimeMs);

    DebugConsole_Setup();

    rLeftEnc.Initialize(g_uiSamplingTimeMs, false, true, 90.0f);
    rRightEnc.Initialize(g_uiSamplingTimeMs, true, true, -90.0f);
    //Encoder_Setup(EN_ENCODER_0,0xFFFFFFFF);
    //Encoder_Setup(EN_ENCODER_1,0xFFFFFFFF);
    TickLed.SetPeriod(500, 0.5);
}

void MainTask_Update()
{
    if(SystickTimer_IsSamplingTimeElapsed()){
        rLeftEnc.Update();
        rRightEnc.Update();
        //Encoder_Update(EN_ENCODER_0);
        //Encoder_Update(EN_ENCODER_1);
        //DebugConsole_Printf("Left,%d, Right,%d\n", (int32_t)rLeftEnc.GetCount(), (int32_t)rRightEnc.GetCount());
        DebugConsole_Printf("Left,%f, Right,%f\n", rLeftEnc.GetDegree(), rRightEnc.GetDegree());
        //DebugConsole_Printf("Cnt,%d,DPS,%f, RPS,%f\n",  (int32_t)rRightEnc.GetCount(), rRightEnc.GetDPS(), rRightEnc.GetRPS());

        TickLed.Update();
    }
}
