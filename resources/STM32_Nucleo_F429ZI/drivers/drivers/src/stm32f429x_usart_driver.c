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
