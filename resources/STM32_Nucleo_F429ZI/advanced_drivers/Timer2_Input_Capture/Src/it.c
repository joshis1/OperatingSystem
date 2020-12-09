/*
 * it.c
 *
 *  Created on: 27/11/2020
 *      Author: sjoshi
 */

#include "it.h"

extern TIM_HandleTypeDef tim2;

void SysTick_Handler()
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM2_IRQHandler()
{
	HAL_TIM_IRQHandler(&tim2);
}
