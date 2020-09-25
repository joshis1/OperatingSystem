/*
 * stm32f429x.h
 *
 *  Created on: 14/09/2020
 *      Author: sjoshi
 */

#ifndef STM32F429X_H_
#define STM32F429X_H_

#include <stdint.h>


#define _VO  volatile


/** Reference manual - page 71 **/
#define FLASH_BASEADDR        (0x08000000U)
#define SRAM1_BASEADDR        (0x20000000U)
#define SRAM1                 (SRAM1_BASEADDR)
#define SRAM1_SIZE            (0x1C000U)  //112KB i.e 112 * 1024
#define SRAM2_BASEADDR        (SRAM1_BASEADDR + SRAM1_SIZE)
#define SRAM2_SIZE            (0x4000U) //16KB i.e. 16 * 1024 
#define SRAM3_BASEADDR        (SRAM2_BASEADDR + SRAM2_SIZE)
#define SRAM3_SIZE            (‭0x10000‬U) //64KB i.e. 64 * 1024
#define SYSTEM_MEMORY         (0x1FFF0000U)
#define ROM                   (SYSTEM_MEMORY)


//Table 1. STM32F4xx register boundary addresses

//AHB1 is 180 Mhz
//APB1 is 45  Mhz
//APB2 is 90  Mhz 

#define AHB1_BASEADDR         (0x40020000U)
#define APB2_BASEADDR         (0x40010000U)
#define APB1_BASEADDR         (0x40000000U)
#define AHB2_BASEADDR         (0x50000000U)
#define AHB3_BASEADDR         (0xA0000000U)


//Table 1. STM32F4xx register boundary addresses
//You can see each GPIO base address is 0x400 apart i.e. 1024 bytes 

//GPIO is on ABH1 bus - i.e. high speed 
#define GPIOA_BASEADDR         (0x40020000U)
#define GPIOB_BASEADDR         (0x40020400U) 
#define GPIOC_BASEADDR         (0x40020800U)
#define GPIOD_BASEADDR         (0x40020C00U)
#define GPIOE_BASEADDR         (0x40021000U)
#define GPIOF_BASEADDR         (0x40021400U)
#define GPIOG_BASEADDR         (0x40021800U)
#define GPIOH_BASEADDR         (0x40021C00U)
#define GPIOI_BASEADDR         (0x40022000U)
#define GPIOJ_BASEADDR         (0x40022400U)
#define GPIOK_BASEADDR         (0x40022800U)


//SPI peripherals 

#define SPI1_BASEADDR          (0x40013000U)    //APB2 bus
#define SPI2_BASEADDR          (0x40003800U)    //APB1 bus
#define SPI3_BASEADDR          (0x40003C00U)    //APB1 bus
#define SPI4_BASEADDR          (0x40013400U)    //APB2 bus
#define SPI5_BASEADDR          (0x40015000U)    //APB2 bus
#define SPI6_BASEADDR          (0x40015400U)    //APB2 bus

//I2c peripherals 

#define I2C1_BASEADDR          (0x40005400U)  //APB1 bus 
#define I2C2_BASEADDR          (0x40005800U)  //APB1 bus
#define I2C3_BASEADDR          (0x40005C00U) // APB1 bus 


//UART and USART peripherals 

#define USART1_BASEADDR        (0x40011000U) // APB2 bus 
#define USART2_BASEADDR        (0x40004400U) // APB1 bus 
#define USART3_BASEADDR        (0x40004800U) // APB1 bus 
#define USART6_BASEADDR        (0x40011400U) // APB2 bus 

#define UART4_BASEADDR         (0x40004C00U)  // APB1 bus 
#define UART5_BASEADDR         (0x40005000U)  // APB1 bus 
#define UART7_BASEADDR         (0x40007800U)  // APB1 bus 
#define UART8_BASEADDR         (0x40007C00U)  // APB1 bus 

//Processor specific - ARM Cortex M4 - Generic user guide
//NVIC - Interrupt Set enable register
// Interrupt set enable register
#define NVIC_ISER0             (0xE000E100U)  // 0 to 31
#define NVIC_ISER1             (0xE000E100U + 0x04) //32 to 63
#define NVIC_ISER2             (0xE000E100U + 0x08) // 64 to 95


//Interrupt clear enable register
#define NVIC_ICER0             (0XE000E180U)
#define NVIC_ICER1             (0XE000E180U + 0x04) //32 to 63
#define NVIC_ICER2             (0XE000E180U + 0x08) // 64 to 95

//Interrupt priority register
#define NVIC_IPR0              (0xE000E400U)

//Reset and Clock Control
#define RCC_BASEADDR           (0x40023800U) // AHB1 bus 

