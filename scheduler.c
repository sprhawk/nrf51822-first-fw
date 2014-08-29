#include "utility.h"
#include "scheduler.h"

#include "softdevice_handler.h"
#include "app_scheduler.h"
#include "app_timer.h"

#define SCHEDULER_MAX_EVENT_SIZE MAX(APP_TIMER_SCHED_EVT_SIZE, 0)
#define SCHEDULER_QUEUE_SIZE    10


void scheduler_init(void)
{
    APP_SCHED_INIT(SCHEDULER_MAX_EVENT_SIZE, SCHEDULER_QUEUE_SIZE);
}

