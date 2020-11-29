/*
 * main.c
 *
 *  Created on: 27/11/2020
 *      Author: sjoshi
 */

//#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "main.h"

void SystemClockConfig(void);
void USART3_Init(void);
void Error_Handler(void);


UART_HandleTypeDef huart3;

int main()
{
  HAL_Init();
  SystemClockConfig();
  USART3_Init();

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
