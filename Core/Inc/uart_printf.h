#ifndef STM32F769XX_UART_PRINTF_H
#define STM32F769XX_UART_PRINTF_H

#include "usart.h"

void UartMutexInit(void);
void UartTransmit(UART_HandleTypeDef *huart, const void *data, uint16_t size);
void UartPrintRaw(const void *data, uint16_t size);
int UartPrintf(const char *format, ...);

#endif
