/*
 * stm32f429x_usart_driver.c
 *
 *  Created on: 8/10/2020
 *      Author: sjoshi
 */

#include "stm32f429x_usart_driver.h"
#include "stm32f429x_rcc_driver.h"

void USART_PeriClockControl(USART_RegDef_t *pUsart, uint8_t EnOrDi)
{

	if( EnOrDi == ENABLE)
	{
		if(pUsart == USART1)
		{
			USART1_PCLK_EN();
		}
		else if(pUsart == USART2)
		{
			USART2_PCLK_EN();
		}
		else if(pUsart == USART3)
		{
			USART3_PCLK_EN();
		}
		else if(pUsart == USART6)
		{
			USART6_PCLK_EN();
		}
	}
	else
	{
		if(pUsart == USART1)
		{
			USART1_PCLK_DI();
		}
		else if(pUsart == USART2)
		{
			USART2_PCLK_DI();
		}
		else if(pUsart == USART3)
		{
			USART3_PCLK_DI();
		}
		else if(pUsart == USART6)
		{
			USART6_PCLK_DI();
		}
	}
}

void USART_Init(USART_Handle_t *pUSARTHandle)
{
	uint32_t tempReg = 0;

	if(pUSARTHandle->usart_Config.usart_mode  == USART_MODE_ONLY_TX)
	{
		tempReg = (0x1 << USART_CR1_TE);
	}
	else if(pUSARTHandle->usart_Config.usart_mode  == USART_MODE_ONLY_RX)
	{
		tempReg = (0x1 << USART_CR1_RE);
	}
	else if(pUSARTHandle->usart_Config.usart_mode  == USART_MODE_BOTH_TX_RX)
	{
		tempReg = (0x1 << USART_CR1_RE);
		tempReg |= (0x1 << USART_CR1_TE);
	}

	//word bits - 8 bits or 9 bits -  0 and 1 respectively
	tempReg |= pUSARTHandle->usart_Config.usart_wordLength << USART_CR1_M;

	//Parity bit

	if(pUSARTHandle->usart_Config.usart_parityControl == USART_PARITY_ENABLE_ODD)
	{
		tempReg |= (0x1 << USART_CR1_PCE); //enable parity selection
		tempReg |= (0x1 << USART_CR1_PS); //odd parity is 1.
	}
	else if(pUSARTHandle->usart_Config.usart_parityControl == USART_PARITY_ENABLE_EVEN)
	{
		tempReg |= (0x1 << USART_CR1_PCE); //enable parity selection
		tempReg &= ~(0x1 << USART_CR1_PS); //Even parity is 0.
	}
	else if(pUSARTHandle->usart_Config.usart_parityControl == USART_PARITY_DISABLE)
	{
		tempReg &= ~(0x1 << USART_CR1_PCE); //disable parity selection
	}

	pUSARTHandle->pUSARTx->USART_CR1 = tempReg;


	/**** CR2 Configuration *******/
	tempReg = 0;
	tempReg |= pUSARTHandle->usart_Config.usart_number_of_stopBits << USART_CR2_STOPBITS;
	pUSARTHandle->pUSARTx->USART_CR2 = tempReg;

	/**** CR3 Configuration ****/
	tempReg = 0;
	if(pUSARTHandle->usart_Config.usart_hwFlowControl == USART_HW_CONTROL_CTS)
	{
		tempReg |=  (0x1 << USART_CR3_CTSE);
	}
	else if(pUSARTHandle->usart_Config.usart_hwFlowControl == USART_HW_CONTROL_CTS)
	{
		tempReg |=  (0x1 << USART_CR3_RTSE);
	}
	else if(pUSARTHandle->usart_Config.usart_hwFlowControl == USART_HW_CONTROL_CTS_RTS)
	{
		tempReg |=  ((0x1 << USART_CR3_RTSE) | (0x1 << USART_CR3_CTSE));
	}

	pUSARTHandle->pUSARTx->USART_CR3 = tempReg;

	/**BRR - Baud Rate Configuration **/
	USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->usart_Config.usart_baudRate);

}

void USART_IRQInterruptConfig(uint8_t irqNumber, uint8_t EnorDi)
{
	uint8_t bit_pos = 0;
	bit_pos = irqNumber % 32;
	volatile uint32_t *nvic_reg;

	if(EnorDi == ENABLE)
	{
		if(irqNumber >= 0 && irqNumber < 32)
		{
			nvic_reg = (uint32_t *)NVIC_ISER0;
			*nvic_reg  |= (0x1U << bit_pos);
		}
		else if(irqNumber >= 32 && irqNumber < 64)
		{
			nvic_reg = (uint32_t *)NVIC_ISER1;
			*nvic_reg  |= (0x1U << bit_pos);

		}
		else if(irqNumber >= 64 && irqNumber < 96)
		{
			nvic_reg = (uint32_t *)NVIC_ISER2;
			*nvic_reg  |= (0x1U << bit_pos);
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
			*nvic_reg  |= (0x1U << bit_pos);
		}
		else if(irqNumber >= 32 && irqNumber < 64)
		{
			nvic_reg = (uint32_t *)NVIC_ICER1;
			*nvic_reg  |= (0x1U << bit_pos);
		}
		else if(irqNumber >= 64 && irqNumber < 96)
		{
			nvic_reg = (uint32_t *)NVIC_ICER2;
			*nvic_reg  |= (0x1U << bit_pos);
		}
		else
		{
			//probably shouldn't reach here.
		}
	}
}

