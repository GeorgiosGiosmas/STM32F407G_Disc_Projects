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

ADC_HandleTypeDef adc1_potensiometer, adc2_heat_sensor;
ADC_ChannelConfTypeDef potensiometer_channel, heat_sensor_channel;
UART_HandleTypeDef usart1;
TIM_HandleTypeDef pwm_timer;

int main(void)
{

	char uart_buffer[50] = {0};
	volatile uint32_t potensiometer_value, heat_sensor_value, intermediate;
	volatile double potensiometer, temp_sensor, int_val = 0;

	/* Initializes low level hardware at the processor level */
	HAL_Init();

	// Sets other clock sources besides HSI
	if( SystemClock_Config(HSI_8) != Execution_Succesfull)
		return Execution_Failed;

	// Configure the ADCs
	if( ADC_Config(&adc1_potensiometer, ADC1, &potensiometer_channel, ADC_CHANNEL_0) != Execution_Succesfull)
		return Execution_Failed;

	if( ADC_Config(&adc2_heat_sensor, ADC2, &heat_sensor_channel, ADC_CHANNEL_1) != Execution_Succesfull)
		return Execution_Failed;

	// Configure the USART1 module
	if( USART1_Config(&usart1) != Execution_Succesfull )
		return Execution_Failed;

	// Configure the PWM timer.
	if( TIM2_PWM_Config(&pwm_timer) != Execution_Succesfull )
		return Execution_Failed;

	// Start PWM Timer.
	HAL_TIM_PWM_Start(&pwm_timer, TIM_CHANNEL_3);

	// Infinite loop.
	for(;;){

		// Start potensiometer reading.
		HAL_ADC_Start(&adc1_potensiometer);
		if(HAL_ADC_PollForConversion(&adc1_potensiometer, 5) == HAL_OK){
			potensiometer_value = HAL_ADC_GetValue(&adc1_potensiometer);
			int_val = (double)potensiometer_value;
			// Update the PWM signal.
			intermediate = (int_val / 4096) * 19;
			__HAL_TIM_SET_COMPARE(&pwm_timer, TIM_CHANNEL_3, intermediate);
			Delay(20); // Delay ~100ms

			potensiometer = potensiometer_value * 0.0007324;
			sprintf(uart_buffer, "The Potensiometer has a value of: %1.2f Volt.\n\r", potensiometer);
			HAL_UART_Transmit_IT(&usart1, (const uint8_t* )uart_buffer, 50);
		}
		HAL_ADC_Stop(&adc1_potensiometer);

		Delay(100); // Delay ~100ms

		// Start heat sensor reading.
		HAL_ADC_Start(&adc2_heat_sensor);
		if(HAL_ADC_PollForConversion(&adc2_heat_sensor, 5) == HAL_OK){
			heat_sensor_value = HAL_ADC_GetValue(&adc2_heat_sensor);
			temp_sensor = (heat_sensor_value * 0.07324) - 50;
			sprintf(uart_buffer, "The Temp Sensor has a value of: %1.2f Celsius.\n\r", temp_sensor);
			HAL_UART_Transmit_IT(&usart1, (const uint8_t* )uart_buffer, 50);
		}
		HAL_ADC_Stop(&adc2_heat_sensor);

		Delay(5000); // Delay 5 sec

	}

	// Stop PWM timer.
	HAL_TIM_PWM_Stop(&pwm_timer, TIM_CHANNEL_3);

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

ReturnStatus ADC_Config(ADC_HandleTypeDef *adc_handle, ADC_TypeDef *adc_instance, ADC_ChannelConfTypeDef *channel, uint32_t channel_num)
{
	memset(adc_handle, 0, sizeof(*adc_handle));
	memset(adc_instance, 0, sizeof(*adc_instance));
	memset(channel, 0, sizeof(*channel));

	adc_handle->Instance = adc_instance;
	adc_handle->Init.Resolution = ADC_RESOLUTION_12B;
	adc_handle->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	adc_handle->Init.DataAlign = ADC_DATAALIGN_RIGHT;
	adc_handle->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;

	if( HAL_ADC_Init(adc_handle) != HAL_OK){
		return Execution_Failed;
	}

	channel->Channel = channel_num;
	channel->Rank = channel_num;
	channel->SamplingTime = ADC_SAMPLETIME_56CYCLES;

	if( HAL_ADC_ConfigChannel(adc_handle, channel) != HAL_OK){
		return Execution_Failed;
	}

	return Execution_Succesfull;
}

ReturnStatus USART1_Config(UART_HandleTypeDef *uart_handle)
{
	memset(uart_handle, 0, sizeof(*uart_handle));

	uart_handle->Instance = USART1;
	uart_handle->Init.BaudRate = 115200;
	uart_handle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart_handle->Init.Mode = UART_MODE_TX;
	uart_handle->Init.OverSampling = UART_OVERSAMPLING_16;
	uart_handle->Init.Parity = UART_PARITY_NONE;
	uart_handle->Init.StopBits = UART_STOPBITS_1;
	uart_handle->Init.WordLength = UART_WORDLENGTH_8B;

	if( HAL_UART_Init(uart_handle) != HAL_OK )
		return Execution_Failed;

	return Execution_Succesfull;

}

ReturnStatus TIM2_PWM_Config(TIM_HandleTypeDef *timer)
{
	memset(timer, 0, sizeof(*timer));

	TIM_OC_InitTypeDef pwm_config;
	memset(&pwm_config, 0, sizeof(pwm_config));

	timer->Instance = TIM2;
	timer->Init.Prescaler = 799;
	timer->Init.CounterMode = TIM_COUNTERMODE_UP;
	timer->Init.Period = 19;
	timer->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	timer->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	timer->Init.RepetitionCounter = 0;

	if( HAL_TIM_PWM_Init(timer) != HAL_OK)
		return Execution_Failed;

	pwm_config.OCMode = TIM_OCMODE_PWM1;
	//pwm_config.Pulse = ( timer->Init.Period * 80 ) / 100; //-> 50% Duty Cycle
	pwm_config.Pulse = 0;
	pwm_config.OCPolarity = TIM_OCPOLARITY_HIGH;
	pwm_config.OCFastMode = TIM_OCFAST_DISABLE;

	if (HAL_TIM_PWM_ConfigChannel(timer, &pwm_config, TIM_CHANNEL_3) != HAL_OK)
		return Execution_Failed;

	return Execution_Succesfull;

}
void Delay(uint32_t ms)
{
	DWT->CTRL |= (1 << 0); // Enable the DTW counter of the CortexM4
	uint32_t start = DWT->CYCCNT;
	uint32_t ticks = ms * (HAL_RCC_GetHCLKFreq() / 1000);

	while ((DWT->CYCCNT - start) < ticks);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

}


