#include "utility.h"

#include "nrf.h"
#include "nrf_sdm.h"
#include "nrf_soc.h"

#include "softdevice_handler.h"
#include "app_error.h"
#include "app_timer.h"

#include "config.h"
#include "hardware.h"
#include "scheduler.h"
#include "timer.h"
#include "gpiote.h"
#include "ble.h"

#include "ble/ble_app.h"
#include "sys_handler.h"

#ifdef DEBUG
#include "uart.h"
#endif

static void power_manage(void);

int main()
{

    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, true);
    scheduler_init();
#ifdef DEBUG
    uart_init();
    printf("\r\n __DEBUGGING__\r\n");
#endif

    sys_evt_handler_init();

    leds_init();
    timer_init();
    start_timer();

    gpiote_toggle_led_init();
    start_gpiote_timer();

    ble_app_init();
    ble_app_adv_start();
    while(1) {
        app_sched_execute();
        power_manage();
    }
    return 0;
}

void power_manage(void)
{
    APP_ERROR_CHECK(sd_app_evt_wait());
}

// error handler
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
#ifdef DEBUG
    // printf will add about 35KB size of code
    printf("%s:%lu -- err:0x%04lx\r\n", (char *)p_file_name, line_num, error_code);
#else
    sd_nvic_SystemReset();
#endif
}

void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}
