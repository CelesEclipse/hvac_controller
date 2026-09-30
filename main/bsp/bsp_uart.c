#include <string.h>
#include "bsp_uart.h"
#include "driver/uart.h"
#include "esp_err.h"
#include "hal/uart_types.h"
#include "soc/clk_tree_defs.h"

#define CLI_UART    UART_NUM_0
#define RX_BUF_SIZE 256
#define BAUD_RATE   115200

esp_err_t bsp_uart_init(void)
{
    const uart_config_t uart_cfg = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .stop_bits = UART_STOP_BITS_1,
        .parity = UART_PARITY_DISABLE,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };
    esp_err_t ret = uart_driver_install(CLI_UART, RX_BUF_SIZE, 0, 0, NULL, 0);
    if (ret != ESP_OK) return ret;

    return uart_param_config(CLI_UART, &uart_cfg);
}

int bsp_uart_read_byte(uint8_t * out)
{
    return uart_read_bytes(CLI_UART, out, 1, 0);
}

void bsp_uart_write(const char * s)
{
    uart_write_bytes(CLI_UART, s, strlen(s));
}
