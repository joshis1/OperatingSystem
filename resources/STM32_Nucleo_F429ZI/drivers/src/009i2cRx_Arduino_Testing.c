/*
 * 009i2cRx_Arduino_Testing.c
 *
 *  Created on: 3/10/2020
 *      Author: sjoshi
 */



#include "stm32f429x.h"
#include "stm32f429x_gpio_driver.h"
#include "stm32f429x_i2c_driver.h"
#include <string.h>  // for memset and strlen
#include <stdlib.h>

/**
 *  PB8 - I2C1_SCL  ---   A5 (Arduino)
 *  PB9 - I2C1_SDA   ---  A4(Arduino)
 */

I2C_Handle_t i2c_handle;


extern void initialise_monitor_handles();

void delay(uint32_t delay_val)
{
	for(uint32_t i = 0; i < delay_val; i++)
	{
       //take cpu and delay it.
	}
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


void gpio_i2c_inits()
{
	//Port B clock Enable
	GPIO_PeriClockControl(GPIOB, ENABLE);
	GPIO_Handle_t  gpioB8_sclk, gpioB9_sda;

	memset(&gpioB8_sclk,0, sizeof(GPIO_Handle_t));
	memset(&gpioB9_sda,0, sizeof(GPIO_Handle_t));

	gpioB8_sclk.pGPIOx = GPIOB;
	gpioB8_sclk.gpio_pinConfig.gpio_pinMode = GPIO_MODE_ALTFN;
	gpioB8_sclk.gpio_pinConfig.gpio_pinAltFuncMode = 4;
	gpioB8_sclk.gpio_pinConfig.gpio_pinNumber = 8;
	gpioB8_sclk.gpio_pinConfig.gpio_pinPuPdControl = GPIO_PIN_PU;
	gpioB8_sclk.gpio_pinConfig.gpio_opType = GPIO_OP_TYPE_OD;
	GPIO_Init(&gpioB8_sclk);

	gpioB9_sda.pGPIOx = GPIOB;
	gpioB9_sda.gpio_pinConfig.gpio_pinMode = GPIO_MODE_ALTFN;
	gpioB9_sda.gpio_pinConfig.gpio_pinAltFuncMode = 4;
	gpioB9_sda.gpio_pinConfig.gpio_pinNumber = 9;
	gpioB9_sda.gpio_pinConfig.gpio_pinPuPdControl = GPIO_PIN_PU;
	gpioB9_sda.gpio_pinConfig.gpio_opType = GPIO_OP_TYPE_OD;
	GPIO_Init(&gpioB9_sda);

}

void i2c_inits()
{
	i2c_handle.pI2Cx = I2C1;
	i2c_handle.i2c_pinConfig.I2C_ACKControl = I2C_ACK_ENABLE;
	i2c_handle.i2c_pinConfig.I2C_DeviceAddress = 0x61; //dummy - not required here.
	i2c_handle.i2c_pinConfig.I2C_FMDutyCycle = I2C_FM_DUTY_2; // doesn't matter since we are using Standard Mode
	i2c_handle.i2c_pinConfig.I2C_SclSpeed = I2C_SCL_SPEED_STANDARD_MODE;
    I2C_Init(&i2c_handle);
}

int main()
{
	initialise_monitor_handles();
	gpio_button_init();
	gpio_i2c_inits();  //AF functions enabled
	I2C_PeriClockControl(I2C1, ENABLE);
	i2c_inits();
	I2C_PeriControl(i2c_handle.pI2Cx, ENABLE); //Enable I2C peripheral
	I2C_ManageAcking(i2c_handle.pI2Cx, ENABLE); //Enable Auto Acknowledge
    uint8_t cmd_len_read_code = 0x51;
    uint8_t cmd_data_read_code = 0x52;
    uint8_t read_len_byte = 0;

	while(1)
	{
		if(GPIO_ReadFromInputPin(GPIOC, 13))
		{
			delay(250 * 1000);
		    I2C_MasterDataSend(&i2c_handle, &cmd_len_read_code, sizeof(cmd_len_read_code),0x68, ENABLE);
		    // now Read 1 byte - Read len
		    I2C_MasterDataReceive(&i2c_handle, &read_len_byte,1,0x68 , ENABLE);
		    uint8_t *pBuffer = malloc(sizeof(uint8_t)* (read_len_byte + 1 ));

		    I2C_MasterDataSend(&i2c_handle, &cmd_data_read_code, sizeof(cmd_data_read_code),0x68, ENABLE);
		    I2C_MasterDataReceive(&i2c_handle, pBuffer,read_len_byte,0x68, DISABLE);
		    uint8_t lastIndex = sizeof(uint8_t) * (read_len_byte + 1);

		    pBuffer[lastIndex] = '\0';
		    printf("Read message from Slave is %s\r\n", pBuffer);
		    free(pBuffer);
		}
	}

}
