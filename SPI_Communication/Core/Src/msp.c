/*
 * msp.c
 *
 *  Created on: Apr 15, 2025
 *      Author: gigio
 */

#include <string.h>
#include "stm32f4xx_hal.h"

void HAL_MspInit(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// Enable necessary IRQs
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(SysTick_IRQn, 5, 0);

	HAL_NVIC_EnableIRQ(MemoryManagement_IRQn);
	HAL_NVIC_EnableIRQ(BusFault_IRQn);
	HAL_NVIC_EnableIRQ(UsageFault_IRQn);
	HAL_NVIC_EnableIRQ(SysTick_IRQn);

}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	UNUSED(huart);

	GPIO_InitTypeDef usart2_gpio;
	memset(&usart2_gpio, 0, sizeof(usart2_gpio));

	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	usart2_gpio.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	usart2_gpio.Speed = GPIO_SPEED_FREQ_LOW;
	usart2_gpio.Pull = GPIO_PULLUP;
	usart2_gpio.Mode = GPIO_MODE_AF_PP;
	usart2_gpio.Alternate = GPIO_AF7_USART2;

	HAL_GPIO_Init(GPIOA, &usart2_gpio);

	HAL_NVIC_SetPriority(USART2_IRQn, 9, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);

}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	UNUSED(hspi);

	GPIO_InitTypeDef spi2_gpio;
	memset(&spi2_gpio, 0, sizeof(spi2_gpio));

	__HAL_RCC_SPI2_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	spi2_gpio.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	spi2_gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	spi2_gpio.Pull = GPIO_NOPULL;
	spi2_gpio.Mode = GPIO_MODE_AF_PP;
	spi2_gpio.Alternate = GPIO_AF5_SPI2;

	HAL_GPIO_Init(GPIOB, &spi2_gpio);

	HAL_NVIC_SetPriority(SPI2_IRQn, 9, 0);
	HAL_NVIC_EnableIRQ(SPI2_IRQn);

}
