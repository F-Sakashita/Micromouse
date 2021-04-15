#ifndef WALL_SENSOR_H
#define WALL_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum{
    EN_WALLSENSOR_POS_FIRST = 0,
    EN_WALLSENSOR_POS_RF = 0,   /* 右前 */
    EN_WALLSENSOR_POS_RS,       /* 右横 */
    EN_WALLSENSOR_POS_LS,       /* 左横 */
    EN_WALLSENSOR_POS_LF,       /* 左前 */
    EN_WALLSENSOR_POS_LAST
}EN_WALLSENSOR_POS;

void WallSensor_SetValueLedOn(EN_WALLSENSOR_POS enPos, uint16_t usValue);
void WallSensor_SetValueLedOff(EN_WALLSENSOR_POS enPos, uint16_t usValue);
void WallSensor_Update();
void WallSensor_GetValue(uint16_t *pValue, uint8_t ucSize);

#ifdef __cplusplus
}
#endif

#endif