void USART_IRQPriorityConfig(uint8_t irqNumber, uint16_t priority)
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

void USART_PeriControl(USART_RegDef_t *pUsart, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pUsart->USART_CR1 |= (0x1 << USART_CR1_UE);
	}
	else
	{
		pUsart->USART_CR1 &= ~(0x1 << USART_CR1_UE);
	}
}


void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t len)
{
   uint16_t *pData;

   for(uint32_t i = 0; i < len; i++)
   {
	   while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_SR_TXE));

	   if(pUSARTHandle->usart_Config.usart_wordLength == USART_WORD_LEN_9_BITS)
	   {
		   pData = (uint16_t*)pTxBuffer;
		   pUSARTHandle->pUSARTx->USART_DR = (*pData & (uint16_t)0x01FF);

		   if(pUSARTHandle->usart_Config.usart_parityControl == USART_PARITY_DISABLE)
		   {
			   //No parity is used in this transfer. so, 9bits of user data will be sent
			   //Implement the code to increment pTxBuffer twice
			   pTxBuffer++;
			   pTxBuffer++;
		   }
		   else
		   {
			   //Parity bit is used in this transfer . so , 8bits of user data will be sent
			   //The 9th bit will be replaced by parity bit by the hardware
			   pTxBuffer++;
		   }
	   }
	   // Word Len 8 bits
	   else
	   {
		   pUSARTHandle->pUSARTx->USART_DR = (*pTxBuffer & (uint8_t)0xFF);
		   pTxBuffer++;
	   }
   }
   //Implement the code to wait till TC flag is set in the SR
   while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_SR_TC));

}
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len)
{
    for(uint32_t i = 0; i < len; i++)
    {
       //wait until RXNE flag is set in the SR
 	   while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_SR_RXNE));

	   if(pUSARTHandle->usart_Config.usart_wordLength == USART_WORD_LEN_9_BITS)
	   {
		   //We are going to receive 9bit data in a frame

		   //check are we using USART_ParityControl control or not

		   if(pUSARTHandle->usart_Config.usart_parityControl == USART_PARITY_DISABLE)
		   {
			   *((uint16_t *)pRxBuffer) = (pUSARTHandle->pUSARTx->USART_DR & (uint16_t)0x01FF);
                pRxBuffer++;
                pRxBuffer++;
		   }
		   else
		   {
			   //Parity is used, so, 8bits will be of user data and 1 bit is parity
			   *pRxBuffer = (pUSARTHandle->pUSARTx->USART_DR & (uint8_t)0xFF);
			   pRxBuffer++;
		   }
	   }
	   else
	   {
		   //We are going to receive 8bit data in a frame
		   //check are we using USART_ParityControl control or not
		   if(pUSARTHandle->usart_Config.usart_parityControl == USART_PARITY_DISABLE)
		   {
			   //No parity is used , so all 8bits will be of user data

			   //read 8 bits from DR
			   *pRxBuffer = (pUSARTHandle->pUSARTx->USART_DR & (uint8_t)0xFF);
		   }
		   else
		   {
			   //Parity is used, so , 7 bits will be of user data and 1 bit is parity
			   //read only 7 bits , hence mask the DR with 0X7F
			   *pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->USART_DR & (uint8_t)0x7F);
		   }
		   pRxBuffer++;
	   }
    }
}

uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t len)
{
	uint8_t txstate = pUSARTHandle->txState;

	if(txstate != USART_BUSY_IN_TX)
	{
		pUSARTHandle->txlen = len;
		pUSARTHandle->pTxBuffer = pTxBuffer;
		pUSARTHandle->txState = USART_BUSY_IN_TX;

		//Implement the code to enable interrupt for TXE
		pUSARTHandle->pUSARTx->USART_CR1 |= (0x1 << USART_CR1_TXEIE);

		//Implement the code to enable interrupt for TC
		pUSARTHandle->pUSARTx->USART_CR1 |= (0x1 << USART_CR1_TCIE);
	}

	return txstate;

}


uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t len)
{
	uint8_t rxstate = pUSARTHandle->rxState;

	if(rxstate != USART_BUSY_IN_RX)
	{
		pUSARTHandle->rxlen = len;
		pUSARTHandle->pRxBuffer = pRxBuffer;
		pUSARTHandle->rxState = USART_BUSY_IN_RX;

		//Implement the code to enable interrupt for RXNE
		pUSARTHandle->pUSARTx->USART_CR1 |= (0x1 << USART_CR1_RXNEIE);
	}

	return rxstate;

}

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUsart, uint8_t flag)
{
	if(pUsart->USART_SR & (0x1 << flag))
	{
		return 1;
	}

	return 0;
}

void USART_SetBaudRate(USART_RegDef_t *pUsart, uint32_t baudRate)
{

	//Variable to hold the APB clock
	uint32_t PCLKx;

	uint32_t usartdiv;

	//variables to hold Mantissa and Fraction values
	uint32_t M_part,F_part;

  uint32_t tempreg=0;

  //Get the value of APB bus clock in to the variable PCLKx
  if(pUsart == USART1 || pUsart == USART6)
  {
	   //USART1 and USART6 are hanging on APB2 bus
	   PCLKx = RCC_GetAPB2_PClkValue();
  }else
  {
	   PCLKx = RCC_GetAPB1_PClkValue();
  }

  //Check for OVER8 configuration bit
  if(pUsart->USART_CR1 & (1 << USART_CR1_OVER8))
  {
	   //OVER8 = 1 , over sampling by 8
	   usartdiv = ((25 * PCLKx) / (2 *baudRate));
  }else
  {
	   //over sampling by 16
	  usartdiv = ((25 * PCLKx) / (4 *baudRate));
  }

  //Calculate the Mantissa part
  M_part = usartdiv/100;

  //Place the Mantissa part in appropriate bit position . refer USART_BRR
  tempreg |= M_part << 4;

  //Extract the fraction part
  F_part = (usartdiv - (M_part * 100));

  //Calculate the final fractional
  if(pUsart->USART_CR1 & ( 1 << USART_CR1_OVER8))
   {
	  //OVER8 = 1 , over sampling by 8
	  F_part = ((( F_part * 8)+ 50) / 100)& ((uint8_t)0x07);

   }else
   {
	   //over sampling by 16
	   F_part = ((( F_part * 16)+ 50) / 100) & ((uint8_t)0x0F);

   }

  //Place the fractional part in appropriate bit position . refer USART_BRR
  tempreg |= F_part;

  //copy the value of tempreg in to BRR register
  pUsart->USART_BRR = tempreg;
}


void USART_IRQHandling(USART_Handle_t *pUSARTHandle)
{

	uint32_t temp1 , temp2;
	uint16_t *pdata;

/*************************Check for TC flag ********************************************/

    //Implement the code to check the state of TC bit in the SR
	temp1 = pUSARTHandle->pUSARTx->USART_SR & ( 1 << USART_SR_TC);

	 //Implement the code to check the state of TCEIE bit
	temp2 = pUSARTHandle->pUSARTx->USART_CR1 & ( 1 << USART_CR1_TCIE);

	if(temp1 && temp2 )
	{
		//this interrupt is because of TC

		//close transmission and call application callback if TxLen is zero
		if ( pUSARTHandle->txState == USART_BUSY_IN_TX)
		{
			//Check the TxLen . If it is zero then close the data transmission
			if(! pUSARTHandle->txlen )
			{
				//Implement the code to clear the TC flag
				pUSARTHandle->pUSARTx->USART_SR &= ~( 1 << USART_SR_TC);

				//Implement the code to clear the TCIE control bit
				pUSARTHandle->pUSARTx->USART_CR1 &= ~(1 << USART_CR1_TCIE);

				//Reset the application state
				pUSARTHandle->txState = USART_READY;

				//Reset Buffer address to NULL
				pUSARTHandle->pTxBuffer = NULL;

				//Reset the length to zero
				pUSARTHandle->txlen = 0;

				//Call the applicaton call back with event USART_EVENT_TX_CMPLT
				USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_TX_CMPLT);
			}
		}
	}

