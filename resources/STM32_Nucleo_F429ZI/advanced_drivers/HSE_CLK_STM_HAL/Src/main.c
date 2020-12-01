/*
 * main.c
 *
 *  Created on: 27/11/2020
 *      Author: sjoshi
 */

//#include "stm32f4xx.h"
#include "main.h"

void SystemClockConfig(void);
void USART3_Init(void);
void Error_Handler(void);

uint8_t lower_to_upper(uint8_t data);


UART_HandleTypeDef huart3;
int reception_complete = 0;

char *user_data = "Advanced stm32 \r\n";
uint8_t data_buffer[1024];
uint8_t count = 0;
uint8_t recvd_data;

uint8_t lower_to_upper(uint8_t data)
{
	if(data >= 'a' && data <= 'z')
	{
		//lower case
		return (data-32);
	}
	return data;
}

int main()
{
	HAL_Init();
	SystemClockConfig();
	USART3_Init();

	if (HAL_OK != HAL_UART_Transmit(&huart3,(uint8_t *)user_data, strlen(user_data), HAL_MAX_DELAY))
	{
		Error_Handler();
	}

	while(!reception_complete)
	{
		HAL_UART_Receive_IT(&huart3, &recvd_data,1);
	}

	if (HAL_OK != HAL_UART_Transmit(&huart3,(uint8_t *)data_buffer, count, HAL_MAX_DELAY))
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(recvd_data == '\r')
	{
		data_buffer[count++] = '\n';
		reception_complete = 1;
	}
	else
	{
		data_buffer[count++] = lower_to_upper(recvd_data);
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
