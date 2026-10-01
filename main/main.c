#include <esp_log.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "bsp/bsp_gpio.h"
#include "bsp/bsp_timer.h"
#include "bsp/bsp_uart.h"
#include "drivers/led.h"
#include "cli/cli.h"
#include "services/control_services.h"

static const char *TAG = "MAIN";

#define HEARTBEAT_MS  10000

void app_main(void)
{
    ESP_LOGI(TAG, "HVAC controller");
    led_init();
    bsp_gpio_init_button();

    if (bsp_timer_init_tick() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize bsp timer");
        return;
    }
    if (bsp_uart_init() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize bsp uart");
        return;
    }
    control_init();

    uint32_t last_hb   = bsp_timer_ticks();
    uint32_t last_ctrl = last_hb;

    while (1) {
        uint8_t c;
        while (bsp_uart_read_byte(&c) == 1) {
            cli_feed((char)c);
        }

        uint32_t now = bsp_timer_ticks();

        if ((uint32_t)(now - last_hb) >= HEARTBEAT_MS) {
            last_hb += HEARTBEAT_MS;
            ESP_LOGI(TAG, "heartbeat, uptime %lu ms", (unsigned long)now);
        }

        if ((uint32_t)(now - last_ctrl) >= CONTROL_PERIOD_MS) {
            last_ctrl += CONTROL_PERIOD_MS;
            control_step();
        }

        if (bsp_gpio_take_button_event()) {
            vTaskDelay(pdMS_TO_TICKS(20));
            (void)bsp_gpio_take_button_event();
            if (bsp_gpio_button_is_pressed()) {
                led_toggle();
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
