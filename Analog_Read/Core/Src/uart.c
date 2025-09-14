/*
 * uart.c
 *
 *  Created on: Sep 7, 2025
 *      Author: gegio
 */

#include "uart.h"

uint8_t UART_GPIO_Init(void)
{

}

uint8_t UART_Enable_Clock(UART_Handler *uart)
{
	switch(uart->id)
	{
		case UART_1:
			__HAL_RCC_USART1_CLK_ENABLE();
			break;
		case UART_2:
			__HAL_RCC_USART2_CLK_ENABLE();
			break;
		case UART_3:
			__HAL_RCC_USART3_CLK_ENABLE();
			break;
		case UART_4:
			__HAL_RCC_UART4_CLK_ENABLE();
			break;
		case UART_5:
			__HAL_RCC_UART5_CLK_ENABLE();
			break;
		case UART_6:
			__HAL_RCC_USART6_CLK_ENABLE();
			break;
		default:
			UART_Error_Handler();
	}
}

uint8_t UART_Configure_BRR_Reg(UART_Handler *uart)
{
	uint32_t fPCLK = HAL_RCC_GetPCLK1Freq();
	uint32_t OVER8 = (uart->OverSampling >> 15) & 0xFFFFFFFF;
	uint32_t USARTDIV = fPCLK / (8 * (2 - OVER8) * (uart->BaudRate));

	uart->Instance->BRR = 0x00;
}

uint8_t UART_Init(UART_Handler *uart)
{

	// Initialize the PINs for UART communication.
	UART_GPIO_Init();

	// Enable clock for UART module.
	UART_Enable_Clock(uart->Instance);

	// Enable the UART module.
	uart->Instance->CR1 |= ((uint32_t)(01UL << 13));

	// Initialize UART module.
	uart->Instance->CR1 |= (uart->WordLength | uart->OverSampling | uart->Mode | uart->Parity );
	uart->Instance->CR2 |= uart->StopBits;
	uart->Instance->BRR |= uart->BaudRate;

}

void UART_Error_Handler(void)
{
	// When Error Occurs during UART Communication.
	for(;;);

}
