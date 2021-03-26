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
    bIsBrake[EN_MOTOR_CH_A] = false;
    bIsBrake[EN_MOTOR_CH_B] = false;
    fReqDuty[EN_MOTOR_CH_A] = 0.0f;
    fReqDuty[EN_MOTOR_CH_B] = 0.0f;
    bInitialized = false;
    fMaxDuty = 1.0f;
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
TB6612FNG *TB6612FNG::GetInstance(void)
{
    static TB6612FNG self;
    return &self;
}

void TB6612FNG::Initialize(float fMaxDuty)
{
    if(0.0f > fMaxDuty){
        return;
    }
    TIM_StartPWM(stMotorChConfig[EN_MOTOR_CH_A].pTIMDef_Pwm, stMotorChConfig[EN_MOTOR_CH_A].TIMCh_Pwm);
    TIM_StartPWM(stMotorChConfig[EN_MOTOR_CH_B].pTIMDef_Pwm, stMotorChConfig[EN_MOTOR_CH_B].TIMCh_Pwm);
    LL_GPIO_ResetOutputPin(pGPIO_PortStandby, GPIO_PinStandby);
    bInitialized = true;
    this->fMaxDuty = fMaxDuty;

    //printf("TIM1 Ena Cnt:%d", LL_TIM_IsEnabledCounter(TIM1));
    //printf("TIM1 CC1 Ena:%d\n", LL_TIM_CC_IsEnabledChannel(TIM1, LL_TIM_CHANNEL_CH1));
    //printf("TIM1 CC2 Ena:%d\n", LL_TIM_CC_IsEnabledChannel(TIM1, LL_TIM_CHANNEL_CH2));
}

void TB6612FNG::EnableStandby()
{
    
}

void TB6612FNG::DisableStandby()
{
    
}

void TB6612FNG::SetDirection(MOTOR_CH enChannel, bool bDirection)
{
    if(EN_MOTOR_CH_LAST <= enChannel){
        return;
    }
    stMotorChConfig[enChannel].bDirReverse = bDirection;
}

void TB6612FNG::SetDuty(MOTOR_CH enChannel, float fDuty)
{
    if(EN_MOTOR_CH_LAST <= enChannel){
        return;
    }
    if(fMaxDuty < fDuty){
        fDuty = fMaxDuty;
    }else if(-1.0 * fMaxDuty > fDuty){
        fDuty = -1.0 * fMaxDuty;
    }

    this->fReqDuty[enChannel] = fDuty;
}

void TB6612FNG::Stop(MOTOR_CH enChannel)
{
    if(EN_MOTOR_CH_LAST <= enChannel){
        return;
    }
    this->fReqDuty[enChannel] = 0.0f;
}

void TB6612FNG::SetBrake(MOTOR_CH enChannel, bool bEnableBrake)
{
    if(EN_MOTOR_CH_LAST <= enChannel){
        return;
    }
    bIsBrake[enChannel] = bEnableBrake;
}

void TB6612FNG::Update()
{
    if(bInitialized){
        SetDirAndPwm(&stMotorChConfig[EN_MOTOR_CH_A], fReqDuty[EN_MOTOR_CH_A], bIsBrake[EN_MOTOR_CH_A]);
        SetDirAndPwm(&stMotorChConfig[EN_MOTOR_CH_B], fReqDuty[EN_MOTOR_CH_B], bIsBrake[EN_MOTOR_CH_B]);
        LL_GPIO_SetOutputPin(pGPIO_PortStandby, GPIO_PinStandby);
    }
}

