/*
 * stm32f429x_gpio_driver.c
 *
 *  Created on: 15Sep.,2020
 *      Author: shreyas.joshi
 */


#include "stm32f429x_gpio_driver.h"

/*********************************************************************
 * @fn      		  - GPIO_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - base address of the gpio peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi)
{
	if( EnOrDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}
		else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}
		else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}
		else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}
		else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}
		else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}
		else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_EN();
		}
		else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}
		else if(pGPIOx == GPIOI)
		{
			GPIOI_PCLK_EN();
		}
	}
	else
	{

		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}
		else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}
		else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}
		else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}
		else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}
		else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_DI();
		}
		else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_DI();
		}
		else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_DI();
		}
		else if(pGPIOx == GPIOI)
		{
			GPIOI_PCLK_DI();
		}
	}

}

/*********************************************************************
 * @fn      		  - GPIO_Init
 *
 * @brief             - This function initializes GPIO
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
   uint32_t temp = 0;
   uint8_t pin_val;

   if(pGPIOHandle->gpio_pinConfig.gpio_pinMode <= GPIO_MODE_ANALOG)
   {
	   //setting mode
	   temp = pGPIOHandle->gpio_pinConfig.gpio_pinMode << ( 2 * pGPIOHandle->gpio_pinConfig.gpio_pinNumber);
	   pGPIOHandle->pGPIOx->moder &= ~(0x3 << (2 * pGPIOHandle->gpio_pinConfig.gpio_pinNumber));
	   pGPIOHandle->pGPIOx->moder |= temp;
   }
   else
   {
	   //Interrupt mode.
   }

   // configure the speed
   temp = pGPIOHandle->gpio_pinConfig.gpio_pinSpeed << ( 2 * pGPIOHandle->gpio_pinConfig.gpio_pinNumber);
   pGPIOHandle->pGPIOx->ospeedr &= ~(0x3 << ( 2 * pGPIOHandle->gpio_pinConfig.gpio_pinNumber));
   pGPIOHandle->pGPIOx->ospeedr |= temp;

   //configure the pupd - internal - pull up, pull down resistor
   temp = pGPIOHandle->gpio_pinConfig.gpio_pinPuPdControl << ( 2 * pGPIOHandle->gpio_pinConfig.gpio_pinNumber);
   pGPIOHandle->pGPIOx->pupdr &= ~(0x3 << ( 2 * pGPIOHandle->gpio_pinConfig.gpio_pinNumber));
   pGPIOHandle->pGPIOx->pupdr |= temp;

   //configure the optype
   pGPIOHandle->pGPIOx->otyper |= pGPIOHandle->gpio_pinConfig.gpio_opType << pGPIOHandle->gpio_pinConfig.gpio_pinNumber;

   //configure the speed
   pGPIOHandle->pGPIOx->ospeedr &= ~(0x1 << pGPIOHandle->gpio_pinConfig.gpio_pinNumber);
   pGPIOHandle->pGPIOx->ospeedr |= temp;

   //configure the alt func
   if(pGPIOHandle->gpio_pinConfig.gpio_pinMode == GPIO_MODE_ALTFN)
   {
	   temp = pGPIOHandle->gpio_pinConfig.gpio_pinAltFuncMode; // 4 bit value

	   if(pGPIOHandle->gpio_pinConfig.gpio_pinNumber < 8)  /** 0 to 7 AF - low register */
	   {
		   pGPIOHandle->pGPIOx->afrl &= ~(0xF << (pGPIOHandle->gpio_pinConfig.gpio_pinNumber * 4));
		   pGPIOHandle->pGPIOx->afrl |= (temp << (pGPIOHandle->gpio_pinConfig.gpio_pinNumber * 4));

	   }
	   else   /**  AF High register. **/
	   {
		   pin_val = pGPIOHandle->gpio_pinConfig.gpio_pinNumber % 8;
		   pGPIOHandle->pGPIOx->afrh &= ~(0xF << (pin_val * 4));
		   pGPIOHandle->pGPIOx->afrh |= (temp << (pin_val * 4));
	   }

   }

}

/*********************************************************************
 * @fn      		  - GPIO_DeInit
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if(pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}
	else if(pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}
	else if(pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}
	else if(pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}
	else if(pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}
	else if(pGPIOx == GPIOF)
	{
		GPIOF_REG_RESET();
	}
	else if(pGPIOx == GPIOG)
	{
		GPIOG_REG_RESET();
	}
	else if(pGPIOx == GPIOH)
	{
		GPIOH_REG_RESET();
	}
	else if(pGPIOx == GPIOI)
	{
		GPIOI_REG_RESET();
	}
	else
	{
		//Nothing.
	}
}

/*********************************************************************
 * @fn      		  - GPIO_ReadFromInputPin
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
uint8_t  GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber)
{
	uint8_t val = 0;

	val = (pGPIOx->idr >> pinNumber) & 0x1;

	return val;
}

/*********************************************************************
 * @fn      		  - GPIO_ReadFromInputPort
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t val = 0;
	val = pGPIOx->idr;

	return val;
}

/*********************************************************************
 * @fn      		  - GPIO_WriteToOutputPin
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx,  uint8_t pinNumber, uint8_t val)
{
	if(val == 0)
	{
		pGPIOx->odr &= ~(0x1U << pinNumber);
	}
	else
	{
		pGPIOx->odr |= (0x1U << pinNumber);
	}
}

/*********************************************************************
 * @fn      		  - GPIO_WriteToOutputPort
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t val)
{
	pGPIOx->odr = val; //set the value
}

/*********************************************************************
 * @fn      		  - GPIO_ToggleOutputPin
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx,  uint8_t pinNumber)
{
	pGPIOx->odr ^= (0x1 << pinNumber);
}

/*********************************************************************
 * @fn      		  - GPIO_IRQConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_IRQConfig(uint8_t irqNumber, uint8_t irqPriority, uint8_t EnorDi)
{

}

/*********************************************************************
 * @fn      		  - GPIO_IRQHandling
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -  none
 *
 * @Note              -  none
 */
void GPIO_IRQHandling(uint8_t pinNumber)
{

}

