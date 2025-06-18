/*
 * it.c
 *
 *  Created on: Sep 12, 2024
 *      Author: gegio
 */

#include "it.h"

extern TIM_HandleTypeDef pwm_timer;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&pwm_timer);
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


