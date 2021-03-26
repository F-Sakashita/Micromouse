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
    }MOTOR_CH;

    static TB6612FNG *GetInstance(void);
    void Initialize(float fMaxDuty);
    void EnableStandby();
    void DisableStandby();
    void SetDirection(MOTOR_CH enChannel, bool bDirection);
    void SetDuty(MOTOR_CH enChannel, float fDuty);
    void Stop(MOTOR_CH enChannel);
    void SetBrake(MOTOR_CH enChannel, bool bEnableBrake);
    void Update();

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

    bool bInitialized;
    GPIO_TypeDef *pGPIO_PortStandby;
    uint32_t        GPIO_PinStandby;
    
    bool bIsBrake[EN_MOTOR_CH_LAST];
    float fReqDuty[EN_MOTOR_CH_LAST];
    float fMaxDuty;
};



#endif /* TB6612FNG_HPP */
