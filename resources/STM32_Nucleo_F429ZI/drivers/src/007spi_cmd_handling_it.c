/*
 * 007spi_cmd_handling_it.c
 *
 *  Created on: 26/09/2020
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


#define COMMAND_SENSOR_READ     (0x51)

extern void initialise_monitor_handles();
void delay(uint32_t delay_val);

SPI_Handle_t spi2;
uint8_t ReadByte;

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
	SPI_PeriClockControl(SPI2,ENABLE);  //Enable SPI2

	spi2.pSPIx = SPI2;
	spi2.spi_pinConfig.busConfig = Full_Duplex;
	spi2.spi_pinConfig.cpha = CPHA_LOW;
	spi2.spi_pinConfig.cpol = CPOL_LOW;
	spi2.spi_pinConfig.deviceMode = master_mode;
	spi2.spi_pinConfig.dff = SPI_DFF_8BITS;
	spi2.spi_pinConfig.sclkSpeed = SCLK_SPEED_DIV128;
	spi2.spi_pinConfig.ssm = SSM_DISABLE;

	SPI_Init(&spi2);

}

void gpio_button_init()
{
	GPIO_Handle_t gpioButton;
	gpioButton.pGPIOx = GPIOC; //Port C
	gpioButton.gpio_pinConfig.gpio_pinMode = GPIO_MODE_IN; // Input mode
	gpioButton.gpio_pinConfig.gpio_pinNumber = 13;
	/**Already the button has internal pull up resistor **/
	gpioButton.gpio_pinConfig.gpio_pinPuPdControl = GPIO_NO_PUPD; // no pull up or pull down
	GPIO_PeriClockControl(GPIOC,ENABLE); //Enable port C RCC clock
	GPIO_Init(&gpioButton);
}

uint8_t RxContFlag = 0;

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

	uint8_t commandCode = COMMAND_SENSOR_READ;


	while(1)
	{
		if(GPIO_ReadFromInputPin(GPIOC, 13))
		{
			delay(250 * 1000);
			SPI_IRQInterruptConfig(IRQ_SPI2, ENABLE);
			SPI_PeriControl(SPI2, ENABLE); // Very important this makes the SPI enable bit to 1.
			SPI_SendData(SPI2, &commandCode, 1);
			RxContFlag = 1;
			while(RxContFlag == 1)
			{
				while ( ! (SPI_ReceiveDataIT(&spi2,&ReadByte,1) == SPI_READY) );
			}
			while(SPI_GetFlagStatus(SPI2,SPI_SR_BSY)); //wait until the bsy flag is 0.
			SPI_PeriControl(SPI2, DISABLE); // After SPI communication disable the SPI
		}
	}

}

void delay(uint32_t delay_val)
{
	for(uint32_t i = 0; i < delay_val; i++)
	{
       //take cpu and delay it.
	}
}

void SPI2_IRQHandler()
{
	SPI_IRQHandling(&spi2);
}

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t event)
{

	if(event == SPI_EVENT_RX_COMPLETE)
	{
        printf("Received something\r\n");
	}
}

