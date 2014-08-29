#include "utility.h"
#include "scheduler.h"

#include "softdevice_handler.h"
#include "app_scheduler.h"
#include "app_timer.h"
#include "config.h"

void scheduler_init(void)
{
    APP_SCHED_INIT(SCHEDULER_MAX_EVENT_SIZE, SCHEDULER_QUEUE_SIZE);
}

