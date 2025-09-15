/*
 * uart.c
 *
 *  Created on: Sep 7, 2025
 *      Author: gegio
 */

#include "uart.h"

__weak UART_Execution_Status UART_GPIO_Init(void)
{
	/*
	 * Configure the GPIO pins for UART communication according to the Reference Manual.
	 * This function should by implemented by the user in his source file before running
	 * UART_Init function.
	 */
	for(;;);

	return Execution_Uart_Succesfull;
}

UART_Execution_Status UART_Enable_Clock(UART_Handler *uart)
{

	switch(uart->id)
	{
		case UART_ID_1:
			__HAL_RCC_USART1_CLK_ENABLE();
			break;
		case UART_ID_2:
			__HAL_RCC_USART2_CLK_ENABLE();
			break;
		case UART_ID_3:
			__HAL_RCC_USART3_CLK_ENABLE();
			break;
		case UART_ID_4:
			__HAL_RCC_UART4_CLK_ENABLE();
			break;
		case UART_ID_5:
			__HAL_RCC_UART5_CLK_ENABLE();
			break;
		case UART_ID_6:
			__HAL_RCC_USART6_CLK_ENABLE();
			break;
		default:
			UART_Error_Handler();
	}

	return Execution_Uart_Succesfull;
}

UART_Execution_Status UART_Enable_IRQs(UART_Handler *uart)
{

	switch(uart->id)
	{
		case UART_ID_1:
			HAL_NVIC_SetPriority(USART1_IRQn, 15, 0);
			HAL_NVIC_EnableIRQ(USART1_IRQn);
			break;
		case UART_ID_2:
			HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
			HAL_NVIC_EnableIRQ(USART2_IRQn);
			break;
		case UART_ID_3:
			HAL_NVIC_SetPriority(USART3_IRQn, 15, 0);
			HAL_NVIC_EnableIRQ(USART3_IRQn);
			break;
		case UART_ID_4:
			HAL_NVIC_SetPriority(UART4_IRQn, 15, 0);
			HAL_NVIC_EnableIRQ(UART4_IRQn);
			break;
		case UART_ID_5:
			HAL_NVIC_SetPriority(UART5_IRQn, 15, 0);
			HAL_NVIC_EnableIRQ(UART5_IRQn);
			break;
		case UART_ID_6:
			HAL_NVIC_SetPriority(USART6_IRQn, 15, 0);
			HAL_NVIC_EnableIRQ(USART6_IRQn);
			break;
		default:
			UART_Error_Handler();
	}

	return Execution_Uart_Succesfull;
}

UART_Execution_Status UART_Configure_BRR_Reg(UART_Handler *uart)
{
	uint32_t fPCLK = HAL_RCC_GetPCLK1Freq();
	uint32_t OVER8 = (uart->OverSampling >> 15) & 0xFFFFFFFF;
	float USARTDIV = fPCLK / (8 * (2 - OVER8) * (uart->BaudRate));

	uart->Instance->BRR |= 0x00;
}

UART_Execution_Status UART_Init(UART_Handler *uart)
{

	// Enable clock for UART module.
	UART_Enable_Clock(uart->Instance);

	// Initialize the PINs for UART communication.
	UART_GPIO_Init();

	// Enable IRQ.
	UART_Enable_IRQs(uart);

	// Enable the UART module.
	uart->Instance->CR1 |= ((uint32_t)(01UL << 13));

	// Initialize UART module.
	uart->Instance->CR1 |= (uart->WordLength | uart->OverSampling | uart->Mode | uart->Parity );
	uart->Instance->CR2 |= uart->StopBits;

	if(UART_Configure_BRR_Reg(uart) != Execution_Uart_Succesfull)
		return Execution_Uart_Failed;

	return Execution_Uart_Succesfull;
}

UART_Execution_Status UART_Transmit(UART_Handler *uart, uint8_t *pData, uint32_t sizeData)
{

}

UART_Execution_Status UART_Receive(UART_Handler *uart, uint8_t *pData, uint32_t sizeData)
{

}

void UART_Error_Handler(void)
{
	// When Error Occurs during UART Communication.
	for(;;);

}
