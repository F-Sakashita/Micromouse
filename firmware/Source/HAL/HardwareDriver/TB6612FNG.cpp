#include "TB6612FNG.hpp"
#include "main.h"
#include "tim.h"
#include <stdio.h>

TB6612FNG::MotorChConfig_t TB6612FNG::stMotorChConfig[TB6612FNG::EN_MOTOR_CH_LAST] = {
    /* Motor Channel A Config */
    {
        {MOTOR_L_DIR0_GPIO_Port,    MOTOR_L_DIR1_GPIO_Port},
        {MOTOR_L_DIR0_Pin,          MOTOR_L_DIR1_Pin},
        TIM1,
        LL_TIM_CHANNEL_CH2,
        true
    },
    /* Motor Channel B Config */
    {
        {MOTOR_R_DIR0_GPIO_Port,    MOTOR_R_DIR1_GPIO_Port},
        {MOTOR_R_DIR0_Pin,          MOTOR_R_DIR1_Pin},
        TIM1,
        LL_TIM_CHANNEL_CH1,
        true
    }
};

/* Private */
TB6612FNG::TB6612FNG()
{
    pGPIO_PortStandby   = MOTOR_STBY_GPIO_Port;
    GPIO_PinStandby = MOTOR_STBY_Pin;
    for(uint8_t ucCount = EN_MOTOR_CH_FIRST; ucCount < EN_MOTOR_CH_LAST; ucCount ++){
        bIsBrake[ucCount] = false;
        fReqDuty[ucCount] = 0.0f;
        fMaxDuty[ucCount] = 1.0f;
        bInitialized[ucCount] = false;
    }
    bEnableStandby = false;
}

void TB6612FNG::SetDirAndPwm(MotorChConfig_t *pMotorChDef, float fReqDuty, bool bEnableBrake)
{
    float fDuty = 0.0f;

    if(true == pMotorChDef->bDirReverse){
        fReqDuty = -1.0 * fReqDuty;
    }

    if(!bEnableBrake){
        if(0.0f < fReqDuty){
            fDuty = fReqDuty;
            LL_GPIO_SetOutputPin(pMotorChDef->pGPIO_PortDir[0], pMotorChDef->GPIO_PinDir[0]);
            LL_GPIO_ResetOutputPin(pMotorChDef->pGPIO_PortDir[1], pMotorChDef->GPIO_PinDir[1]);
        }else if (0.0f > fReqDuty){
            fDuty = -1.0f * fReqDuty;
            LL_GPIO_ResetOutputPin(pMotorChDef->pGPIO_PortDir[0], pMotorChDef->GPIO_PinDir[0]);
            LL_GPIO_SetOutputPin(pMotorChDef->pGPIO_PortDir[1], pMotorChDef->GPIO_PinDir[1]);
        }else{
            LL_GPIO_ResetOutputPin(pMotorChDef->pGPIO_PortDir[0], pMotorChDef->GPIO_PinDir[0]);
            LL_GPIO_ResetOutputPin(pMotorChDef->pGPIO_PortDir[1], pMotorChDef->GPIO_PinDir[1]);
        }
    }else{
        fDuty = 0.5f;
        LL_GPIO_SetOutputPin(pMotorChDef->pGPIO_PortDir[0], pMotorChDef->GPIO_PinDir[0]);
        LL_GPIO_SetOutputPin(pMotorChDef->pGPIO_PortDir[1], pMotorChDef->GPIO_PinDir[1]);
    }

    TIM_SetPWMDuty(pMotorChDef->pTIMDef_Pwm, pMotorChDef->TIMCh_Pwm, fDuty);
}

/* Public */
TB6612FNG& TB6612FNG::GetInstance()
{
    static TB6612FNG self;
    return self;
}
bool TB6612FNG::Initialize(EN_MOTOR_CH enChannel, float fMaxDuty)
{
    if(EN_MOTOR_CH_LAST <= enChannel){
        return false;
    }

    if(0.0f > fMaxDuty){
        return false;
    }
    TIM_StartPWM(stMotorChConfig[enChannel].pTIMDef_Pwm, stMotorChConfig[enChannel].TIMCh_Pwm);

    bInitialized[enChannel] = true;
    this->fMaxDuty[enChannel] = fMaxDuty;
    if(!IsEnableStandby()){
        EnableStandby();
    }
    return true;
}

void TB6612FNG::EnableStandby()
{
    LL_GPIO_SetOutputPin(pGPIO_PortStandby, GPIO_PinStandby);
    bEnableStandby = true;
}

void TB6612FNG::DisableStandby()
{
    LL_GPIO_ResetOutputPin(pGPIO_PortStandby, GPIO_PinStandby);
    bEnableStandby = false;
}

void TB6612FNG::SetDirReverse(EN_MOTOR_CH enChannel, bool bDirReverse)
{
    if(EN_MOTOR_CH_LAST <= enChannel){
        return;
    }
    stMotorChConfig[enChannel].bDirReverse = bDirReverse;
}

void TB6612FNG::SetDuty(EN_MOTOR_CH enChannel, float fDuty)
{
    if(EN_MOTOR_CH_LAST <= enChannel){
        return;
    }
    if(fMaxDuty[enChannel] < fDuty){
        fDuty = fMaxDuty[enChannel];
    }else if(-1.0 * fMaxDuty[enChannel] > fDuty){
        fDuty = -1.0 * fMaxDuty[enChannel];
    }

    this->fReqDuty[enChannel] = fDuty;
}

void TB6612FNG::Stop(EN_MOTOR_CH enChannel)
{
    if(EN_MOTOR_CH_LAST <= enChannel){
        return;
    }
    this->fReqDuty[enChannel] = 0.0f;
}

void TB6612FNG::SetBrake(EN_MOTOR_CH enChannel, bool bEnableBrake)
{
    if(EN_MOTOR_CH_LAST <= enChannel){
        return;
    }
    bIsBrake[enChannel] = bEnableBrake;
}

bool TB6612FNG::IsBrake(EN_MOTOR_CH enChannel)
{
    if(EN_MOTOR_CH_LAST <= enChannel){
        return false;
    }
    return bIsBrake[enChannel];
}

float TB6612FNG::IsDuty(EN_MOTOR_CH enChannel)
{
    if(EN_MOTOR_CH_LAST <= enChannel){
        return 0.0f;
    }
    return fReqDuty[enChannel];
}

void TB6612FNG::Update(EN_MOTOR_CH enChannel)
{
    if(EN_MOTOR_CH_LAST <= enChannel){
        return;
    }
    if(bEnableStandby && bInitialized[enChannel]){
        SetDirAndPwm(&stMotorChConfig[enChannel], fReqDuty[enChannel], bIsBrake[enChannel]);
    }
}

