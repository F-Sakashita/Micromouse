#ifndef WALL_SENSOR_H
#define WALL_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef enum{
    EN_WALLSENSOR_POS_FIRST = 0,
    EN_WALLSENSOR_POS_RF = 0,   /* 右前 */
    EN_WALLSENSOR_POS_RS,       /* 右横 */
    EN_WALLSENSOR_POS_LS,       /* 左横 */
    EN_WALLSENSOR_POS_LF,       /* 左前 */
    EN_WALLSENSOR_POS_LAST
}EN_WALLSENSOR_POS;

bool WallSensor_Initialize(void);
void WallSensor_InterruptTimer(void);
void WallSensor_InterruptDMA(void);
void WallSensor_Update();
uint16_t WallSensor_GetValue(EN_WALLSENSOR_POS enWallSensorPos);
float WallSensor_GetDistance(EN_WALLSENSOR_POS enWallSensorPos);

uint16_t WallSensor_GetOnValue(EN_WALLSENSOR_POS enWallSensorPos);
uint16_t WallSensor_GetOffValue(EN_WALLSENSOR_POS enWallSensorPos);

#ifdef __cplusplus
}
#endif

#endif
