/*
 * main.c
 *
 *  Created on: 27/11/2020
 *      Author: sjoshi
 */

//#include "stm32f4xx.h"
#include "main.h"
#include <stdio.h> //sprintf
#include <string.h> //memset and strlen

/*TIM6 is a basic timer - 16 bits counter
 *
 */
/**Connect PA9 to PA0 to test 50Khz clock
 * the 50Khz is generated via. Timer 6 i.e. basic timer.
 * The 50Khz is generated to GPIO PA9.
 * so connect PA9 to PA0 so that input capture can work.
 */

/** Connect PA8 to PA0 to test LSE - i.e. 32.768Khz
 *
 */

void Timer2_Init(void);
void Error_Handler(void);
void Gpio_Init(void);
void SystemClockConfig(uint32_t sysclk);
void LSE_Configuration(void);
void USART3_Init(void);
void Timer6_Init(void);

TIM_HandleTypeDef tim2;
uint32_t input_captures[2];
uint8_t count = 1;
uint8_t is_capture_done = 0;
double capture_difference;
double timer2_cnt_freq;
double timer2_time;
double user_signal_time_period;
double user_signal_frq;

UART_HandleTypeDef huart3;
TIM_HandleTypeDef tim6;
char usr_msg[100];

/** Connect PA8 to PA0
 *
 * LSE (32.768 Khz)
 *
 * **/


int main()
{
	HAL_Init();
	SystemClockConfig(CLK_50MHZ);
	Gpio_Init();
	Timer2_Init();
	USART3_Init();

	LSE_Configuration();
	Timer6_Init();

	//start the timer 6- basically 50Khz toggling
	// as input to Timer 2 channel.
	if( HAL_OK != HAL_TIM_Base_Start_IT(&tim6))
	{
		Error_Handler();
	}

	//start the input capture - timer 2
	HAL_TIM_IC_Start_IT(&tim2, TIM_CHANNEL_1);

	while(1)
	{
		if(is_capture_done)
		{

			if(input_captures[0] > input_captures[1])
			{
				capture_difference = (0xFFFFFFFF - input_captures[0]) + input_captures[1];
			}
			else
			{
				capture_difference = input_captures[1] - input_captures[0];
				timer2_cnt_freq = (HAL_RCC_GetPCLK1Freq() * 2) /(tim2.Init.Prescaler + 1);
				timer2_time = 1/timer2_cnt_freq;
				user_signal_time_period = capture_difference * timer2_time;
				user_signal_frq = 1/user_signal_time_period;
				sprintf(usr_msg,"User signal freq is %f Hz\r\n",user_signal_frq);
				HAL_UART_Transmit(&huart3,(uint8_t *)usr_msg, strlen(usr_msg), HAL_MAX_DELAY);
				is_capture_done = 0;
			}


		}
	}

	return 0;
}


void Error_Handler(void)
{
	while(1);
}

void Gpio_Init(void)
{
	GPIO_InitTypeDef pa9; //connect PA9 to PA0 and check the frequency.
	__HAL_RCC_GPIOA_CLK_ENABLE();
	pa9.Mode = GPIO_MODE_OUTPUT_PP;
	pa9.Pin = GPIO_PIN_9;
	pa9.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &pa9);
}

/** Clock is running at 16 Mhz
 * 1 count will take 1/(16 * 10 ^6) seconds
 * 10^6 * 16 seconds = 1 count
 * we need 100 milli seconds = 0.1 seconds
 * 16 * 10^6 * 0.1 = 1600000 counts
 * 16 bit counter max value is 65535.
 * So, we need to decrease the counter clock so that
 * count can fit.
 * How about 16Mhz/25 = 16/25 Mhz
 * 1 count will take = 25/16 *10^6 seconds.
 * 10^6 * 16 /25 = 1 seconds
 * we need 100 milliseconds i.e. 100/1000 = 0.1 seconds
 * 16*10^6*0.1/25 = 64000 - this will fit in the 16-bit counter
 */

