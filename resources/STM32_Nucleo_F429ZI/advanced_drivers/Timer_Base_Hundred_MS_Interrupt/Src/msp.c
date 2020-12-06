/*
 * msp.c
 *
 *  Created on: 27/11/2020
 *      Author: sjoshi
 */

#include "stm32f4xx_hal.h"

void HAL_MspInit(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MspInit could be implemented in the user file
   */
  //1. Setup the priority grouping of the ARM cortex Mx processor
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  //2. Enable the required system exceptions of the ARM cortex Mx processor
  SCB->SHCSR |= 0x7 << 16; //Enable usage fault, memory fault and bus fault.
  //3. Configure the priority for system exceptions.
  HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
  HAL_NVIC_SetPriority(BusFault_IRQn,0,0);
  HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM6_CLK_ENABLE();
	//Enable the IRQ of TIM6
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	//Setup the priority
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn,15,0);
}
