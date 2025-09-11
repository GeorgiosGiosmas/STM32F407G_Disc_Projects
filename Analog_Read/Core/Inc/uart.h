/*
 * uart.h
 *
 *  Created on: Sep 7, 2025
 *      Author: gegio
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include <stdint.h>

/*
 * @brief UART Initialization Structure.
 */
typedef struct{

	USART_TypeDef                 *Instance;
	uint32_t 					   BaudRate;
	uint32_t 					   StopBits;
	uint32_t 					   Parity;
	uint32_t 					   WordLength;
	uint32_t 					   OverSampling;
	uint32_t					   Mode;

} UART_Handler;

/*
 * @brief UART OverSampling.
 */
#define OVERSAMPLING16	( 0x00000000UL )
#define OVERSAMPLING8	((uint32_t)(01UL << 15))

/*
 * @brief UART Word Length.
 */
#define WORDLENGTH8		( 0x00000000UL )
#define WORDLENGTH9		((uint32_t)(01UL << 12))

/*
 * @brief UART Parity.
 */
#define PARITYEVEN		( 0x00000000UL )
#define PARITYODD		((uint32_t)(01UL << 9))

/*
 * @brief UART mode.
 */
#define RXMODE			((uint32_t)(01UL << 2))
#define TXMODE			((uint32_t)(01UL << 3))
#define TXRXMODE		((uint32_t)(01UL << 2 | 01UL << 3))

/*
 * @brief UART Stop Bits.
 */
#define STOPBITS1		( 0x00000000UL )
#define STOPBITS2		((uintt32_t)(02UL << 12))

/*
 * @brief UART Pins Initializations.
 */
uint8_t UART_GPIO_Init(void);

/*
 * @brief UART clock Enable.
 */
uint8_t UART_Enable_Clock(USART_TypeDef *uart_instance);

/*
 * @brief UART Initialization function.
 */
uint8_t UART_Init(UART_Handler *uart);

/*
 * @brief UART Transmit function.
 */
uint8_t UART_Transmit(UART_Handler *uart, uint8_t *pData, uint32_t sizeData);

/*
 * @brief UART Receive function.
 */
uint8_t UART_Receive(UART_Handler *uart, uint8_t *pData, uint32_t sizeData);




#endif /* INC_UART_H_ */
