#ifndef TB6612FNG_HPP
#define TB6612FNG_HPP

#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_tim.h"

class TB6612FNG{
public:
    typedef enum{
        EN_MOTOR_CH_FIRST = 0u,
        EN_MOTOR_CH_A      = 0u,
        EN_MOTOR_CH_B,
        EN_MOTOR_CH_LAST
    }EN_MOTOR_CH;

    static TB6612FNG &GetInstance(void);
    bool Initialize(EN_MOTOR_CH enChannel, float fMaxDuty);
    void EnableStandby();
    void DisableStandby();
    bool IsEnableStandby(){
        return bEnableStandby;
    }
    void SetDirReverse(EN_MOTOR_CH enChannel, bool bDirReverse);
    void SetDuty(EN_MOTOR_CH enChannel, float fDuty);
    void Stop(EN_MOTOR_CH enChannel);
    void SetBrake(EN_MOTOR_CH enChannel, bool bEnableBrake);
    bool IsBrake(EN_MOTOR_CH enChannel);
    float IsDuty(EN_MOTOR_CH enChannel);
    void Update(EN_MOTOR_CH enChannel);

private:
    TB6612FNG();
    ~TB6612FNG(){}

    typedef struct{
        GPIO_TypeDef    *pGPIO_PortDir[2];   /* Dir GPIO Port x2 */
        uint32_t        GPIO_PinDir[2];     /* Dir GPIO Pin x2 */
        TIM_TypeDef     *pTIMDef_Pwm;       /* PWM TIM */
        uint8_t         TIMCh_Pwm;          /* PWM TIM Channel */
        bool            bDirReverse;         /* false: Normal, true: Reverse */
    }MotorChConfig_t;

    void SetDirAndPwm(MotorChConfig_t *pMotorChDef, float fReqDuty, bool bEnableBrake=false);

    static TB6612FNG self;
    static MotorChConfig_t stMotorChConfig[EN_MOTOR_CH_LAST];

    bool bInitialized[EN_MOTOR_CH_LAST];
    bool bEnableStandby;
    GPIO_TypeDef *pGPIO_PortStandby;
    uint32_t        GPIO_PinStandby;
    
    bool bIsBrake[EN_MOTOR_CH_LAST];
    float fReqDuty[EN_MOTOR_CH_LAST];
    float fMaxDuty[EN_MOTOR_CH_LAST];
};



#endif /* TB6612FNG_HPP */
