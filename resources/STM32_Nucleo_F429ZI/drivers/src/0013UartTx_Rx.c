/*
 * 0013UartTx_Rx.c
 *
 *  Created on: 9Oct.,2020
 *      Author: shreyas.joshi
 */


#include <string.h>  // for memset and strlen
#include "stm32f429x_usart_driver.h"
#include "stm32f429x_gpio_driver.h"



/**
 *  PA2 - USART2 Tx  ---   D0 - Rx - Digital Pin (Arduino)
 *  PA3 - USART2 Rx  ---   D1 - Tx - Digital Pin (Arduino)
 */

char *msg[3] = {"hihihihihihi123", "Hello How are you ?" , "Today is Monday !"};
//reply from arduino will be stored here
char rx_buf[1024] ;

USART_Handle_t usart2_handle;

//This flag indicates reception completion
uint8_t rxCmplt = 0;

uint8_t g_data = 0;

extern void initialise_monitor_handles();

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
	usart2_handle.pUSARTx = USART2;
	usart2_handle.usart_Config.usart_baudRate = USART_STD_BAUD_115200;
	usart2_handle.usart_Config.usart_hwFlowControl = USART_HW_CONTROL_NONE;
	usart2_handle.usart_Config.usart_mode = USART_MODE_BOTH_TX_RX;
	usart2_handle.usart_Config.usart_number_of_stopBits = USART_STOP_BITS_1;
	usart2_handle.usart_Config.usart_parityControl = USART_PARITY_DISABLE;
	usart2_handle.usart_Config.usart_wordLength = USART_WORD_LEN_8_BITS;
	USART_Init(&usart2_handle);
}

int main()
{
	uint32_t cnt = 0;

	gpio_button_init();
	gpio_usart_inits();  //AF7 functions enabled
	USART_PeriClockControl(USART2, ENABLE);
	usart_inits();
	USART_IRQInterruptConfig(IRQ_USART2, ENABLE);
	USART_PeriControl(USART2, ENABLE);

	printf("Application is running\n");


	while(1)
	{
		if(GPIO_ReadFromInputPin(GPIOC, 13))
		{
			delay(250 * 1000);
			// Next message index ; make sure that cnt value doesn't cross 2
			cnt = cnt % 3;
			//First lets enable the reception in interrupt mode
			//this code enables the receive interrupt
			while ( USART_ReceiveDataIT(&usart2_handle,rx_buf,strlen(msg[cnt])) != USART_READY );
			//Send the msg indexed by cnt in blocking mode
			USART_SendData(&usart2_handle,(uint8_t*)msg[cnt],strlen(msg[cnt]));
			printf("Transmitted : %s\n",msg[cnt]);
			//Now lets wait until all the bytes are received from the arduino .
			//When all the bytes are received rxCmplt will be SET in application callback
			while(rxCmplt != 1);
			//just make sure that last byte should be null otherwise %s fails while printing
			rx_buf[strlen(msg[cnt])+ 1] = '\0';
			//Print what we received from the arduino
			printf("Received    : %s\n",rx_buf);
			//invalidate the flag
			rxCmplt = 0;

			//move on to next message indexed in msg[]
			cnt ++;
		}
	}
}

void USART2_IRQHandler(void)
{
	USART_IRQHandling(&usart2_handle);
}

void USART_ApplicationEventCallback( USART_Handle_t *pUSARTHandle,uint8_t ApEv)
{
	if(ApEv == USART_EVENT_RX_CMPLT)
	{
		rxCmplt = 1;

	}else if (ApEv == USART_EVENT_TX_CMPLT)
	{
		printf("USART_EVENT_TX_CMPLT \r\n");
	}
}

