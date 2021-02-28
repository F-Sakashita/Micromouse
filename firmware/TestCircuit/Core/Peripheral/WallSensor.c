#include "WallSensor.h"
#include "adc.h"
#include <stdio.h>

static uint16_t g_WallSensorLedOnValue[EN_WALLSENSOR_POS_LAST] = {0};
static uint16_t g_WallSensorLedOffValue[EN_WALLSENSOR_POS_LAST] = {0};
static uint16_t g_WallSensorValue[EN_WALLSENSOR_POS_LAST] = {0};
static bool bAvailable  = false;

void WallSensor_SetValueLedOn(EN_WALLSENSOR_POS enPos, uint16_t usValue)
{
    if(EN_WALLSENSOR_POS_LAST <= enPos){
        return;
    }
    g_WallSensorLedOnValue[enPos] = usValue;
    bAvailable = true;
    
    if(enPos == EN_WALLSENSOR_POS_RF){
        //printf("Right Front ON:%d\n",usValue);
    }
    
}
void WallSensor_SetValueLedOff(EN_WALLSENSOR_POS enPos, uint16_t usValue)
{
    if(EN_WALLSENSOR_POS_LAST <= enPos){
        return;
    }
    g_WallSensorLedOffValue[enPos] = usValue;
    bAvailable = false;
    
    if(enPos == EN_WALLSENSOR_POS_RF){
        //printf("Right Front Off:%d\n",usValue);
    }
}
void WallSensor_Update()
{
    if(bAvailable){
        for(EN_WALLSENSOR_POS enPos = EN_WALLSENSOR_POS_FIRST; enPos < EN_WALLSENSOR_POS_LAST; enPos++){
            g_WallSensorValue[enPos] = g_WallSensorLedOnValue[enPos] - g_WallSensorLedOffValue[enPos];
        }
    }
}
void WallSensor_GetValue(uint16_t *pValue, uint8_t ucSize)
{
    memcpy(pValue, g_WallSensorValue, ucSize * sizeof(uint16_t));
}
