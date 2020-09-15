/*
 * stm32f429x_gpio_driver.h
 *
 *  Created on: 15Sep.,2020
 *      Author: shreyas.joshi
 */

#ifndef INC_STM32F429X_GPIO_DRIVER_H_
#define INC_STM32F429X_GPIO_DRIVER_H_

#include "stm32f429x.h"


typedef struct
{
  uint8_t gpio_pinNumber;
  uint8_t gpio_pinMode;
  uint8_t gpio_pinSpeed;
  uint8_t gpio_pinPuPdControl;
  uint8_t gpio_opType;
  uint8_t gpio_pinAltFuncMode;
}GPIO_PinConfig_t;

typedef struct
{
	GPIO_RegDef_t *pGPIOx;
	GPIO_PinConfig_t gpio_pinConfig;
}GPIO_Handle_t;



#endif /* INC_STM32F429X_GPIO_DRIVER_H_ */
