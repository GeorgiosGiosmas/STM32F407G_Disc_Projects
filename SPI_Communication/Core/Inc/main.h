/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"

// If no PLL clock available set this to 1.
#define NO_PLL	( 1 )

typedef enum
{
	Execution_Succesfull 	= 0,
	Execution_Failed 		= 1
} ReturnStatus;

/*
 * @brief This is the enumeration type for the supported clocks.
 */
typedef enum
{
	HSI_16	=	0,
	HSI_8	=	1,
	HSE_4	=	2,
	HSE_2	=	3,
	PLL_84	=	4,
	PLL_42	=	5,
} Clock_Source_t;

/*
 * @brief This function sets the appropriate clock source for our circuit.
 */
uint8_t SystemClock_Config(Clock_Source_t clk);

/*
 * @brief This function configures the USART2 peripheral.
 */
uint8_t USART2_Configuration(void);

/*
 * @brief This function configures the SP1 peripheral.
 */
uint8_t SPI2_Configuration(void);

/*
 * @brief Function for handling errors.
 */
void Error_Handler(void);

/*
 * @brief This function implements delay in ms with the DWT module(Debugging and profiling feature in Arm Cortex-M processors, measures CPU cycles).
 */
void Delay(uint32_t ms);


#endif /* __MAIN_H */
