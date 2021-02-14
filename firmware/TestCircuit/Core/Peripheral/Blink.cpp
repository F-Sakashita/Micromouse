#include "Blink.hpp"
#include "SystickTimer.h"

/*
 * Public member functions
 */
Blink::Blink(GPIO_TypeDef *pGPIOx, uint32_t ulOutputPin, uint32_t ulPeriodMs)
{
    this->pGPIOx = pGPIOx;
    this->ulOutputPin = ulOutputPin;
    ulOnTimeMs = 0;
    ulOffTimeMs = 0;
    ulDelayTimeMs = 0;
    bStartState = false;
    bNowState = false;
    bOldState = false;
    bFinishDelay = false;
    bFirst = true;
    bChangeStateFlag = false;
    ulWaitTime = 0;
    ulStartTimeMs = 0;
    SetPeriod(ulPeriodMs);
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
    this->ulDelayTimeMs = other.ulDelayTimeMs;
    this->ulOffTimeMs = other.ulOffTimeMs;
    this->ulOnTimeMs = other.ulOnTimeMs;
    this->ulOutputPin = other.ulOutputPin;
    this->ulPeriodMs = other.ulPeriodMs;
    this->ulStartTimeMs = other.ulStartTimeMs;
    this->ulWaitTime = other.ulWaitTime;
}

void Blink::Update()
{
    if(bFirst){
        bFirst = false;
        bNowState = bStartState;
        ulStartTimeMs = SystickTimer_GetTimeMS();
    }

    if(ulDelayTimeMs == 0 && !bFinishDelay){
        bFinishDelay = true;
        bChangeStateFlag = true;
        bOldState = !bNowState;
    }

    if(!bFinishDelay){
        if(SystickTimer_IsTimeElapsed(ulStartTimeMs, ulDelayTimeMs)){
            bFinishDelay = true;
            ulStartTimeMs = SystickTimer_GetTimeMS();
            bChangeStateFlag = true;
        }
    }else{
        /* 状態の変更 */
        if(bChangeStateFlag){
            bChangeStateFlag = false;
            if(!bOldState){
                ulWaitTime = ulOnTimeMs; 
            }else{
                ulWaitTime = ulOffTimeMs;
            }
            bNowState = !bOldState;
        }

        if(SystickTimer_IsTimeElapsed(ulStartTimeMs, ulWaitTime)){
            ulStartTimeMs = SystickTimer_GetTimeMS();
            bChangeStateFlag = true;
        }
    }

    SetState(bNowState);
    bOldState = bNowState;
}

bool Blink::IsState()
{
    return (bool)LL_GPIO_IsOutputPinSet(pGPIOx, ulOutputPin);
}

/*
 * Private member functions
 */
void Blink::SetState(bool bState)
{
    if(true == bState){
        LL_GPIO_SetOutputPin(pGPIOx, ulOutputPin);
    }else{
        LL_GPIO_ResetOutputPin(pGPIOx, ulOutputPin);
    }
}