//System configuration and controller
#define SYSCFG_BASEADDR        (0x40013800U) // APB2 bus 


//External interrupt and event controller
#define EXTI_BASEADDR        (0x40013C00U) // APB2 bus 


//8.4.11 GPIO register map - Reference manual.
typedef struct 
{
	_VO uint32_t moder;    // -- mode register - like input, output, AF, Analog - address offset   0x00
	_VO uint32_t otyper;   //- output type - push pull or open drain - address offset   0x04
	_VO uint32_t ospeedr;  //- output speed register - address offset   0x08
	_VO uint32_t pupdr;    //- pull up and pull down register - address offset   0x0C
	_VO uint32_t idr;      //- input data register - address offset   0x10
	_VO uint32_t odr;      //- output data register - address offset   0x14
	_VO uint32_t bsrr;     //- bit set reset bit - address offset   0x18
	_VO uint32_t lckr;     //- lock register - address offset   0x1C
	_VO uint32_t afrl;     //- AF low register i.e. 0 to 7 pins - address offset   0x20
	_VO uint32_t afrh;     //- AF high register i.e. 8 to 15 pins - address offset   0x24
}GPIO_RegDef_t;

#define GPIOA   ((GPIO_RegDef_t *)GPIOA_BASEADDR)
#define GPIOB   ((GPIO_RegDef_t *)GPIOB_BASEADDR)
#define GPIOC   ((GPIO_RegDef_t *)GPIOC_BASEADDR)
#define GPIOD   ((GPIO_RegDef_t *)GPIOD_BASEADDR)
#define GPIOE   ((GPIO_RegDef_t *)GPIOE_BASEADDR)
#define GPIOF   ((GPIO_RegDef_t *)GPIOF_BASEADDR)
#define GPIOG   ((GPIO_RegDef_t *)GPIOG_BASEADDR)
#define GPIOH   ((GPIO_RegDef_t *)GPIOH_BASEADDR)
#define GPIOI   ((GPIO_RegDef_t *)GPIOI_BASEADDR)
#define GPIOJ   ((GPIO_RegDef_t *)GPIOJ_BASEADDR)
#define GPIOK   ((GPIO_RegDef_t *)GPIOK_BASEADDR)


/** Table 34. RCC register map and reset values **/
typedef struct 
{
	_VO uint32_t RCC_CR;    //     - address offset   0x00
	_VO uint32_t RCC_PLLCFGR;   // -  address offset   0x04
	_VO uint32_t RCC_CFGR;     //- address offset   0x08
	_VO uint32_t RCC_CIR;    //- address offset   0x0C
	_VO uint32_t RCC_AHB1RSTR;      //- address offset   0x10
	_VO uint32_t RCC_AHB2RSTR;      //- address offset   0x14
	_VO uint32_t RCC_AHB3RSTR;     //- address offset   0x18
	_VO uint32_t Reserved_1;     //- address offset   0x1C
	_VO uint32_t RCC_APB1RSTR;     //- address offset   0x20
	_VO uint32_t RCC_APB2RSTR;     //- address offset   0x24
	_VO uint32_t Reserved_2;    // - address offset   0x28
	_VO uint32_t Reserved_3;   //  - address offset   0x2C
	_VO uint32_t RCC_AHB1ENR;  //-   address offset   0x30
	_VO uint32_t RCC_AHB2ENR;    //- address offset   0x34
	_VO uint32_t RCC_AHB3ENR;      //- address offset   0x38
	_VO uint32_t Reserved_val;      //- address offset   0x3C
	_VO uint32_t RCC_APB1ENR;      //- address offset   0x40
	_VO uint32_t RCC_APB2ENR;     //- address offset   0x44
	_VO uint32_t Reserved_4;     //address offset   0x48
	_VO uint32_t Reserved_5;     //- address offset   0x4C
	_VO uint32_t RCC_AHB1LPENR;     //- address offset   0x50
	_VO uint32_t RCC_AHB2LPENR;    // - address offset   0x54
	_VO uint32_t RCC_AHB3LPENR;   //-  address offset   0x58
	_VO uint32_t Reserved_6;  //-  address offset   0x5C
	_VO uint32_t RCC_APB1LPENR;    //- address offset   0x60
	_VO uint32_t RCC_APB2LPENR;      //- address offset   0x64
	_VO uint32_t Reserved_7;      //- address offset   0x68
	_VO uint32_t Reserved_8;     //- address offset   0x6C
	_VO uint32_t RCC_BDCR;     //- address offset   0x70
	_VO uint32_t RCC_CSR;     //- address offset   0x74
	_VO uint32_t Reserved_9;     //- address offset   0x78
	_VO uint32_t Reserved_10;    // - address offset   0x7C
	_VO uint32_t RCC_SSCGR;   //- address offset   0x80
	_VO uint32_t RCC_PLLI2SCFGR;  //- address offset   0x84
}RCC_RegDef_t;


