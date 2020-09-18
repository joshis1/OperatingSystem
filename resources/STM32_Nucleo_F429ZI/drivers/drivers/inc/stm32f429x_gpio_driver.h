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

/*
 *  @GPIO_PIN_MODES
 *  GPIO pin possible modes
 */

#define GPIO_MODE_IN       (0)  /** Input mode  **/
#define GPIO_MODE_OUT      (1)  /** Output mode **/
#define GPIO_MODE_ALTFN    (2)  /** Alternate function mode **/
#define GPIO_MODE_ANALOG   (3)  /** Analog mode **/
#define GPIO_MODE_IT_FT    (4)  /** Falling edge **/
#define GPIO_MODE_IT_RT    (5)  /** Rising edge **/
#define GPIO_MODE_IT_RFT   (6)  /** Rising and Falling edge **/


/*
 *  GPIO pin possible output types
 */
#define GPIO_OP_TYPE_PP  (0)  /** Push Pull **/
#define GPIO_OP_TYPE_OD  (1)  /** Open drain **/

/*
 *  @GPIO Pin Speed
 *  GPIO pin possible output speed
 */
#define GPIO_SPEED_LOW    (0)
#define GPIO_SPEED_MEDIUM (1)
#define GPIO_SPEED_FAST   (2)
#define GPIO_SPEED_HIGH   (3)

/*
 * GPIO pin pull and pull down configuration macros
 *
 */
#define GPIO_NO_PUPD  (0)
#define GPIO_PIN_PU   (1)
#define GPIO_PIN_PD   (2)


#define GPIO_BASEADDR_TO_CODE(x) (x == GPIOA) ? 0 : \
		                         (x == GPIOB) ? 1 : \
		                         (x == GPIOC) ? 2 : \
		                         (x == GPIOD) ? 3 : \
		                         (x == GPIOE) ? 4 : \
		                         (x == GPIOF) ? 5 : \
		                         (x == GPIOG) ? 6 : \
		                         (x == GPIOH) ? 7 : \
		                         (x == GPIOI) ? 8 : 0


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


/**IRQ handling  - NVIC side i.e. Processor side **/
void GPIO_IRQInterruptConfig(uint8_t irqNumber, uint8_t EnorDi); //interrupt configuration
void GPIO_IRQPriorityConfig(uint8_t irqNumber, uint16_t priority); //interrupt priority configuration
void GPIO_IRQHandling(uint8_t pinNumber);



#endif /* INC_STM32F429X_GPIO_DRIVER_H_ */