void Timer2_Init(void)
{
	TIM_IC_InitTypeDef tim2_ic_config;
	tim2.Instance = TIM2;
	tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2.Init.Period = 0xFFFFFFFF;
	tim2.Init.Prescaler = 1;
	if( HAL_OK != HAL_TIM_IC_Init(&tim2))
	{
		Error_Handler();
	}
	tim2_ic_config.ICPolarity = TIM_ICPOLARITY_RISING;
	tim2_ic_config.ICSelection = TIM_ICSELECTION_DIRECTTI;
	tim2_ic_config.ICPrescaler = TIM_ICPSC_DIV1;
	tim2_ic_config.ICFilter = 0;

	if(HAL_OK != HAL_TIM_IC_ConfigChannel(&tim2, &tim2_ic_config, TIM_CHANNEL_1))
	{
		Error_Handler();
	}
}

void SystemClockConfig(uint32_t sysclk)
{
	RCC_OscInitTypeDef oscillator_init;
	RCC_ClkInitTypeDef clk_init;
	uint32_t fLatency = FLASH_ACR_LATENCY_0WS;

	memset(&oscillator_init,0, sizeof(oscillator_init));

	oscillator_init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_HSE;
	oscillator_init.HSIState = RCC_HSI_ON;
	oscillator_init.LSEState = RCC_LSE_ON;
	oscillator_init.HSEState = RCC_HSE_ON;
	oscillator_init.HSICalibrationValue = 16; // assuming we are running at normal operating environment
	oscillator_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	oscillator_init.PLL.PLLState = RCC_PLL_ON;

	switch(sysclk)
	{
	case CLK_50MHZ:
		oscillator_init.PLL.PLLM = 16; // so 16/16 for HSI is 1 Mhz.
		oscillator_init.PLL.PLLN = 100; // now multiply i.e. 100Mhz
		oscillator_init.PLL.PLLP = 2; //now divide by 2 i.e. 50Mhz. which will be HCLK i.e. AHB Clock.
		oscillator_init.PLL.PLLQ = 4; // now divide by 2 i..e 25Mhz which will be PCLK1 and PCLK2 - APB1 and APB2
		fLatency = FLASH_ACR_LATENCY_1WS;
		break;
	case CLK_84MHZ:
		oscillator_init.PLL.PLLM = 16; // so 16/16 for HSI is 1 Mhz.
		oscillator_init.PLL.PLLN = 168; // now multiply i.e. 168Mhz
		oscillator_init.PLL.PLLP = 2; //now divide by 2 i.e. 84Mhz. which will be HCLK i.e. AHB Clock.
		oscillator_init.PLL.PLLQ = 4; // now divide by 4 i..e 42Mhz which will be PCLK1 and PCLK2 - APB1 and APB2
		fLatency = FLASH_ACR_LATENCY_2WS;
		break;
	case CLK_120MHZ:
		oscillator_init.PLL.PLLM = 16; // so 16/16 for HSI is 1 Mhz.
		oscillator_init.PLL.PLLN = 240; // now multiply i.e. 240 Mhz
		oscillator_init.PLL.PLLP = 2; //now divide by 2 i.e. 120Mhz. which will be HCLK i.e. AHB Clock.
		oscillator_init.PLL.PLLQ = 4; //now divide by 4 i.e. 30Mhz. which will be PCLK1 i.e. APB1 and APB2 Clock
		fLatency = FLASH_ACR_LATENCY_3WS;
		break;

	}

	if(HAL_OK != HAL_RCC_OscConfig(&oscillator_init))
	{
		Error_Handler();
	}

	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV4;

	if( HAL_OK != HAL_RCC_ClockConfig(&clk_init,fLatency) )
	{
		Error_Handler();
	}
}

void LSE_Configuration(void)
{
	//MCO1 pin(PA8)
	HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_LSE,RCC_MCODIV_1);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(!is_capture_done)
	{
		if(count == 1)
		{
			input_captures[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			count++;
		}
		else
		{
			input_captures[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			is_capture_done = 1;
			count = 1;
		}
	}
}

void USART3_Init(void)
{
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	if(HAL_OK != HAL_UART_Init(&huart3))
	{
		Error_Handler();
	}
}

void Timer6_Init(void)
{
	//50Khz generation here.
	tim6.Instance = TIM6;
	tim6.Init.Prescaler = 9;  //the prescaler is actually 25 - (24 +1)
	tim6.Init.Period = 50 -1; //-1 is required since the event is updated one count late.
	if(HAL_OK != HAL_TIM_Base_Init(&tim6))
	{
		Error_Handler();
	}
}
