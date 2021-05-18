#include "Odometory.hpp"

RotaryEncoder &Odometory::rLeftEnc = RotaryEncoder::GetInstance(RotaryEncoder::EN_ENC_LEFT);
RotaryEncoder &Odometory::rRightEnc = RotaryEncoder::GetInstance(RotaryEncoder::EN_ENC_RIGHT);
IMU &Odometory::rImu = IMU::GetInstance();

/* Private */
Odometory::Odometory() :
    stInitPosture{0.0f, 0.0f, 0.0f},
    stNowPosture{0.0f, 0.0f, 0.0f},
    stNowVelocity{0.0f, 0.0f, 0.0f},
    bInitialized(false),
    bResetPosFlag(false),
	bEnableUpdate(false),
	uiSamplingTimeMs(1u)
{

}

/* Public */
bool Odometory::Initialize(uint32_t uiSamplingTimeMs, Posture_t stInitPosture)
{
    bool bResult = true;
    bResult &= rLeftEnc.Initialize(uiSamplingTimeMs, false, false);
    bResult &= rRightEnc.Initialize(uiSamplingTimeMs, true, false);
    bResult &= rImu.Initialize(uiSamplingTimeMs);
    if(!bResult){
        bInitialized = false;
        return false;
    }
    bInitialized = true;
    this->stInitPosture = stInitPosture;
    this->uiSamplingTimeMs = uiSamplingTimeMs;
    return true;
}

void Odometory::Update()
{
    if(bInitialized){
        rLeftEnc.Update();
        rRightEnc.Update();
        rImu.Update();

        
        if(bResetPosFlag){
            bEnableUpdate  = false;
            stNowPosture = stResetPosture;
            stNowVelocity.fValueX = 0.0f;
            stNowVelocity.fValueY = 0.0f;
            stNowVelocity.fRad = 0.0f;
            bResetPosFlag = false;
        }else if(rImu.IsGyroOffsetCompleted()){
            bEnableUpdate = true;
            stNowVelocity.fRad = rImu.GetGyroRPS().fValueZ;
            stNowPosture.fRad = Calc_ConvDegToRad(rImu.GetGyroDeg().fValueZ);
            float fStraightVel = (rLeftEnc.GetRPS() + rRightEnc.GetRPS()) / (2.0f * ROBOT_PARAM_WHEEL_DIAM_MM);
            stNowVelocity.fValueX = fStraightVel * arm_cos_f32(stNowPosture.fRad);
            stNowVelocity.fValueY = fStraightVel * arm_sin_f32(stNowPosture.fRad);
            stNowPosture.fValueX += stNowVelocity.fValueX / (static_cast<float>(uiSamplingTimeMs) / 1000.0f);
            stNowPosture.fValueY += stNowVelocity.fValueY / (static_cast<float>(uiSamplingTimeMs) / 1000.0f);
        }else{
            bEnableUpdate  = false;
            stNowPosture = stInitPosture;
        }
    }
}

void Odometory::ResetPosture(Posture_t stPosture)
{
    if(!bResetPosFlag){
        stResetPosture = stPosture;
        bResetPosFlag = true;
    }
}
