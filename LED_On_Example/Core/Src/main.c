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

TIM_HandleTypeDef timer6;
UART_HandleTypeDef usart3;
volatile uint8_t next_pattern = 0;
volatile uint8_t next_leds_on = 0;
char buffer[50] = { 0 };


int main(void)
{

	/* Initializes low level hardware at the processor level */
	HAL_Init();

	// Sets other clock sources besides HSI, if necessary --> Look at Clock_Source_t.
	if( SystemClock_Config(HSE_4) != Execution_Succesfull)
		return Execution_Failed;

	//volatile uint64_t freq = HAL_RCC_GetHCLKFreq(); // Checks the speed of the Enabled Clock Source

	// Configure port D with the LEDs
	if( GPIO_Set(GPIOD) != Execution_Succesfull)
		return Execution_Failed;

	// Configure the TIMER6 module.
	if ( Timer_Configuration() != Execution_Succesfull)
		return Execution_Failed;

	// Configure the BUTTON.
	if ( Button_Configuration() != Execution_Succesfull)
		return Execution_Failed;

	if( USART3_Configuration() != Execution_Succesfull)
		return Execution_Failed;

	// Start TIMER6.
	if( HAL_TIM_Base_Start_IT(&timer6) != HAL_OK )
		return Execution_Failed;

	// Infinite loop.
	for(;;);

	return Execution_Succesfull;
}

uint8_t SystemClock_Config(Clock_Source_t clk)
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


uint8_t GPIO_Set(GPIO_TypeDef *gpio)
{
	GPIO_InitTypeDef gpio_led;

	memset(&gpio_led, 0, sizeof(gpio_led));

	// Enable the clock to GPIOD
	__HAL_RCC_GPIOD_CLK_ENABLE();

	gpio_led.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	gpio_led.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_led.Pull = GPIO_NOPULL;
	gpio_led.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(gpio, &gpio_led);

	return Execution_Succesfull;
}

uint8_t Timer_Configuration(void)
{
	memset(&timer6, 0, sizeof(timer6));
	timer6.Instance = TIM6;
	// timer6.Init.CounterMode = TIM_COUNTERMODE_UP; ==> For a basic counter isn't configurable
	timer6.Init.Prescaler = 400;  // ==> OutputClock(clock that timer uses to count) = InputClock / (prescaler  1)
	timer6.Init.Period = 9999; // ----- Counts for ~10 seconds -----

	if ( HAL_TIM_Base_Init(&timer6) != HAL_OK)
		return Execution_Failed;

	return Execution_Succesfull;
}

uint8_t Button_Configuration(void)
{
	GPIO_InitTypeDef gpio_button;

	memset(&gpio_button, 0, sizeof(gpio_button));

	// Enable the clock to GPIOA
	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio_button.Pin = GPIO_PIN_0;
	gpio_button.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_button.Pull = GPIO_NOPULL;
	gpio_button.Mode = GPIO_MODE_IT_RISING;

	HAL_GPIO_Init(GPIOA, &gpio_button);

	return Execution_Succesfull;
}

uint8_t USART3_Configuration(void)
{
	memset(&usart3, 0, sizeof(usart3));

	usart3.Instance = USART3;
	usart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	usart3.Init.BaudRate = 115200;
	usart3.Init.Mode = UART_MODE_TX;
	usart3.Init.OverSampling = UART_OVERSAMPLING_16;
	usart3.Init.Parity = UART_PARITY_NONE;
	usart3.Init.StopBits = UART_STOPBITS_1;
	usart3.Init.WordLength = UART_WORDLENGTH_8B;

	if( HAL_UART_Init(&usart3) != HAL_OK )
		return Execution_Failed;

	return Execution_Succesfull;
}

void Choose_Pattern(void)
{
	switch (next_pattern)
	{
		case 0:

			sprintf(buffer, "This is the first LED pattern!!!\r\n");
			if( HAL_UART_Transmit_IT(&usart3, (uint8_t *)buffer, 50) != HAL_OK )
				return ;

			if(next_leds_on == 0)
			{
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13 | GPIO_PIN_15, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_14, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_14, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13 | GPIO_PIN_15, GPIO_PIN_SET);
			}

			break;

		case 1:

			sprintf(buffer, "This is the second LED pattern!!!\r\n");
			if( HAL_UART_Transmit_IT(&usart3, (uint8_t *)buffer, 50) != HAL_OK )
				return ;

			if(next_leds_on == 0)
			{
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_SET);
			}

			break;

		case 2:

			sprintf(buffer, "This is the third LED pattern!!!\r\n");
			if( HAL_UART_Transmit_IT(&usart3, (uint8_t *)buffer, 50) != HAL_OK )
				return ;

			if(next_leds_on == 0)
			{
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_15, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_15, GPIO_PIN_SET);
			}

			break;

		case 3:

			sprintf(buffer, "This is the fourth LED pattern!!!\r\n");
			if( HAL_UART_Transmit_IT(&usart3, (uint8_t *)buffer, 50) != HAL_OK )
				return ;

			if(next_leds_on == 0)
			{
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
			}

			break;
		default:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	UNUSED(htim);
	next_leds_on ^= 1;
	Choose_Pattern();
}

// Selects the next pattern after the button is pressed.
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	next_pattern += 1;

	if(next_pattern == 4)
		next_pattern = 0;

	Choose_Pattern();
}

// Delay function.
void Delay(uint32_t ms)
{
	DWT->CTRL |= (1 << 0); // Enable the DTW counter of the CortexM4
	uint32_t start = DWT->CYCCNT;
	uint32_t ticks = ms * (HAL_RCC_GetHCLKFreq() / 1000);

	while ((DWT->CYCCNT - start) < ticks);
}

