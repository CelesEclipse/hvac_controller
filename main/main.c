#include <esp_log.h>
#include <esp_rom_sys.h>
#include <stdint.h>
#include "bsp/bsp_gpio.h"
#include "drivers/led.h"
#include "bsp/bsp_timer.h"

extern volatile bool g_btn_pressed;
static const char * TAG = "MAIN";

void app_main(void)
{
    ESP_LOGI(TAG, "HVAC controller");
    led_init();
    bsp_gpio_init_button();

    bsp_timer_init_tick();
    uint32_t last_hb = 0;
    ESP_LOGI(TAG, "Timer init done");

    while (1) {
        uint32_t now = bsp_timer_ticks();
        if ((uint32_t)(now - last_hb) >= 1000) {
            last_hb += 1000;
            ESP_LOGI(TAG, "heartbeat, uptime %lu ms", (unsigned long)now);
        }

        if (g_btn_pressed) {
            g_btn_pressed = false;
            led_toggle();
            esp_rom_delay_us(200 * 1000);
        }
        esp_rom_delay_us(10 * 1000);
    }
}
