/*
 * timer.c
 *
 *  Created on: Nov 19, 2024
 *      Author: shrey
 */


#include "timer.h"
#include "main.h"


#define ONE_MSEC_LOAD (168000)

#define CTRL_ENABLE			(1U<<0)
#define CTRL_CLCKSRC		(1U<<2)
#define CTRL_TICKINT		(1U<<1)
#define CTRL_COUNTFLAG		(1U<<16)

#define MAX_DELAY			0xFFFFFFFFU

#define RCC_HSEON		(1U<<16)

uint32_t g_curr_tick;
uint32_t tick_freq = 1;

void tick_increment(void)
{
	g_curr_tick += tick_freq;
}

uint32_t get_tick(void)
{
	return g_curr_tick;
}

void SysTick_Handler(void)
{
	tick_increment();
}

void delay_ms(uint32_t delay)
{
	uint32_t tickstart =  get_tick();
	uint32_t wait = delay;

	if(wait < MAX_DELAY)
	{
			wait += (uint32_t)(tick_freq);
	}

	while((get_tick() - tickstart) < wait){}


}

void timebase_init(void)
{
	/*Reload the timer with number of clocks per millisecond*/
	SysTick->LOAD = ONE_MSEC_LOAD - 1;

	/*Clear SysTick current value register*/
	SysTick->VAL = 0;

    /*Enable systick and select clk src as internal*/
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLCKSRC ;

	/*Enable systick interrupt*/
	SysTick->CTRL |= CTRL_TICKINT;


}


void clock_config(void)
{
    // Enable HSE
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY)); // Wait until HSE is ready

    // Enable Power interface clock
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // Configure Voltage Regulator to Scale 1 mode (necessary for higher clock speeds)
    PWR->CR |= PWR_CR_VOS_0; // VOS scale 1 mode

    // Set AHB Prescaler to 1 (HCLK = SYSCLK)
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    // Set APB1 Prescaler to 4 (PCLK1 = 42 MHz)
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

    // Set APB2 Prescaler to 2 (PCLK2 = 84 MHz)
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

    // Configure PLL: HSE as PLL source, PLLM = 8, PLLN = 336, PLLP = 2, PLLQ = 7
    RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC_HSE |
                   (8 << RCC_PLLCFGR_PLLM_Pos) |
                   (336 << RCC_PLLCFGR_PLLN_Pos) |
                   (0 << RCC_PLLCFGR_PLLP_Pos) |  // PLLP = 2 (00b)
                   (7 << RCC_PLLCFGR_PLLQ_Pos);

    // Enable PLL
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY)); // Wait until PLL is ready

    // Set Flash latency to 5 wait states for 168 MHz
    FLASH->ACR = FLASH_ACR_LATENCY_5WS;

    // Select PLL as system clock source
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL); // Wait until PLL is system clock source
}

