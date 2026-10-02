#include "app/app_state.h"
#include "actuator.h"
#include "esp_log.h"
#include <stdint.h>

static const char * TAG = "ACTUATOR";

static uint8_t  s_fan = 0;
static bool     s_comp = 0;
static bool     s_alarm = false;

esp_err_t actuator_init(void)
{
    s_fan = 0;
    s_comp = false; 
    s_alarm = false;
    return ESP_OK;
}

esp_err_t actuator_set_fan(uint8_t percent)
{
    if (percent > FAN_MAX_PCT) return ESP_ERR_INVALID_ARG;
    if (percent != s_fan) ESP_LOGI(TAG, "fan %u%% -> %u%%", s_fan, percent);
    s_fan = percent;
    return ESP_OK;
}

esp_err_t actuator_set_compressor(bool on)
{
    if (on != s_comp) ESP_LOGI(TAG, "compressor %s", on ? "ON" : "OFF");
    s_comp = on;
    return ESP_OK;
}

esp_err_t actuator_set_alarm(bool on)
{
    if (on != s_alarm) ESP_LOGW(TAG, "alarm %s", on ? "ON" : "OFF");
    s_alarm = on;
    return ESP_OK;
}

uint8_t actuator_get_fan(void)        { return s_fan; }
bool    actuator_get_compressor(void) { return s_comp; }
bool    actuator_get_alarm(void)      { return s_alarm; }
