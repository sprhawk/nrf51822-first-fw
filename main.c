#include "utility.h"

#include "nrf.h"
#include "nrf_sdm.h"
#include "nrf_soc.h"
#include "softdevice_handler.h"
#include "app_error.h"
#include "app_timer.h"

#include "hardware.h"
#include "scheduler.h"
#include "timer.h"

static app_timer_id_t g_timer1;

static void power_manage(void);
static void timer_timeout_timer1(void * p_context);

int main()
{

    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, true);
    scheduler_init();

    leds_init();
    timer_init();

    CHECK_ERROR(app_timer_create(&g_timer1, APP_TIMER_MODE_REPEATED, timer_timeout_timer1));
    CHECK_ERROR(app_timer_start(g_timer1, 10000, NULL));

    while(1) {
        app_sched_execute();
        power_manage();
    }
    return 0;
}

void power_manage(void)
{
    CHECK_ERROR(sd_app_evt_wait());
}

// timers

void timer_timeout_timer1(void * p_context)
{
    led_toggle();
}

// error handler
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
}

void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}
