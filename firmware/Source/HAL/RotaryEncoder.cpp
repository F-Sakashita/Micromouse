#include "RotaryEncoder.hpp"
#include "Calculation.h"

const RotaryEncoder::EncoderConfig_t RotaryEncoder::stEncConfig[EN_ENC_LAST] = {
    /*  enEncNum        fCountPerCircle    					uiResolution    */
    {   EN_ENCODER_1,   4096.0f*4.0f*30.0f/8.0f,               0xFFFFFFFF  },     /* EN_ENC_LEFT */
    {   EN_ENCODER_0,   4096.0f*4.0f*30.0f/8.0f,               0xFFFFFFFF  },     /* EN_ENC_RIGHT */
};

RotaryEncoder::RotaryEncoder() : 
    bReverse(false),
    uiSamplingTimeMs(1u),
    bInitialized(false),
    enName(EN_ENC_LAST),
    fStartDegree(0.0f),
    llNowCount(0),
    llOldCount(0),
    iDiffCount(0),
    fDiffRate(0.0f),
    fNowDegree(0.0f),
    fOldDegree(0.0f),
    fNowRadian(0.0f),
    fOldRadian(0.0f),
    fNowDps(0.0f),
    fNowRps(0.0f),
    bEnableUpdateAngle(false)
{

}


void RotaryEncoder::UpdateDps()
{
    fNowDps = fDiffRate * 360.0f / static_cast<float>(uiSamplingTimeMs);
}
void RotaryEncoder::UpdateRps()
{
    fNowRps = fDiffRate * 2.0f * M_PI / static_cast<float>(uiSamplingTimeMs);
}
void RotaryEncoder::UpdateDegree()
{
    fNowDegree += fNowDps * static_cast<float>(uiSamplingTimeMs);
}
void RotaryEncoder::UpdateRadian()
{
    fNowRadian += fNowRps * static_cast<float>(uiSamplingTimeMs);
}

bool RotaryEncoder::Initialize(uint32_t uiSamplingTimeMs, bool bReverse, bool bEnableUpdateAngle, float fStartDeg)
{
    if(EN_ENC_LAST <= enName){
        return false;
    }
    if(uiSamplingTimeMs <= 0){
        return false;
    }

    pEncConfig = &stEncConfig[enName];

    Encoder_Setup(pEncConfig->enEncNum, pEncConfig->uiResolution);

    this->uiSamplingTimeMs = uiSamplingTimeMs;
    this->bReverse = bReverse;
    this->bEnableUpdateAngle = bEnableUpdateAngle;
    fStartDegree = fStartDeg;
    fNowDegree = fStartDegree;
    fNowRadian = Calc_ConvDegToRad(fNowDegree);
    Encoder_ClearCount(pEncConfig->enEncNum);

    this->bInitialized = true;
    return true;
}

void RotaryEncoder::Update()
{
    if(!bInitialized){
        return;
    }
    
    Encoder_Update(pEncConfig->enEncNum);
    llNowCount = Encoder_IsCount(pEncConfig->enEncNum);
    if(bReverse){
        llNowCount = -1 * llNowCount;
    }
    iDiffCount = static_cast<int32_t>(llNowCount - llOldCount);
    fDiffRate = static_cast<float>(iDiffCount) / (pEncConfig->fCountPerCircle);

    UpdateDps();
    UpdateRps();

    if(bEnableUpdateAngle){
        UpdateDegree();
        UpdateRadian();
    }

    llOldCount = llNowCount;
    fOldDegree = fNowDegree;
    fOldRadian = fNowRadian;
}

float RotaryEncoder::GetDegree(EN_DEG_MODE enMode)
{
    float fResult = 0.0f;
    fResult = fNowDegree;
    switch (enMode)
    {
    case EN_DEG_MODE_0_360:
        fResult = Calc_ConvDegRange0To360(fResult);
        break;
    case EN_DEG_MODE_M180_180:
        fResult = Calc_ConvDegRangeM180ToP180(fResult);
        break;
    case EN_DEG_MODE_UNLIMITED:
    default:
        /* do nothing */
        break;
    }

    return fResult;
}
float RotaryEncoder::GetRadian(EN_RAD_MODE enMode)
{
    float fResult = 0.0f;

    fResult = fNowRadian;
    switch (enMode)
    {
    case EN_RAD_MODE_0_2PI:
        fResult = Calc_ConvRadRange0To2PI(fResult);
        break;
    case EN_RAD_MODE_MPI_PI:
        fResult = Calc_ConvRadRangeMPIToPI(fResult);
        break;
    case EN_RAD_MODE_UNLIMITED:
    default:
        /* do nothing */
        break;
    }

    return fResult;
}
