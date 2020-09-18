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
	uint32_t temp1;

	if(pGPIOHandle->gpio_pinConfig.gpio_pinMode <= GPIO_MODE_ANALOG)
	{
		//setting mode
		temp = pGPIOHandle->gpio_pinConfig.gpio_pinMode << ( 2 * pGPIOHandle->gpio_pinConfig.gpio_pinNumber);
		pGPIOHandle->pGPIOx->moder &= ~(0x3 << (2 * pGPIOHandle->gpio_pinConfig.gpio_pinNumber));
		pGPIOHandle->pGPIOx->moder |= temp;
	}
	else //Interrupt mode.
	{
		/** Peripheral side interrupt configuration **/
		// Falling edge trigger.
		if(pGPIOHandle->gpio_pinConfig.gpio_pinMode  == GPIO_MODE_IT_FT )
		{
			EXTI->EXTI_FTSR |= (0x1U << pGPIOHandle->gpio_pinConfig.gpio_pinNumber);
			EXTI->EXTI_RTSR &= ~(0x1U << pGPIOHandle->gpio_pinConfig.gpio_pinNumber);
		}
		//Rising edge trigger
		else if(pGPIOHandle->gpio_pinConfig.gpio_pinMode  == GPIO_MODE_IT_RT)
		{
			EXTI->EXTI_RTSR |= (0x1U << pGPIOHandle->gpio_pinConfig.gpio_pinNumber);
			EXTI->EXTI_FTSR &= ~(0x1U << pGPIOHandle->gpio_pinConfig.gpio_pinNumber);
		}
		// Falling and Rising Edge trigger interrupt
		else if(pGPIOHandle->gpio_pinConfig.gpio_pinMode  == GPIO_MODE_IT_RFT )
		{
			EXTI->EXTI_FTSR |= (0x1U << pGPIOHandle->gpio_pinConfig.gpio_pinNumber);
			EXTI->EXTI_RTSR |= (0x1U << pGPIOHandle->gpio_pinConfig.gpio_pinNumber);
		}

		/** SYSCFG CLK Enable **/
		uint8_t port;
		temp = pGPIOHandle->gpio_pinConfig.gpio_pinNumber / 4;
		temp1 = pGPIOHandle->gpio_pinConfig.gpio_pinNumber % 4;
		port = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		//Enabled the EXTI-line now.
		SYSCFG->SYSCFG_EXTICR[temp] &= ~(0xF << (temp1 *4)); //clear and then enable
		SYSCFG->SYSCFG_EXTICR[temp] |= ( port << (temp1 * 4));
		// unmask the interrupt i.e. enable it.
		EXTI->EXTI_IMR |= (0x1U << pGPIOHandle->gpio_pinConfig.gpio_pinNumber);
		/** Peripheral side interrupt configuration Ends here **/
	}

	//configure the pupd - internal - pull up, pull down resistor
	temp = pGPIOHandle->gpio_pinConfig.gpio_pinPuPdControl << ( 2 * pGPIOHandle->gpio_pinConfig.gpio_pinNumber);
	pGPIOHandle->pGPIOx->pupdr &= ~(0x3 << ( 2 * pGPIOHandle->gpio_pinConfig.gpio_pinNumber));
	pGPIOHandle->pGPIOx->pupdr |= temp;

	//configure the optype
	if(pGPIOHandle->gpio_pinConfig.gpio_pinMode == GPIO_MODE_OUT)
	{
		pGPIOHandle->pGPIOx->otyper |= pGPIOHandle->gpio_pinConfig.gpio_opType << pGPIOHandle->gpio_pinConfig.gpio_pinNumber;

		// configure the speed
		temp = pGPIOHandle->gpio_pinConfig.gpio_pinSpeed << ( 2 * pGPIOHandle->gpio_pinConfig.gpio_pinNumber);
		pGPIOHandle->pGPIOx->ospeedr &= ~(0x3 << ( 2 * pGPIOHandle->gpio_pinConfig.gpio_pinNumber));
		pGPIOHandle->pGPIOx->ospeedr |= temp;

	}

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
 * @fn      		  - GPIO_IRQInterruptConfig
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
void GPIO_IRQInterruptConfig(uint8_t irqNumber, uint8_t EnorDi)
{
	uint8_t bit_pos = 0;
	bit_pos = irqNumber % 32;
	volatile uint32_t *nvic_reg;

	if(EnorDi == ENABLE)
	{
		if(irqNumber >= 0 && irqNumber < 32)
		{
			nvic_reg = (uint32_t *)NVIC_ISER0;
			*nvic_reg  = (0x1U << bit_pos);
		}
		else if(irqNumber >= 32 && irqNumber < 64)
		{
			nvic_reg = (uint32_t *)NVIC_ISER1;
			*nvic_reg  = (0x1U << bit_pos);

		}
		else if(irqNumber >= 64 && irqNumber < 96)
		{
			nvic_reg = (uint32_t *)NVIC_ISER2;
			*nvic_reg  = (0x1U << bit_pos);
		}
		else
		{
			//probably shouldn't reach here.
		}
	}

	else
	{
		if(irqNumber >= 0 && irqNumber < 32)
		{
			nvic_reg = (uint32_t *)NVIC_ICER0;
			*nvic_reg  = (0x1U << bit_pos);
		}
		else if(irqNumber >= 32 && irqNumber < 64)
		{
			nvic_reg = (uint32_t *)NVIC_ICER1;
			*nvic_reg  = (0x1U << bit_pos);
		}
		else if(irqNumber >= 64 && irqNumber < 96)
		{
			nvic_reg = (uint32_t *)NVIC_ICER2;
			*nvic_reg  = (0x1U << bit_pos);
		}
		else
		{
			//probably shouldn't reach here.
		}
	}
}

/*********************************************************************
 * @fn      		  - GPIO_IRQPriorityConfig
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
void GPIO_IRQPriorityConfig(uint8_t irqNumber, uint16_t priority)
{
   uint8_t nvic_priority_reg_offset = 0;
   uint8_t bit_pos = 0;
   volatile uint32_t *nvic_register;
   //each PR can fit 4 IRQNumber
   nvic_priority_reg_offset = irqNumber / 4;
   nvic_register = (uint32_t *)(NVIC_IPR0 + (nvic_priority_reg_offset * 4));
   bit_pos = irqNumber % 4;
   //lower 4 bits of priority are not implemented in STM32
   // each priority value takes 8 bits
   *nvic_register = (priority << ((bit_pos * 8) + 4));
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
	// clear the pending bit if the IRQ pending bit is set.
	if( EXTI->EXTI_PR & (0x1U << pinNumber))
	{
		EXTI->EXTI_PR |= (0x1U << pinNumber);
	}
}

