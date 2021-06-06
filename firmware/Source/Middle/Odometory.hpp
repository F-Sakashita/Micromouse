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

        bool InitializeVel(uint32_t uiSamplingTimeMs);
        bool InitializePos(uint32_t uiSamplingTimeMs, Posture_t stInitPosture);

        void UpdateVelocity();
        void UpdatePosition(const Posture_t &stVel);

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

        //外部参照可能にする
        static RotaryEncoder &rLeftEnc;
        static RotaryEncoder &rRightEnc;

    private:
        enum{
        	EN_ODOMETORY_FIRST = 0,
            EN_ODOMETORY_POS = 0,
            EN_ODOMETORY_VEL,
            EN_ODOMETORY_LAST
        };
        Odometory();
        ~Odometory(){};
        Odometory(const Odometory &other);

        Posture_t stInitPosture;
        Posture_t stResetPosture;
        Posture_t stNowPosture;
        Posture_t stNowVelocity;

        bool bInitialized[EN_ODOMETORY_LAST];
        uint32_t uiSamplingTimeMs[EN_ODOMETORY_LAST];

        bool bResetPosFlag;
        bool bEnableUpdate;
        

        static IMU &rImu;
};

#endif
