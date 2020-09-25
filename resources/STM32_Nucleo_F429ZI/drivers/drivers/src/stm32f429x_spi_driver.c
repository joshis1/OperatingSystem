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

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint8_t flag)
{
	if(pSPIx->SPI_SR & (0x1 << flag))
	{
		return 1;
	}

	return 0;
}

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len)
{
	while( len > 0)
	{
		while(!(SPI_GetFlagStatus(pSPIx, SPI_SR_TXE))); //Wait until Tx buffer is not empty
		if(pSPIx->SPI_CR1 & (0x1 << SPI_CR1_DFF))
		{
			//16-bits i.e. 2 bytes are written.
			pSPIx->SPI_DR = *(uint16_t *)pTxBuffer;
			len--;
			len--;
			(uint16_t *)pTxBuffer++;
		}
		else
		{
			// 8-bits i.e. 1 byte is written
			pSPIx->SPI_DR = *pTxBuffer;
			len--;
			pTxBuffer++;
		}
	}
}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len)
{
	while(len > 0)
	{
		while(!(SPI_GetFlagStatus(pSPIx, SPI_SR_RXNE))); // Wait until the Rx buffer is  empty.
		if(pSPIx->SPI_CR1 & (0x1 << SPI_CR1_DFF))
		{
			// 16 bits i.e. 2 bytes to be read
			*(uint16_t *)pRxBuffer = pSPIx->SPI_DR;
			len--;
			len--;
			(uint16_t *)pRxBuffer++;
		}
		else
		{
			// 8 bits i.e. 1 byte to be read
			*pRxBuffer = pSPIx->SPI_DR;
			len--;
			pRxBuffer++;
		}
	}
}

/*
 *  Internal Slave Select -only useful if Software Select Management is used.
 */
void SPI_SSIControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if( EnOrDi == ENABLE)
	{
		pSPIx->SPI_CR1 |= (0x1 << SPI_CR1_SSI);
	}
	else
	{
		pSPIx->SPI_CR1 &= ~(0x1 << SPI_CR1_SSI);
	}
}

/*
 *  Hardware Slave Select -only useful if Software Select Management is 0 i.e. hardware based management.
 *  Used only when the MCU is in the master mode.
 *  the NSS pin will be 0 whenever the SPE bit is enabled from the Master - automatically.
 *  Once the SPE bit is 0 then the NSS will be held high and the Slave will be deselect.
 */
void SPI_SSOEControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if( EnOrDi == ENABLE)
	{
		pSPIx->SPI_CR2 |= (0x1 << SPI_CR2_SSOE);
	}
	else
	{
		pSPIx->SPI_CR2 &= ~(0x1 << SPI_CR2_SSOE);
	}
}

