/*
 * uart.c
 *
 *  Created on: Sep 7, 2025
 *      Author: gegio
 */

#include "uart.h"

uint8_t UART_Enable_Clock(USART_TypeDef *uart_instance)
{

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
