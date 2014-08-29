#include "utility.h"

#include "nrf.h"
#include "hardware.h"
#include "scheduler.h"
#include "nrf_sdm.h"
#include "nrf_soc.h"
#include "softdevice_handler.h"
#include "app_error.h"

#define CHECK_ERROR(f) do \
                        { \
                            uint32_t err_code = 0; \
                            err_code = f; \
                            APP_ERROR_CHECK(err_code); \
                        } while(0);


static void power_manage(void);

int main()
{

    SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_XTAL_20_PPM, true);
    scheduler_init();

    leds_init();

    while(1) {
        app_sched_execute();
        power_manage();
        led_toggle();
    }
    return 0;
}

void power_manage(void)
{
    CHECK_ERROR(sd_app_evt_wait());
}

void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
}

void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(0xDEADBEEF, line_num, p_file_name);
}
