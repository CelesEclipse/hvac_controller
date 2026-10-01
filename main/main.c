#include <esp_log.h>
#include <esp_rom_sys.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include "bsp/bsp_gpio.h"
#include "drivers/led.h"
#include "bsp/bsp_timer.h"
#include "bsp/bsp_uart.h"
#include "cli/cli.h"

static const char * TAG = "MAIN";

#define CLI_TEST    0

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

    uint32_t last_hb = 0;
    ESP_LOGI(TAG, "Timer init done");

    while (1) {
#if CLI_TEST
        uint8_t c;
        while (bsp_uart_read_byte(&c) == 1) {
            cli_feed(c);
        }
#else
        uint32_t now = bsp_timer_ticks();
        if ((uint32_t)(now - last_hb) >= 1000) {
            last_hb += 1000;
            ESP_LOGI(TAG, "heartbeat, uptime %lu ms", (unsigned long)now);
        }

        if (bsp_gpio_take_button_event()) {
            vTaskDelay(pdMS_TO_TICKS(20));
            (void)bsp_gpio_take_button_event();
            if (bsp_gpio_button_is_pressed()) {
                led_toggle();
            }
        }
#endif
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
