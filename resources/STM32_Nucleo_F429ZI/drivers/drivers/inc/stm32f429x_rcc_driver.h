/*
 * stm32f429x_rcc_driver.h
 *
 *  Created on: 9/10/2020
 *      Author: sjoshi
 */

#ifndef INC_STM32F429X_RCC_DRIVER_H_
#define INC_STM32F429X_RCC_DRIVER_H_

#include "stm32f429x.h"

uint64_t RCC_GetAPB1_PClkValue();

uint64_t RCC_GetAPB2_PClkValue();

#endif /* INC_STM32F429X_RCC_DRIVER_H_ */
