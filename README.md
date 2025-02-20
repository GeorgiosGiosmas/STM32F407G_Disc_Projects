# Project_1:  "LED_On_Breadboard"

## Simple project with external buttons triggering the LEDs to ON state for a couple of seconds via interrupts.

### First LED ON:
![First LED on](images/first_led_on.jpg)


### Second LED ON: 
![Second LED on](images/second_led_on.jpg)

# Project_2: "Analog_Read" 

## Utilization of two ADCs(ADC1 and ADC2) to capture analog outputs from a temperature sensor and a potensiometer. The potensiometer's value regulates a PWM(generated from TIMER2) signal which adjusts the brightness of an LED. The values captured from the ADC modules are forwarded to USART1 which is connected to a UART->USB converter that sends messages to Tera Term in our PC.

### Potensiometer with Low output:
![analog_read_1](images/project_analog_1.jpg)

### Potensiometer with High output:
![analog_read_2](images/project_analog_2.jpg)

### USART1 outputs in Tera Term for the previous cases:
![analog_read_3](images/project_analog_3.jpg)