#define RCC   ((RCC_RegDef_t *)RCC_BASEADDR)

/** Table 41. SYSCFG register map and reset values **/
typedef struct
{
	_VO uint32_t SYSCFG_MEMRMP;    //     - address offset   0x00
	_VO uint32_t SYSCFG_PMC;   // -  address offset   0x04
	_VO uint32_t SYSCFG_EXTICR[4];     //- address offset   0x08
	_VO uint32_t Reserved_1;     //- address offset   0x18
	_VO uint32_t Reserved_2;     //- address offset   0x1C
	_VO uint32_t SYSCFG_CMPCR;     //- address offset   0x20
}SYSCFG_RegDef_t;


#define SYSCFG ((SYSCFG_RegDef_t *)SYSCFG_BASEADDR)

/** Table 34. RCC register map and reset values **/
typedef struct
{
	_VO uint32_t EXTI_IMR;    //     - address offset   0x00
	_VO uint32_t EXTI_EMR;   // -  address offset   0x04
	_VO uint32_t EXTI_RTSR;     //- address offset   0x08
	_VO uint32_t EXTI_FTSR;    //- address offset   0x0C
	_VO uint32_t EXTI_SWIER;      //- address offset   0x10
	_VO uint32_t EXTI_PR;      //- address offset   0x14
}EXTI_RegDef_t;


#define EXTI ((EXTI_RegDef_t *)EXTI_BASEADDR)



#define GPIOA_PCLK_EN() (RCC->RCC_AHB1ENR |= (1U <<0))
#define GPIOB_PCLK_EN() (RCC->RCC_AHB1ENR |= (1U <<1))
#define GPIOC_PCLK_EN() (RCC->RCC_AHB1ENR |= (1U <<2))
#define GPIOD_PCLK_EN() (RCC->RCC_AHB1ENR |= (1U <<3))
#define GPIOE_PCLK_EN() (RCC->RCC_AHB1ENR |= (1U <<4))
#define GPIOF_PCLK_EN() (RCC->RCC_AHB1ENR |= (1U <<5))
#define GPIOG_PCLK_EN() (RCC->RCC_AHB1ENR |= (1U <<6))
#define GPIOH_PCLK_EN() (RCC->RCC_AHB1ENR |= (1U <<7))
#define GPIOI_PCLK_EN() (RCC->RCC_AHB1ENR |= (1U <<8))


#define I2C1_PCLK_EN() (RCC->RCC_APB1ENR |= (1U <<21))
#define I2C2_PCLK_EN() (RCC->RCC_APB1ENR |= (1U <<22))
#define I2C3_PCLK_EN() (RCC->RCC_APB1ENR |= (1U <<23))

#define SPI1_PCLK_EN() (RCC->RCC_APB2ENR |= (1U <<12))  //APB2 -- connected to
#define SPI2_PCLK_EN() (RCC->RCC_APB1ENR |= (1U <<14))
#define SPI3_PCLK_EN() (RCC->RCC_APB1ENR |= (1U <<15))


#define GPIOA_PCLK_DI() (RCC->RCC_AHB1ENR &= ~(1U <<0))
#define GPIOB_PCLK_DI() (RCC->RCC_AHB1ENR &= ~(1U <<1))
#define GPIOC_PCLK_DI() (RCC->RCC_AHB1ENR &= ~(1U <<2))
#define GPIOD_PCLK_DI() (RCC->RCC_AHB1ENR &= ~(1U <<3))
#define GPIOE_PCLK_DI() (RCC->RCC_AHB1ENR &= ~(1U <<4))
#define GPIOF_PCLK_DI() (RCC->RCC_AHB1ENR &= ~(1U <<5))
#define GPIOG_PCLK_DI() (RCC->RCC_AHB1ENR &= ~(1U <<6))
#define GPIOH_PCLK_DI() (RCC->RCC_AHB1ENR &= ~(1U <<7))
#define GPIOI_PCLK_DI() (RCC->RCC_AHB1ENR &= ~(1U <<8))


#define I2C1_PCLK_DI() (RCC->RCC_APB1ENR &= ~(1U <<21))
#define I2C2_PCLK_DI() (RCC->RCC_APB1ENR &= ~(1U <<22))
#define I2C3_PCLK_DI() (RCC->RCC_APB1ENR &= ~(1U <<23))

