/*
 * stm32f429x_i2c_driver.c
 *
 *  Created on: 28Sep.,2020
 *      Author: shreyas.joshi
 */

#include "stm32f429x_i2c_driver.h"

uint16_t ahb_prescaler_vals[] = {2,4,8,16,64,128,256,512};
uint8_t apb2_prescaler_vals[] = {2,4,8,16};


static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhase(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr);
static void I2C_ClearAddrFlag(I2C_RegDef_t *pI2Cx);
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);

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
	uint32_t tempReg = 0;
	uint16_t ccrValue;

	/** CR1 **/
	tempReg = pI2CHandle->i2c_pinConfig.I2C_ACKControl << I2C_CR1_ACK;
    pI2CHandle->pI2Cx->I2C_CR1 |= tempReg;

    /** CR2 - Freq **/
    tempReg = 0;
    tempReg |= RCC_GetAPB_PClkValue()/(1000 * 1000);
    pI2CHandle->pI2Cx->I2C_CR2 = tempReg & 0x3F; //6 bits

    /** OAR1 - Own Address register 1 **/
    tempReg = 0;
    tempReg |= pI2CHandle->i2c_pinConfig.I2C_DeviceAddress << 1;
    tempReg |= 0x1 << 14; // should always be kept 1 by the software - page - 866
    pI2CHandle->pI2Cx->I2C_OAR1 = tempReg;

    /**CCR Registers **/
    tempReg = 0;
    if( pI2CHandle->i2c_pinConfig.I2C_SclSpeed == I2C_SCL_SPEED_STANDARD_MODE)
    {
    	//Standard Mode -- 100Kbps
        ccrValue =  RCC_GetAPB_PClkValue() / ( 2 * pI2CHandle->i2c_pinConfig.I2C_SclSpeed);
        tempReg |= ccrValue & 0xFFF; // 12 bits
    }
    else
    {
    	//fast mode
        tempReg |= 0x1 << I2C_F_S; //enable the fast mode
        tempReg |= pI2CHandle->i2c_pinConfig.I2C_FMDutyCycle << I2C_DUTY;

        if(pI2CHandle->i2c_pinConfig.I2C_FMDutyCycle ==  I2C_FM_DUTY_2 )
        {
        	ccrValue = (RCC_GetAPB_PClkValue() / ( 3 *  pI2CHandle->i2c_pinConfig.I2C_SclSpeed ) );
        }
        else
        {
        	//I2C_FM_DUTY_16_9
        	ccrValue = (RCC_GetAPB_PClkValue() / ( 25 *  pI2CHandle->i2c_pinConfig.I2C_SclSpeed ) );
        }

        tempReg |= ccrValue & 0xFFF; // 12 bits
    }

    pI2CHandle->pI2Cx->I2C_CCR = tempReg;

    //Trise - settings

    tempReg = 0;

    /** For max Trise value - check I2C specifications -
     * Table 10. Characteristics of the SDA and SCL bus
     * lines for Standard, Fast, and Fast-mode Plus I2C-bus devices **/
    if( pI2CHandle->i2c_pinConfig.I2C_SclSpeed == I2C_SCL_SPEED_STANDARD_MODE)
    {
    	//Standard Mode -- 100Kbps -- 1000 nano seconds.
    	tempReg =  (RCC_GetAPB_PClkValue() / (1000 * 1000)) + 1;
    }
    else
    {
    	//fast Mode - 300 nano seconds
    	tempReg =  ((RCC_GetAPB_PClkValue() * 300)/(1000 * 1000 * 1000)) + 1;
    }

    pI2CHandle->pI2Cx->I2C_TRISE = tempReg & 0x3F; // [5:0] 6 bits

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

	if(pI2Cx->I2C_SR1 & (0x1 << flag))
	{
		return 1;
	}

	return 0;
}

