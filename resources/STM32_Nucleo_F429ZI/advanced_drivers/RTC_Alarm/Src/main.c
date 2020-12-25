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
#include <stdarg.h>

void Error_Handler(void);
void Gpio_Init(void);
void SystemClockConfig(uint32_t sysclk);
void LSE_Configuration(void);
void USART3_Init(void);
void RTC_Init(void);
void RTC_CalendarConfig(void);
void printmsg(char *format, ...);
static char* getDayOfWeek(uint8_t number);
void RTC_Alarm_Config(void);

//In order to wakeup connect a jumper PA0 to Vdd


UART_HandleTypeDef huart3;
RTC_HandleTypeDef hrtc;
char usr_msg[100];

/** Connect PA8 to PA0
 *
 * LSE (32.768 Khz)
 *
 * **/

/**
 * http://www.bittiming.can-wiki.info/
 */


int main()
{
	HAL_Init();
	SystemClockConfig(CLK_50MHZ);
	Gpio_Init();
	USART3_Init();

	LSE_Configuration();

	RTC_Init();

	RTC_CalendarConfig();
	RTC_Alarm_Config();

	printmsg("Booting up..\r\n");

	//Enable clock to Back up SRAM.
	__HAL_RCC_BKPSRAM_CLK_ENABLE();
	//Enable Clock for Power Controller block
	__HAL_RCC_PWR_CLK_ENABLE();
	//Enable Access  to  Back up SRAM.
	HAL_PWR_EnableBkUpAccess();

	__HAL_RCC_PWR_CLK_ENABLE();

	uint32_t *pBackupSRAM = (uint32_t *)BKPSRAM_BASE;
	char write_buf[] = "Hello";

	if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB))
	{
		printmsg("Wakeup from Stand by mode \r\n");
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB); //clear the stand by flag.
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU); //clear the wakeup flag.

		uint8_t data = (uint8_t)*pBackupSRAM;
		if(data != 'H')
		{
			printmsg("Back up SRAM failed to read \r\n");
		}
		else
		{
			printmsg("Back up SRAM successful\r\n");
		}
	}
	else
	{
		printmsg("Writing to Backup SRAM\r\n");
		for(uint32_t i = 0; i < strlen(write_buf) +1 ; i++)
		{
			*(pBackupSRAM +i) = write_buf[i];
		}
	}


	while(1)
	{
		__WFI(); // Save current Wait for interrupt.
	}

	return 0;
}


void Error_Handler(void)
{
	while(1);
}

void Gpio_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef ledgpio;
	GPIO_InitTypeDef button_gpio;
	ledgpio.Pin = GPIO_PIN_7;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB,&ledgpio);

	button_gpio.Pull = GPIO_NOPULL;
	button_gpio.Mode = GPIO_MODE_IT_FALLING;
	button_gpio.Pin = GPIO_PIN_13;

	HAL_GPIO_Init(GPIOC,&button_gpio);//button is PC13

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);  // button interrupt priority set.

	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); // button interrupt Enable
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

void RTC_Init(void)
{
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 0x7F; // 128
	hrtc.Init.SynchPrediv = 0XFF; //255 or 255+1 i.e 256
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

	if(HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_Handler();
	}
}

void RTC_CalendarConfig(void)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef date;
	sTime.Hours = 11;
	sTime.Minutes = 44;
	sTime.TimeFormat = RTC_HOURFORMAT12_AM;
	if(HAL_OK != HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN))
	{
		Error_Handler();
	}

	date.Date = 18;
	date.Month = RTC_MONTH_DECEMBER;
	date.Year = 20; //basically it means 2020, the min value of Year is 2000.
	date.WeekDay = RTC_WEEKDAY_FRIDAY;

	if(HAL_OK != HAL_RTC_SetDate(&hrtc, &date,RTC_FORMAT_BIN))
	{
		Error_Handler();
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef date;

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	printmsg("Current time is %02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);

	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	printmsg("Current date is %02d-%02d-%02d <%s> \r\n", date.Date,date.Month, date.Year, getDayOfWeek(date.WeekDay));

	printmsg("Going to Standby mode \r\n");
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);  //Wake up pin is PA0.
	HAL_PWR_EnableBkUpReg(); //Enable the backup voltage regulator so that Back up SRAM is sustained
	HAL_PWR_EnterSTANDBYMode(); //Enter Power Stand by mode

}

//Variadic functions
void printmsg(char *format, ...)
{
	char str[80];
	/**Extract the argument list using VA Apis*/
	va_list args;
	va_start(args, format);
	//vsprintf - sends formatted output to a string, using an argument list
	vsprintf(str, format, args);
	HAL_UART_Transmit(&huart3, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
	va_end(args);
}

static char* getDayOfWeek(uint8_t number)
{
	char *weekday[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
	return weekday[number -1];
}

void RTC_Alarm_Config(void)
{
	RTC_AlarmTypeDef alarmA_set;

	memset(&alarmA_set,0, sizeof(alarmA_set));

	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

	alarmA_set.Alarm = RTC_ALARM_A;
	alarmA_set.AlarmTime.Minutes = 45;
	alarmA_set.AlarmTime.Seconds = 9;
	alarmA_set.AlarmMask = RTC_ALARMMASK_HOURS | RTC_ALARMMASK_DATEWEEKDAY;
	alarmA_set.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;

	if(HAL_OK != HAL_RTC_SetAlarm_IT(&hrtc, &alarmA_set,RTC_FORMAT_BIN))
	{
		Error_Handler();
	}

}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
}
