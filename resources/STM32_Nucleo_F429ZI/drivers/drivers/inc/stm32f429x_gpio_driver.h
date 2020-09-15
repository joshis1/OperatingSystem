/*
 * stm32f429x_gpio_driver.h
 *
 *  Created on: 15Sep.,2020
 *      Author: shreyas.joshi
 */

#ifndef INC_STM32F429X_GPIO_DRIVER_H_
#define INC_STM32F429X_GPIO_DRIVER_H_

#include "stm32f429x.h"


#define GPIO_PIN_SET        SET
#define GPIO_PIN_RESET      RESET

typedef struct
{
  uint8_t gpio_pinNumber;
  uint8_t gpio_pinMode;
  uint8_t gpio_pinSpeed;
  uint8_t gpio_pinPuPdControl;
  uint8_t gpio_opType;
  uint8_t gpio_pinAltFuncMode;
}GPIO_PinConfig_t;

typedef struct
{
	GPIO_RegDef_t *pGPIOx;
	GPIO_PinConfig_t gpio_pinConfig;
}GPIO_Handle_t;


/** APIs **/
/** Peripheral clock enable/disable - RCC - Enable/Disable  **/
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi);

/**Init and De-Init **/
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);  // Initialize Port and Pin
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);  // Use RCC - reset register RCC_AHB1RSTR


/** Read and Write **/
uint8_t  GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx,  uint8_t pinNumber, uint8_t val);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t val);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx,  uint8_t pinNumber);


/**IRQ handling **/
void GPIO_IRQConfig(uint8_t irqNumber, uint8_t irqPriority, uint8_t EnorDi); //interrupt configuration
void GPIO_IRQHandling(uint8_t pinNumber);



#endif /* INC_STM32F429X_GPIO_DRIVER_H_ */
