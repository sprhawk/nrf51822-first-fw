#include "timer.h"
#include "app_timer.h"
#include "config.h"
#include "utility.h"
#include "hardware.h"

app_timer_id_t g_timer1;

void timer_init(void)
{
    APP_TIMER_INIT(TIMER_PRESCALER, TIMER_MAX_TIMERS, TIMER_OP_QUEUE_SIZE, true);
}

    
// timers

void start_timer()
{
    CHECK_ERROR(app_timer_create(&g_timer1, APP_TIMER_MODE_REPEATED, timer_timeout_timer1));
    CHECK_ERROR(app_timer_start(g_timer1, 10000, NULL));
}

void timer_timeout_timer1(void * p_context)
{
    led_toggle();
}