/**CCR value **/
/**
 *  Page - 873 - Reference Manual
 *
 * if Duty = 0 and Fast Mode
 * T high = CCR * T pclk1
 * T low =  2 * CCR * T pclk1
 *
 * T high + T low = 3 CCR * T pclk1
 * T pclk is APB1 - peripheral
 * Tpclk we are configuring is 1/16Mhz since we are using HSI.
 * Tpclk = 62.5 * 10^-9
 * Let's say we want Clk to be 100Khz.
 * 1/100Khz is T high + T low
 * 5 *10^-6 = 3 * CCR * 62.5 * 10 ^ -9
 * 0.02666 * 10^3 = CCR
 * 26 = CCR
 * or in hex = 0x1A
 *
 * **************************************************
 *  If Standard Mode
 *  T high = CCR * T pclk1
 *  T low = CCR * T pclk1
 * T high + T low = 2 CCR * T pclk
 * we want Clk to 100Khz
 * 5 *10- ^ -6 = 2 CCR * 62.5 * 10 ^ -9
 * 0.04 *10 ^-6 = CCR * 10 ^ -9
 * 40 = CCR
 * or in hex = 0x28
 *
 */

/**
 * GetAPB1 - Clock Frequency  - Peripheral clock frequency of APB1
 *
 */

uint64_t RCC_GetAPB_PClkValue()
{
   uint8_t system_clk_src;
   uint32_t systemClk;
   uint16_t ahb_prescaler;
   uint16_t apb2_prescaler;
   uint8_t ahb_prescalerVal;
   uint8_t apb2_prescalerVal;

   system_clk_src = ( RCC->RCC_CFGR >> 2 ) & 0x3;

   if(system_clk_src == 0) //HSI oscillator
   {
	   systemClk = 16 * 1000 * 1000; //16 Mhz.
   }
   else if (system_clk_src == 1) //HSE oscillator
   {
	   systemClk = 8 * 1000 * 1000; //16 Mhz.
   }
   else
   {
       //PLL
   }

   ahb_prescaler = (RCC->RCC_CFGR >> 4) & 0xF;
   if( ahb_prescaler < 8)
   {
	   ahb_prescalerVal = 1;
   }
   else
   {
     ahb_prescalerVal = ahb_prescaler_vals[ahb_prescaler - 8];
   }

   apb2_prescaler = (RCC->RCC_CFGR >> 13) & 0x7;
   if(apb2_prescaler < 4)
   {
	   apb2_prescalerVal = 1;
   }
   else
   {
	   apb2_prescalerVal = apb2_prescaler_vals[apb2_prescaler - 4];
   }

   return ((systemClk /ahb_prescalerVal))/apb2_prescalerVal;
}


void I2C_MasterDataSend(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t len, uint8_t slaveAddr)
{
	  //1. Generate the start condition
	  I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	  //2. Confirm that start generation is completed by checking the SB Flag in the SR1,
	  // Note: Until SB is cleared SCL will be stretched (pull to low)
	  while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_SB));

	  //3. Send the address of the slave with r/nw bit set to w(0) - total 8 bits.
	  I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, slaveAddr);

	  //4. Confirm that address phase is completed by checking the ADDR flag in the SR1.
	  while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_ADDR));

	  //5. Clear the Addr flag according to its software sequence
	  // until Addr is cleared SCL will be stretched - pulled to low.
      I2C_ClearAddrFlag(pI2CHandle->pI2Cx);

      //6. Send the data until len becomes 0.
      while(len > 0)
      {
    	  while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_TXE));
    	  pI2CHandle->pI2Cx->I2C_DR = *pTxBuffer;
    	  pTxBuffer++;
    	  len--;
      }

      //7. when len becomes zero wait for TXE=1 and BTF ( byte transfer complete)  =  1 before generating the stop condition.
      // Note TXE=1 and BTF=1 means that both SR and DR are empty and next transmission should begin when BTF = 1 and SCL will be
      // stretched ( pulled to low)
      while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_TXE));
      while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_BTF));

      //8. Generate the STOP condition and master need not to wait for the completion of stop condition
      // Note - generating stop, automatically clears the BTF.
      I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
}



static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->I2C_CR1 |= 0x1 << I2C_CR1_START;
}

static void I2C_ExecuteAddressPhase(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr)
{
	slaveAddr = slaveAddr << 1;
	slaveAddr &= ~(0x1);
	pI2Cx->I2C_DR = slaveAddr;
}

static void I2C_ClearAddrFlag(I2C_RegDef_t *pI2Cx)
{
	uint32_t dummyRead = pI2Cx->I2C_SR1;
	dummyRead = pI2Cx->I2C_SR2;
	(void)dummyRead;
}

static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->I2C_CR1 |= 0x1 << I2C_CR1_STOP;
}

