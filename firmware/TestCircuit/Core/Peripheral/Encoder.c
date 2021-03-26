#include "Encoder.h"

typedef struct{
    TIM_TypeDef *pTIMx;
    int32_t     iOverflowCount;
    uint32_t    uiRawCount;
    int64_t     llCount;
    uint32_t    uiResolution;
    bool        bStartFlag;
}Encoder_t;

static Encoder_t g_stEncoderSelf[EN_ENCODER_LAST];

void Encoder_Setup(EN_ENCODER_NUM  enEncNum, TIM_TypeDef *pTIMx, uint32_t uiResolution)
{
    if(EN_ENCODER_LAST <= enEncNum){
        return;
    }
    Encoder_t *pSelf = &g_stEncoderSelf[enEncNum];
    pSelf->pTIMx = pTIMx;
    pSelf->uiResolution = uiResolution;
    pSelf->bStartFlag = false;
    pSelf->uiRawCount = 0u;
    pSelf->iOverflowCount = 0;
    pSelf->llCount = 0;
    
    LL_TIM_EnableCounter(pTIMx);
    /* 割り込み許可 */
    LL_TIM_EnableIT_UPDATE(pTIMx);
}

void Encoder_Update(EN_ENCODER_NUM  enEncNum)
{
    if(EN_ENCODER_LAST <= enEncNum){
        return;
    }
    Encoder_t *pSelf = &g_stEncoderSelf[enEncNum];
    pSelf->uiRawCount = LL_TIM_GetCounter(pSelf->pTIMx);
    pSelf->llCount = (int64_t)pSelf->uiRawCount + ((int64_t)(pSelf->iOverflowCount) * (int64_t)pSelf->uiResolution);
}
uint32_t Encoder_IsRawCount(EN_ENCODER_NUM  enEncNum)
{
    if(EN_ENCODER_LAST <= enEncNum){
        return 0u;
    }
    Encoder_t *pSelf = &g_stEncoderSelf[enEncNum];
    return pSelf->uiRawCount;
}
int64_t Encoder_IsCount(EN_ENCODER_NUM  enEncNum)
{
    if(EN_ENCODER_LAST <= enEncNum){
        return 0;
    }
    Encoder_t *pSelf = &g_stEncoderSelf[enEncNum];
    return pSelf->llCount;
}

void Encoder_ClearCount(EN_ENCODER_NUM enEncNum)
{
    if(EN_ENCODER_LAST <= enEncNum){
        return;
    }
    Encoder_t *pSelf = &g_stEncoderSelf[enEncNum];
    pSelf->llCount = 0;
    pSelf->uiRawCount = 0;
    pSelf->iOverflowCount = 0;
}

void Encoder_Interrupt(EN_ENCODER_NUM  enEncNum, EN_ENCODER_OVERFLOW enOverflow)
{
    if(EN_ENCODER_LAST <= enEncNum){
        return;
    }
    Encoder_t *pSelf = &g_stEncoderSelf[enEncNum];
    switch(enOverflow){
    case EN_ENCODER_OVERFLOW_PLUS:
        pSelf->iOverflowCount ++;
        break;
    case EN_ENCODER_OVERFLOW_MINUS:
        pSelf->iOverflowCount --;
        break;
    case EN_ENCODER_OVERFLOW_NONE:
    default:
        break;
    }
}
