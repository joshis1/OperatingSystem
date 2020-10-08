/*
 * stm32f429x_usart_driver.h
 *
 *  Created on: 8/10/2020
 *      Author: sjoshi
 */

#ifndef INC_STM32F429X_USART_DRIVER_H_
#define INC_STM32F429X_USART_DRIVER_H_

#include "stm32f429x.h"


typedef struct
{
	uint8_t  usart_mode;  /**@usart_mode **/
	uint32_t usart_baudRate;  /**@usart_baudRate **/
	uint8_t  usart_number_of_stopBits; /**@stopBits **/
	uint8_t  usart_wordLength;  /**@wordlength **/
	uint8_t  usart_parityControl;  /**@parityControl **/
	uint8_t  usart_hwFlowControl; /**@HardwareFlowControl **/
}USART_Config_t;

typedef struct
{
	USART_RegDef_t *pUSARTx;
	USART_Config_t usart_Config;
	uint8_t txState;
	uint8_t rxState;
	uint32_t txlen;
	uint32_t rxlen;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;

}USART_Handle_t;


/** @usart_mode  **/
#define USART_MODE_ONLY_RX    (0)
#define USART_MODE_ONLY_TX    (1)
#define USART_MODE_BOTH_TX_RX (2)

/** @usart_baudRate **/
#define USART_STD_BAUD_1200 (1200)
#define USART_STD_BAUD_2400 (2400)
#define USART_STD_BAUD_9600 (9600)
#define USART_STD_BAUD_19200 (19200)
#define USART_STD_BAUD_57600 (57600)
#define USART_STD_BAUD_115200 (115200)

/**@stopBits **/
#define USART_STOP_BITS_1   (0)
#define USART_STOP_BITS_0_5 (1)
#define USART_STOP_BITS_2   (2)
#define USART_STOP_BITS_1_5 (3)


/** @wordlength **/
#define USART_WORD_LEN_8_BITS (0)
#define USART_WORD_LEN_9_BITS (1)

/**@parityControl **/
#define USART_PARITY_ENABLE_ODD (0)
#define USART_PARITY_ENABLE_EVEN (1)
#define USART_PARITY_DISABLE     (2)

/**@HardwareFlowControl **/
#define USART_HW_CONTROL_NONE     (0)
#define USART_HW_CONTROL_CTS      (1)
#define USART_HW_CONTROL_RTS      (2)
#define USART_HW_CONTROL_CTS_RTS  (3)

/***USART CR1 ***
 *
 */
#define USART_CR1_SBK  (0) //send Break
#define USART_CR1_RWU  (1) //Receiver Wakeup
#define USART_CR1_RE   (2) //Receiver Enable
#define USART_CR1_TE   (3)  //Transmitter Enable
#define USART_CR1_IDLEIE  (4) //IDLE Interrupt Enable
#define USART_CR1_RXNEIE  (5) //RXNE Interrupt Enable
#define USART_CR1_TCIE  (6) //Transmission complete interrupt Enable
#define USART_CR1_TXEIE  (7) //TXE interrupt Enable
#define USART_CR1_PEIE  (8) //PE Interrupt Enable
#define USART_CR1_PS    (9) //Parity Selection
#define USART_CR1_PCE    (10) //Parity Control Enable
#define USART_CR1_WAKE    (11) //Wakeup Method
#define USART_CR1_M    (12) //Word length
#define USART_CR1_UE   (13)  // USART Enable
#define USART_CR1_OVER8 (15) // OverSampling Mode

/** USART CR2 **
 *
 */



#define USART_CR2_ADDR  (0) //send Break
#define USART_CR2_LBDL  (5) //Line Break detection length
#define USART_CR2_LBDIE   (6) //Line Break detection interrupt enable
#define USART_CR2_LBCL   (8)  //Last bit clock pulse
#define USART_CR2_CPHA  (9) //Clock Phase
#define USART_CR2_CPOL  (10) //Clock Polarity
#define USART_CR2_CLKEN  (11) //Clock Enable
#define USART_CR2_STOPBITS  (12) //STOP bits
#define USART_CR2_LINEN  (14) //LIN Mode Enable

/*** USART CR3 **
 *
 */

#define USART_CR3_EIE    (0) //Error Interrupt Enable
#define USART_CR3_IREN   (1) //IrDA mode Enable
#define USART_CR3_IRLP   (2) //IrDA lower power
#define USART_CR3_HDSEL  (3)  //Half duplex selection
#define USART_CR3_NACK   (4) //Smart Card NACK Enable
#define USART_CR3_SCEN   (5) //Smart Card Mode Enable
#define USART_CR3_DMAR   (6) //DMA Enable Receiver
#define USART_CR3_DMAT   (7) //DMA Enable Transmitter
#define USART_CR3_RTSE   (8) //RTS enable
#define USART_CR3_CTSE   (9) //CTS enable
#define USART_CR3_CTSIE  (10) //CTS Interrupt Enable
#define USART_CR3_ONEBIT (11) //One Sample bit Method Enabled

/** USART Status Register **/

#define USART_SR_PE      (0) //Parity Error
#define USART_SR_FE      (1) //Framing Error
#define USART_SR_NF      (2) //Noise Detected Flag
#define USART_SR_ORE     (3)  //Over Run Error
#define USART_SR_IDLE    (4) //IDLE line detected
#define USART_SR_RXNE    (5) //Read Data register not empty
#define USART_SR_TC      (6) //Transmission complete
#define USART_SR_TXE     (7) //Transmit Data register Empty
#define USART_SR_LBD     (8) //Line Break Detection Flag
#define USART_SR_CTS     (9) //CTS Flag


/***USART States **/
#define USART_BUSY_IN_TX (0)
#define USART_BUSY_IN_RX (1)
#define USART_READY (2)

void USART_PeriClockControl(USART_RegDef_t *pUsart, uint8_t EnOrDi);

void USART_Init(USART_Handle_t *pUSARTHandle);

void USART_IRQInterruptConfig(uint8_t irqNumber, uint8_t EnorDi);

void USART_IRQPriorityConfig(uint8_t irqNumber, uint16_t priority);

void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t len);
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len);

uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len);

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUsart, uint8_t flag);

#endif /* INC_STM32F429X_USART_DRIVER_H_ */
