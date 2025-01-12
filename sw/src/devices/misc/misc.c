#include "misc.h"

volatile bool button1_pressed;

void button_init(STM32_Pin button_pin) { 
    gpio_set_mode(GPIO_MODE_INPUT, button_pin);

    exti_enable_on_pin(button_pin, EXTI_TRIGGER_ON_FALL);
}

void led_init(STM32_Pin led_pin) { 
    gpio_set_mode(GPIO_MODE_OUTPUT, led_pin);
}