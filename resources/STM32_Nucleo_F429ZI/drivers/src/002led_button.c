/*
 * 002led_button.c
 *
 *  Created on: 17/09/2020
 *      Author: sjoshi
 */

#include "stm32f429x.h"
#include "stm32f429x_gpio_driver.h"


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



	gpioButton.pGPIOx = GPIOC; //Port C
	gpioButton.gpio_pinConfig.gpio_pinMode = GPIO_MODE_IN; // Input mode
	gpioButton.gpio_pinConfig.gpio_pinNumber = 13;
	/**Already the button has internal pull up resistor **/
	gpioButton.gpio_pinConfig.gpio_pinPuPdControl = GPIO_NO_PUPD; // no pull up or pull down
	GPIO_PeriClockControl(GPIOC,ENABLE); //Enable port C RCC clock
	GPIO_Init(&gpioButton);


	while(1)
	{
		if(GPIO_ReadFromInputPin(GPIOC, 13))
		{
			GPIO_ToggleOutputPin(GPIOB,7);
			delay(250 * 1000);
		}
	}

}
