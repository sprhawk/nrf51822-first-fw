#include "timer.h"
#include "app_timer.h"
#include "config.h"


void timer_init(void)
{
    APP_TIMER_INIT(TIMER_PRESCALER, TIMER_MAX_TIMERS, TIMER_OP_QUEUE_SIZE, true);
}

    
