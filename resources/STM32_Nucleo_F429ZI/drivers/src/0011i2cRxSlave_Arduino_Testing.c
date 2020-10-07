/*
 * 0011i2cRxSlave_Arduino_Testing.c
 *
 *  Created on: 7Oct.,2020
 *      Author: shreyas.joshi
 */


/** You cannot put break points here since Arduino Master cannot do
 * Clock stretch -- i.e. lowering the clock - wait state
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

uint8_t tx_buffer[32] = "Hello STM32 is Slave";

uint8_t buffer_count = 0;

uint8_t command_code = 0x14; //some invalid code - i.e. not defined code.

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
	i2c_handle.i2c_pinConfig.I2C_DeviceAddress = 0x69; //STM32 Slave Address
	i2c_handle.i2c_pinConfig.I2C_FMDutyCycle = I2C_FM_DUTY_2; // doesn't matter since we are using Standard Mode
	i2c_handle.i2c_pinConfig.I2C_SclSpeed = I2C_SCL_SPEED_STANDARD_MODE;
    I2C_Init(&i2c_handle);
}

int main()
{
	gpio_i2c_inits();  //AF functions enabled
	I2C_PeriClockControl(I2C1, ENABLE);
	i2c_inits();

	I2C_IRQInterruptConfig(IRQ_I2C1_EV, ENABLE);
	I2C_IRQInterruptConfig(IRQ_I2C1_ER, ENABLE);

	I2C_SlaveEnableDisableCallbackEvents(I2C1, ENABLE);

	I2C_PeriControl(i2c_handle.pI2Cx, ENABLE); //Enable I2C peripheral
	I2C_ManageAcking(i2c_handle.pI2Cx, ENABLE); //Enable Auto Acknowledge

	while(1)
	{

	}

}

void I2C1_EV_IRQHandler()
{
	I2C_Event_IRQHandling(&i2c_handle);
}

void I2C1_ER_IRQHandler()
{
	I2C_Error_IRQHandling(&i2c_handle);
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t event)
{
	uint8_t dummy_debug = 0;

	if( event == I2C_SLAVE_EVENT_DATA_REQUEST)
	{
		if(command_code == 0x51)
		{
			I2C_SlaveDataSend(pI2CHandle->pI2Cx, strlen((char *) tx_buffer));
		}
		if(command_code == 0x52)
		{
			//this will hit multiple times i.e. equal to the length send above.
			I2C_SlaveDataSend(pI2CHandle->pI2Cx, tx_buffer[buffer_count++]);
		}

	}

	if( event == I2C_SLAVE_EVENT_DATA_RECEIVE)
	{
		//got the command code from here -- first -- expecting -0x51 for len and then 0x52 for sending the data.
		command_code = I2C_SlaveDataReceive(pI2CHandle->pI2Cx);
	}


	if(event == I2C_EVENT_ACK_ERROR)
	{
		//Stop sending the data
		command_code = 0x14; //some invalid code - i.e. not defined code.
		buffer_count = 0;
	}

	if( event == I2C_EVENT_STOP)
	{
		//stop received.
		dummy_debug = 1;
		(void)dummy_debug;
	}
}
