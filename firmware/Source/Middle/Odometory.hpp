#ifndef ODOMETORY_HPP
#define ODOMETORY_HPP

#include "Calculation.h"
#include "RotaryEncoder.hpp"
#include "IMU.hpp"

#define ROBOT_PARAM_TREAD_MM  (68.0f) //トレッド
#define ROBOT_PARAM_WHEEL_DIAM_MM   (25.0f) //タイヤ径

class Odometory{
    public:
        static Odometory &GetInstance(void){
            static Odometory self;
            return self;
        }

        bool Initialize(uint32_t uiSamplingTimeMs, Posture_t stInitPosture);
        void Update();
        void ResetPosture(Posture_t stPosture);

        const float& GetAngle(){
            return stNowPosture.fRad;
        }
        const float& GetAngleVel(){
            return stNowVelocity.fRad;
        }
        const Posture_t& GetPosture(){
            return stNowPosture;
        }
        const Posture_t& GetVelocity(){
            return stNowVelocity;
        }

        bool IsEnableUpdate(){
            return bEnableUpdate;
        }

    private:
        Odometory();
        ~Odometory(){};
        Odometory(const Odometory &other);

        Posture_t stInitPosture;
        Posture_t stResetPosture;
        Posture_t stNowPosture;
        Posture_t stNowVelocity;

        bool bInitialized;
        bool bResetPosFlag;
        bool bEnableUpdate;
        uint32_t uiSamplingTimeMs;
        
        static RotaryEncoder &rLeftEnc;
        static RotaryEncoder &rRightEnc;
        static IMU &rImu;
};

#endif
