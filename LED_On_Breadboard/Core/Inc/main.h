/*
 * main.h
 *
 *  Created on: Sep 12, 2024
 *      Author: gegio
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f4xx_hal.h"

/* ====== Program flow =======
 *
 * Στο main.c γίνεται high level initialization του περιφερειακου δηλαδή
 * καλειται η custom UART_Init(), SPI_Init() και λοιπά.
 *
 * Στο msp.c έχουμε low level initialization πχ setting IRQ number, τιμές στους registers
 * και καθορισμός των pins που θα χρησιμοποιηθούν. Πχ Uart_Msp_Init(), SPI_Msp_Init()
 * και λοιπά.
 */

/* ============ Header Files ==============
 *
 *   #include "stm32f4xx.h ==> This is for MCU family
 *
 *   #include "stm32f407xx.h ==> This is for the MCU. It describes the microcontroller, its IRQ
 *   							 definitions, memory map, register details, peripheral clock
 *   							 management macros and others.
 *
 *	 #include "stm32f4xx_hal.h ==> This is for Cube HAL. For the SDK.
 *	 							( WE INCLUDE ONLY THIS FILE )
 */

/*
 *	@brief	This is a custom enumeration for the return status
 */

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
ReturnStatus SystemClock_Config(Clock_Source_t clk);

/*
 * @brief This function configures the appropriate GPIO pins as LED outputs.
 */
ReturnStatus GPIO_Set(GPIO_TypeDef *gpio);

/*
 * @brief This function sets up the buttons.
 */
ReturnStatus Button_Configuration(void);

/*
 * @brief This function implements delay in ms with the DWT module(Debugging and profiling feature in Arm Cortex-M processors, measures CPU cycles).
 */
void Delay(uint32_t ms);

#endif /* INC_MAIN_H_ */
