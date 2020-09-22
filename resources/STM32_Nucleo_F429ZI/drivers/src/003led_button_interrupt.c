/*
 * 003led_button_interrupt.c
 *
 *  Created on: 18Sep.,2020
 *      Author: shreyas.joshi
 */

#include "stm32f429x.h"
#include "stm32f429x_gpio_driver.h"
#include <string.h>


//Toggle PB7 LED i.e. User Blue LED when the user button is pressed.
// User button - blue button is PC13

void delay(uint32_t delay_val)
{
	for(uint32_t i = 0; i < delay_val; i++)
	{
       //take cpu and delay it.
	}
}

int main()
{
	GPIO_Handle_t  gpioLed, gpioButton;
	memset(&gpioLed, 0, sizeof(gpioLed));
	memset(&gpioButton, 0, sizeof(gpioButton));


	gpioLed.pGPIOx = GPIOB; //Port B

	gpioLed.gpio_pinConfig.gpio_pinMode = GPIO_MODE_OUT; // output mode
	gpioLed.gpio_pinConfig.gpio_pinNumber = 7;
	gpioLed.gpio_pinConfig.gpio_opType = GPIO_OP_TYPE_PP; // Push Pull mode.
	/**Remember Push Pull mode doesn't need internal pull up or pull down register **/
	/** Here the micro-controller is doing the current sourcing **/
	gpioLed.gpio_pinConfig.gpio_pinPuPdControl = GPIO_NO_PUPD; // no pull up or pull down
	/**Output speed optional **/
	gpioLed.gpio_pinConfig.gpio_pinSpeed = GPIO_SPEED_MEDIUM;

	GPIO_PeriClockControl(GPIOB,ENABLE); //Enable port B RCC clock
	GPIO_Init(&gpioLed);


	/** Button Interrupt Configuration **/
	gpioButton.pGPIOx = GPIOC; //Port C
	gpioButton.gpio_pinConfig.gpio_pinMode = GPIO_MODE_IT_RT; // Rising edge trigger - Interrupt
	gpioButton.gpio_pinConfig.gpio_pinNumber = 13;
	// Very important make it no pull up pull down otherwise button won't work
	gpioButton.gpio_pinConfig.gpio_pinPuPdControl = GPIO_NO_PUPD;
	GPIO_PeriClockControl(GPIOC,ENABLE); //Enable port C RCC clock
	GPIO_Init(&gpioButton);
	//NVIC Side i.e. Processor based configuration
	GPIO_IRQPriorityConfig(IRQ_EXTI15_10, 15);
	GPIO_IRQInterruptConfig(IRQ_EXTI15_10, ENABLE);


	while(1)
	{
		// looping here.
	}

}

//ISR - defined in startup_stm32.s - can be overridden here.
// this is .weak that means can be overridden.

void EXTI15_10_IRQHandler(void)
{
	// due to button debouncing we add this delay otherwise we will get more number of interrupts.
	delay(250 * 1000);
	GPIO_IRQHandling(13);  //EXTI - pin number
	GPIO_ToggleOutputPin(GPIOB,7);
}
