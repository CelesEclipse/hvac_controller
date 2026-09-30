#ifndef _BSP_UART_H_
#define _BSP_UART_H_

#include <stdint.h>
#include <esp_err.h>

esp_err_t   bsp_uart_init(void);
int         bsp_uart_read_byte(uint8_t * out);
void        bsp_uart_write(const char * s);

#endif
