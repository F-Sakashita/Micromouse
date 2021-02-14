#ifndef BLINK_HPP
#define BLINK_HPP

#include "stm32f4xx_ll_gpio.h"

class Blink{
private:
    GPIO_TypeDef *pGPIOx;
	uint32_t	ulOutputPin;
    uint32_t    ulPeriodMs;
    uint32_t    ulOnTimeMs;
    uint32_t    ulOffTimeMs;
    uint32_t    ulDelayTimeMs;
    bool bStartState;
    bool bNowState;
    bool bOldState;
    bool bFinishDelay;
    bool bFirst;
    bool bChangeStateFlag;
    uint32_t    ulWaitTime;
    uint32_t    ulStartTimeMs;
    void SetState(bool bState);
    
public:
    Blink(GPIO_TypeDef *pGPIOx, uint32_t ulOutputPin, uint32_t ulPeriodMs=1000);
    ~Blink();
    Blink(const Blink &other);

    void SetPeriod(uint32_t ulPeriodMs, float fOnPercentage=0.5){
        this->ulPeriodMs = ulPeriodMs;
        ulOnTimeMs = (uint32_t)((float)ulPeriodMs * fOnPercentage);
        ulOffTimeMs = ulPeriodMs - ulOnTimeMs;
    }

    void SetOnTime(uint32_t ulOnTimeMs){
        this->ulOnTimeMs = ulOnTimeMs;
    }
    void SetOffTime(uint32_t ulOffTimeMs){
        this->ulOffTimeMs = ulOffTimeMs;
    }
    void SetStartDelayTime(uint32_t ulDelayTimeMs){
        this->ulDelayTimeMs = ulDelayTimeMs;
    }
    void SetStartState(bool bState){
        this->bStartState = bState;
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