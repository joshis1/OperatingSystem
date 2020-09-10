/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FLASH_SECTOR2_BASE_ADDRESS (0x08008000U)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

ETH_HandleTypeDef heth;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
volatile uint8_t button_press = 0;
uint8_t bl_rx_buffer[256];

uint8_t supported_commands[] = {
                               BL_GET_VER ,
                               BL_GET_HELP,
                               BL_GET_CID,
                               BL_GET_RDP_STATUS,
                               BL_GO_TO_ADDR,
                               BL_FLASH_ERASE,
                               BL_MEM_WRITE,
                               BL_READ_SECTOR_P_STATUS};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */
void bootloader_uart_read_data();
void bootloader_jump_to_user_app();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void printmsg(char *format,...)
 {
	char str[80];

	/*Extract the the argument list using VA apis */
	va_list args;
	va_start(args, format);
	vsprintf(str, format,args);
	HAL_UART_Transmit(&huart3,(uint8_t *)str, strlen(str),HAL_MAX_DELAY);
	va_end(args);
 }
 
 uint8_t bootloader_verify_crc(uint8_t *pData, uint8_t len, uint32_t host_crc)
 {
	 uint32_t computedCRCValue = 0xff;
	 for (uint32_t i = 0; i < len; i++)
	 {
		 uint32_t data = pData[i];
		 computedCRCValue = HAL_CRC_Accumulate(&hcrc, &data, 1);
	 }
	 
	 __HAL_CRC_DR_RESET(&hcrc);
	 
	 if(computedCRCValue == host_crc)
	 {
		 return 0;  //success
	 }
	 return 1; // failure
 }
 
 uint8_t verify_go_address(uint32_t go_address)
 {
	  if( go_address >= SRAM1_BASE && go_address <= SRAM1_END)
		{
			return 0;
		}
		else  if( go_address >= SRAM2_BASE && go_address <= SRAM2_END)
		{
			return 0;
		}
		else if( go_address >= SRAM3_BASE && go_address <= SRAM3_END)
	  {
			return 0;
		}
		else if( go_address >= FLASH_BASE && go_address <= FLASH_END)
    {
      return 0;
    }	
    else if (go_address >= SYSTEM_MEMORY_START && go_address <= SYSTEM_MEMORY_END)
		{
      return 0;
    }			
		else
		{
			return 1;
		}
	 return 1;
 }
 
 /*This function sends NACK */
void bootloader_send_nack(void)
{
	uint8_t nack = BL_NACK;
	HAL_UART_Transmit(&huart3,&nack,1,HAL_MAX_DELAY);
}
 
void bootloader_send_ack(uint8_t command_code, uint8_t follow_len)
{
   uint8_t ack_buf[2];
   ack_buf[0] = BL_ACK;
   ack_buf[1] = follow_len;
	
	HAL_UART_Transmit(&huart3,ack_buf,2,HAL_MAX_DELAY);
    	
}

/* This function writes data in to C_UART */
void bootloader_uart_write_data(uint8_t *pBuffer,uint32_t len)
{
    /*you can replace the below ST's USART driver API call with your MCUs driver API call */
	HAL_UART_Transmit(&huart3,pBuffer,len,HAL_MAX_DELAY);

}

/** bootloader get version **/
void bootloader_handle_getver_cmd(uint8_t *bl_rx_buffer)
{
	uint32_t command_len = 0;
	uint32_t host_crc = 0;
	command_len = bl_rx_buffer[0] + 1;
	uint8_t bl_version;
	
	host_crc = *(uint32_t *) (bl_rx_buffer + command_len - 4);
	//bl_len is the length to follow so bl_len will point to the end of the packet.
	// we know the last 4 bytes are CRC so we should 4 i.e. 4th last element is the base address of CRC.
	if (bootloader_verify_crc(&bl_rx_buffer[0], command_len - 4, host_crc ) == 0)
	{
		// success i.e. return is 0.
		bootloader_send_ack(bl_rx_buffer[0], 1);
		bl_version = 0x10;
		bootloader_uart_write_data(&bl_version, 1);
		
	}
	else
	{
		 //failure. i.e. return is 1.
		bootloader_send_nack();
	}
	
}

