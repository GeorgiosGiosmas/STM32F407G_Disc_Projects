/*
 * lcd.c
 *
 *  Created on: Apr 4, 2025
 *      Author: gegio
 */

#include "lcd.h"

void LCD_Give_Command(uint8_t *RS, uint8_t *RW, uint8_t *command)
{
	RS = 0; RW = 0;

	for(uint8_t i=0; i<8; i++)
	{
		uint8_t bit = (command >> i) & 0x01;
		HAL_GPIO_WritePin(GPIOx, (1 << i), bit);
	}

	Delay(100);
}

void LCD_Print_data(uint8_t *RS, uint8_t *RW, uint8_t *data, uint8_t size_of_data)
{
	RS = 1; RW = 0;
	uint8_t max_characters = 32;

	for(uint8_t i=0; i<size_of_data; i++)
	{
		while(max_characters > 0)
		{
			for(uint8_t j=0; j<8; j++)
			{
				uint8_t bit = (*(data+i) >> i) & 0x01;
				HAL_GPIO_WritePin(GPIOx, (1 << j), bit);
			}

			max_characters -= 1;
			Delay(100);
		}
	}
}
