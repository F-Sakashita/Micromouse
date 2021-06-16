#include "Blink.hpp"
#include "SystickTimer.h"
#include <stddef.h>

/*
 * Public member functions
 */
Blink::Blink()
{
    uiOnTimeMs = 0;
    uiOffTimeMs = 0;
    uiDelayTimeMs = 0;
    bStartState = false;
    bNowState = false;
    bOldState = false;
    bFinishDelay = false;
    bFirst = true;
    bChangeStateFlag = false;
    uiWaitTime = 0;
    uiStartTimeMs = 0;
    bInitialized = false;
    bBlinking = false;
    bResetTimeFlag = false;
    SetPeriod(uiPeriodMs);
    SetState(false);
}

Blink::~Blink()
{

}

Blink::Blink(const Blink &other)
{
    this->bChangeStateFlag = other.bChangeStateFlag;
    this->bFinishDelay = other.bFinishDelay;
    this->bFirst = other.bFirst;
    this->bNowState = other.bNowState;
    this->bOldState = other.bOldState;
    this->bStartState = other.bStartState;
    this->pGPIOx = other.pGPIOx;
    this->uiDelayTimeMs = other.uiDelayTimeMs;
    this->uiOffTimeMs = other.uiOffTimeMs;
    this->uiOnTimeMs = other.uiOnTimeMs;
    this->uiOutputPin = other.uiOutputPin;
    this->uiPeriodMs = other.uiPeriodMs;
    this->uiStartTimeMs = other.uiStartTimeMs;
    this->uiWaitTime = other.uiWaitTime;
    this->bInitialized = other.bInitialized;
    this->bBlinking = other.bBlinking;
    this->bResetTimeFlag = other.bResetTimeFlag;
}

bool Blink::Initialize(GPIO_TypeDef *pGPIOx, uint32_t uiOutputPin, uint32_t uiPeriodMs)
{
    if(NULL == pGPIOx || 0u == uiPeriodMs){
        return false;
    }
    this->pGPIOx = pGPIOx;
    this->uiOutputPin = uiOutputPin;
    SetPeriod(uiPeriodMs);
    SetState(false);
    
    bInitialized = true;
    return true;
}

void Blink::Update()
{
    if(!bInitialized){
        return;
    }
    
    if(bFirst){
        bFirst = false;
        bNowState = bStartState;
        uiStartTimeMs = SystickTimer_GetTimeMS();
    }

    if(uiDelayTimeMs == 0 && !bFinishDelay){
        bFinishDelay = true;
        bChangeStateFlag = true;
        bOldState = !bNowState;
    }

    if(!bFinishDelay){
        if(SystickTimer_IsTimeElapsed(uiStartTimeMs, uiDelayTimeMs)){
            bFinishDelay = true;
            uiStartTimeMs = SystickTimer_GetTimeMS();
            bChangeStateFlag = true;
        }
    }else{
        /* 状態の変更 */
        if(bChangeStateFlag){
            bChangeStateFlag = false;
            if(!bOldState){
                uiWaitTime = uiOnTimeMs; 
            }else{
                uiWaitTime = uiOffTimeMs;
            }
            bNowState = !bOldState;
        }
        
        if(bBlinking && !bResetTimeFlag){
            //点灯時
            if(SystickTimer_IsTimeElapsed(uiStartTimeMs, uiWaitTime)){
                uiStartTimeMs = SystickTimer_GetTimeMS();
                bChangeStateFlag = true;
            }
        }else{
            //常時点灯または消灯時
            uiStartTimeMs = SystickTimer_GetTimeMS();
            bChangeStateFlag = false;
            bResetTimeFlag =false;
        }
    }

    SetState(bNowState);
    bOldState = bNowState;
}

bool Blink::IsState()
{
    return (bool)LL_GPIO_IsOutputPinSet(pGPIOx, uiOutputPin);
}

/*
 * Private member functions
 */
void Blink::SetState(bool bState)
{
    if(true == bState){
        LL_GPIO_SetOutputPin(pGPIOx, uiOutputPin);
    }else{
        LL_GPIO_ResetOutputPin(pGPIOx, uiOutputPin);
    }
}
