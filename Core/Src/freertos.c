/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "uart_printf.h"
#include "uart_assert.h"
#include "lwesp/lwesp.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

static const osSemaphoreAttr_t InitSemaphoreAttributes = {
  .name = "InitSemaphoreName"
};

static osSemaphoreId_t InitSemaphore;

extern UART_RingBuf Uart5_RingBuf;

/* USER CODE END Variables */
/* Definitions for KrnlTaskName */
osThreadId_t KrnlTaskNameHandle;
const osThreadAttr_t KrnlTaskName_attributes = {
  .name = "KrnlTaskName",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UserTaskName */
osThreadId_t UserTaskNameHandle;
const osThreadAttr_t UserTaskName_attributes = {
  .name = "UserTaskName",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UartTaskName */
osThreadId_t UartTaskNameHandle;
const osThreadAttr_t UartTaskName_attributes = {
  .name = "UartTaskName",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void KrnlTask(void *argument);
void UserTask(void *argument);
void UartTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  UartMutexInit();
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  InitSemaphore = osSemaphoreNew(1, 0, &InitSemaphoreAttributes);
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of KrnlTaskName */
  KrnlTaskNameHandle = osThreadNew(KrnlTask, NULL, &KrnlTaskName_attributes);

  /* creation of UserTaskName */
  UserTaskNameHandle = osThreadNew(UserTask, NULL, &UserTaskName_attributes);

  /* creation of UartTaskName */
  UartTaskNameHandle = osThreadNew(UartTask, NULL, &UartTaskName_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_KrnlTask */
/**
  * @brief  Function implementing the KrnlTaskName thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_KrnlTask */
void KrnlTask(void *argument)
{
  /* USER CODE BEGIN KrnlTask */
  assert(lwesp_init(0, 1) == lwespOK);

  osSemaphoreRelease(InitSemaphore);
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END KrnlTask */
}

/* USER CODE BEGIN Header_UserTask */
/**
* @brief Function implementing the UserTaskName thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_UserTask */
void UserTask(void *argument)
{
  /* USER CODE BEGIN UserTask */
  osSemaphoreAcquire(InitSemaphore, osWaitForever);

  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END UserTask */
}

/* USER CODE BEGIN Header_UartTask */
/**
* @brief Function implementing the UartTaskName thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_UartTask */
void UartTask(void *argument)
{
  /* USER CODE BEGIN UartTask */
  UART_RingBuf *uart_ringbuf = &Uart5_RingBuf;
  size_t end = uart_ringbuf->end, begin = end;
  /* Infinite loop */
  for (;;)
  {
    if (uart_ringbuf->end > end)
    {
      end = uart_ringbuf->end;
      assert(lwesp_input_process(uart_ringbuf->buffer + begin, end - begin) == lwespOK);
#if LWESP_CFG_AT_ECHO
      UartPrintRaw(uart_ringbuf->buffer + begin, end - begin);
#endif
      begin = end;
    }
    else if (uart_ringbuf->end < end)
    {
      end = uart_ringbuf->end;
      assert(lwesp_input_process(uart_ringbuf->buffer + begin, uart_ringbuf->size - begin) == lwespOK);
#if LWESP_CFG_AT_ECHO
      UartPrintRaw(uart_ringbuf->buffer + begin, uart_ringbuf->size - begin);
#endif
      assert(lwesp_input_process(uart_ringbuf->buffer, end) == lwespOK);
#if LWESP_CFG_AT_ECHO
      UartPrintRaw(uart_ringbuf->buffer, end);
#endif
      begin = end;
    }
    osDelay(1);
  }
  /* USER CODE END UartTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
