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

void SystemClockConfigHSE_Max();
void USART3_Init(void);
void Error_Handler(void);

UART_HandleTypeDef huart3;

char msg[1024];

/**
 * PLL - Phase locked loop - engine of the MCU is used to generate different
 * high frequency O/P clocks by taking input clock sources such as HSE or HSI
 * By using PLL you can drive Sysclk upto 180Mhz in STM32F429ZI
 */

/**
 *    HSE - 8Mhz
 *                    -->  /M  --> VCO                |     ----->  /P  --> HCLK
 *                               <-- /N (Multipler) --|     ------> /Q  --> APB1
 *                                                          ----->  /R   --> APB2
 *    HSI - 16Mhz
 *
 *  Here /N is the multiplier - /P minimum value is 2 and this feeds to HCLK i.e. AHB
 *  -- Ensure that the input clock to VCO is within 1MHz < fvco <= 2Mhz
 *  -- The output clock limitation is 100Mhz < f o/p vco <= 432Mhz
 *   /Q minimum value is 2 is feed to APB1 also 2 is the default value i.e after reset
 *   /R minimum value is 2 is feed to APB2 also 2 is the default value i.e after reset
 *   Heart of the main PLL - is VCO - Voltage Controlled Oscillator.
 */

/** APP - HCLk frequency using PLL - use HSI as PLL's input source and repeat the same
 * exercise using HSE as input sources.
 * 50Mhz, 84Mhz and 120 Mhz.
 */

int main()
{
	HAL_Init();
	SystemClockConfigHSE_Max();

	__HAL_RCC_HSI_DISABLE(); //Saves some current

	/* LETS REDO THE SYSTICK CONFIGURATION */

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	USART3_Init();

	memset(msg,0,strlen(msg));

	sprintf(msg,"SYSCLK: %ld \r\n", HAL_RCC_GetSysClockFreq());

	if (HAL_OK != HAL_UART_Transmit(&huart3,(uint8_t *)msg, strlen(msg), HAL_MAX_DELAY))
	{
		Error_Handler();
	}

	memset(msg,0,strlen(msg));
	sprintf(msg,"HCLK: %ld hz \r\n", HAL_RCC_GetHCLKFreq());
	if (HAL_OK != HAL_UART_Transmit(&huart3,(uint8_t *)msg, strlen(msg), HAL_MAX_DELAY))
	{
		Error_Handler();
	}

	memset(msg,0,strlen(msg));
	sprintf(msg,"PCLK1: %ld hz\r\n", HAL_RCC_GetPCLK1Freq());
	if (HAL_OK != HAL_UART_Transmit(&huart3,(uint8_t *)msg, strlen(msg), HAL_MAX_DELAY))
	{
		Error_Handler();
	}

	memset(msg,0,strlen(msg));
	sprintf(msg,"PCLK2 %ld hz\r\n", HAL_RCC_GetPCLK2Freq());
	if (HAL_OK != HAL_UART_Transmit(&huart3,(uint8_t *)msg, strlen(msg), HAL_MAX_DELAY))
	{
		Error_Handler();
	}


	while(1);

	return 0;
}

void SystemClockConfigHSE_Max()
{
	RCC_OscInitTypeDef oscillator_init;
	RCC_ClkInitTypeDef clk_init;
	uint32_t fLatency = FLASH_ACR_LATENCY_0WS;

	// Enable the clock for power control
	__HAL_RCC_PWR_CLK_ENABLE();
	//set regulator voltage scaling to 1.
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	//Turn on over drive mode - enable
	HAL_PWREx_EnableOverDrive();

	memset(&oscillator_init,0, sizeof(oscillator_init));

	oscillator_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	oscillator_init.HSEState = RCC_HSE_BYPASS;
	oscillator_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	oscillator_init.PLL.PLLState = RCC_PLL_ON;
	oscillator_init.PLL.PLLM = 8; // so 8/8 for HSE is 1 Mhz.
	oscillator_init.PLL.PLLN = 360; // now multiply i.e. 360 Mhz
	oscillator_init.PLL.PLLP = 2; //now divide by 2 i.e. 180Mhz. which will be HCLK i.e. AHB Clock.
	oscillator_init.PLL.PLLQ = 8; //now divide by 4 i.e. 45Mhz. which will be PCLK1 i.e. APB1 and APB2 Clock
	fLatency = FLASH_ACR_LATENCY_5WS;


	if(HAL_OK != HAL_RCC_OscConfig(&oscillator_init))
	{
		Error_Handler();
	}

	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV8;

	if( HAL_OK != HAL_RCC_ClockConfig(&clk_init,fLatency) )
	{
		Error_Handler();
	}
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