#include "uart_printf.h"
#include "uart_assert.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdarg.h>

static osMutexAttr_t Uart1TxMutexAttributes = {
  .name = "Uart1TxMutex",
  .attr_bits = osMutexRecursive
};

static osMutexAttr_t Uart5TxMutexAttributes = {
  .name = "Uart5TxMutex",
  .attr_bits = osMutexRecursive
};

static osMutexId_t Uart1TxMutex;
static osMutexId_t Uart5TxMutex;

void UartMutexInit(void)
{
  Uart1TxMutex = osMutexNew(&Uart1TxMutexAttributes);
  Uart5TxMutex = osMutexNew(&Uart5TxMutexAttributes);
}

static void UartMutexAcquire(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    osMutexAcquire(Uart1TxMutex, osWaitForever);
  }
  else if (huart->Instance == UART5)
  {
    osMutexAcquire(Uart5TxMutex, osWaitForever);
  }
}

static void UartMutexRelease(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    osMutexRelease(Uart1TxMutex);
  }
  else if (huart->Instance == UART5)
  {
    osMutexRelease(Uart5TxMutex);
  }
}

void UartTransmit(UART_HandleTypeDef *huart, const void *data, uint16_t size)
{
  if (data && size)
  {
    UartMutexAcquire(huart);
    assert(HAL_UART_Transmit(huart, (uint8_t *)data, size, 0xffff) == HAL_OK);
    UartMutexRelease(huart);
  }
}

#define UART_PRINT_BUFFER_SIZE 8192

static char UartPrintBuffer[UART_PRINT_BUFFER_SIZE];

void UartPrintRaw(const void *data, uint16_t size)
{
  UartTransmit(&huart1, data, size);
}

int UartPrintf(const char *format, ...)
{
  va_list ap;
  va_start(ap, format);
  int send = vsnprintf(UartPrintBuffer, UART_PRINT_BUFFER_SIZE, format, ap);
  if (send > 0)
  {
    UartPrintRaw(UartPrintBuffer, send);
  }
  va_end(ap);
  return send;
}
