/*
 * stm32f429x_spi_driver.h
 *
 *  Created on: 22Sep.,2020
 *      Author: shreyas.joshi
 */

#ifndef INC_STM32F429X_SPI_DRIVER_H_
#define INC_STM32F429X_SPI_DRIVER_H_

#include "stm32f429x.h"
/*
 * @deviceMode
 */
typedef enum
{
	slave_mode = 0,
	master_mode
}Device_mode_e;

/*
 * @busConfig
 */
typedef enum
{
	Full_Duplex = 0,
	HALF_Duplex,
	SIMPLEX_RX_ONLY
}Bus_config_e;


/*
 * @sclkSpeed
 */
typedef enum
{
	SCLK_SPEED_DIV2 = 0,
	SCLK_SPEED_DIV4,
	SCLK_SPEED_DIV8,
	SCLK_SPEED_DIV16,
	SCLK_SPEED_DIV32,
	SCLK_SPEED_DIV64,
	SCLK_SPEED_DIV128,
	SCLK_SPEED_DIV256,
}SclkSpeed_e;


/*
 * @dff
 */
typedef enum
{
	SPI_DFF_8BITS = 0,
	SPI_DFF_16BITS
}Dff_e;

/*
 * @Cpol
 */
typedef enum
{
	CPOL_LOW = 0,
	CPOL_HIGH
}Cpol_e;

/*
 * @Cpha
 */
typedef enum
{
	CPHA_LOW = 0,
	CPHA_HIGH
}Cpha_e;

/*
 * @ssm
 */
typedef enum
{
	SSM_DISABLE = 0,
	SSM_ENABLE
}Ssm_e;


typedef struct
{
	Device_mode_e deviceMode;
	Bus_config_e busConfig;
	SclkSpeed_e sclkSpeed;
	Dff_e dff;
	Cpol_e cpol;
	Cpha_e cpha;
	Ssm_e ssm;
}SPI_Config_t;



typedef struct
{
	SPI_RegDef_t *pSPIx;
	SPI_Config_t spi_pinConfig;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint8_t txLen;
	uint8_t rxLen;
	uint8_t txState;
	uint8_t rxState;
}SPI_Handle_t;

/** APIs **/
/** Peripheral clock enable/disable - RCC - Enable/Disable  **/
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

/**Init and De-Init **/
void SPI_Init(SPI_Handle_t *pSPIHandle);  // Initialize SPI
void SPI_DeInit(SPI_RegDef_t *pSPIx);  // Use RCC - reset register RCC_AHB1RSTR

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t len);

void SPI_SSIControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

void SPI_PeriControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

void SPI_SSOEControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint8_t flag);

/**Interrupt handling **/
/**IRQ handling  - NVIC side i.e. Processor side **/
void SPI_IRQInterruptConfig(uint8_t irqNumber, uint8_t EnorDi); //interrupt configuration
void SPI_IRQPriorityConfig(uint8_t irqNumber, uint16_t priority); //interrupt priority configuration

/**Interrupt based SPI send and receive **/
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t len);

void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);


/** SPI CR1 registers - SPI control register 1 (SPI_CR1) **/
#define SPI_CR1_CPHA      (0)
#define SPI_CR1_CPOL      (1)
#define SPI_CR1_MSTR      (2)
#define SPI_CR1_BR        (3)
#define SPI_CR1_SPIEN     (6)
#define SPI_CR1_LSBFIRST  (7)
#define SPI_CR1_SSI       (8)
#define SPI_CR1_SSM       (9)
#define SPI_CR1_RXONLY    (10)
#define SPI_CR1_DFF       (11)
#define SPI_CR1_CRCNEXT   (12)
#define SPI_CR1_CRCEN     (13)
#define SPI_CR1_BIDIOE    (14)
#define SPI_CR1_BIDIMODE  (15)

/** SPI CR2 registers - SPI control register 1 (SPI_CR2) **/
#define SPI_CR2_RXDMAEN      (0)
#define SPI_CR2_TXDMAEN      (1)
#define SPI_CR2_SSOE         (2)
/*** 3 is reserved **/
#define SPI_CR2_FRF            (4)
#define SPI_CR2_ERRIE          (5)
#define SPI_CR2_RXNEIE         (6)
#define SPI_CR2_TXEIE          (7)


/** SPI status register (SPI_SR) **/
#define SPI_SR_RXNE        (0)
#define SPI_SR_TXE         (1)
#define SPI_SR_CHSIDE      (2)
#define SPI_SR_UDR         (3)
#define SPI_SR_CRCERR      (4)
#define SPI_SR_MODF        (5)
#define SPI_SR_OVR         (6)
#define SPI_SR_BSY         (7)
#define SPI_SR_FRE         (8)


#define SPI2_RESET()     do \
                            { \
	                           RCC->RCC_APB1RSTR |= (0x1 << 14); \
	                           RCC->RCC_APB1RSTR &= ~(0x1 << 14); \
                            }while(0)

#define SPI3_RESET()     do \
                            { \
	                           RCC->RCC_APB1RSTR |= (0x1 << 15); \
	                           RCC->RCC_APB1RSTR &= ~(0x1 << 15); \
                            }while(0)

#define SPI1_RESET()     do \
                            { \
	                           RCC->RCC_APB2RSTR |= (0x1 << 12); \
	                           RCC->RCC_APB2RSTR &= ~(0x1 << 12); \
                            }while(0)


/** SPI State **/
#define SPI_READY       (0)
#define SPI_BSY_IN_RX   (1)
#define SPI_BSY_IN_TX   (2)


#endif /* INC_STM32F429X_SPI_DRIVER_H_ */
