/*
 * stm32f429x_usart_driver.c
 *
 *  Created on: 8/10/2020
 *      Author: sjoshi
 */

#include "stm32f429x_usart_driver.h"

void USART_PeriClockControl(USART_RegDef_t *pUsart, uint8_t EnOrDi)
{

	if( EnOrDi == ENABLE)
	{
		if(pUsart == USART1)
		{
			USART1_PCLK_EN();
		}
		else if(pUsart == USART2)
		{
			USART2_PCLK_EN();
		}
		else if(pUsart == USART3)
		{
			USART3_PCLK_EN();
		}
		else if(pUsart == USART6)
		{
			USART6_PCLK_EN();
		}
	}
	else
	{
		if(pUsart == USART1)
		{
			USART1_PCLK_DI();
		}
		else if(pUsart == USART2)
		{
			USART2_PCLK_DI();
		}
		else if(pUsart == USART3)
		{
			USART3_PCLK_DI();
		}
		else if(pUsart == USART6)
		{
			USART6_PCLK_DI();
		}
	}
}

void USART_Init(USART_Handle_t *pUSARTHandle)
{
	uint32_t tempReg = 0;

	if(pUSARTHandle->usart_Config.usart_mode  == USART_MODE_ONLY_RX)
	{
		tempReg = (0x1 << USART_CR1_TE);
	}
	else if(pUSARTHandle->usart_Config.usart_mode  == USART_MODE_ONLY_TX)
	{
		tempReg = (0x1 << USART_CR1_RE);
	}
	else if(pUSARTHandle->usart_Config.usart_mode  == USART_MODE_BOTH_TX_RX)
	{
		tempReg = (0x1 << USART_CR1_RE);
		tempReg |= (0x1 << USART_CR1_TE);
	}

	//word bits - 8 bits or 9 bits -  0 and 1 respectively
	tempReg |= pUSARTHandle->usart_Config.usart_wordLength << USART_CR1_M;

	//Parity bit

	if(pUSARTHandle->usart_Config.usart_parityControl == USART_PARITY_ENABLE_ODD)
	{
		tempReg |= (0x1 << USART_CR1_PCE); //enable parity selection
		tempReg |= (0x1 << USART_CR1_PS); //odd parity is 1.
	}
	else if(pUSARTHandle->usart_Config.usart_parityControl == USART_PARITY_ENABLE_EVEN)
	{
		tempReg |= (0x1 << USART_CR1_PCE); //enable parity selection
		tempReg &= ~(0x1 << USART_CR1_PS); //Even parity is 0.
	}
	else if(pUSARTHandle->usart_Config.usart_parityControl == USART_PARITY_DISABLE)
	{
		tempReg &= ~(0x1 << USART_CR1_PCE); //disable parity selection
	}

	pUSARTHandle->pUSARTx->USART_CR1 = tempReg;


	/**** CR2 Configuration *******/
	tempReg = 0;
	tempReg |= pUSARTHandle->usart_Config.usart_number_of_stopBits << USART_CR2_STOPBITS;
	pUSARTHandle->pUSARTx->USART_CR2 = tempReg;

	/**** CR3 Configuration ****/
	tempReg = 0;
	if(pUSARTHandle->usart_Config.usart_hwFlowControl == USART_HW_CONTROL_CTS)
	{
		tempReg |=  (0x1 << USART_CR3_CTSE);
	}
	else if(pUSARTHandle->usart_Config.usart_hwFlowControl == USART_HW_CONTROL_CTS)
	{
		tempReg |=  (0x1 << USART_CR3_RTSE);
	}
	else if(pUSARTHandle->usart_Config.usart_hwFlowControl == USART_HW_CONTROL_CTS_RTS)
	{
		tempReg |=  ((0x1 << USART_CR3_RTSE) | (0x1 << USART_CR3_CTSE));
	}

	pUSARTHandle->pUSARTx->USART_CR3 = tempReg;

	/**BRR - Baud Rate Configuration **/
	tempReg = 0;

}

