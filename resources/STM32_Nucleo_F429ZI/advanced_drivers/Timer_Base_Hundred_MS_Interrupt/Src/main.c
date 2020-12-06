/*
 * main.c
 *
 *  Created on: 27/11/2020
 *      Author: sjoshi
 */

//#include "stm32f4xx.h"
#include "main.h"
#include <stdio.h> //sprintf
#include <string.h> //memset and strlen

/*TIM6 is a basic timer - 16 bits counter
 *
 */

void Timer6_Init(void);
void Error_Handler(void);
void Led_Init(void);

TIM_HandleTypeDef tim6;


int main()
{
	HAL_Init();
	Led_Init();
	Timer6_Init();

	//start the timer.
	if( HAL_OK != HAL_TIM_Base_Start_IT(&tim6))
	{
		Error_Handler();
	}

	while(1);

	return 0;
}


void Error_Handler(void)
{
	while(1);
}

void Led_Init(void)
{
	GPIO_InitTypeDef led;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	led.Mode = GPIO_MODE_OUTPUT_PP;
	led.Pin = GPIO_PIN_7;
	led.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &led);
}

/** Clock is running at 16 Mhz
 * 1 count will take 1/(16 * 10 ^6) seconds
 * 10^6 * 16 seconds = 1 count
 * we need 100 milli seconds = 0.1 seconds
 * 16 * 10^6 * 0.1 = 1600000 counts
 * 16 bit counter max value is 65535.
 * So, we need to decrease the counter clock so that
 * count can fit.
 * How about 16Mhz/25 = 16/25 Mhz
 * 1 count will take = 25/16 *10^6 seconds.
 * 10^6 * 16 /25 = 1 seconds
 * we need 100 milliseconds i.e. 100/1000 = 0.1 seconds
 * 16*10^6*0.1/25 = 64000 - this will fit in the 16-bit counter
 */

void Timer6_Init(void)
{
	tim6.Instance = TIM6;
	tim6.Init.Prescaler = 24;  //the prescaler is actually 25 - (24 +1)
	tim6.Init.Period = 64000 -1; //-1 is required since the event is updated one count late.
	if(HAL_OK != HAL_TIM_Base_Init(&tim6))
	{
		Error_Handler();
	}
}
