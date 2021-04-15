#include "DebugConsole.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#ifdef ENABLE_UART_PRINTF
#include "usart.h"
#endif

#ifdef ENABLE_UART_PRINTF
static USART_TypeDef* g_pUARTx = NULL;
#endif

void DebugConsole_Setup()
{
#ifdef ENABLE_UART_PRINTF
    g_pUARTx = USART2;
#endif
}

void DebugConsole_Printf(const char * pFormat, ...)
{
	va_list va;
	va_start(va, pFormat);
	vprintf(pFormat, va);
	va_end(va);
}

#ifdef ENABLE_UART_PRINTF
int __io_putchar(int ch)
{
    if(NULL == g_pUARTx){
        return 0;
    }
	USART_TransmitByte(g_pUARTx, (uint8_t)ch);
	return ch;
}
#endif