void USART_IRQInterruptConfig(uint8_t irqNumber, uint8_t EnorDi)
{
	uint8_t bit_pos = 0;
	bit_pos = irqNumber % 32;
	volatile uint32_t *nvic_reg;

	if(EnorDi == ENABLE)
	{
		if(irqNumber >= 0 && irqNumber < 32)
		{
			nvic_reg = (uint32_t *)NVIC_ISER0;
			*nvic_reg  |= (0x1U << bit_pos);
		}
		else if(irqNumber >= 32 && irqNumber < 64)
		{
			nvic_reg = (uint32_t *)NVIC_ISER1;
			*nvic_reg  |= (0x1U << bit_pos);

		}
		else if(irqNumber >= 64 && irqNumber < 96)
		{
			nvic_reg = (uint32_t *)NVIC_ISER2;
			*nvic_reg  |= (0x1U << bit_pos);
		}
		else
		{
			//probably shouldn't reach here.
		}
	}

	else
	{
		if(irqNumber >= 0 && irqNumber < 32)
		{
			nvic_reg = (uint32_t *)NVIC_ICER0;
			*nvic_reg  |= (0x1U << bit_pos);
		}
		else if(irqNumber >= 32 && irqNumber < 64)
		{
			nvic_reg = (uint32_t *)NVIC_ICER1;
			*nvic_reg  |= (0x1U << bit_pos);
		}
		else if(irqNumber >= 64 && irqNumber < 96)
		{
			nvic_reg = (uint32_t *)NVIC_ICER2;
			*nvic_reg  |= (0x1U << bit_pos);
		}
		else
		{
			//probably shouldn't reach here.
		}
	}
}

void USART_IRQPriorityConfig(uint8_t irqNumber, uint16_t priority)
{
	uint8_t nvic_priority_reg_offset = 0;
	uint8_t bit_pos = 0;
	volatile uint32_t *nvic_register;
	//each PR can fit 4 IRQNumber
	nvic_priority_reg_offset = irqNumber / 4;
	nvic_register = (uint32_t *)(NVIC_IPR0 + (nvic_priority_reg_offset * 4));
	bit_pos = irqNumber % 4;
	//lower 4 bits of priority are not implemented in STM32
	// each priority value takes 8 bits
	*nvic_register = (priority << ((bit_pos * 8) + 4));
}

void USART_PeriControl(USART_RegDef_t *pUsart, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pUsart->USART_CR1 |= (0x1 << USART_CR1_UE);
	}
	else
	{
		pUsart->USART_CR1 &= ~(0x1 << USART_CR1_UE);
	}
}


void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t len)
{
   uint16_t *pData;

   for(uint32_t i = 0; i < len; i++)
   {
	   while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_SR_TXE));

	   if(pUSARTHandle->usart_Config.usart_wordLength == USART_WORD_LEN_9_BITS)
	   {
		   pData = (uint16_t*)pTxBuffer;
		   pUSARTHandle->pUSARTx->USART_DR = (*pData & (uint16_t)0x01FF);

		   if(pUSARTHandle->usart_Config.usart_parityControl == USART_PARITY_DISABLE)
		   {
			   //No parity is used in this transfer. so, 9bits of user data will be sent
			   //Implement the code to increment pTxBuffer twice
			   pTxBuffer++;
			   pTxBuffer++;
		   }
		   else
		   {
			   //Parity bit is used in this transfer . so , 8bits of user data will be sent
			   //The 9th bit will be replaced by parity bit by the hardware
			   pTxBuffer++;
		   }
	   }
	   // Word Len 8 bits
	   else
	   {
		   pUSARTHandle->pUSARTx->USART_DR = (*pTxBuffer & (uint8_t)0xFF);
		   pTxBuffer++;
	   }
   }
   //Implement the code to wait till TC flag is set in the SR
   while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_SR_TC));

}
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len)
{
    for(uint32_t i = 0; i < len; i++)
    {
       //wait until RXNE flag is set in the SR
 	   while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_SR_RXNE));

    }
}

uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t len)
{

}


uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len)
{

}

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUsart, uint8_t flag)
{
	if(pUsart->USART_SR & (0x1 << flag))
	{
		return 1;
	}

	return 0;
}

