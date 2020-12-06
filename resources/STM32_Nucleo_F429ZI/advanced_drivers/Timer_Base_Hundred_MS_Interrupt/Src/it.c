/*
 * it.c
 *
 *  Created on: 27/11/2020
 *      Author: sjoshi
 */

#include "it.h"

extern TIM_HandleTypeDef tim6;

void SysTick_Handler()
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}

void TIM6_DAC_IRQHandler()
{
	HAL_TIM_IRQHandler(&tim6);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
}
