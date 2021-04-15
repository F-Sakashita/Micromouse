#ifndef CALCULATION_H
#define CALCULATION_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#ifdef ARM_MATH_CM4
#include "arm_math.h"
#else
#include <math.h>
#endif

float Calc_ConvDegToRad(float fDegree);
float Calc_ConvRadToDeg(float fRadian);
float Calc_GetAverage(const float *pData, uint32_t uiSize);
float Calc_GetSeqAverage(float fLastAverage, float fNewData, uint32_t uiSamplingCnt);
float Calc_ConvDegRange0To360(float fDegree);
float Calc_ConvDegRangeM180ToP180(float fDegree);
float Calc_ConvRadRange0To2PI(float fRadian);
float Calc_ConvRadRangeMPIToPI(float fRadian);

#ifdef __cplusplus
}
#endif

#endif //CALCULATION
