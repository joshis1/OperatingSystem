/*
 * it.c
 *
 *  Created on: 27/11/2020
 *      Author: sjoshi
 */

#include "it.h"

extern RTC_HandleTypeDef hrtc;

void SysTick_Handler()
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void EXTI15_10_IRQHandler()
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}

void RTC_Alarm_IRQHandler()
{
  HAL_RTC_AlarmIRQHandler(&hrtc);
}
