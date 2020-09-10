#include "led.h"
#include "stm32f4xx.h"


#define GPIO_MODE_MASK       ~0x3u
#define GPIO_MODE_OUTPUT      0x1u

void led_init()
{
	//DocID028599 - Page 24 - User Manual
	RCC->AHB1ENR |= (0x1 <<1) ;

	GPIOB->MODER &= ~(0x3 << 0);  //PB0 -- Green LED
	GPIOB->MODER |= (0x1 << 0);  //01b is output mode

	GPIOB->MODER &= ~(0x3 << 14);  //PB7 -- Blue LED
	GPIOB->MODER |= (0x1 << 14);  //01b is output mode

	GPIOB->MODER &= ~(0x3 << 28);  //PB14 -- Blue LED
    GPIOB->MODER |= (0x1 << 28);  //01b is output mode

}

void led_on(led_e led_color)
{
	switch(led_color)
	{
	case GREEN_LED_PIN:
		GPIOB->ODR |= 0x1 << GREEN_LED_PIN;
		break;
	case  BLUE_LED_PIN:
		GPIOB->ODR |= 0x1 << BLUE_LED_PIN;
		break;

	case RED_LED_PIN:
		GPIOB->ODR |= 0x1 << RED_LED_PIN;
		break;
	}

}

void led_off(led_e led_color)
{
	switch(led_color)
	{
	case GREEN_LED_PIN:
		GPIOB->ODR &= ~(0x1 << GREEN_LED_PIN);
		break;
	case  BLUE_LED_PIN:
		GPIOB->ODR &= ~(0x1 << BLUE_LED_PIN);
		break;

	case RED_LED_PIN:
		GPIOB->ODR &= ~(0x1 << RED_LED_PIN);
		break;
	}

}

void delay(uint32_t delay)
{
	for(volatile int i = 0; i < delay; i++);
}
