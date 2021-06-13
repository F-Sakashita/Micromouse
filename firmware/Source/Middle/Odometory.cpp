#include "Odometory.hpp"

RotaryEncoder &Odometory::rLeftEnc = RotaryEncoder::GetInstance(RotaryEncoder::EN_ENC_LEFT);
RotaryEncoder &Odometory::rRightEnc = RotaryEncoder::GetInstance(RotaryEncoder::EN_ENC_RIGHT);
IMU &Odometory::rImu = IMU::GetInstance();

/* Private */
Odometory::Odometory() :
    stInitPosture{0.0f, 0.0f, 0.0f},
    stNowPosture{0.0f, 0.0f, 0.0f},
    stNowVelocity{0.0f, 0.0f, 0.0f},
    bResetPosFlag(false),
	bEnableUpdate(false)
{
	for(uint8_t ucCount = EN_ODOMETORY_FIRST; ucCount < EN_ODOMETORY_LAST; ucCount ++){
		uiSamplingTimeMs[ucCount] = 0u;
		bInitialized[ucCount] = false;
	}
}

/* Public */
bool Odometory::InitializeVel(uint32_t uiSamplingTimeMs)
{
    bool bResult = true;
    bResult &= rLeftEnc.Initialize(uiSamplingTimeMs, false, false);
    bResult &= rRightEnc.Initialize(uiSamplingTimeMs, true, false);
    bResult &= rImu.Initialize(uiSamplingTimeMs, true, false);
    if(!bResult){
        bInitialized[EN_ODOMETORY_VEL] = false;
        return false;
    }
    //ジャイロ補正を有効にする(Update関数で実際に補正動作が始まり，自動で終了する．)
    //5000 ms / uiSamplingTimeMs
    rImu.EnableGyroOffestCalc(5000u / uiSamplingTimeMs);

    bInitialized[EN_ODOMETORY_VEL] = true;
    this->uiSamplingTimeMs[EN_ODOMETORY_VEL] = uiSamplingTimeMs;
    return true;
}

bool Odometory::InitializePos(uint32_t uiSamplingTimeMs, Posture_t stInitPosture)
{
    if(uiSamplingTimeMs <= 0u){
        return false;
    }
    this->uiSamplingTimeMs[EN_ODOMETORY_POS] = uiSamplingTimeMs;
    this->stInitPosture = stInitPosture;
    bInitialized[EN_ODOMETORY_POS] = true;
    return true;
}

void Odometory::UpdateVelocity()
{
    if(!bInitialized[EN_ODOMETORY_VEL]){
        return;
    }
    rLeftEnc.Update();
    rRightEnc.Update();
    rImu.Update();

    if(bResetPosFlag){
        stNowVelocity.fValueX = 0.0f;
        stNowVelocity.fValueY = 0.0f;
        stNowVelocity.fRad = 0.0f;
    }else if(rImu.IsGyroOffsetCompleted()){
        bEnableUpdate = true;
        stNowVelocity.fRad = rImu.GetGyroRPS().fValueZ;
        stNowPosture.fRad = Calc_ConvDegToRad(rImu.GetGyroDeg().fValueZ);
        /*
            Vr,l = rad/sec * diameter / 2
            V = (Vr + vl) / 2
        */
        float fStraightVel = ROBOT_PARAM_WHEEL_DIAM_MM * (rLeftEnc.GetRPS() + rRightEnc.GetRPS()) / 4.0f;   //mm/s
        stNowVelocity.fValueX = fStraightVel * arm_cos_f32(stNowPosture.fRad);
        stNowVelocity.fValueY = fStraightVel * arm_sin_f32(stNowPosture.fRad);
    }else{
        bEnableUpdate = false;
    }
}

void Odometory::UpdatePosition(const Posture_t &stVel)
{
    if(!bInitialized[EN_ODOMETORY_POS]){
        return;
    }

    //Mutexによる排他制御必要？
    if(bResetPosFlag){
        stNowPosture = stResetPosture;
        bResetPosFlag = false;
    }else if(bEnableUpdate){
        stNowPosture.fValueX += stVel.fValueX * (static_cast<float>(uiSamplingTimeMs[EN_ODOMETORY_POS]) / 1000.0f);   //mm
        stNowPosture.fValueY += stVel.fValueY * (static_cast<float>(uiSamplingTimeMs[EN_ODOMETORY_POS]) / 1000.0f);   //mm
    }else{
        stNowPosture = stInitPosture;
    }
}

void Odometory::ResetPosture(Posture_t stPosture)
{
    if(!bResetPosFlag){
        stResetPosture = stPosture;
        bResetPosFlag = true;
    }
}

void Odometory::RestartCalibration()
{
    if(rImu.IsGyroOffsetCompleted() && bInitialized){
    	rImu.EnableGyroOffestCalc(5000u / uiSamplingTimeMs[EN_ODOMETORY_VEL]);
    }
}
