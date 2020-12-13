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


void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
	//PA-5 -- LED -- TIM2 CH1  - AF1
	//PB-3 -- TIM2 CH2 - AF1
	//PB-10 - TIM2 CH3 - AF1
	//PB-11 - TIM2 CH4 - AF1

	GPIO_InitTypeDef tim2ch1_gpio;
	GPIO_InitTypeDef tim4ch2_gpio;

	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE(); // For BLue Led user.
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	tim2ch1_gpio.Pin = GPIO_PIN_5;
	tim2ch1_gpio.Mode = GPIO_MODE_AF_PP;
	tim2ch1_gpio.Pull = GPIO_NOPULL;
	tim2ch1_gpio.Speed = GPIO_SPEED_FREQ_LOW;
	tim2ch1_gpio.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &tim2ch1_gpio);

	tim2ch1_gpio.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOB, &tim2ch1_gpio);

	tim2ch1_gpio.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOB, &tim2ch1_gpio);

	tim2ch1_gpio.Pin = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOB, &tim2ch1_gpio);


	tim4ch2_gpio.Pin = GPIO_PIN_7;
	tim4ch2_gpio.Mode = GPIO_MODE_AF_PP;
	tim4ch2_gpio.Pull = GPIO_NOPULL;
	tim4ch2_gpio.Speed = GPIO_SPEED_FREQ_LOW;
	tim4ch2_gpio.Alternate = GPIO_AF2_TIM4;
	HAL_GPIO_Init(GPIOB, &tim4ch2_gpio);

	//Enable the IRQ of TIM2
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
	//Setup the priority
	HAL_NVIC_SetPriority(TIM2_IRQn,15,0);
	HAL_NVIC_SetPriority(TIM4_IRQn,15,0);
}
