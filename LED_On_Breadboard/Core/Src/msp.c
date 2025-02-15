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
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

