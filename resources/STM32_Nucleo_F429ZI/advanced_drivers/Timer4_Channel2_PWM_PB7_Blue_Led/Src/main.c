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


void Timer4_Init(void);
void Error_Handler(void);
void SystemClockConfigHSE(uint32_t sysclk);
void GPIO_Init(void);


/** User Leds
 * User LD1 - green user LED is connected to STM32 I/O PB0
 * LD2 -- a blue user LED is connected to PB7
 * LD3 -- a red user LED is connected to PB14
 * user LEDs are on when the I/O is high value, and are off when the I/O is low.
 */

/** Change the blue user LED brightness using PWM **/
/** As we can see that we need to use Timer 4 Channel 2-  AF2 to change the brightness
 *  of the Blue LED
 */


TIM_HandleTypeDef tim4;
uint16_t brightness = 0;

int main()
{
	HAL_Init();
	//Higher Clock will have more precision
	SystemClockConfigHSE(CLK_50MHZ);
	GPIO_Init();
	Timer4_Init();

	if(HAL_OK != HAL_TIM_PWM_Start(&tim4, TIM_CHANNEL_2))
	{
		Error_Handler();
	}

	while(1)
	{
		while(brightness < tim4.Init.Period)
		{
			brightness+=20;
			__HAL_TIM_SET_COMPARE(&tim4, TIM_CHANNEL_2, brightness);
			HAL_Delay(1);
		}

		while(brightness > 0 )
		{
			brightness-=20;
			__HAL_TIM_SET_COMPARE(&tim4, TIM_CHANNEL_2, brightness);
			HAL_Delay(1);
		}
	}

	return 0;
}


void Error_Handler(void)
{
	while(1);
}

void Timer4_Init(void)
{
	TIM_OC_InitTypeDef  pwm_config;
	tim4.Instance = TIM4;
	tim4.Init.Period = 10000-1;// 1 second - ARR
	tim4.Init.Prescaler = 4;
	if(HAL_OK != HAL_TIM_PWM_Init(&tim4))
	{
		Error_Handler();
	}
	memset(&pwm_config,0,sizeof(pwm_config));
	pwm_config.OCMode = TIM_OCMODE_PWM1;
	pwm_config.OCPolarity = TIM_OCPOLARITY_HIGH;

	pwm_config.Pulse = (tim4.Init.Period * 80)/100;
	if(HAL_OK != HAL_TIM_PWM_ConfigChannel(&tim4, &pwm_config, TIM_CHANNEL_2))
	{
		Error_Handler();
	}

}

void SystemClockConfigHSE(uint32_t sysclk)
{
	RCC_OscInitTypeDef oscillator_init;
	RCC_ClkInitTypeDef clk_init;
	uint32_t fLatency = FLASH_ACR_LATENCY_0WS;

	memset(&oscillator_init,0, sizeof(oscillator_init));

	oscillator_init.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_HSI;
	oscillator_init.HSEState = RCC_HSE_ON;
	oscillator_init.LSEState = RCC_LSE_ON;
	oscillator_init.HSIState = RCC_HSI_ON;
	oscillator_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	oscillator_init.PLL.PLLState = RCC_PLL_ON;

	switch(sysclk)
	{
	case CLK_50MHZ:
		oscillator_init.PLL.PLLM = 4; // so 8/8 for HSE is 1 Mhz.
		oscillator_init.PLL.PLLN = 50; // now multiply i.e. 100Mhz
		oscillator_init.PLL.PLLP = RCC_PLLP_DIV2; //now divide by 2 i.e. 50Mhz. which will be HCLK i.e. AHB Clock.
		oscillator_init.PLL.PLLQ = 2; // now divide by 2 i..e 25Mhz which will be PCLK1 and PCLK2 - APB1 and APB2
		fLatency = FLASH_ACR_LATENCY_1WS;
		break;
	case CLK_84MHZ:
		oscillator_init.PLL.PLLM = 8; // so 8/8 for HSE is 1 Mhz.
		oscillator_init.PLL.PLLN = 168; // now multiply i.e. 168Mhz
		oscillator_init.PLL.PLLP = 2; //now divide by 2 i.e. 84Mhz. which will be HCLK i.e. AHB Clock.
		oscillator_init.PLL.PLLQ = 4; // now divide by 4 i..e 42Mhz which will be PCLK1 and PCLK2 - APB1 and APB2
		fLatency = FLASH_ACR_LATENCY_2WS;
		break;
	case CLK_120MHZ:
		oscillator_init.PLL.PLLM = 8; // so 8/8 for HSE is 1 Mhz.
		oscillator_init.PLL.PLLN = 240; // now multiply i.e. 240 Mhz
		oscillator_init.PLL.PLLP = 2; //now divide by 2 i.e. 120Mhz. which will be HCLK i.e. AHB Clock.
		oscillator_init.PLL.PLLQ = 4; //now divide by 4 i.e. 30Mhz. which will be PCLK1 i.e. APB1 and APB2 Clock
		fLatency = FLASH_ACR_LATENCY_3WS;
		break;

	}

	if(HAL_OK != HAL_RCC_OscConfig(&oscillator_init))
	{
		Error_Handler();
	}

	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

	if( HAL_OK != HAL_RCC_ClockConfig(&clk_init,fLatency) )
	{
		Error_Handler();
	}
	/*Configure the systick timer interrupt frequency (for every 1 ms) */
	uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
	HAL_SYSTICK_Config(hclk_freq/1000);

	/**Configure the Systick
	 */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void GPIO_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_7;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB,&ledgpio);
}
