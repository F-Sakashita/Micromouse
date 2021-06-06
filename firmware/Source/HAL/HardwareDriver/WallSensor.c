#include "WallSensor.h"
#include "adc.h"
#include "tim.h"
#include "main.h"

typedef struct{
    uint16_t    usLedOnValue;
    uint16_t    usLedOffValue;
    uint16_t    usSensorValue;
    float       fDistance;
    GPIO_TypeDef *pGPIOx;
    uint32_t    uiPinNum;
    uint8_t     usAdcChRank;
}WallSensorTable_t;

static WallSensorTable_t g_stWallSensorTable[EN_WALLSENSOR_POS_LAST] = {
    {0u,    0u,     0u,     0.0f,	WALL_LED0_GPIO_Port, WALL_LED0_Pin, 1},     //EN_WALLSENSOR_POS_RF
    {0u,    0u,     0u,     0.0f,	WALL_LED1_GPIO_Port, WALL_LED1_Pin, 2},     //EN_WALLSENSOR_POS_RS
    {0u,    0u,     0u,     0.0f,	WALL_LED2_GPIO_Port, WALL_LED2_Pin, 3},     //EN_WALLSENSOR_POS_LS
    {0u,    0u,     0u,     0.0f,	WALL_LED3_GPIO_Port, WALL_LED3_Pin, 4},     //EN_WALLSENSOR_POS_LF
};
static bool g_bAvailable  = false;
static uint8_t g_ucTargetWallSenser = EN_WALLSENSOR_POS_FIRST;

static void WallSensor_SetValueLedOn(EN_WALLSENSOR_POS enPos, uint16_t usValue)
{
    if(EN_WALLSENSOR_POS_LAST <= enPos){
        return;
    }
    WallSensorTable_t *pTable = &g_stWallSensorTable[enPos];
    pTable->usLedOnValue = usValue;
    g_bAvailable = false;
}
static void WallSensor_SetValueLedOff(EN_WALLSENSOR_POS enPos, uint16_t usValue)
{
    if(EN_WALLSENSOR_POS_LAST <= enPos){
        return;
    }
    WallSensorTable_t *pTable = &g_stWallSensorTable[enPos];
    pTable->usLedOffValue = usValue;
    g_bAvailable = true;
}

bool WallSensor_Initialize(void)
{
    for(uint8_t ucCount = 0; ucCount < EN_WALLSENSOR_POS_LAST; ucCount++){
        WallSensorTable_t *pTable = &g_stWallSensorTable[ucCount];
        LL_GPIO_ResetOutputPin(pTable->pGPIOx, pTable->uiPinNum);
    }
    Adc_StartConvert(EN_ADC_NUM_2);
    return true;
}

/* AD変換開始用タイマー割り込み */
void WallSensor_InterruptTimer(void)
{
    //do nothing
    //LL_GPIO_TogglePin(DBG_LED1_GPIO_Port, DBG_LED1_Pin);
}

/* DMA割り込み */
void WallSensor_InterruptDMA(void)
{
    static uint8_t ucWallLedNum = EN_WALLSENSOR_POS_RF;
    static bool bFirst = true;

    //LL_GPIO_TogglePin(DBG_LED2_GPIO_Port, DBG_LED2_Pin);
    WallSensorTable_t *pTable = &g_stWallSensorTable[ucWallLedNum];

    if(0u == LL_GPIO_IsOutputPinSet(pTable->pGPIOx, pTable->uiPinNum)){
        //if(!bFirst){
            WallSensor_SetValueLedOff((EN_WALLSENSOR_POS)ucWallLedNum, Adc_GetAdcChannelValue(EN_ADC_NUM_2, pTable->usAdcChRank));
        //}
        //LL_GPIO_SetOutputPin(pTable->pGPIOx, pTable->uiPinNum);
    }else{
        WallSensor_SetValueLedOn((EN_WALLSENSOR_POS)ucWallLedNum, Adc_GetAdcChannelValue(EN_ADC_NUM_2, pTable->usAdcChRank));
        //LL_GPIO_ResetOutputPin(pTable->pGPIOx, pTable->uiPinNum);
        ucWallLedNum ++;
        if(EN_WALLSENSOR_POS_LAST <= ucWallLedNum){
            ucWallLedNum = EN_WALLSENSOR_POS_FIRST;
            //bFirst = false;
        }
    }

    LL_GPIO_TogglePin(pTable->pGPIOx, pTable->uiPinNum);

    LL_ADC_ClearFlag_EOCS(ADC2);
}

void WallSensor_Update()
{
    if(g_bAvailable){
        for(EN_WALLSENSOR_POS enPos = EN_WALLSENSOR_POS_FIRST; enPos < EN_WALLSENSOR_POS_LAST; enPos++){
            WallSensorTable_t *pTable = &g_stWallSensorTable[enPos];
            pTable->usSensorValue = pTable->usLedOnValue - pTable->usLedOffValue;
            pTable->fDistance = (float)pTable->usSensorValue;
        }
    }
}

uint16_t WallSensor_GetValue(EN_WALLSENSOR_POS enWallSensorPos)
{
    if(EN_WALLSENSOR_POS_LAST <= enWallSensorPos){
        return 0u;
    }
    WallSensorTable_t *pTable = &g_stWallSensorTable[enWallSensorPos];
    return pTable->usSensorValue;
}

uint16_t WallSensor_GetOnValue(EN_WALLSENSOR_POS enWallSensorPos)
{
    if(EN_WALLSENSOR_POS_LAST <= enWallSensorPos){
        return 0u;
    }
    WallSensorTable_t *pTable = &g_stWallSensorTable[enWallSensorPos];
    return pTable->usLedOnValue;
}

uint16_t WallSensor_GetOffValue(EN_WALLSENSOR_POS enWallSensorPos)
{
    if(EN_WALLSENSOR_POS_LAST <= enWallSensorPos){
        return 0u;
    }
    WallSensorTable_t *pTable = &g_stWallSensorTable[enWallSensorPos];
    return pTable->usLedOffValue;
}

float WallSensor_GetDistance(EN_WALLSENSOR_POS enWallSensorPos)
{
    if(EN_WALLSENSOR_POS_LAST <= enWallSensorPos){
        return 0.0f;
    }
    WallSensorTable_t *pTable = &g_stWallSensorTable[enWallSensorPos];
    return pTable->fDistance;
}
