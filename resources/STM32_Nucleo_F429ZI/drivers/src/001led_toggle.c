/*
 * 001led_toggle.c
 *
 *  Created on: 16Sep.,2020
 *      Author: shreyas.joshi
 */

#include "stm32f429x.h"
#include "stm32f429x_gpio_driver.h"


//Toggle PB7 LED i.e. User Blue LED

void delay(uint32_t delay_val)
{
	for(uint32_t i = 0; i < delay_val; i++)
	{
       //take cpu and delay it.
	}
}

int main()
{
	GPIO_Handle_t  gpioLed;
	gpioLed.pGPIOx = GPIOB; //Port B

	gpioLed.gpio_pinConfig.gpio_pinMode = GPIO_MODE_OUT; // output mode
	gpioLed.gpio_pinConfig.gpio_pinNumber = 7;
	gpioLed.gpio_pinConfig.gpio_opType = GPIO_OP_TYPE_PP; // Push Pull mode.
	/**Remember Push Pull mode doesn't need internal pull up or pull down register **/
	gpioLed.gpio_pinConfig.gpio_pinPuPdControl = GPIO_NO_PUPD; // no pull up or pull down
    /**Output speed optional **/
	gpioLed.gpio_pinConfig.gpio_pinSpeed = GPIO_SPEED_MEDIUM;

	GPIO_PeriClockControl(GPIOB,ENABLE); //Enable port B RCC clock
	GPIO_Init(&gpioLed);


	while(1)
	{
		delay(500 * 1000);
		GPIO_ToggleOutputPin(GPIOB,7);
	}

}
