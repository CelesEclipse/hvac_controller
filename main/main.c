#include <esp_log.h>
#include <esp_rom_sys.h>
#include "bsp/bsp_gpio.h"
#include "drivers/led.h"

extern volatile bool g_btn_pressed;

static const char * TAG = "MAIN";
void app_main(void)
{
    ESP_LOGI(TAG, "HVAC controller");
    led_init();
    bsp_gpio_init_button();

    while (1) {
        if (g_btn_pressed) {
            g_btn_pressed = false;
            led_toggle();
            esp_rom_delay_us(200 * 1000);
        }
        esp_rom_delay_us(10 * 1000);
    }
}
