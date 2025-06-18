/*
 * it.c
 *
 *  Created on: Apr 15, 2025
 *      Author: gigio
 */

#include "it.h"

extern UART_HandleTypeDef usart2;
extern SPI_HandleTypeDef spi2;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&usart2);
}

void SPI2_IRQHandler(void)
{
	HAL_SPI_IRQHandler(&spi2);
}

void HardFault_Handler(void)
{
	// Halt execution, when HardFault error occurs.
	for(;;);
}

void MemManage_Handler(void)
{
	// Halt execution, when MemManage error occurs.
	for(;;);
}

void BusFault_Handler(void)
{
	// Halt execution, when BusFault error occurs.
	for(;;);
}

void UsageFault_Handler(void)
{
	// Halt execution, when UsageFault error occurs.
	for(;;);
}
