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
void CAN1_Init(void);
void CAN1_Tx(void);
void CAN1_Rx(void);
void CAN1Filter_Config(void);


UART_HandleTypeDef huart3;
CAN_HandleTypeDef can1Handle;
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
	//Gpio_Init();
	USART3_Init();

	//LSE_Configuration();

	CAN1_Init(); // CAN from sleep to Init state.

	CAN1Filter_Config();  //Still CAN is in the Init state.

	HAL_CAN_Start(&can1Handle); // start the CAN from Init to normal mode.

	CAN1_Tx();

	CAN1_Rx();

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
	GPIO_InitTypeDef pa9; //connect PA9 to PA0 and check the frequency.
	__HAL_RCC_GPIOA_CLK_ENABLE();
	pa9.Mode = GPIO_MODE_OUTPUT_PP;
	pa9.Pin = GPIO_PIN_9;
	pa9.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &pa9);
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

void CAN1_Init(void)
{
	can1Handle.Instance = CAN1;
	can1Handle.Init.Mode = CAN_MODE_LOOPBACK;
	can1Handle.Init.AutoBusOff = DISABLE;
	can1Handle.Init.AutoRetransmission = ENABLE;
	can1Handle.Init.AutoWakeUp = DISABLE;
	can1Handle.Init.ReceiveFifoLocked = DISABLE;  //not locked on overrun
	can1Handle.Init.TimeTriggeredMode = DISABLE;
	can1Handle.Init.TransmitFifoPriority = DISABLE;
	can1Handle.Init.Prescaler= 5;
	can1Handle.Init.SyncJumpWidth = CAN_SJW_1TQ;
	can1Handle.Init.TimeSeg1 = CAN_BS1_8TQ;
	can1Handle.Init.TimeSeg2 = CAN_BS2_1TQ;

	if(HAL_OK != HAL_CAN_Init(&can1Handle))
	{
		Error_Handler();
	}


}

void CAN1_Tx(void)
{
   CAN_TxHeaderTypeDef txHeader;

   uint8_t our_message[5] = {'H','E','L','L','O'};
   uint32_t mailbox;

   txHeader.DLC = 5; // "Hello is 5 characters long"
   txHeader.StdId = 0x65D; // Arbitration ID
   txHeader.IDE = CAN_ID_STD; // standard CAN
   txHeader.RTR = CAN_RTR_DATA; // data.

   if(HAL_OK != HAL_CAN_AddTxMessage(&can1Handle, &txHeader, our_message, &mailbox))
   {
	   Error_Handler();
   }

   while(HAL_CAN_IsTxMessagePending(&can1Handle, mailbox));

   sprintf(usr_msg, "Message Transmitted\r\n");

   HAL_UART_Transmit(&huart3, (uint8_t*)usr_msg, strlen(usr_msg), HAL_MAX_DELAY);

}

void CAN1_Rx(void)
{
   CAN_RxHeaderTypeDef rxHeader;
   char recvd_msg[5];

   while(!HAL_CAN_GetRxFifoFillLevel(&can1Handle, CAN_RX_FIFO0));

   if(HAL_OK != HAL_CAN_GetRxMessage(&can1Handle, CAN_RX_FIFO0, &rxHeader, (uint8_t*)recvd_msg))
   {
	   Error_Handler();
   }

   HAL_UART_Transmit(&huart3, (uint8_t*)recvd_msg, strlen(recvd_msg), HAL_MAX_DELAY);
}

void CAN1Filter_Config(void)
{
   CAN_FilterTypeDef canFilter;
   canFilter.FilterActivation = ENABLE;
   canFilter.FilterBank = 0; //Bank 0
   canFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0; //Filter 0
   canFilter.FilterIdHigh = 0; // accept all messages
   canFilter.FilterIdLow = 0; // accept all messages
   canFilter.FilterMaskIdHigh = 0; // accept all messages
   canFilter.FilterMaskIdLow = 0; //accept all messages
   canFilter.FilterMode = CAN_FILTERMODE_IDMASK; // use ID and Mask for filtering messages.
   canFilter.FilterScale = CAN_FILTERSCALE_32BIT;

   if(HAL_OK != HAL_CAN_ConfigFilter(&can1Handle, &canFilter))
   {
	   Error_Handler();
   }
}

/***
 * CAN- Controller Area Network
 * Multi master serial communication bus
 * Design for high speed, high noise immunity and error detection
 * features
 * CAN offers data communication upto 1 Mbit/sec
 * It uses differential signals
 * Reliability, excellent error handling
 * Automatic re-transmission of faulty messages
 * Automatic bus disconnection of nodes that are suspected to be physically faulty
 * Functional addressing - data message do not contain source or destination
 * addresses, only identifies relating to their function and or priority.
 *
 * CAN transmission are broadcast types.
 * All devices can hear the transmission
 * All nodes will pick up the traffic on the bus
 * No way to send a data specifically to a node by its address or something.
 * Hot Pluggable - can be plugged without halting other nodes.
 * CAN - max 40 meters.
 *
 */


/** CAN logic 1 - recessive state - voltage is zero
 *  CAN logic 0 - dominant state - voltage is 2V around.
 *
 *  11-bit identifier field  + R bit.
 *  CAN version 2.0 A and B.
 *  CAN A - standard format with an 11-bit identifier
 *  CAN B - extended format with a 29-bit identifier.
 *
 */
