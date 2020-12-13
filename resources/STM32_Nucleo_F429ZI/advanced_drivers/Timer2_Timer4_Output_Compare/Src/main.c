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


void Timer2_Init(void);
void Timer4_Init(void);
void Error_Handler(void);
void SystemClockConfigHSE(uint32_t sysclk);
void GPIO_Init(void);


TIM_HandleTypeDef tim2;

//For Blue LED - Timer 4 Channel 2 - PB7 LED
TIM_HandleTypeDef tim4;

/** Write an application by using TIMER2 to produce a square waveforms of
 *  500Hz -- Channel 1
 *  1Khz - Channel 2
 *  2Khz - Channel 3
 *  4Khz - Channel 4
 */

/** Output compare is reverse of input capture
 *  Store the value in CCR1 i.e. Pulse Value
 *  Produce waveforms using Output compare mode.
 *  When the value of CNT equals CCR1 then the o/p channel will be toggled.
 */

/** User Leds
 * User LD1 - green user LED is connected to STM32 I/O PB0
 * LD2 -- a blue user LED is connected to PB7
 * LD3 -- a red user LED is connected to PB14
 * user LEDs are on when the I/O is high value, and are off when the I/O is low.
 */

/*
 *  25Mhz- Clock running
 *  We need 500Hz- 1/500 = 0.002, time on = 0.001
 *  freq on = 1000
 *  counts = 25 * 10^6/1000 = 25000
 *
 *  We need 1Khz - 1/1000 = 0.001, time on - 0.0005
 *  freq on - 2000
 *  counts = 25* 10^6/ 2000 = 12500
 *
 *  We need 2Khz - 1/2000 =0.0005, time on - 0.00025
 *  freq on - 4000
 *  counts = 25*10^6/4000 = 6250
 *
 *  We need 4Khz - 1/4000 = 0.00025, time on - 0.000125
 *  freq on - 8000
 *  counts = 25 *10^6/8000 = 3125
 */

uint32_t pulse[4] = {25000, 12500, 6250, 3125};




int main()
{
	HAL_Init();
	SystemClockConfigHSE(CLK_50MHZ);
	//GPIO_Init();
	Timer2_Init();
	Timer4_Init(); // For blue user LED

	if(HAL_OK !=HAL_TIM_OC_Start_IT(&tim4,TIM_CHANNEL_2))
	{
		Error_Handler();
	}

#if 1

	if(HAL_OK !=HAL_TIM_OC_Start_IT(&tim2,TIM_CHANNEL_1))
	{
		Error_Handler();
	}

	if(HAL_OK !=HAL_TIM_OC_Start_IT(&tim2,TIM_CHANNEL_2))
	{
		Error_Handler();
	}

	if(HAL_OK !=HAL_TIM_OC_Start_IT(&tim2,TIM_CHANNEL_3))
	{
		Error_Handler();
	}

	if(HAL_OK !=HAL_TIM_OC_Start_IT(&tim2,TIM_CHANNEL_4))
	{
		Error_Handler();
	}
#endif

	while(1);

	return 0;
}


void Error_Handler(void)
{
	while(1);
}

void Timer4_Init(void)
{
	TIM_OC_InitTypeDef  oc_config;
	tim4.Instance = TIM4;
	tim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim4.Init.Period = 0xFFFF - 1;
	tim4.Init.Prescaler = 5000; //basically this will become 2.
	//Timer 2 running at 25Mhz.
	if(HAL_OK != HAL_TIM_OC_Init(&tim4))
	{
		Error_Handler();
	}

	memset(&oc_config,0,sizeof(oc_config));

	oc_config.OCMode = TIM_OCMODE_TOGGLE;
	oc_config.OCPolarity = TIM_OCPOLARITY_HIGH;


	oc_config.Pulse = pulse[2];
	if(HAL_OK !=HAL_TIM_OC_ConfigChannel(&tim4, &oc_config, TIM_CHANNEL_2))
	{
		Error_Handler();
	}
}


void Timer2_Init(void)
{
	TIM_OC_InitTypeDef  oc_config;
	tim2.Instance = TIM2;
	tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2.Init.Period = 0xFFFF - 1;
	tim2.Init.Prescaler = 5000; //basically this will become 2.
	//Timer 2 running at 25Mhz.
	if(HAL_OK != HAL_TIM_OC_Init(&tim2))
	{
		Error_Handler();
	}

	memset(&oc_config,0,sizeof(oc_config));

	oc_config.OCMode = TIM_OCMODE_TOGGLE;
	oc_config.OCPolarity = TIM_OCPOLARITY_HIGH;

	oc_config.Pulse = pulse[0];
	if(HAL_OK !=HAL_TIM_OC_ConfigChannel(&tim2, &oc_config, TIM_CHANNEL_1))
	{
		Error_Handler();
	}

	oc_config.Pulse = pulse[1];
	if(HAL_OK !=HAL_TIM_OC_ConfigChannel(&tim2, &oc_config, TIM_CHANNEL_2))
	{
		Error_Handler();
	}

	oc_config.Pulse = pulse[2];
	if(HAL_OK !=HAL_TIM_OC_ConfigChannel(&tim2, &oc_config, TIM_CHANNEL_3))
	{
		Error_Handler();
	}
	oc_config.Pulse = pulse[3];
	if(HAL_OK !=HAL_TIM_OC_ConfigChannel(&tim2, &oc_config, TIM_CHANNEL_4))
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

uint32_t ccr_content = 0;

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1, ccr_content + pulse[0]);
	}
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_2, ccr_content + pulse[1]);
	}

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_3);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_3, ccr_content + pulse[2]);
	}

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_4);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_4, ccr_content + pulse[3]);
	}
}

void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&ledgpio);

	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef led_bluegpio;
	led_bluegpio.Pin = GPIO_PIN_7;
	led_bluegpio.Mode = GPIO_MODE_OUTPUT_PP;
	led_bluegpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB,&led_bluegpio);
}
