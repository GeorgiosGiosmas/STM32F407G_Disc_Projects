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

	// Enable EXTI0 for button.
	HAL_NVIC_SetPriority(EXTI0_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM6_CLK_ENABLE();

	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef usart3_gpio;

	memset(&usart3_gpio, 0, sizeof(usart3_gpio));

	__HAL_RCC_USART3_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	usart3_gpio.Pin = GPIO_PIN_10 | GPIO_PIN_11;
	usart3_gpio.Speed = GPIO_SPEED_FREQ_LOW;
	usart3_gpio.Pull = GPIO_PULLUP;
	usart3_gpio.Mode = GPIO_MODE_AF_PP;
	usart3_gpio.Alternate = GPIO_AF7_USART3;

	HAL_GPIO_Init(GPIOB, &usart3_gpio);

	HAL_NVIC_SetPriority(USART3_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);


}
