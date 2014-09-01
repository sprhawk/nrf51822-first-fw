#ifndef _CONFIG_H_
#define _CONFIG_H_ 

#define SCHEDULER_MAX_EVENT_SIZE MAX(APP_TIMER_SCHED_EVT_SIZE, BLE_STACK_HANDLER_SCHED_EVT_SIZE)
#define SCHEDULER_QUEUE_SIZE    10

#define TIMER_PRESCALER                 0
#define TIMER_MAX_TIMERS                4       
#define TIMER_OP_QUEUE_SIZE             5      

#define TOGGLE_LED_GPIOTE_CHANNEL       0

#define MAIN_BUTTON_PIN BUTTON_0
#define MAIN_LED_PIN    LED_0

#define SECONDARY_BUTTON_PIN BUTTON_1
#define SECONDARY_LED_PIN    LED_1


// BLE related
#define BLE_DEVICE_NAME "air-beacon"

#endif // _CONFIG_H_

