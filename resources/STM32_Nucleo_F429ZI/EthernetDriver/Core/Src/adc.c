/*
 * adc.c
 *
 *  Created on: Dec 6, 2024
 *      Author: shrey
 */


//PC0 i.e. ADC123_IN10

#include <stdio.h>
#include "adc.h"

#define GPIOCEN  (1U << 2)
#define ADC1_EN  (1U << 8)
#define CR2_CON  (1U << 1)
#define CR2_ADON (1U << 0)
#define CR2_SWSTART (1U << 30)

//Status register
#define SR_EOC (1U << 1)

//PC0 - channel 0 , Rank 1

#define CH10_RANK1 (10U <<0)

void pc0_adc1_init()
{

	/*1. Enable clock access to ADC Pin's port GPIOC */
	/*2. Set pin as analog input.*/
	/*3. Enable clock access to ADC module */
	/*4. Set ADC to continuous conversion */
	/*5. Set Sequencer length to 1. */
	/*6. Set ADC Channel */
	/*7. Enable ADC module */
	/*8. Start conversion */

	/*GPIOC is connected to ADHB1 bus */

	//enable clock access to pin port i.e. GPIOC
	RCC->AHB1ENR |= GPIOCEN;
	//Set pin as an analog input
	GPIOC->MODER |= (3U << 0); // Sets PC0 as analog mode

	//Enable clock access to ADC1 enable
	RCC->APB2ENR |= ADC1_EN;

	// Continuous conversion
	ADC1->CR2 |= CR2_CON;

	//Set Sequencer length to 1.

	/*If you have three sensors connected to channels 1, 2, and 3,
	 *
	 * and you want the ADC to read all three in each cycle,
	 * you would set the sequence length to 3.
	 * Here's how you would do it: */

	//ADC->SQR1 Bits 23:20 L[3:0]: Regular channel sequence length
	// 1 Sequence length only
	// Set sequencer length to 1
	ADC1->SQR1 &= ~(0xF << 20); // Clear the length bits

	// Set ADC Channel.
	//PC0 is PC0 (ADC123_IN10)i.e. Channel Input 10.

	ADC1->SQR3 |= CH10_RANK1; //PC0 is channel 10

	//Enable ADC module
	ADC1->CR2 |= CR2_ADON;

	//ADC1 - start conversion
	ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc1_get_data(void)
{
	while(!(ADC1->SR & (SR_EOC)))
	{
       printf("Sensory not ready");
	}
	//return the value
	return ADC1->DR;

}
