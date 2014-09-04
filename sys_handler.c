#include "sys_handler.h"

#include <stdio.h>

#include "softdevice_handler.h"

static void sys_evt_handler(uint32_t evt);

void sys_evt_handler_init()
{
    softdevice_sys_evt_handler_set(sys_evt_handler);
}

void sys_evt_handler(uint32_t evt)
{
#ifdef DEBUG
    printf("sys_evt:%lu\r\n", evt);
#endif

    switch(evt) {
        default:
            break;
    }
}
