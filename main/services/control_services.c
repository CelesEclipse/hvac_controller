#include <stdbool.h>
#include <stdint.h>
#include "control_services.h"
#include "app/app_state.h"
#include "drivers/temp_sensor.h"
#include "esp_err.h"
#include "esp_log.h"

static const char * TAG = "CONTROL";
static bool s_sensor_ok = true;

void control_init(void)
{
    esp_err_t err = temp_sensor_init();
    if (err != ESP_OK)
        ESP_LOGE(TAG, "sensor init failed: %s", esp_err_to_name(err));
}

void control_step(void)
{
    int16_t t = 0;
    bool ok = (temp_sensor_read_x10(&t) == ESP_OK)
            && (t >= SENSOR_MIN_C * 10) && (t <= SENSOR_MAX_C * 10);

    if (ok) {
        app_state_set_measured(t);
        if (app_state_get()->alarm == ALARM_SENSOR) {
            app_state_set_alarm(ALARM_NONE);
        }
    } else {
        app_state_set_alarm(ALARM_SENSOR);
    }

    if (ok != s_sensor_ok) {
        s_sensor_ok = ok;
        if (ok) ESP_LOGI(TAG, "sensor recovered");
        else    ESP_LOGW(TAG, "sensor invalid");
    }
}
