#include "DebugQueue.hpp"
#include <cstdarg>
#include <cstring>
#include <cstdio>

bool DebugQueue::Printf(uint32_t uiTimeout, const char *pFormat, ...)
{
    DebugMsg_t TxData;
    va_list arg;
	va_start(arg, pFormat);
    vsnprintf(TxData.cData, DEBUG_QUEUE_CHAR_MAX_SIZE, pFormat, arg);
    va_end(arg);

    TxData.uiTimestamp = osKernelGetTickCount();
    TxData.taskId = osThreadGetId();
    
    return Push(&TxData, uiTimeout);
}