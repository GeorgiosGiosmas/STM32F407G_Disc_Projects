/*
 * it.c
 *
 *  Created on: Sep 12, 2024
 *      Author: gegio
 */

#include "it.h"
//#include "main.h"

extern TIM_HandleTypeDef timer5;
extern UART_HandleTypeDef uart4;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&timer6);
	TIMER6_IRQ_Handler();
}

void EXTI2_IRQHandler(void)
{
	Delay(200); // Delay 200ms
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void EXTI3_IRQHandler(void)
{
	Delay(200); // Delay 200ms
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void UART4_IRQHandler(void)
{
	HAL_UART_IRQHandler(&uart4);
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


