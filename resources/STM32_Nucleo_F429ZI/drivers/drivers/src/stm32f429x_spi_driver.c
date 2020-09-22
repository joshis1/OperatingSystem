/*
 * stm32f429x_spi_driver.c
 *
 *  Created on: 22Sep.,2020
 *      Author: shreyas.joshi
 */

#include "stm32f429x_spi_driver.h"

/** APIs **/
/** Peripheral clock enable/disable - RCC - Enable/Disable  **/
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if( EnOrDi == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}
		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}
		else if(pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
	}
	else
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_DI();
		}
		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_DI();
		}
		else if(pSPIx == SPI3)
		{
			SPI3_PCLK_DI();
		}
	}
}

/**Init and De-Init **/
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	uint32_t spi_cr1 = 0;

	spi_cr1 |= pSPIHandle->spi_pinConfig.cpha << SPI_CR1_CPHA;
	spi_cr1 |= pSPIHandle->spi_pinConfig.cpol << SPI_CR1_CPOL;
	spi_cr1 |= pSPIHandle->spi_pinConfig.deviceMode << SPI_CR1_MSTR;
	spi_cr1 |= pSPIHandle->spi_pinConfig.sclkSpeed << SPI_CR1_BR;
	spi_cr1 |= pSPIHandle->spi_pinConfig.ssm  << SPI_CR1_SSM;
	spi_cr1 |= pSPIHandle->spi_pinConfig.dff << SPI_CR1_DFF;

	if(pSPIHandle->spi_pinConfig.busConfig == Full_Duplex)
	{
		spi_cr1 &= ~(0x1 << SPI_CR1_BIDIMODE);
	}
	else if(pSPIHandle->spi_pinConfig.busConfig == SIMPLEX_RX_ONLY)
	{
		spi_cr1 &= ~(0x1 << SPI_CR1_BIDIMODE);
		spi_cr1 |= (0x1 << SPI_CR1_RXONLY);
	}
	else if(pSPIHandle->spi_pinConfig.busConfig == HALF_Duplex)
	{
		spi_cr1 |= (0x1 << SPI_CR1_BIDIMODE);
	}

	pSPIHandle->pSPIx->SPI_CR1 = spi_cr1;

}
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if(pSPIx == SPI1)
	{
		SPI1_RESET();
	}
	else if(pSPIx == SPI2)
	{
		SPI2_RESET();
	}
	else if(pSPIx == SPI3)
	{
		SPI3_RESET();
	}
}

void SPI_PeriControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->SPI_CR1 |= (0x1 << SPI_CR1_SPIEN);
	}
	else
	{
		pSPIx->SPI_CR1 &= ~(0x1 << SPI_CR1_SPIEN);
	}
}

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{

}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{

}
