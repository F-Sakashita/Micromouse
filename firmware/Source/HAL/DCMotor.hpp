#ifndef DC_MOTOR_HPP
#define DC_MOTOR_HPP

#include "TB6612FNG.hpp"

class DCMotor{
    public:
        typedef enum{
            EN_MOTOR_FIRST = 0,
            EN_MOTOR_LEFT = 0,
            EN_MOTOR_RIGHT,
            EN_MOTOR_LAST,
        }EN_MOTOR_NAME;

        static DCMotor &GetInstance(EN_MOTOR_NAME enMotorName){
            static DCMotor self[EN_MOTOR_LAST];
            self[enMotorName].SetMotorName(enMotorName);
            return self[enMotorName];
        }

        bool Initialize(float fMaxDuty = 1.0f);
        void SetDuty(float fDuty);
        void Stop();
        bool IsEnableBrake();
        void EnableBrake();
        void DisableBrake();
        float IsNowDuty();
        void Update();

    private:
        typedef struct{
            TB6612FNG::EN_MOTOR_CH enMotorCh;
            bool bDirReverse;
        }MotorConfig_t;

        DCMotor();
        ~DCMotor(){;}
        DCMotor(const DCMotor &other);

        void SetMotorName(EN_MOTOR_NAME enMotorName){
            enName = enMotorName;
        }

        EN_MOTOR_NAME enName;
        bool bInitialized;
        
        static const MotorConfig_t stConfig[EN_MOTOR_LAST];
        static TB6612FNG &rTB6612FNG;
};

#endif  /* DC_MOTOR_HPP */