/*
 * it.c
 *
 *  Created on: 27/11/2020
 *      Author: sjoshi
 */

#include "it.h"

extern UART_HandleTypeDef huart3;

void SysTick_Handler()
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}

void USART3_IRQHandler()
{
  HAL_UART_IRQHandler(&huart3);
}
