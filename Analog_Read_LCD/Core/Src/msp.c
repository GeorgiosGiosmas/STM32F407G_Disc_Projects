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

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	UNUSED(hadc);
	GPIO_InitTypeDef gpio_analog;

	memset(&gpio_analog, 0, sizeof(gpio_analog));

	// Enable the clocks.
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_ADC2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio_analog.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	gpio_analog.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_analog.Pull = GPIO_NOPULL;
	gpio_analog.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA, &gpio_analog);

}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	UNUSED(htim);

	GPIO_InitTypeDef pwm_gpio;
	memset(&pwm_gpio, 0, sizeof(pwm_gpio));

	// Enable clock in TIM8.
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	pwm_gpio.Pin = GPIO_PIN_2;
	pwm_gpio.Speed = GPIO_SPEED_FREQ_LOW;
	pwm_gpio.Mode = GPIO_MODE_AF_PP;
	pwm_gpio.Alternate = GPIO_AF1_TIM2;
	pwm_gpio.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA, &pwm_gpio);

	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void LCD_Configure_GPIOS(void)
{
	GPIO_InitTypeDef lcd_gpio;
	memset(&lcd_gpio, 0, sizeof(lcd_gpio));

	// Enable necessary clocks.
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	lcd_gpio.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
	lcd_gpio.Speed = GPIO_SPEED_FREQ_LOW;
	lcd_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	lcd_gpio.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOC, &lcd_gpio);

	lcd_gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
	HAL_GPIO_Init(GPIOD, &lcd_gpio);
}
