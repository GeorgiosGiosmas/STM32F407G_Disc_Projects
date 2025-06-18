/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "main.h"

#define TRUE	1
#define FALSE	0

UART_HandleTypeDef usart2;
SPI_HandleTypeDef spi2;
char buffer[100] = { 0 };
uint8_t i = 0, rcv_data, reception_complete = FALSE;

int main(void)
{

	/* Initializes low level hardware at the processor level */
	HAL_Init();

	// Sets other clock sources besides HSI, if necessary --> Look at Clock_Source_t.
	if( SystemClock_Config(HSE_4) != Execution_Succesfull)
		return Execution_Failed;

	// Configure the USART2 Module.
	if( USART2_Configuration() != Execution_Succesfull )
		return Execution_Failed;

	// Configure the SPI2 module.
	if( SPI2_Configuration() != Execution_Succesfull )
		return Execution_Failed;

	// Reception of data from UART.
	while(reception_complete != TRUE)
	{
		HAL_UART_Receive_IT(&usart2, &rcv_data, 1);
	}

	// Transmission of received data to the Arduino Uno through SPI2.
	if( HAL_SPI_Transmit_IT(&spi2, buffer, 100) != HAL_OK )
		return Execution_Failed;

	// Infinite Loop.
	for(;;);

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

uint8_t USART2_Configuration(void)
{
	memset(&usart2, 0, sizeof(usart2));

	usart2.Instance = USART2;
	usart2.Init.BaudRate = 115200;
	usart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	usart2.Init.Mode = UART_MODE_TX_RX;
	usart2.Init.OverSampling = UART_OVERSAMPLING_16;
	usart2.Init.Parity = UART_PARITY_NONE;
	usart2.Init.StopBits = UART_STOPBITS_1;
	usart2.Init.WordLength = UART_WORDLENGTH_8B;

	if( HAL_UART_Init(&usart2) != HAL_OK )
		return Execution_Failed;

	return Execution_Succesfull;
}

uint8_t SPI2_Configuration(void)
{
	memset(&spi2, 0, sizeof(spi2));

	spi2.Instance = SPI2;
	spi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	spi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	spi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	spi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	spi2.Init.Mode = SPI_MODE_MASTER;
	spi2.Init.TIMode = SPI_TIMODE_DISABLE;
	spi2.Init.NSS = SPI_NSS_HARD_OUTPUT;
	spi2.Init.DataSize = SPI_DATASIZE_8BIT;
	spi2.Init.Direction = SPI_DIRECTION_2LINES;
	spi2.Init.FirstBit = SPI_FIRSTBIT_MSB;

	if( HAL_SPI_Init(&spi2) != HAL_OK )
		return Execution_Failed;

	return Execution_Succesfull;

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if( rcv_data == '\r' )
	{
		reception_complete = TRUE;
		buffer[i++] = '\r';
	}
	else
		buffer[i++] = rcv_data;

}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{

  __disable_irq();

  for(;;);

}
