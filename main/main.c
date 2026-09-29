#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include "drivers/led.h"
#include "freertos/projdefs.h"

static const char * TAG = "MAIN";
void app_main(void)
{
    ESP_LOGI(TAG, "HVAC controller");
    led_init();
    while (1) {
        led_toggle();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