/** bootloader get help - supported commands **/
void bootloader_handle_gethelp_cmd(uint8_t *pBuffer)
{
	uint32_t command_len = 0;
	uint32_t host_crc = 0;
	command_len = bl_rx_buffer[0] + 1;
	uint8_t bl_version;
	
	host_crc = *(uint32_t *) (bl_rx_buffer + command_len - 4);
	//bl_len is the length to follow so bl_len will point to the end of the packet.
	// we know the last 4 bytes are CRC so we should 4 i.e. 4th last element is the base address of CRC.
	if (bootloader_verify_crc(&bl_rx_buffer[0], command_len - 4, host_crc ) == 0)
	{
		// success i.e. return is 0.
		bootloader_send_ack(bl_rx_buffer[0], sizeof(supported_commands));
		bootloader_uart_write_data(supported_commands, sizeof(supported_commands));
	}
	else
	{
		 //failure. i.e. return is 1.
		bootloader_send_nack();
	}
}

/** bootloader get chip identification number **/
/** CID - Chip identification number 2 bytes **/
void bootloader_handle_getcid_cmd(uint8_t *pBuffer)
{
	/**MCU ID Code **/
	/** DBGMCU_IDCODE - register - 0xE004 2000  **/
	/** DEV_ID[11:0] **/
	/* 0x419 **/
	/** Reference manual page 1745 **/
	volatile uint32_t *pDbgMCU = (uint32_t *)0xE0042000U;
	uint16_t mcu_id = *pDbgMCU & ( 0x0FFF);
	uint32_t command_len = 0;
	uint32_t host_crc = 0;
	command_len = bl_rx_buffer[0] + 1;
	
	host_crc = *(uint32_t *) (bl_rx_buffer + command_len - 4);
	//bl_len is the length to follow so bl_len will point to the end of the packet.
	// we know the last 4 bytes are CRC so we should 4 i.e. 4th last element is the base address of CRC.
	if (bootloader_verify_crc(&bl_rx_buffer[0], command_len - 4, host_crc ) == 0)
	{
		// success i.e. return is 0.
		// 2 bytes MCU Id 
		bootloader_send_ack(bl_rx_buffer[0], 2);
		bootloader_uart_write_data((uint8_t *)&mcu_id, 2);
	}
	else
	{
		 //failure. i.e. return is 1.
		bootloader_send_nack();
	}
	
	
	
}

/** Flash Read protection status **/
void bootloader_handle_getrdp_cmd(uint8_t *pBuffer)
{
	// option bytes - Reference Manual - Table 16 - Bits 15:8 
	volatile uint32_t *pOptionBytes = (uint32_t *)0x1FFFC000U;
  uint8_t protection_level = (*pOptionBytes) >> 8;
	uint32_t command_len = 0;
	uint32_t host_crc = 0;
	command_len = pBuffer[0] + 1;
	
	host_crc = *(uint32_t *) (pBuffer + command_len - 4);
	//bl_len is the length to follow so bl_len will point to the end of the packet.
	// we know the last 4 bytes are CRC so we should 4 i.e. 4th last element is the base address of CRC.
	if (bootloader_verify_crc(pBuffer, command_len - 4, host_crc ) == 0)
	{
		// success i.e. return is 0.
		// 2 bytes MCU Id 
		bootloader_send_ack(*pBuffer, 1);
		/* 0xAA means no protection level */
		bootloader_uart_write_data((uint8_t *)&protection_level, 1);
	}
	else
	{
		 //failure. i.e. return is 1.
		bootloader_send_nack();
	}
}

void bootloader_handle_go_cmd(uint8_t *pBuffer)
{
  uint32_t go_address;
	uint32_t command_len = 0;
	uint32_t host_crc = 0;
	command_len = pBuffer[0] + 1;
	uint8_t valid = 0;
	uint8_t invalid = 1;
	
	host_crc = *(uint32_t *) (pBuffer + command_len - 4);
	//bl_len is the length to follow so bl_len will point to the end of the packet.
	// we know the last 4 bytes are CRC so we should 4 i.e. 4th last element is the base address of CRC.
	if (bootloader_verify_crc(pBuffer, command_len - 4, host_crc ) == 0)
	{
		bootloader_send_ack(*pBuffer, 1);
		// success i.e. return is 0.
		//4 byte base address - length to follow ( 1byte)->Command Code ( 1 byte) -> Memory Address ( 4 bytes) -> CRC ( 4 bytes).
		go_address = *((uint32_t *)&pBuffer[2]);
		if( verify_go_address(go_address) == 0) // success 
		{ 
			// valid is 0 
			bootloader_uart_write_data(&valid,1);
			go_address +=1; //Make T-bit to 1.
			void (*lets_jump)() = (void *)go_address;
			lets_jump();
		}
		else
		{
			bootloader_uart_write_data(&invalid,1);		
		}
	}
	else
	{
		 //failure. i.e. return is 1.
		bootloader_send_nack();
	}
}

