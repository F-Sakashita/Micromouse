#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>

bool DebugConsole_Initialize();
void DebugConsole_Printf(const char * pFormat, ...);

#ifdef ENABLE_UART_PRINTF
int __io_putchar(int ch);   /*  */
#endif

#ifdef __cplusplus
}
#endif

#endif
