#ifndef ENCODER_H
#define ENCODER_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_ll_tim.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum{
    EN_ENCODER_FIRST = 0,
    EN_ENCODER_0 = 0,
    EN_ENCODER_1,
    EN_ENCODER_LAST
}EN_ENCODER_NUM;

typedef enum{
    EN_ENCODER_OVERFLOW_FIRST = 0,
    EN_ENCODER_OVERFLOW_NONE = 0,
    EN_ENCODER_OVERFLOW_PLUS,
    EN_ENCODER_OVERFLOW_MINUS,
    EN_ENCODER_OVERFLOW_LAST,
}EN_ENCODER_OVERFLOW;

bool Encoder_Initialize(EN_ENCODER_NUM  enEncNum, uint32_t uiResolution);
void Encoder_IsStart(EN_ENCODER_NUM enEncNum);
void Encoder_Update(EN_ENCODER_NUM  enEncNum);
uint32_t Encoder_IsRawCount(EN_ENCODER_NUM  enEncNum);
int64_t Encoder_IsCount(EN_ENCODER_NUM  enEncNum);
void Encoder_ClearCount(EN_ENCODER_NUM enEncNum);
void Encoder_Interrupt(EN_ENCODER_NUM  enEncNum, EN_ENCODER_OVERFLOW enOverflow);

#ifdef __cplusplus
}
#endif

#endif /* ENCODER_H */