uint8_t execute_flash_erase(uint8_t sector_number, uint8_t number_of_sectors)
{
	HAL_StatusTypeDef status;
	uint32_t SectorError;
	FLASH_EraseInitTypeDef flash;
	
	if( sector_number == 0xff) //mass erase.
	{
		flash.TypeErase = FLASH_TYPEERASE_MASSERASE;
	}
	else // sector erase.
	{
		flash.TypeErase = FLASH_TYPEERASE_SECTORS;
		flash.Sector = sector_number;  // initial sector.
		flash.NbSectors = number_of_sectors;
	}
	flash.Banks = FLASH_BANK_1;
	flash.VoltageRange = FLASH_VOLTAGE_RANGE_3; //2.7 to 3.6V
	HAL_FLASH_Unlock(); // unlock the flash so that it can be erased.
	status = HAL_FLASHEx_Erase(&flash, &SectorError);
	HAL_FLASH_Lock();  // lock the flash back.
	return status;
}

//length to follow ( 1 byte) ->Command Code  ( 1 byte) -> Sector Number  ( 1 byte) -> Number of sectors  ( 1 byte) -> CRC ( 4 bytes)
void bootloader_handle_flash_erase_cmd(uint8_t *pBuffer)
{
	// if sector number is 0xff then perform mass erase.
	uint32_t command_len = 0;
	uint32_t host_crc = 0;
	command_len = pBuffer[0] + 1;
	uint8_t status;
	
	host_crc = *(uint32_t *) (pBuffer + command_len - 4);
	//bl_len is the length to follow so bl_len will point to the end of the packet.
	// we know the last 4 bytes are CRC so we should 4 i.e. 4th last element is the base address of CRC.
	if (bootloader_verify_crc(pBuffer, command_len - 4, host_crc ) == 0)
	{
		// success i.e. return is 0.
		bootloader_send_ack(*pBuffer, 1);
		//PB7 - Blue LED - Turn on - while the Sector is getting erased.
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		status = execute_flash_erase(pBuffer[2], pBuffer[3]);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		bootloader_uart_write_data((uint8_t *)&status, 1);
	}
	else
	{
		 //failure. i.e. return is 1.
		bootloader_send_nack();
	}
	
	
}

uint8_t execute_mem_write(uint8_t *pBuffer, uint8_t size, uint32_t mem_address)
{
  uint8_t status;
	HAL_FLASH_Unlock();
	
	for ( uint32_t i = 0; i < size; i++)
	{
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,mem_address+i,pBuffer[i]);
	}
	
	HAL_FLASH_Lock();
	
	return status;
}
 
/** length to follow ( 1 byte) -> Command Code ( 1 byte) -> Base Memory address ( 4 bytes) -> Payload Length ( 1 byte) -> Payload ( x bytes) - > CRC. */
void bootloader_handle_mem_write_cmd(uint8_t *pBuffer)
{
	
	uint32_t command_len = 0;
	uint32_t host_crc = 0;
	command_len = pBuffer[0] + 1;
	uint8_t status;
	uint32_t mem_address;
	uint8_t payload_len; 
	uint8_t *payloadBuffer;
	
	host_crc = *(uint32_t *) (pBuffer + command_len - 4);
	//bl_len is the length to follow so bl_len will point to the end of the packet.
	// we know the last 4 bytes are CRC so we should 4 i.e. 4th last element is the base address of CRC.
	if (bootloader_verify_crc(pBuffer, command_len - 4, host_crc ) == 0)
	{
		bootloader_send_ack(*pBuffer, 1);
		mem_address =  *((uint32_t *)&pBuffer[2]);
		payload_len = pBuffer[6];
		payloadBuffer = &pBuffer[7];
		
		status = execute_mem_write(payloadBuffer,payload_len, mem_address);
		bootloader_uart_write_data((uint8_t *)&status, 1);
	}
	else
	{
		
		 //failure. i.e. return is 1.
		bootloader_send_nack();
	}
}

