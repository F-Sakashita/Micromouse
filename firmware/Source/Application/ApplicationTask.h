#ifndef APPLICATION_TASK_H
#define APPLICATION_TASK_H

#ifdef  __cplusplus
extern "C"
{
#endif

#include "cmsis_os.h"

bool ApplicationTask_Initialize();
void ApplicationTask_Update();

#ifdef  __cplusplus
}
#endif

#endif //DEBUG_TASK_H
