/*
 * it.c
 *
 *  Created on: Sep 12, 2024
 *      Author: gegio
 */

#include "it.h"

extern TIM_HandleTypeDef timer6;
extern UART_HandleTypeDef usart3;

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

void EXTI0_IRQHandler(void)
{
	// Delay for ~200 ms
	for(uint32_t i = 0; i < 80000; i++);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void USART3_IRQHandler(void)
{
	HAL_UART_IRQHandler(&usart3);

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


