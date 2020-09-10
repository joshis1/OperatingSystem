/*
 * led.h
 *
 *  Created on: 23/08/2020
 *      Author: sjoshi
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include <stdint.h>

typedef enum led
{
	GREEN_LED_PIN     = 0u,
	BLUE_LED_PIN      =   7u,
	RED_LED_PIN       =    14u,
}led_e;

void led_init();

void led_on(led_e led_color);

void led_off(led_e led_color);

void delay(uint32_t delay);

#endif /* INC_LED_H_ */
