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

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  //USART3 peripheral
   GPIO_InitTypeDef gpio;
  //Enable the clock for USART3 peripheral
  __HAL_RCC_USART3_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  //do the pin muxing configurations
  gpio.Mode = GPIO_MODE_AF_PP;
  gpio.Pull = GPIO_PULLUP;
  gpio.Speed = GPIO_SPEED_FREQ_LOW;
  gpio.Alternate = GPIO_AF7_USART3;
  gpio.Pin = GPIO_PIN_8; //Tx
  HAL_GPIO_Init(GPIOD, &gpio);
  gpio.Pin = GPIO_PIN_9; //Rx
  HAL_GPIO_Init(GPIOD, &gpio);
  //Enable the IRQ and setup the priority - NVIC settings
  HAL_NVIC_EnableIRQ(USART3_IRQn);
  HAL_NVIC_SetPriority(USART3_IRQn,15,0);
}

