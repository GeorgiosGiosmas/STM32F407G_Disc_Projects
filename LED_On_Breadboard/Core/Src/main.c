/*
 * main.c
 *
 *  Created on: Sep 12, 2024
 *      Author: gegio
 */
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "main.h"

int main(void)
{

	/* Initializes low level hardware at the processor level */
	HAL_Init();

	// Sets other clock sources besides HSI
	if( SystemClock_Config(HSE_2) != Execution_Succesfull)
		return Execution_Failed;

	// Configure port D with the LEDs
	if( GPIO_Set(GPIOD) != Execution_Succesfull)
		return Execution_Failed;

	// Configure the button
	if ( Button_Configuration() != Execution_Succesfull)
		return Execution_Failed;

	// Infinite loop.
	for(;;);

	return Execution_Succesfull;
}

ReturnStatus SystemClock_Config(Clock_Source_t clk)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	memset(&osc_init, 0, sizeof(osc_init));
	memset(&clk_init, 0, sizeof(clk_init));

	switch (clk)
	{
		case HSI_16:
			break;
		case HSI_8:
			osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
			osc_init.HSIState = RCC_HSI_ON;
			if ( HAL_RCC_OscConfig(&osc_init) != HAL_OK )
				return Execution_Failed;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

			if ( HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_0WS) != HAL_OK )
				return Execution_Failed;
			break;

		case HSE_4:
			osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
			osc_init.HSEState = RCC_HSE_ON; // YOU HAVE TO CHECK ON THE SCHEMATIC WHETHER HSE IS BYPASSED OR NOT!!!
			if ( HAL_RCC_OscConfig(&osc_init) != HAL_OK )
				return Execution_Failed;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			if ( HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_0WS) != HAL_OK )
				return Execution_Failed;

			__HAL_RCC_HSI_DISABLE(); //Disable HSI to reduce Power Consumption.

			break;

		case HSE_2:
				osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
				osc_init.HSEState = RCC_HSE_ON; // YOU HAVE TO CHECK ON THE SCHEMATIC WHETHER HSE IS BYPASSED OR NOT!!!
				if ( HAL_RCC_OscConfig(&osc_init) != HAL_OK )
					return Execution_Failed;

				clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
				clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
				clk_init.AHBCLKDivider = RCC_SYSCLK_DIV4;
				clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
				clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

				if ( HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_0WS) != HAL_OK )
					return Execution_Failed;

				__HAL_RCC_HSI_DISABLE(); //Disable HSI to reduce Power Consumption.

				break;

		case PLL_84:
#if !NO_PLL
				osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
				osc_init.HSEState = RCC_HSE_ON;

				// Enable the clock to the power ccontroller.
				__HAL_RCC_PWR_CLK_ENABLE();

				// Set regulator voltage scale as 1.
				__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

				// Turn on the over drive mode.

				osc_init.PLL.PLLState = RCC_PLL_ON;
				osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;
				osc_init.PLL.PLLM = 16;
				osc_init.PLL.PLLN = 336;
				osc_init.PLL.PLLP = 2;
				osc_init.PLL.PLLQ = 7;

				if ( HAL_RCC_OscConfig(&osc_init) != HAL_OK )
					return Execution_Failed;

				clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
				clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
				clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
				clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
				clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

				if ( HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_0WS) != HAL_OK )
					return Execution_Failed;

				__HAL_RCC_HSI_DISABLE(); //Disable HSI to reduce Power Consumption.

				break;
#endif
		case PLL_42:
#if !NO_PLL
				osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
				osc_init.HSEState = RCC_HSE_ON;

				// Enable the clock to the power ccontroller.
				__HAL_RCC_PWR_CLK_ENABLE();

				// Set regulator voltage scale as 1.
				__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

				// Turn on the over drive mode.

				osc_init.PLL.PLLState = RCC_PLL_ON;
				osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;
				osc_init.PLL.PLLM = 16;
				osc_init.PLL.PLLN = 336;
				osc_init.PLL.PLLP = 2;
				osc_init.PLL.PLLQ = 7;

				if ( HAL_RCC_OscConfig(&osc_init) != HAL_OK )
					return Execution_Failed;

				clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
				clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
				clk_init.AHBCLKDivider = RCC_SYSCLK_DIV4;
				clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
				clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

				if ( HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_0WS) != HAL_OK )
					return Execution_Failed;

				__HAL_RCC_HSI_DISABLE(); //Disable HSI to reduce Power Consumption.

				break;
#endif
		default:
	}

	return Execution_Succesfull;
}


ReturnStatus GPIO_Set(GPIO_TypeDef *gpio)
{
	GPIO_InitTypeDef gpio_led;

	memset(&gpio_led, 0, sizeof(gpio_led));

	// Enable the clock to GPIOD
	__HAL_RCC_GPIOD_CLK_ENABLE();

	gpio_led.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	gpio_led.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_led.Pull = GPIO_NOPULL;
	gpio_led.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(gpio, &gpio_led);

	return Execution_Succesfull;
}

ReturnStatus Button_Configuration(void)
{
	GPIO_InitTypeDef gpio_button;

	memset(&gpio_button, 0, sizeof(gpio_button));

	// Enable the clock to GPIOB
	__HAL_RCC_GPIOB_CLK_ENABLE();

	gpio_button.Pin = GPIO_PIN_10 | GPIO_PIN_12;
	gpio_button.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_button.Pull = GPIO_NOPULL;
	gpio_button.Mode = GPIO_MODE_IT_RISING;

	HAL_GPIO_Init(GPIOB, &gpio_button);

	return Execution_Succesfull;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_10){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
		Delay(2000);	//Delay for 2 sec
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
	}
	else if(GPIO_Pin == GPIO_PIN_12){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
		Delay(2000);	//Delay for 2 sec
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
	}
}

void Delay(uint32_t ms)
{
	DWT->CTRL |= (1 << 0); // Enable the DTW counter of the CortexM4
	uint32_t start = DWT->CYCCNT;
	uint32_t ticks = ms * (HAL_RCC_GetHCLKFreq() / 1000);

	while ((DWT->CYCCNT - start) < ticks);
}

