/*
 * uart.c
 *
 *  Created on: Nov 28, 2024
 *      Author: shrey
 */

#include "uart.h"

#define GPIODEN (1U <<3)
#define USART3EN (1U <<18)

#define UART_DATAWIDTH_8B (0x00000000)
#define UART_PARITY_NONE (0x00000000)
#define UART_STOPBITS_1 (0x00000000) // 1 stop bit

#define TX_EN (1U << 3)
#define UART3_EN (1U << 13)//different then STM32F7xx.

#define SYS_BAUDRATE (115200U)
#define APB1_CLOCK    (42000000U)

static void config_uart_parameters(USART_TypeDef *USARTx, uint32_t DataWidth,uint32_t parity, uint32_t stopBits);
static uint32_t compute_uart_div(uint32_t peripclk, uint32_t baudRate);
static void uart_set_baudrate(uint32_t peripclk, uint32_t baudRate);

static void uart_write(uint8_t value);


int __io_putchar(int ch)
{
   uart_write(ch);
   return ch;
}


void uart3_tx_init(void)
{
   // enable clock access to UART 3 pins port.
   // check alternate function
   // enable clock access to UART3
   // Clock access for GPIO

  // PD 8 USART3_TX AF7
  // PD 9 USART3_RX AF7

	//Enable clock access to UART 3 port D.
    RCC->AHB1ENR |= GPIODEN;

    // GPIOD_MODER - AF - 10
    // Bit 16 and Bit 17 // 10

    GPIOD->MODER &= ~(1U <<16);
    GPIOD->MODER |= (1U <<17);

    // GPIO AF High register.
    // AF7 -- 0111:
    // set Alternate function high

    GPIOD->AFR[1] |= (1U <<0);
    GPIOD->AFR[1] |= (1U <<1);
    GPIOD->AFR[1] |= (1U <<2);
    GPIOD->AFR[1] &= ~(1U <<3);

    // Enable clock access to UART3
    RCC->APB1ENR |= USART3EN;

    // configure UART parameters
    // set baudrate
    // enable uart3 module

    config_uart_parameters(USART3, UART_DATAWIDTH_8B,UART_PARITY_NONE, UART_STOPBITS_1);

    // set baud rate
    uart_set_baudrate(APB1_CLOCK ,SYS_BAUDRATE);
    // Set transfer direction
    //TE enable.

    USART3->CR1 |= TX_EN;

    // enable usart 3 module
    USART3->CR1 |= UART3_EN;

}

static void config_uart_parameters(USART_TypeDef *USARTx, uint32_t DataWidth,uint32_t Parity, uint32_t StopBits)
{
  MODIFY_REG(USARTx->CR1, USART_CR1_PS | USART_CR1_PCE | USART_CR1_M, Parity | DataWidth);
  MODIFY_REG(USARTx->CR2, USART_CR2_STOP, StopBits);
}

static uint32_t compute_uart_div(uint32_t peripclk, uint32_t baudRate)
{
    return ((peripclk + (baudRate / 2U)) / baudRate);
}

static void uart_set_baudrate(uint32_t peripclk, uint32_t baudRate)
{
	USART3->BRR = compute_uart_div(peripclk, baudRate);
}

static void uart_write(uint8_t value)
{
	//make sure Transmit Data register is empty
	// TxE bit.

	while(!(USART3->SR & (1U <<7)))
	{
		//stuck here or not
		// Wait here until TxE bit is set here.
	}
	USART3->DR = value;
}


//baud rate - 115200
// 8 bits
// 1 stop bit
// Parity  = None
// Hardware flow control = None

//Real term is used.- Read Me.
