/*
 * msp.c
 *
 *  Created on: Sep 12, 2024
 *      Author: gegio
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
	HAL_NVIC_SetPriority(SysTick_IRQn, 3, 0);

	HAL_NVIC_EnableIRQ(MemoryManagement_IRQn);
	HAL_NVIC_EnableIRQ(BusFault_IRQn);
	HAL_NVIC_EnableIRQ(UsageFault_IRQn);
	HAL_NVIC_EnableIRQ(SysTick_IRQn);

	// Enable EXTI2 and EXTI3 for buttons.
	HAL_NVIC_SetPriority(EXTI2_IRQn, 15, 0);
	HAL_NVIC_SetPriority(EXTI3_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	UNUSED(htim);
	__HAL_RCC_TIM5_CLK_ENABLE();

	HAL_NVIC_SetPriority(TIM5_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);

}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
	GPIO_InitTypeDef uart4_gpio;
	memset(&uart4_gpio, 0, sizeof(uart4_gpio));

	__HAL_RCC_UART4_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	uart4_gpio.Pin = GPIO_PIN_10 | GPIO_PIN_11;
	uart4_gpio.Speed = GPIO_SPEED_FREQ_LOW;
	uart4_gpio.Pull = GPIO_PULLUP;
	uart4_gpio.Mode = GPIO_MODE_AF_PP;
	uart4_gpio.Alternate = GPIO_AF8_UART4;

	HAL_GPIO_Init(GPIOC, &uart4_gpio);

	HAL_NVIC_SetPriority(UART4_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(UART4_IRQn);
}