uint8_t execute_enable_disable_protection_flash(uint8_t sector_details, uint8_t protection_mode, uint8_t disable)
{
	uint8_t status = 0;
	//Check the page - Table 1. STM32F4xx register boundary addresses
	// base address is 0x40023C00 + flash OPTCTR 0x14 Page - 108.
	volatile uint32_t *pOPTCR = (uint32_t *) (0x40023C00 + 0x14); 
	
	HAL_FLASH_OB_Unlock();
	// wait until the flash isn't busy.
	while ( __HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);
	
   if(disable)
	 {
		 // disable rw protection - change to default value 
		 *pOPTCR &= ~(0x1U << 31);  //SPRMOD -- 0
		 // bits 16 to 27 i.e. 11 bits make it to 1.
		 *pOPTCR |= (0XFF7 << 16);
	 }
	 else
	 {
		 
		 if(protection_mode == 1) // write protection
		 {
			  
			 // Bit 31 should be 0 i.e not enabling PCROP - Page 108 - SPRMOD 
			 *pOPTCR &= ~(0x1U << 31);
			 
			 // bit 16 to 27 for setting up sectors that need to be protected.
			 // 0 to make it write protective.
			 *pOPTCR &= ~(sector_details << 16);
		 }
		 else // Read write protection.
		 {
			  //Bit 31 - should be 1 
			  *pOPTCR |= ( 0x1U << 31);
			  // bit 16 to 27 for setting up sectors that need to be protected.
			  *pOPTCR |= (sector_details << 16);		 
		 }
	 }
	 //set the option Start bit.
	 *pOPTCR |= (0x1U << 1);
	 while ( __HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY) != RESET);
	 HAL_FLASH_OB_Lock();
	 
	return status;
}

/* length to follow (1byte)-> Command ( 1byte) ->  sector details (1 byte) -> protection mode ( 1 byte) -> CRC ( 4 bytes) */
void bootloader_handle_en_rw_protect(uint8_t *pBuffer)
{
	uint32_t command_len = 0;
	uint32_t host_crc = 0;
	command_len = pBuffer[0] + 1;
	uint8_t status;
  uint8_t sector_details;
	uint8_t protection_mode;
	
	host_crc = *(uint32_t *) (pBuffer + command_len - 4);
	//bl_len is the length to follow so bl_len will point to the end of the packet.
	// we know the last 4 bytes are CRC so we should 4 i.e. 4th last element is the base address of CRC.
	if (bootloader_verify_crc(pBuffer, command_len - 4, host_crc ) == 0)
	{
		bootloader_send_ack(*pBuffer, 1);
		sector_details = pBuffer[2];
		protection_mode = pBuffer[3];
		status = execute_enable_disable_protection_flash(sector_details,protection_mode, 0);
		bootloader_uart_write_data((uint8_t *)&status, 1);
	}
	else
	{
		 //failure. i.e. return is 1.
		bootloader_send_nack();
	}
	
}

void bootloader_handle_dis_rw_protect(uint8_t *pBuffer)
{
	uint32_t command_len = 0;
	uint32_t host_crc = 0;
	command_len = pBuffer[0] + 1;
	uint8_t status;
  uint8_t sector_details;
	uint8_t protection_mode;
	
	host_crc = *(uint32_t *) (pBuffer + command_len - 4);
	//bl_len is the length to follow so bl_len will point to the end of the packet.
	// we know the last 4 bytes are CRC so we should 4 i.e. 4th last element is the base address of CRC.
	if (bootloader_verify_crc(pBuffer, command_len - 4, host_crc ) == 0)
	{
		bootloader_send_ack(*pBuffer, 1);
		sector_details = pBuffer[2];
		protection_mode = pBuffer[3];
		status = execute_enable_disable_protection_flash(sector_details,protection_mode, 1);
		bootloader_uart_write_data((uint8_t *)&status, 1);
	}
	else
	{
		//failure. i.e. return is 1.
		bootloader_send_nack();
	}
	
}

void bootloader_handle_mem_read (uint8_t *pBuffer)
{


}

uint16_t execute_read_protection()
{
	FLASH_OBProgramInitTypeDef OBInit;
	// First unlock the OB( option byte) memory access
	HAL_FLASH_OB_Unlock();
	//get the OB configuration details
	HAL_FLASHEx_OBGetConfig(&OBInit);
	HAL_FLASH_OB_Lock();
	
	return (uint16_t)OBInit.WRPSector;
	
}

