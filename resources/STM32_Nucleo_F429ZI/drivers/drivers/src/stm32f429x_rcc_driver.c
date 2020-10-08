/*
 * stm32f429x_rcc_driver.c
 *
 *  Created on: 9/10/2020
 *      Author: sjoshi
 */

#include "stm32f429x_rcc_driver.h"

uint16_t ahb_prescaler_vals[] = {2,4,8,16,64,128,256,512};
uint8_t apb_prescaler_vals[] = {2,4,8,16};

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

uint64_t RCC_GetAPB2_PClkValue()
{
   uint8_t system_clk_src;
   uint32_t systemClk;
   uint16_t ahb_prescaler;
   uint16_t apb1_prescaler;
   uint8_t ahb_prescalerVal;
   uint8_t apb1_prescalerVal;

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

   apb1_prescaler = (RCC->RCC_CFGR >> 13) & 0x7;
   if(apb1_prescaler < 4)
   {
	   apb1_prescalerVal = 1;
   }
   else
   {
	   apb1_prescalerVal = apb_prescaler_vals[apb1_prescaler - 4];
   }

   return ((systemClk /ahb_prescalerVal))/apb1_prescalerVal;
}

uint64_t RCC_GetAPB1_PClkValue()
{
	uint8_t system_clk_src;
	uint32_t systemClk;
	uint16_t ahb_prescaler;
	uint16_t apb1_prescaler;
	uint8_t ahb_prescalerVal;
	uint8_t apb1_prescalerVal;

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

	apb1_prescaler = (RCC->RCC_CFGR >> 10) & 0x7;
	if(apb1_prescaler < 4)
	{
		apb1_prescalerVal = 1;
	}
	else
	{
		apb1_prescalerVal = apb_prescaler_vals[apb1_prescaler - 4];
	}

	return ((systemClk /ahb_prescalerVal))/apb1_prescalerVal;
}
