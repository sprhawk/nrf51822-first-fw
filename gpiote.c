#include "gpiote.h"

#include "nrf.h"
#include "nrf_soc.h"
#include "nrf_gpio.h"
#include "nrf_gpiote.h"

#include "hardware.h"
#include "config.h"
#include "utility.h"

// gpiote test
void start_gpiote_timer()
{
    NRF_TIMER2->TASKS_CLEAR = 1;
    NRF_TIMER2->PRESCALER = 4;
    NRF_TIMER2->CC[0] = 5000 * 10000UL;
    NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;
    NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
    NRF_TIMER2->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos;

    NRF_TIMER2->TASKS_START = 1;
}

void gpiote_toggle_led_init()
{
    sd_ppi_channel_assign(TOGGLE_LED_GPIOTE_CHANNEL, &(NRF_TIMER2->EVENTS_COMPARE[0]) ,&(NRF_GPIOTE->TASKS_OUT[0]));

    nrf_gpio_cfg_output(SECONDARY_LED_PIN);
    nrf_gpio_pin_clear(SECONDARY_LED_PIN);
    nrf_gpiote_task_config(TOGGLE_LED_GPIOTE_CHANNEL, SECONDARY_LED_PIN, NRF_GPIOTE_POLARITY_TOGGLE, NRF_GPIOTE_INITIAL_VALUE_LOW);
    sd_ppi_channel_enable_set(PPI_CHEN_CH0_Msk);
}
