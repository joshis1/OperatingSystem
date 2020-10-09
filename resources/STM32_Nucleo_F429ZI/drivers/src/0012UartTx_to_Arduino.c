/*
 * 0012UartTx_to_Arduino.c
 *
 *  Created on: 9/10/2020
 *      Author: sjoshi
 */

#include <string.h>  // for memset and strlen
#include "stm32f429x_usart_driver.h"
#include "stm32f429x_gpio_driver.h"

/*** Arduino Sketch
 *
 *  001UARTRxString.ino
 */

/**
 *  PA2 - USART2 Tx  ---   D0 - Rx - Digital Pin (Arduino)
 *  PA3 - USART2 Rx  ---   D1 - Tx - Digital Pin (Arduino)
 */

uint8_t testBuffer[] = "Hello from STM32-USART";

USART_Handle_t usart_handle;

void delay(uint32_t delay_val)
{
	for(uint32_t i = 0; i < delay_val; i++)
	{
       //take cpu and delay it.
	}
}

void gpio_button_init()
{
	GPIO_Handle_t gpioButton;
	gpioButton.pGPIOx = GPIOC; //Port C
	gpioButton.gpio_pinConfig.gpio_pinMode = GPIO_MODE_IN; // Input mode
	gpioButton.gpio_pinConfig.gpio_pinNumber = 13;
	/**Already the button has internal pull up resistor **/
	gpioButton.gpio_pinConfig.gpio_pinPuPdControl = GPIO_NO_PUPD; // no pull up or pull down
	GPIO_PeriClockControl(GPIOC,ENABLE); //Enable port C RCC clock
	GPIO_Init(&gpioButton);

}

void gpio_usart_inits()
{
	//Port A clock Enable
	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Handle_t  gpioA2_tx, gpioA3_rx;

	memset(&gpioA2_tx,0, sizeof(GPIO_Handle_t));
	memset(&gpioA3_rx,0, sizeof(GPIO_Handle_t));

	gpioA2_tx.pGPIOx = GPIOA;
	gpioA2_tx.gpio_pinConfig.gpio_pinMode = GPIO_MODE_ALTFN;
	gpioA2_tx.gpio_pinConfig.gpio_pinAltFuncMode = 7;
	gpioA2_tx.gpio_pinConfig.gpio_pinNumber = 2;
	gpioA2_tx.gpio_pinConfig.gpio_pinPuPdControl = GPIO_PIN_PU;
	gpioA2_tx.gpio_pinConfig.gpio_opType = GPIO_OP_TYPE_PP;
	GPIO_Init(&gpioA2_tx);

	gpioA3_rx.pGPIOx = GPIOA;
	gpioA3_rx.gpio_pinConfig.gpio_pinMode = GPIO_MODE_ALTFN;
	gpioA3_rx.gpio_pinConfig.gpio_pinAltFuncMode = 7;
	gpioA3_rx.gpio_pinConfig.gpio_pinNumber = 3;
	gpioA3_rx.gpio_pinConfig.gpio_pinPuPdControl = GPIO_PIN_PU;
	gpioA3_rx.gpio_pinConfig.gpio_opType = GPIO_OP_TYPE_PP;
	GPIO_Init(&gpioA3_rx);

}

void usart_inits()
{
	usart_handle.pUSARTx = USART2;
	usart_handle.usart_Config.usart_baudRate = USART_STD_BAUD_115200;
	usart_handle.usart_Config.usart_hwFlowControl = USART_HW_CONTROL_NONE;
	usart_handle.usart_Config.usart_mode = USART_MODE_ONLY_TX;
	usart_handle.usart_Config.usart_number_of_stopBits = USART_STOP_BITS_1;
	usart_handle.usart_Config.usart_parityControl = USART_PARITY_DISABLE;
	usart_handle.usart_Config.usart_wordLength = USART_WORD_LEN_8_BITS;
	USART_Init(&usart_handle);
}

int main()
{
	gpio_button_init();
	gpio_usart_inits();  //AF7 functions enabled
	USART_PeriClockControl(USART2, ENABLE);
	usart_inits();
	USART_PeriControl(USART2, ENABLE);


	while(1)
	{
		if(GPIO_ReadFromInputPin(GPIOC, 13))
		{
			delay(250 * 1000);
			USART_SendData(&usart_handle,testBuffer, sizeof(testBuffer));
		}
	}
}
