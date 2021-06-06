#include "Calculation.h"
#include <vector>
#include <float.h>

void Calc_SetCoordValue(Coord_t *pCoord, float fValueX, float fValueY, float fValueZ)
{
    pCoord->fValueX = fValueX;
    pCoord->fValueY = fValueY;
    pCoord->fValueZ = fValueZ; 
}

float Calc_ConvDegToRad(float fDegree)
{
    return (fDegree * M_PI / 180.0f);
}
float Calc_ConvRadToDeg(float fRadian)
{
    return (fRadian * 180.0f / M_PI);
}
float Calc_GetAverage(const float *pData, uint32_t uiSize)
{
    float fAverage = 0.0f;
    float fSum = 0.0f;
    if(uiSize <= 0){
        return 0.0f;
    }
    for(uint32_t uiCount = 0; uiCount < uiSize; uiCount ++){
        fSum += pData[uiCount];
    }
    return fSum / static_cast<float>(uiSize);
}
float Calc_GetSeqAverage(float fLastAverage, float fNewData, uint32_t uiSamplingCnt)
{
    return (fLastAverage * static_cast<float>(uiSamplingCnt) + fNewData) / (static_cast<float>(uiSamplingCnt) + 1.0f);
}

float Calc_ConvDegRange0To360(float fDegree)
{
    float fResult = fmodf(fDegree, 360.0f);
    if(fResult < 0.0f){
        fResult = fResult + 360.0f;
    }
    return fResult;
}
float Calc_ConvDegRangeM180ToP180(float fDegree)
{
    float fResult = fDegree;
    #if 0
    while(fabsf(fResult) > 180.0f){
        if(fResult > 180.0f){
            fResult -= 360.0f;
        }else if(fResult < -180.0f){
            fResult += 360.0f;
        }else{
            /* do nothing */
        }
    }
    #else
    if(fabsf(fResult) - 180.0f <= FLT_EPSILON){
        
    }else{
        fResult = fmodf(fResult, 180.0f);
    }

    #endif
    return fResult;
}
float Calc_ConvRadRange0To2PI(float fRadian)
{
    float fResult = fmodf(fRadian, 2*M_PI);
    if(fResult < 0.0f){
        fResult = fResult + 360.0f;
    }
    return fResult;
}
float Calc_ConvRadRangeMPIToPI(float fRadian)
{
    float fResult = fRadian;
    while(fabsf(fResult) > M_PI){
        if(fResult > M_PI){
            fResult -= 2.0f * M_PI;
        }else if(fResult < -1.0f * M_PI){
            fResult += 2.0f * M_PI;
        }else{
            /* do nothing */
        }
    }
    return fResult;
}