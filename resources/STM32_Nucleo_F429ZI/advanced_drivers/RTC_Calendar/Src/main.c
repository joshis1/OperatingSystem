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

void Error_Handler(void);
void Gpio_Init(void);
void SystemClockConfig(uint32_t sysclk);
void LSE_Configuration(void);
void USART3_Init(void);
void RTC_Init(void);
void RTC_CalendarConfig(void);


UART_HandleTypeDef huart3;
RTC_HandleTypeDef hrtc;
char usr_msg[100];

/** Connect PA8 to PA0
 *
 * LSE (32.768 Khz)
 *
 * **/

/**
 * http://www.bittiming.can-wiki.info/
 */


int main()
{
	HAL_Init();
	SystemClockConfig(CLK_50MHZ);
	Gpio_Init();
	USART3_Init();

	LSE_Configuration();

	RTC_Init();


	while(1)
	{

	}

	return 0;
}


void Error_Handler(void)
{
	while(1);
}

void Gpio_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_7;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB,&ledgpio);
}


void SystemClockConfig(uint32_t sysclk)
{
	RCC_OscInitTypeDef oscillator_init;
	RCC_ClkInitTypeDef clk_init;
	uint32_t fLatency = FLASH_ACR_LATENCY_0WS;

	memset(&oscillator_init,0, sizeof(oscillator_init));

	oscillator_init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_HSE;
	oscillator_init.HSIState = RCC_HSI_ON;
	oscillator_init.LSEState = RCC_LSE_ON;
	oscillator_init.HSEState = RCC_HSE_ON;
	oscillator_init.HSICalibrationValue = 16; // assuming we are running at normal operating environment
	oscillator_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	oscillator_init.PLL.PLLState = RCC_PLL_ON;

	switch(sysclk)
	{
	case CLK_50MHZ:
		oscillator_init.PLL.PLLM = 16; // so 16/16 for HSI is 1 Mhz.
		oscillator_init.PLL.PLLN = 100; // now multiply i.e. 100Mhz
		oscillator_init.PLL.PLLP = 2; //now divide by 2 i.e. 50Mhz. which will be HCLK i.e. AHB Clock.
		oscillator_init.PLL.PLLQ = 4; // now divide by 2 i..e 25Mhz which will be PCLK1 and PCLK2 - APB1 and APB2
		fLatency = FLASH_ACR_LATENCY_1WS;
		break;
	case CLK_84MHZ:
		oscillator_init.PLL.PLLM = 16; // so 16/16 for HSI is 1 Mhz.
		oscillator_init.PLL.PLLN = 168; // now multiply i.e. 168Mhz
		oscillator_init.PLL.PLLP = 2; //now divide by 2 i.e. 84Mhz. which will be HCLK i.e. AHB Clock.
		oscillator_init.PLL.PLLQ = 4; // now divide by 4 i..e 42Mhz which will be PCLK1 and PCLK2 - APB1 and APB2
		fLatency = FLASH_ACR_LATENCY_2WS;
		break;
	case CLK_120MHZ:
		oscillator_init.PLL.PLLM = 16; // so 16/16 for HSI is 1 Mhz.
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
	clk_init.APB2CLKDivider = RCC_HCLK_DIV4;

	if( HAL_OK != HAL_RCC_ClockConfig(&clk_init,fLatency) )
	{
		Error_Handler();
	}
}

void LSE_Configuration(void)
{
	//MCO1 pin(PA8)
	HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_LSE,RCC_MCODIV_1);
}

void USART3_Init(void)
{
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	if(HAL_OK != HAL_UART_Init(&huart3))
	{
		Error_Handler();
	}
}

void RTC_Init(void)
{
   hrtc.Instance = RTC;
   hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
   hrtc.Init.AsynchPrediv = 0x7F; // 128
   hrtc.Init.SynchPrediv = 0XFF; //255 or 255+1 i.e 256
   hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
   hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW;
   hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

   if(HAL_RTC_Init(&hrtc) != HAL_OK)
   {
	   Error_Handler();
   }
}

void RTC_CalendarConfig(void)
{

}

