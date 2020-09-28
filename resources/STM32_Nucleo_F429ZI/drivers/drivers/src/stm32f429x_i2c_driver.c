/*
 * stm32f429x_i2c_driver.c
 *
 *  Created on: 28Sep.,2020
 *      Author: shreyas.joshi
 */

#include "stm32f429x_i2c_driver.h"

void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi)
{

	if( EnOrDi == ENABLE)
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_EN();
		}
		else if(pI2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}
		else if(pI2Cx == I2C3)
		{
			I2C3_PCLK_EN();
		}
	}
	else
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_DI();
		}
		else if(pI2Cx == I2C2)
		{
			I2C2_PCLK_DI();
		}
		else if(pI2Cx == I2C3)
		{
			I2C3_PCLK_DI();
		}
	}
}


void I2C_Init(I2C_Handle_t *pI2CHandle)
{


}

void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{
	if(pI2Cx == I2C1)
	{
		I2C1_RESET();
	}
	else if(pI2Cx == I2C2)
	{
		I2C2_RESET();
	}
	else if(pI2Cx == I2C3)
	{
		I2C3_RESET();
	}

}


void I2C_IRQInterruptConfig(uint8_t irqNumber, uint8_t EnorDi)
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

void I2C_IRQPriorityConfig(uint8_t irqNumber, uint16_t priority)
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

void I2C_PeriControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pI2Cx->I2C_CR1 |= (0x1 << I2C_CR1_PE);
	}
	else
	{
		pI2Cx->I2C_CR1 &= ~(0x1 << I2C_CR1_PE);
	}
}

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint8_t flag)
{

	return 0;
}
