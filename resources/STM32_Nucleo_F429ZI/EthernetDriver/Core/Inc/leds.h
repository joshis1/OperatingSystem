/*
 * leds.h
 *
 *  Created on: Nov 22, 2024
 *      Author: shrey
 */

#ifndef CORE_INC_LEDS_H_
#define CORE_INC_LEDS_H_

#define LEDS_PORT GPIOB

#define USER_LED1 (1U <<0) //green
#define USER_LED2 (1U <<7) //blue
#define USER_LED3 (1U <<14) //red

#define GREEN_LED  USER_LED1
#define BLUE_LED   USER_LED2
#define RED_LED    USER_LED3

void leds_init(void);
void led_on(uint32_t led);
void led_off(uint32_t led);

#endif /* CORE_INC_LEDS_H_ */
