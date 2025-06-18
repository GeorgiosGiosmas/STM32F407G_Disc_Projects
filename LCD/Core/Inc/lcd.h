/*
 * lcd.h
 *
 *  Created on: Apr 4, 2025
 *      Author: gegio
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

/* This header file includes declarations of functions relevant with the functionality of a 2x16 LCD Display */

/*
 * @brief Initialize LCD.
 */


/*
 * @brief Clear LCD screen.
 */

/*
 * @brief Set cursor on the LCD.
 */

/*
 * @brief Give a Command to the LCD.
 */
void LCD_Give_Command(uint8_t *RS, uint8_t *RW, uint8_t *command);

/*
 * @brief Print data on the LCD.
 */
void LCD_Print_data(uint8_t *RS, uint8_t *RW, uint8_t *data, uint8_t size_of_data);


#endif /* INC_LCD_H_ */
