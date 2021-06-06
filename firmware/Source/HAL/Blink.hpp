#ifndef BLINK_HPP
#define BLINK_HPP

#include "stm32f4xx_ll_gpio.h"

class Blink{
private:
    GPIO_TypeDef *pGPIOx;
	uint32_t	uiOutputPin;
    uint32_t    uiPeriodMs;
    uint32_t    uiOnTimeMs;
    uint32_t    uiOffTimeMs;
    uint32_t    uiDelayTimeMs;
    bool bStartState;
    bool bNowState;
    bool bOldState;
    bool bFinishDelay;
    bool bFirst;
    bool bChangeStateFlag;
    uint32_t    uiWaitTime;
    uint32_t    uiStartTimeMs;
    bool bInitialized;
    void SetState(bool bState);
    
public:
    Blink();
    ~Blink();
    Blink(const Blink &other);

    bool Initialize(GPIO_TypeDef *pGPIOx, uint32_t uiOutputPin, uint32_t uiPeriodMs=1000);

    void SetPeriod(uint32_t uiPeriodMs, float fOnPercentage=0.5){
        this->uiPeriodMs = uiPeriodMs;
        uiOnTimeMs = (uint32_t)((float)uiPeriodMs * fOnPercentage);
        uiOffTimeMs = uiPeriodMs - uiOnTimeMs;
    }

    void SetOnTime(uint32_t uiOnTimeMs){
        this->uiOnTimeMs = uiOnTimeMs;
    }
    void SetOffTime(uint32_t uiOffTimeMs){
        this->uiOffTimeMs = uiOffTimeMs;
    }
    void SetStartDelayTime(uint32_t uiDelayTimeMs){
        this->uiDelayTimeMs = uiDelayTimeMs;
    }
    void SetStartState(bool bState){
        this->bStartState = bState;
    }

    void ForceOn(){
        SetState(true);
        bNowState = true;
    }
    void ForceOff(){
        SetState(false);
        bNowState = false;
    }
    
    void On(){
        bNowState = true;
        bChangeStateFlag = false;
    }
    void Off(){
        bNowState = false;
        bChangeStateFlag = false;
    }
    bool IsFinishDelayTime(){
        return bFinishDelay;
    }
    void Reset(){
        bFirst = true;
        bFinishDelay  = false;
    }
    bool IsState();
    void Update();
};

#endif