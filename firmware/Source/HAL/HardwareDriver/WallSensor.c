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
}WallSensorTable_t;

static WallSensorTable_t g_stWallSensorTable[EN_WALLSENSOR_POS_LAST] = {
    {0u,    0u,     0u,     0.0f,	WALL_LED0_GPIO_Port, WALL_LED0_Pin},     //EN_WALLSENSOR_POS_RF
    {0u,    0u,     0u,     0.0f,	WALL_LED1_GPIO_Port, WALL_LED1_Pin},     //EN_WALLSENSOR_POS_RS
    {0u,    0u,     0u,     0.0f,	WALL_LED2_GPIO_Port, WALL_LED2_Pin},     //EN_WALLSENSOR_POS_LS
    {0u,    0u,     0u,     0.0f,	WALL_LED3_GPIO_Port, WALL_LED3_Pin},     //EN_WALLSENSOR_POS_LF
};
static bool g_bAvailable  = false;

static void WallSensor_SetValueLedOn(EN_WALLSENSOR_POS enPos, uint16_t usValue)
{
    if(EN_WALLSENSOR_POS_LAST <= enPos){
        return;
    }
    WallSensorTable_t *pTable = &g_stWallSensorTable[enPos];
    pTable->usLedOnValue = usValue;
    g_bAvailable = true;
}
static void WallSensor_SetValueLedOff(EN_WALLSENSOR_POS enPos, uint16_t usValue)
{
    if(EN_WALLSENSOR_POS_LAST <= enPos){
        return;
    }
    WallSensorTable_t *pTable = &g_stWallSensorTable[enPos];
    pTable->usLedOffValue = usValue;
    g_bAvailable = false;
}

bool WallSensor_Initialize(void)
{
    Adc_StartConvert(EN_ADC_NUM_2);
    return true;
}

/* AD変換開始用タイマー割り込み */
void WallSensor_InterruptTimer(void)
{
    LL_GPIO_TogglePin(DBG_LED0_GPIO_Port, DBG_LED0_Pin);
}

/* DMA割り込み */
void WallSensor_InterruptDMA(void)
{
    static uint8_t ucWallLedNum = 0u;
    static bool bFirst = true;
    LL_GPIO_TogglePin(DBG_LED1_GPIO_Port, DBG_LED1_Pin);

    WallSensorTable_t *pTable = &g_stWallSensorTable[ucWallLedNum];

    if(0u == LL_GPIO_IsOutputPinSet(pTable->pGPIOx, pTable->uiPinNum)){
        if(!bFirst){
            WallSensor_SetValueLedOff((EN_WALLSENSOR_POS)ucWallLedNum, Adc_GetAdcChannelValue(EN_ADC_NUM_2, ucWallLedNum+1));
        }
        LL_GPIO_SetOutputPin(pTable->pGPIOx, pTable->uiPinNum);
    }else{
        WallSensor_SetValueLedOn((EN_WALLSENSOR_POS)ucWallLedNum, Adc_GetAdcChannelValue(EN_ADC_NUM_2, ucWallLedNum+1));
        LL_GPIO_ResetOutputPin(pTable->pGPIOx, pTable->uiPinNum);
        ucWallLedNum ++;
        if(EN_WALLSENSOR_POS_LAST <= ucWallLedNum){
            ucWallLedNum = EN_WALLSENSOR_POS_FIRST;
            bFirst = false;
        }
    }
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

float WallSensor_GetDistance(EN_WALLSENSOR_POS enWallSensorPos)
{
    if(EN_WALLSENSOR_POS_LAST <= enWallSensorPos){
        return 0.0f;
    }
    WallSensorTable_t *pTable = &g_stWallSensorTable[enWallSensorPos];
    return pTable->fDistance;
}
