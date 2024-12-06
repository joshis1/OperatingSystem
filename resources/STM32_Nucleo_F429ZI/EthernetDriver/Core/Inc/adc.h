/*
 * adc.h
 *
 *  Created on: Dec 6, 2024
 *      Author: shrey
 */

#ifndef CORE_INC_ADC_H_
#define CORE_INC_ADC_H_

#include "main.h"


void pc0_adc1_init();

uint32_t adc1_get_data(void);

#endif /* CORE_INC_ADC_H_ */
