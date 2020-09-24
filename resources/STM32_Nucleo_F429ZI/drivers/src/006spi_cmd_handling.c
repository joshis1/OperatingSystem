/*
 * 006spi_cmd_handling.c
 *
 *  Created on: 24/09/2020
 *      Author: sjoshi
 */


/** PB12 - SPI2 - NSS  -AF5
 * PB13 -  SPI2 - SCK  -AF5
 * PB14 -  SPI2 - MISO -AF5
 * PB15 -  SPI2 - MOSI -AF5
 * SPI2 -- testing
 */

/** Adruino
 * Pin 10 - SS - slave select
 * Pin 11 - MOSI
 * Pin 12 - MISO
 * Pin 13 - SCLK
 */

#include "stm32f429x.h"
#include "stm32f429x_gpio_driver.h"
#include "stm32f429x_spi_driver.h"
#include <string.h>  // for memset and strlen


extern void initialise_monitor_handles();

void gpio_spi_inits()
{
	//Port B clock Enable
	GPIO_PeriClockControl(GPIOB, ENABLE);
	GPIO_Handle_t  gpioB12_nss, gpioB13_sck, gpioB14_miso, gpioB15_mosi;

	memset(&gpioB12_nss,0, sizeof(GPIO_Handle_t));
	memset(&gpioB13_sck,0, sizeof(GPIO_Handle_t));
	memset(&gpioB14_miso,0, sizeof(GPIO_Handle_t));
	memset(&gpioB15_mosi,0, sizeof(GPIO_Handle_t));

	gpioB12_nss.pGPIOx = GPIOB;
	gpioB12_nss.gpio_pinConfig.gpio_pinMode = GPIO_MODE_ALTFN;
	gpioB12_nss.gpio_pinConfig.gpio_pinAltFuncMode = 5;
	gpioB12_nss.gpio_pinConfig.gpio_pinNumber = 12;
	gpioB12_nss.gpio_pinConfig.gpio_pinPuPdControl = GPIO_PIN_PU;
	gpioB12_nss.gpio_pinConfig.gpio_opType = GPIO_OP_TYPE_PP;
	GPIO_Init(&gpioB12_nss);

	gpioB13_sck.pGPIOx = GPIOB;
	gpioB13_sck.gpio_pinConfig.gpio_pinMode = GPIO_MODE_ALTFN;
	gpioB13_sck.gpio_pinConfig.gpio_pinAltFuncMode = 5;
	gpioB13_sck.gpio_pinConfig.gpio_pinNumber = 13;
	gpioB13_sck.gpio_pinConfig.gpio_pinPuPdControl = GPIO_PIN_PU;
	gpioB13_sck.gpio_pinConfig.gpio_opType = GPIO_OP_TYPE_PP;
	GPIO_Init(&gpioB13_sck);

	gpioB14_miso.pGPIOx = GPIOB;
	gpioB14_miso.gpio_pinConfig.gpio_pinMode = GPIO_MODE_ALTFN;
	gpioB14_miso.gpio_pinConfig.gpio_pinAltFuncMode = 5;
	gpioB14_miso.gpio_pinConfig.gpio_pinNumber = 14;
	gpioB14_miso.gpio_pinConfig.gpio_pinPuPdControl = GPIO_PIN_PU;
	gpioB14_miso.gpio_pinConfig.gpio_opType = GPIO_OP_TYPE_PP;
	GPIO_Init(&gpioB14_miso);

	gpioB15_mosi.pGPIOx = GPIOB;
	gpioB15_mosi.gpio_pinConfig.gpio_pinMode = GPIO_MODE_ALTFN;
	gpioB15_mosi.gpio_pinConfig.gpio_pinAltFuncMode = 5;
	gpioB15_mosi.gpio_pinConfig.gpio_pinNumber = 15;
	gpioB15_mosi.gpio_pinConfig.gpio_pinPuPdControl = GPIO_PIN_PU;
	gpioB15_mosi.gpio_pinConfig.gpio_opType = GPIO_OP_TYPE_PP;
	GPIO_Init(&gpioB15_mosi);

}

void spi_inits()
{
	SPI_Handle_t spi2;

	SPI_PeriClockControl(SPI2,ENABLE);  //Enable SPI2


	spi2.pSPIx = SPI2;
	spi2.spi_pinConfig.busConfig = Full_Duplex;
	spi2.spi_pinConfig.cpha = CPHA_LOW;
	spi2.spi_pinConfig.cpol = CPOL_LOW;
	spi2.spi_pinConfig.deviceMode = master_mode;
	spi2.spi_pinConfig.dff = SPI_DFF_8BITS;
	spi2.spi_pinConfig.sclkSpeed = SCLK_SPEED_DIV128;
	spi2.spi_pinConfig.ssm = SSM_ENABLE;

	SPI_Init(&spi2);

}

void gpio_button_init()
{
	GPIO_Handle_t gpioButton;
	memset(&gpioButton, 0, sizeof(gpioButton));

	/** Button Interrupt Configuration **/
	gpioButton.pGPIOx = GPIOC; //Port C
	gpioButton.gpio_pinConfig.gpio_pinMode = GPIO_MODE_IT_RT; // Rising edge trigger - Interrupt
	gpioButton.gpio_pinConfig.gpio_pinNumber = 13;
	// Very important make it no pull up pull down otherwise button won't work
	gpioButton.gpio_pinConfig.gpio_pinPuPdControl = GPIO_NO_PUPD;


	GPIO_PeriClockControl(GPIOC,ENABLE); //Enable port C RCC clock


	GPIO_Init(&gpioButton);
	//NVIC Side i.e. Processor based configuration
	GPIO_IRQPriorityConfig(IRQ_EXTI15_10, 15);
	GPIO_IRQInterruptConfig(IRQ_EXTI15_10, ENABLE);

}


uint32_t len = 0;
char user_data[] = "Hello World";


int main()
{
	initialise_monitor_handles();
	gpio_spi_inits();  //AF functions enabled
	printf("gpio spi init done!\r\n");
	spi_inits();   //SPI init initialize - master mode, DFF, duplex, clk speed, cpha, cpol, etc.
	gpio_button_init();
	printf("gpio_button_init done!\r\n");

	SPI_SSOEControl(SPI2, ENABLE); //enable Hardware based Slave Select from Master MCU.
	printf("SPI_SSOEControl done!\r\n");

	while(1)
	{
		//forever
	}

}

void delay(uint32_t delay_val)
{
	for(uint32_t i = 0; i < delay_val; i++)
	{
       //take cpu and delay it.
	}
}

//ISR - defined in startup_stm32.s - can be overridden here.
// this is .weak that means can be overridden.

void EXTI15_10_IRQHandler(void)
{
	// due to button debouncing we add this delay otherwise we will get more number of interrupts.
	delay(250 * 1000);
	GPIO_IRQHandling(13);  //EXTI - pin number

	SPI_PeriControl(SPI2, ENABLE); // Very important this makes the SPI enable bit to 1.
	len = strlen(user_data);
	SPI_SendData(SPI2, (uint8_t *)user_data, len);  //data send here.

	while(SPI_GetFlagStatus(SPI2,SPI_SR_BSY)); //wait until the bsy flag is 0.

	SPI_PeriControl(SPI2, DISABLE); // After SPI communication disable the SPI


}
