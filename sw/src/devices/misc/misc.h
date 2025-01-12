#ifndef MISC_H
#define MISC_H

#include "../stm32l47x/basic.h"
#include "../stm32l47x/gpio.h"

#define BUTTON1_PIN_BANK 'C'
#define BUTTON1_PIN_NO 13
#define BUTTON1_PIN (STM32_Pin){BUTTON1_PIN_BANK, BUTTON1_PIN_NO}
extern volatile bool button1_pressed;

#define LED1_PIN_BANK 'A'
#define LED1_PIN_NO 5
#define LED1_PIN (STM32_Pin){LED1_PIN_BANK, LED1_PIN_NO}

void button_init(STM32_Pin button_pin);

void led_init(STM32_Pin led_pin);

#endif