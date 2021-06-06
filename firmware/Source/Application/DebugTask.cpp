#include "DebugTask.h"
#include "DebugConsole.h"
#include "TaskConfig.h"
//#include "DebugQueue.h"
#include "DebugQueue.hpp"

static bool g_bInitialized = false;
static DebugQueue & g_rDebugQueue = DebugQueue::GetInstance();

bool DebugTask_Initialize(const DebugTask_OsFunc_t *pOsFunc)
{
    bool bResult = true;
    bResult &= g_rDebugQueue.Initialize(pOsFunc->TxQueueId);
    bResult &= DebugConsole_Initialize();
    //if(!DebugQueue_Initialize(pOsFunc->pTxQueueId, NULL)){
    if(!bResult){
    	return false;
    }

    g_bInitialized = true;
    return true;
}

void DebugTask_Update()
{
	uint32_t uiTick = osKernelGetTickCount();
	osDelayUntil(uiTick + DEBUG_TASK_SAMPLING_PERIOD_MS);

    #ifdef ENABLE_DEBUG_CONSOLE

    if(g_bInitialized && !g_rDebugQueue.IsEmpty()){
		DebugMsg_t stData;

        if(g_rDebugQueue.Pop(&stData, 0)){
			DebugConsole_Printf("%-16s,%6.3f,%s\n",
							osThreadGetName(stData.taskId), //送信データをPushしたタスク名
							static_cast<float>(stData.uiTimestamp)/1000.0f, //タイムスタンプ
							stData.cData    //データ
							);
		}

    }
    #endif
}
