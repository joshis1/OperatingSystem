/*
 * it.c
 *
 *  Created on: 27/11/2020
 *      Author: sjoshi
 */

#include "it.h"

extern TIM_HandleTypeDef tim2;
extern TIM_HandleTypeDef tim6;

void SysTick_Handler()
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM2_IRQHandler()
{
	HAL_TIM_IRQHandler(&tim2);
}

void TIM6_DAC_IRQHandler()
{
	HAL_TIM_IRQHandler(&tim6);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//Connect PA-9 to PA-0. PA9 is generating the clock frequency at 50Khz
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
}
