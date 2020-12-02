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

void SystemClockConfig(void);
void USART3_Init(void);
void Error_Handler(void);

UART_HandleTypeDef huart3;

char msg[1024];

int main()
{
	HAL_Init();
	SystemClockConfig();

	RCC_OscInitTypeDef oscillator_init;
	RCC_ClkInitTypeDef clk_init;
	memset(&oscillator_init,0, sizeof(oscillator_init));

	oscillator_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	oscillator_init.HSEState = RCC_HSE_BYPASS;

	if(HAL_OK != HAL_RCC_OscConfig(&oscillator_init))
	{
		Error_Handler();
	}

	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

	if( HAL_OK != HAL_RCC_ClockConfig(&clk_init,FLASH_ACR_LATENCY_0WS) )
	{
		Error_Handler();
	}

	 __HAL_RCC_HSI_DISABLE(); //Saves some current

	 /* LETS REDO THE SYSTICK CONFIGURATION */


	//HCLK is 4Mhz
	// 0.25 micro seconds 1 tick
	// we need systick for every 1 milliseconds.
	// 4000 ticks is 1 milliseconds then.
	HAL_SYSTICK_Config(4000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	USART3_Init();

	memset(msg,0,strlen(msg));

	sprintf(msg,"SYSCLK: %ld \r\n", HAL_RCC_GetSysClockFreq());

	if (HAL_OK != HAL_UART_Transmit(&huart3,(uint8_t *)msg, strlen(msg), HAL_MAX_DELAY))
	{
		Error_Handler();
	}

	memset(msg,0,strlen(msg));
	sprintf(msg,"HCLK: %ld \r\n", HAL_RCC_GetHCLKFreq());
	if (HAL_OK != HAL_UART_Transmit(&huart3,(uint8_t *)msg, strlen(msg), HAL_MAX_DELAY))
	{
		Error_Handler();
	}

	memset(msg,0,strlen(msg));
	sprintf(msg,"PCLK1: %ld \r\n", HAL_RCC_GetPCLK1Freq());
	if (HAL_OK != HAL_UART_Transmit(&huart3,(uint8_t *)msg, strlen(msg), HAL_MAX_DELAY))
	{
		Error_Handler();
	}

	memset(msg,0,strlen(msg));
	sprintf(msg,"PCLK2 %ld \r\n", HAL_RCC_GetPCLK2Freq());
	if (HAL_OK != HAL_UART_Transmit(&huart3,(uint8_t *)msg, strlen(msg), HAL_MAX_DELAY))
	{
		Error_Handler();
	}


	while(1);

	return 0;
}

void SystemClockConfig(void)
{
	//internal RC oscillator - 16 Mhz
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

void Error_Handler(void)
{
	while(1);
}