#define SPI1_PCLK_DI() (RCC->RCC_APB2ENR &= ~(1U <<12))  //APB2 -- connected to
#define SPI2_PCLK_DI() (RCC->RCC_APB1ENR &= ~(1U <<14))
#define SPI3_PCLK_DI() (RCC->RCC_APB1ENR &= ~(1U <<15))


// Page
#define SYSCFG_PCLK_EN() (RCC->RCC_APB2ENR |= (1U <<14))  //APB2 -
#define SYSCFG_PCLK_DI() (RCC->RCC_APB2ENR &= ~(1U <<14))


#define GPIOA_REG_RESET()   do \
                            { \
	                          RCC->RCC_AHB1RSTR |= (0x1 << 0); \
							  RCC->RCC_AHB1RSTR &= ~(0x1 << 0); \
                            }while(0)


#define GPIOB_REG_RESET()   do \
                            { \
	                          RCC->RCC_AHB1RSTR |= (0x1 << 1); \
							  RCC->RCC_AHB1RSTR &= ~(0x1 << 1); \
                            }while(0)


#define GPIOC_REG_RESET()   do \
                            { \
	                          RCC->RCC_AHB1RSTR |= (0x1 << 2); \
							  RCC->RCC_AHB1RSTR &= ~(0x1 << 2); \
                            }while(0)

#define GPIOD_REG_RESET()   do \
                            { \
	                          RCC->RCC_AHB1RSTR |= (0x1 << 3); \
							  RCC->RCC_AHB1RSTR &= ~(0x1 << 3); \
                            }while(0)

#define GPIOE_REG_RESET()   do \
                            { \
	                          RCC->RCC_AHB1RSTR |= (0x1 << 4); \
							  RCC->RCC_AHB1RSTR &= ~(0x1 << 4); \
                            }while(0)

#define GPIOF_REG_RESET()   do \
                            { \
	                          RCC->RCC_AHB1RSTR |= (0x1 << 5); \
							  RCC->RCC_AHB1RSTR &= ~(0x1 << 5); \
                            }while(0)

#define GPIOG_REG_RESET()   do \
                            { \
	                          RCC->RCC_AHB1RSTR |= (0x1 << 6); \
							  RCC->RCC_AHB1RSTR &= ~(0x1 << 6); \
                            }while(0)

#define GPIOH_REG_RESET()   do \
                            { \
	                          RCC->RCC_AHB1RSTR |= (0x1 << 7); \
							  RCC->RCC_AHB1RSTR &= ~(0x1 << 7); \
                            }while(0)

#define GPIOI_REG_RESET()   do \
                            { \
	                          RCC->RCC_AHB1RSTR |= (0x1 << 8); \
							  RCC->RCC_AHB1RSTR &= ~(0x1 << 8); \
                            }while(0)


/** general defines **/

#define ENABLE  (1)
#define DISABLE (0)
#define SET     ENABLE
#define RESET   DISABLE


/** Table 62. Vector table for STM32F42xxx **/
#define IRQ_EXTI0           (6)
#define IRQ_EXTI1           (7)
#define IRQ_EXTI2           (8)
#define IRQ_EXTI3           (9)
#define IRQ_EXTI4           (10)
#define IRQ_EXTI9_5         (23)
#define IRQ_EXTI15_10       (40)

#define IRQ_SPI1            (35)
#define IRQ_SPI2            (36)
#define IRQ_SPI3            (51)

/** SPI Handling **/

/**Table 129. SPI register map **/
typedef struct
{
	_VO uint32_t SPI_CR1;    //     - address offset   0x00
	_VO uint32_t SPI_CR2;   // -  address offset   0x04
	_VO uint32_t SPI_SR;     //- address offset   0x08
	_VO uint32_t SPI_DR;    //- address offset   0x0C
	_VO uint32_t SPI_CRCPR;      //- address offset   0x10
	_VO uint32_t SPI_RXCRCR;      //- address offset   0x14
	_VO uint32_t SPI_TXCRCR;      //- address offset   0x18
	_VO uint32_t SPI_I2SCFGR;      //- address offset   0x1C
	_VO uint32_t SPI_I2SPR;      //- address offset   0x20
}SPI_RegDef_t;

#define SPI1   ((SPI_RegDef_t *)SPI1_BASEADDR)
#define SPI2   ((SPI_RegDef_t *)SPI2_BASEADDR)
#define SPI3   ((SPI_RegDef_t *)SPI3_BASEADDR)
#define SPI4   ((SPI_RegDef_t *)SPI4_BASEADDR)
#define SPI5   ((SPI_RegDef_t *)SPI5_BASEADDR)
#define SPI6   ((SPI_RegDef_t *)SPI6_BASEADDR)



#endif /* STM32F429X_H_ */
