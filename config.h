#ifndef _CONFIG_H_
#define _CONFIG_H_ 

#define SCHEDULER_MAX_EVENT_SIZE MAX(APP_TIMER_SCHED_EVT_SIZE, 0)
#define SCHEDULER_QUEUE_SIZE    10

#define TIMER_PRESCALER                 0
#define TIMER_MAX_TIMERS                4       
#define TIMER_OP_QUEUE_SIZE             5      

#endif // _CONFIG_H_

