#include <stdbool.h>
#include <stdint.h>
#include "control_services.h"
#include "app/app_state.h"
#include "drivers/temp_sensor.h"
#include "drivers/actuator.h"
#include "esp_err.h"
#include "esp_log.h"

static const char * TAG = "CONTROL";
static bool s_sensor_ok = true;
static bool s_act_ok    = true;

void control_init(void)
{
    esp_err_t err_temp = temp_sensor_init();
    if (err_temp != ESP_OK)
        ESP_LOGE(TAG, "sensor init failed: %s", esp_err_to_name(err_temp));

    esp_err_t err_act = actuator_init();
    if (err_act != ESP_OK)
        ESP_LOGE(TAG, "actuator init failed");
}

void control_step(void)
{
    int16_t t = 0;
    bool sen_ok = (temp_sensor_read_x10(&t) == ESP_OK)
            && (t >= SENSOR_MIN_C * 10) && (t <= SENSOR_MAX_C * 10);
    bool act_ok = (actuator_get_fan() == 0)
            && (actuator_get_compressor() == false)
            && (actuator_get_alarm() == false);

    if (sen_ok) {
        app_state_set_measured(t);
        if (app_state_get()->alarm == ALARM_SENSOR) {
            app_state_set_alarm(ALARM_NONE);
        }
    } else {
        app_state_set_alarm(ALARM_SENSOR);
    }

    if (act_ok) {
        // do something here to affect status ?
    }

    if (sen_ok != s_sensor_ok) {
        s_sensor_ok = sen_ok;
        if (sen_ok) ESP_LOGI(TAG, "sensor recovered");
        else    ESP_LOGW(TAG, "sensor invalid");
    }
}
