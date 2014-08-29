#include "hardware.h"
#include "config.h"

void gpio_init(void)
{
}

void leds_init(void)
{
    nrf_gpio_cfg_output(MAIN_LED_PIN);
    nrf_gpio_pin_clear(MAIN_LED_PIN);
}

void led_toggle(void)
{
    nrf_gpio_pin_toggle(MAIN_LED_PIN);
}
