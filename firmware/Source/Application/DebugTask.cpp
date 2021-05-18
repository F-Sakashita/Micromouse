#include "DebugTask.h"
#include "DebugConsole.h"
#include "TaskConfig.h"

static bool g_bInitialized = false;

bool DebugTask_Initialize( osMessageQueueId_t pQueueId, const osEventFlagsId_t *pEventId)
{
    if(!DebugQueue_Initialize(pQueueId, pEventId)){
    	return false;
    }
    DebugConsole_Setup();
    g_bInitialized = true;
    return true;
}

void DebugTask_Update()
{
	uint32_t uiTick = osKernelGetTickCount();
	osDelayUntil(uiTick + DEBUG_TASK_SAMPLING_PERIOD_MS);
    if(g_bInitialized && !DebugQueue_IsEmpty()){
		DebugQueue_t stData;
		uint8_t ucPriority=0;
		if(DebugQueue_Pop(&stData, &ucPriority, 0)){
			DebugConsole_Printf("%-16s,%6.3f,%s\n",
							osThreadGetName(stData.taskId),
							static_cast<float>(stData.uiTimestamp)/1000.0f,
							stData.cData
							);
		}

    }
}
