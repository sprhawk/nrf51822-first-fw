#include "uart.h"

#include "simple_uart.h"
#include "boards.h"

int _write(int file, char *ptr, int len);

void uart_init(void)
{
    simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
}

int _write(int file, char *ptr, int len)
{
    for(int i = 0; i < len; i ++) {
        simple_uart_put((uint8_t)*(ptr + i));
    }
    return len;
}
