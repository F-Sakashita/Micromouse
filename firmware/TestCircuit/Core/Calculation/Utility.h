#ifndef UTILITY_H
#define UTILITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

inline float ConvertDtoR(float fDegree);
inline float ConvertRtoD(float fRadian);
inline float GetSequentialAverage(float fLastAverage, float fData, uint32_t uiSamplingCnt);

#ifdef __cplusplus
}
#endif

#endif /* UTILITY_H */