void bootloader_handle_read_sector_protection_status(uint8_t *pBuffer)
{
	uint32_t command_len = 0;
	uint32_t host_crc = 0;
	command_len = pBuffer[0] + 1;
	uint16_t status;
  uint8_t sector_details;
	uint8_t protection_mode;
	
	host_crc = *(uint32_t *) (pBuffer + command_len - 4);
	//bl_len is the length to follow so bl_len will point to the end of the packet.
	// we know the last 4 bytes are CRC so we should 4 i.e. 4th last element is the base address of CRC.
	if (bootloader_verify_crc(pBuffer, command_len - 4, host_crc ) == 0)
	{
		bootloader_send_ack(*pBuffer, 1);
		status = execute_read_protection();
		bootloader_uart_write_data((uint8_t *)&status, 2);
	}
	else
	{
		//failure. i.e. return is 1.
		bootloader_send_nack();
	}
}

void bootloader_handle_read_otp(uint8_t *pBuffer)
{


}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CRC_Init();
//  MX_ETH_Init();
  MX_USART3_UART_Init();
//  MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */
	//User code -- our code.
	if( button_press == 1)
	{
			//returns 0 if the button is not pressed.

		 bootloader_uart_read_data();
	}
	else
	{
		 bootloader_uart_read_data();
		 //bootloader_jump_to_user_app();
	}

  /* USER CODE END 2 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}


/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD1_Pin */
  GPIO_InitStruct.Pin = LD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD1_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void bootloader_uart_read_data()
{
	 uint8_t recv_len = 0;
	
	  while(1)
		{
			 memset(bl_rx_buffer,0,256);
			  //read 1 bytes i.e. length to follow.
			 HAL_UART_Receive(&huart3, bl_rx_buffer,1,HAL_MAX_DELAY);
			 recv_len = bl_rx_buffer[0];
			 // read all the remaining bytes of th command byte.
			// Command code -- and CRC ( 4 bytes)
			 HAL_UART_Receive(&huart3, &bl_rx_buffer[1], recv_len, HAL_MAX_DELAY);
			 switch(bl_rx_buffer[1])
			 {
				 case BL_GET_VER:
                bootloader_handle_getver_cmd(bl_rx_buffer);
                break;
            case BL_GET_HELP:
                bootloader_handle_gethelp_cmd(bl_rx_buffer);
                break;
            case BL_GET_CID:
                bootloader_handle_getcid_cmd(bl_rx_buffer);
                break;
            case BL_GET_RDP_STATUS:
                bootloader_handle_getrdp_cmd(bl_rx_buffer);
                break;
            case BL_GO_TO_ADDR:
                bootloader_handle_go_cmd(bl_rx_buffer);
                break;
            case BL_FLASH_ERASE:
                bootloader_handle_flash_erase_cmd(bl_rx_buffer);
                break;
            case BL_MEM_WRITE:
                bootloader_handle_mem_write_cmd(bl_rx_buffer);
                break;
            case BL_EN_RW_PROTECT:
                bootloader_handle_en_rw_protect(bl_rx_buffer);
                break;
            case BL_MEM_READ:
                bootloader_handle_mem_read(bl_rx_buffer);
                break;
            case BL_READ_SECTOR_P_STATUS:
                bootloader_handle_read_sector_protection_status(bl_rx_buffer);
                break;
            case BL_OTP_READ:
                bootloader_handle_read_otp(bl_rx_buffer);
                break;
						case BL_DIS_R_W_PROTECT:
                bootloader_handle_dis_rw_protect(bl_rx_buffer);
                break;
             default:
							 printmsg("Invalid options from host\r\n");
                break;
			 }
		}
}

void bootloader_jump_to_user_app()
{
	void (*app_reset_handler)(void);
	uint32_t msp_value;
  msp_value	= *(volatile uint32_t *) FLASH_SECTOR2_BASE_ADDRESS;
  __set_MSP(msp_value);
	uint32_t resethandler_address;
	resethandler_address = *(volatile uint32_t *) (FLASH_SECTOR2_BASE_ADDRESS + 4);
	app_reset_handler = (void*) resethandler_address;
	//jump to reset handler of the application.
	//Driver/CMSIS/system_stm32f4xx.c -  VECT_TAB_OFFSET  0x8000 
	// Jump to reset handler of the user application.
	app_reset_handler();
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
