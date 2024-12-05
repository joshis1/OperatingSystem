/*
 * leds.c
 *
 *  Created on: Nov 22, 2024
 *      Author: shrey
 */

#include "main.h"
#include "leds.h"

void leds_init(void)
{
	//Leds are GPIO output module.
	//enable clock access to Leds Port

	RCC->AHB1ENR |= (1U <<1);

	// configure LEDS pin as output.

	//Green LED Output mode.
	GPIOB->MODER |= (1U <<0);
	GPIOB->MODER  &= ~(1U <<1);

	//Blue LED output mode.Moder pin 7
	GPIOB->MODER |= (1u <<14);
	GPIOB->MODER  &= ~(1U <<15);


	//Red LED output mode. Moder pin 14
	GPIOB->MODER |= (1u <<28);
	GPIOB->MODER  &= ~(1U <<29);

}

void led_on(uint32_t led)
{
	switch(led)
	{
	case GREEN_LED:
		GPIOB->ODR |= GREEN_LED;
		break;

	case BLUE_LED:
		GPIOB->ODR |= BLUE_LED;
		break;
	case RED_LED:
		GPIOB->ODR |= RED_LED;
		break;
	}

}
void led_off(uint32_t led)
{
	switch(led)
	{
	case GREEN_LED:
		GPIOB->ODR &= ~GREEN_LED;
		break;

	case BLUE_LED:
		GPIOB->ODR &= ~BLUE_LED;
		break;
	case RED_LED:
		GPIOB->ODR &= ~RED_LED;
		break;
	}

}
