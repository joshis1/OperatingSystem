/*
 * timer.h
 *
 *  Created on: Nov 19, 2024
 *      Author: shrey
 */

#ifndef CORE_INC_TIMER_H_
#define CORE_INC_TIMER_H_

#include <stdint.h>

//Input Freq = 8 Mhz.
#define PLL_M  8
#define PLL_N  336
#define PLL_P  2

// PLL generates System clk = 168Mhz.

void clock_config();

void delay_ms(uint32_t);

uint32_t get_tick(void);

void tick_increment(void);

void timebase_init(void);

#endif /* CORE_INC_TIMER_H_ */
