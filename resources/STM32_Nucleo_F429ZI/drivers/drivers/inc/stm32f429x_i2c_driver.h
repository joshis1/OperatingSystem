/*
 * stm32f429x_i2c_driver.h
 *
 *  Created on: 28Sep.,2020
 *      Author: shreyas.joshi
 */

#ifndef INC_STM32F429X_I2C_DRIVER_H_
#define INC_STM32F429X_I2C_DRIVER_H_

#include "stm32f429x.h"

/** Page 864 - I2C_CR1 **/

#define I2C_CR1_PE                  (0)
#define I2C_CR1_SMBUS               (1)
#define I2C_CR1_RESERVED            (2)
#define I2C_CR1_SMBTYPE             (3)
#define I2C_CR1_ENARP               (4)
#define I2C_CR1_ENPEC               (5)
#define I2C_CR1_ENGC                (6)
#define I2C_CR1_NOSTRETCH           (7)
#define I2C_CR1_START               (8)
#define I2C_CR1_STOP                (9)
#define I2C_CR1_ACK                 (10)
#define I2C_CR1_POS                 (11)
#define I2C_CR1_PEC                 (12)
#define I2C_CR1_ALERT               (13)
#define I2C_CR1_RESERVED_           (14)
#define I2C_CR1_SWRST               (15)

/************************************************
 *
 * I2C - CR2
 */

#define I2C_CR2_FREQ                 (0) /*[5:0]**/
#define I2C_CR2_RESERVED             (6) /*[7:6]**/
#define I2C_CR2_ITERREN              (8)
#define I2C_CR2_ITEVTEN              (9)
#define I2C_CR2_ITBUFEN              (10)
#define I2C_CR2_DMAEN                (11)
#define I2C_CR2_LAST                 (12)

/***************************************************
 *
 *  I2C - SR1
 */

#define I2C_SR1_SB                 (0)
#define I2C_SR1_ADDR               (1)
#define I2C_SR1_BTF                (2)
#define I2C_SR1_ADD10              (3)
#define I2C_SR1_STOPF              (4)
/** 5 bit - reserved **/
#define I2C_SR1_RXNE               (6)
#define I2C_SR1_TXE                (7)
#define I2C_SR1_BERR               (8)
#define I2C_SR1_ARLO               (9)
#define I2C_SR1_AF                 (10)
#define I2C_SR1_OVR                (11)
#define I2C_SR1_PECERR             (12)
/**13 bit - reserved **/
#define I2C_SR1_TIMEOUT            (14)
#define I2C_SR1_SMBALERT           (15)

/******************************************************
 *
 *  I2C - SR2
 */
#define I2C_SR2_MSL                        (0)
#define I2C_SR2_BUSY                       (1)
#define I2C_SR2_TRA                        (2)
/** Bit 3 Reserved **/
#define I2C_SR2_GENCALL                    (4)
#define I2C_SR2_SMBDEFAULT                 (5)
#define I2C_SR2_SMBHOST                    (6)
#define I2C_DUALF                          (7)
#define I2C_PEC                            (8) /**[15:8] **/

/********************************************************
 *
 * I2C - CCR
 *
 */
#define I2C_CCR_BIT                        (0) /**[11:0] **/
#define I2C_DUTY                           (14) /**[11:0] **/
#define I2C_F_S                            (15)

/**********************************************************
 *
 * I2C - TRISE
 *
 */
#define I2C_TRISE                        (0) /**[5:0] **/


typedef struct
{
	uint32_t  I2C_SclSpeed; /*@I2C_SclSpeed **/
	uint8_t   I2C_DeviceAddress; /**Provided by the user when in slave mode **/
	uint8_t   I2C_ACKControl; /*@I2C_ACKControl **/
	uint16_t  I2C_FMDutyCycle; /*@I2C_FMDutyCycle **/
}I2C_Config_t;

typedef struct
{
	I2C_RegDef_t *pI2Cx;
	I2C_Config_t i2c_pinConfig;
}I2C_Handle_t;

/**
 * @I2C_SclSpeed
 */
#define I2C_SCL_SPEED_STANDARD_MODE       (100000) /**100Kbps **/
#define I2C_SCL_SPEED_FAST_MODE_4k    (400000) /**400Kbps **/
#define I2C_SCL_SPEED_FAST_MODE_2k    (200000) /**200Kbps **/

/**
 * @I2C_ACKControl
 */
#define I2C_ACK_ENABLE   (1)
#define I2C_ACK_DISABLE  (0)

/**
 * @I2C_FMDutyCycle
 */
#define I2C_FM_DUTY_2        (0)
#define I2C_FM_DUTY_16_9     (1)



#define I2C2_RESET()     do \
                            { \
	                           RCC->RCC_APB1RSTR |= (0x1 << 22); \
	                           RCC->RCC_APB1RSTR &= ~(0x1 << 22); \
                            }while(0)

#define I2C3_RESET()     do \
                            { \
	                           RCC->RCC_APB1RSTR |= (0x1 << 23); \
	                           RCC->RCC_APB1RSTR &= ~(0x1 << 23); \
                            }while(0)

#define I2C1_RESET()     do \
                            { \
	                           RCC->RCC_APB1RSTR |= (0x1 << 21); \
	                           RCC->RCC_APB1RSTR &= ~(0x1 << 21); \
                            }while(0)

/**
 * API Prototypes
 */
/** Peripheral clock enable/disable - RCC - Enable/Disable  **/
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);

/**Init and De-Init **/
void I2C_Init(I2C_Handle_t *pI2CHandle);  // Initialize I2C
void I2C_DeInit(I2C_RegDef_t *pI2Cx);  // Use RCC - reset register RCC_APB1RSTR


/**Interrupt handling **/
/**IRQ handling  - NVIC side i.e. Processor side **/
void I2C_IRQInterruptConfig(uint8_t irqNumber, uint8_t EnorDi); //interrupt configuration
void I2C_IRQPriorityConfig(uint8_t irqNumber, uint16_t priority); //interrupt priority configuration

void I2C_PeriControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint8_t flag);

__attribute__((weak)) void I2C_ApplicationEventCallback(I2C_Handle_t *pI2cHandle, uint8_t event);

uint64_t RCC_GetAPB_PClkValue();

#endif /* INC_STM32F429X_I2C_DRIVER_H_ */
