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

TIM_HandleTypeDef timer5;
UART_HandleTypeDef uart4;
volatile uint8_t current_pattern = 0,next_pattern = 0, maintenance = 0,blue_on = 0, counter = 0;
char *buffer = "ATTENTION AHEAD!  LIGHTS UNDER MAINTENANCE!!!\r\n";


int main(void)
{

	/* Initializes low level hardware at the processor level */
	HAL_Init();

	// Sets other clock sources besides HSI.
	if( SystemClock_Config(HSE_4) != Execution_Succesfull)
		return Execution_Failed;

	//volatile uint64_t freq = HAL_RCC_GetHCLKFreq(); // Checks the speed of the Enabled Clock Source

	// Configure PORTB with the LEDs.
	if( GPIO_Configuration() != Execution_Succesfull)
		return Execution_Failed;

	// Configure TIMER5 for handling the LED blinking via interrupts.
	if ( Timer_Configuration(&timer5) != Execution_Succesfull)
		return Execution_Failed;

	// Configure the buttons in PORTA's GPIOs.
	if ( Button_Configuration() != Execution_Succesfull)
		return Execution_Failed;

	// Configure the UART4 module to print a message when in maintenance.
	if( UART4_Configuration(&uart4) != Execution_Succesfull)
		return Execution_Failed;

	// Initial State.
	next_pattern = 2;
	Choose_Pattern();

	// Start TIMER5.
	if( HAL_TIM_Base_Start_IT(&timer5) != HAL_OK )
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


ReturnStatus GPIO_Configuration(void)
{
	GPIO_InitTypeDef gpio_led;
	memset(&gpio_led, 0, sizeof(gpio_led));

	// Enable the clock to GPIOD
	__HAL_RCC_GPIOB_CLK_ENABLE();

	gpio_led.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7;
	gpio_led.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_led.Pull = GPIO_NOPULL;
	gpio_led.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOB, &gpio_led);

	return Execution_Succesfull;
}

ReturnStatus Timer_Configuration(TIM_HandleTypeDef *timer)
{
	memset(timer, 0, sizeof(*timer));

	timer->Instance = TIM5;
	timer->Init.CounterMode = TIM_COUNTERMODE_UP; // ==> For a basic counter isn't configurable
	timer->Init.Prescaler = 400;
	timer->Init.Period = 10000;

	if ( HAL_TIM_Base_Init(timer) != HAL_OK)
		return Execution_Failed;

	return Execution_Succesfull;
}

ReturnStatus Button_Configuration(void)
{
	GPIO_InitTypeDef gpio_button;
	memset(&gpio_button, 0, sizeof(gpio_button));

	// Enable the clock to GPIOA
	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio_button.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	gpio_button.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_button.Pull = GPIO_NOPULL;
	gpio_button.Mode = GPIO_MODE_IT_RISING;

	HAL_GPIO_Init(GPIOA, &gpio_button);

	return Execution_Succesfull;
}

ReturnStatus UART4_Configuration(UART_HandleTypeDef *uart)
{
	memset(uart, 0, sizeof(*uart));

	uart->Instance = UART4;
	uart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart->Init.BaudRate = 115200;
	uart->Init.Mode = UART_MODE_TX;
	uart->Init.OverSampling = UART_OVERSAMPLING_16;
	uart->Init.Parity = UART_PARITY_NONE;
	uart->Init.StopBits = UART_STOPBITS_1;
	uart->Init.WordLength = UART_WORDLENGTH_8B;

	if( HAL_UART_Init(uart) != HAL_OK )
		return Execution_Failed;

	return Execution_Succesfull;
}

void Choose_Pattern(void)
{
	switch (next_pattern)
	{
		// Green Light on. Cars coming through.
		case 0:
			current_pattern = 0;
			if(maintenance == 0)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
			}
			break;

		// Yellow light on.
		case 1:
			current_pattern = 1;
			if(maintenance == 0)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
			}
			break;

		// Red light on. Stop!
		case 2:
			current_pattern = 2;
			if(maintenance == 0)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5 | GPIO_PIN_7, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
			}
			break;

		// Maintenance state. Blue and Yellow LED blink.
		case 3:
			current_pattern = 3;
			if( HAL_UART_Transmit_IT(&uart4, (uint8_t *)buffer, strlen(buffer)) != HAL_OK )
				return ;

			if(blue_on == 0)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_7, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
			}
			break;
		default:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7, GPIO_PIN_RESET);
	}
}

void Delay(uint32_t ms)
{
	DWT->CTRL |= (1 << 0); // Enable the DTW counter of the CortexM4
	uint32_t start = DWT->CYCCNT;
	uint32_t ticks = ms * (HAL_RCC_GetHCLKFreq() / 1000);

	while ((DWT->CYCCNT - start) < ticks);
}

// Called every 1 second.
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	UNUSED(htim);
	if( maintenance == 1 )
	{
		blue_on ^= 1;
		Choose_Pattern();
	}
	if( counter == 15 && current_pattern == 2 )
	{
		counter = 0;
		next_pattern = 0;
		Choose_Pattern();
	}else if( counter == 15 && current_pattern == 0)
	{
		counter = 0;
		next_pattern = 1;
		Choose_Pattern();
	}else if( counter == 5 && current_pattern == 1)
	{
		counter = 0;
		next_pattern = 2;
		Choose_Pattern();
	}else
	{
		counter += 1;
	}
}

// Selects the next pattern after the button is pressed.
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if( GPIO_Pin == GPIO_PIN_2 )
	{
		if( maintenance == 0 )
		{
			counter = 0;
			maintenance = 1;
			next_pattern = 3;
			Choose_Pattern();
		}
	}
	else if ( GPIO_Pin == GPIO_PIN_3 )
	{
		if( maintenance == 1 )
		{
			counter = 0;
			maintenance = 0;
			next_pattern = 2;
			Choose_Pattern();
		}
	}
}