/*************************Check for TXE flag ********************************************/

	//Implement the code to check the state of TXE bit in the SR
	temp1 = pUSARTHandle->pUSARTx->USART_SR & ( 1 << USART_SR_TXE);

	//Implement the code to check the state of TXEIE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->USART_CR1 & ( 1 << USART_CR1_TXEIE);


	if(temp1 && temp2 )
	{
		//this interrupt is because of TXE

		if(pUSARTHandle->txState == USART_BUSY_IN_TX)
		{
			//Keep sending data until Txlen reaches to zero
			if(pUSARTHandle->txlen > 0)
			{
				//Check the USART_WordLength item for 9BIT or 8BIT in a frame
				if(pUSARTHandle->usart_Config.usart_wordLength == USART_WORD_LEN_9_BITS)
				{
					//if 9BIT , load the DR with 2bytes masking the bits other than first 9 bits
					pdata = (uint16_t*)pUSARTHandle->pTxBuffer;

					//loading only first 9 bits , so we have to mask with the value 0x01FF
					pUSARTHandle->pUSARTx->USART_DR = (*pdata & (uint16_t)0x01FF);

					//check for USART_ParityControl
					if(pUSARTHandle->usart_Config.usart_parityControl == USART_PARITY_DISABLE)
					{
						//No parity is used in this transfer , so, 9bits of user data will be sent
						//Implement the code to increment pTxBuffer twice
						pUSARTHandle->pTxBuffer++;
						pUSARTHandle->pTxBuffer++;

						//Implement the code to decrement the length
						pUSARTHandle->txlen--;
						pUSARTHandle->txlen--;
					}
					else
					{
						//Parity bit is used in this transfer . so , 8bits of user data will be sent
						//The 9th bit will be replaced by parity bit by the hardware
						pUSARTHandle->pTxBuffer++;

						//Implement the code to decrement the length
						pUSARTHandle->txlen--;
					}
				}
				else
				{
					//This is 8bit data transfer
					pUSARTHandle->pUSARTx->USART_DR = (*pUSARTHandle->pTxBuffer  & (uint8_t)0xFF);

					//Implement the code to increment the buffer address
					pUSARTHandle->pTxBuffer++;

					//Implement the code to decrement the length
					pUSARTHandle->txlen--;
				}

			}
			if (pUSARTHandle->txlen == 0 )
			{
				//TxLen is zero
				//Implement the code to clear the TXEIE bit (disable interrupt for TXE flag )
				pUSARTHandle->pUSARTx->USART_CR1 &= ~( 1 << USART_CR1_TXEIE);
			}
		}
	}

/*************************Check for RXNE flag ********************************************/

	temp1 = pUSARTHandle->pUSARTx->USART_SR & ( 1 << USART_SR_RXNE);
	temp2 = pUSARTHandle->pUSARTx->USART_CR1 & ( 1 << USART_CR1_RXNEIE);


	if(temp1 && temp2 )
	{
		//this interrupt is because of rxne
		//this interrupt is because of txe
		if(pUSARTHandle->rxState == USART_BUSY_IN_RX)
		{
			//TXE is set so send data
			if(pUSARTHandle->rxlen > 0)
			{
				//Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
				if(pUSARTHandle->usart_Config.usart_wordLength == USART_WORD_LEN_9_BITS)
				{
					//We are going to receive 9bit data in a frame

					//Now, check are we using USART_ParityControl control or not
					if(pUSARTHandle->usart_Config.usart_parityControl == USART_PARITY_DISABLE)
					{
						//No parity is used. so, all 9bits will be of user data

						//read only first 9 bits so mask the DR with 0x01FF
						*((uint16_t*) pUSARTHandle->pRxBuffer) = (pUSARTHandle->pUSARTx->USART_DR  & (uint16_t)0x01FF);

						//Now increment the pRxBuffer two times
						pUSARTHandle->pRxBuffer++;
						pUSARTHandle->pRxBuffer++;

						//Implement the code to decrement the length
						pUSARTHandle->rxlen--;
						pUSARTHandle->rxlen--;
					}
					else
					{
						//Parity is used. so, 8bits will be of user data and 1 bit is parity
						 *pUSARTHandle->pRxBuffer = (pUSARTHandle->pUSARTx->USART_DR  & (uint8_t)0xFF);

						 //Now increment the pRxBuffer
						 pUSARTHandle->pRxBuffer++;

						 //Implement the code to decrement the length
						 pUSARTHandle->rxlen--;
					}
				}
				else
				{
					//We are going to receive 8bit data in a frame

					//Now, check are we using USART_ParityControl control or not
					if(pUSARTHandle->usart_Config.usart_parityControl == USART_PARITY_DISABLE)
					{
						//No parity is used , so all 8bits will be of user data

						//read 8 bits from DR
						 *pUSARTHandle->pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->USART_DR  & (uint8_t)0xFF);
					}

					else
					{
						//Parity is used, so , 7 bits will be of user data and 1 bit is parity

						//read only 7 bits , hence mask the DR with 0X7F
						 *pUSARTHandle->pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->USART_DR  & (uint8_t)0x7F);

					}

					//Now , increment the pRxBuffer
					pUSARTHandle->pRxBuffer++;

					//Implement the code to decrement the length
					pUSARTHandle->rxlen--;
				}


			}//if of >0

			if(! pUSARTHandle->rxlen)
			{
				//disable the rxne
				pUSARTHandle->pUSARTx->USART_CR1 &= ~( 1 << USART_CR1_RXNEIE );
				pUSARTHandle->rxState = USART_READY;
				USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_RX_CMPLT);
			}
		}
	}
}
