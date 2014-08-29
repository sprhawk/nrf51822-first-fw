#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include "boards.h"

#define MAIN_BUTTON_PIN BUTTON_0
#define MAIN_LED_PIN    LED_0

void gpio_init(void);
void leds_init(void);

void led_toggle(void);

#endif
