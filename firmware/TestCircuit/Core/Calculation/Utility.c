#include "Utility.h"
#include <math.h>

float ConvertDtoR(float fDegree)
{
    return fDegree * M_PI / 180.0f;
}
float ConvertRtoD(float fRadian)
{
    return fRadian * 180.0f / M_PI;
}
float GetSequentialAverage(float fLastAverage, float fData, uint32_t uiSamplingCnt)
{
    return (fLastAverage * (float)uiSamplingCnt + fData) / ((float)uiSamplingCnt + 1);
}