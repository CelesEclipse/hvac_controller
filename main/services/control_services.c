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
static hvac_fsm_t s_fsm = FSM_IDLE;

static hvac_fsm_t decide(const hvac_state_t * s, hvac_fsm_t cur, bool sen_ok)
{
    if (!sen_ok)               return FSM_FAULT;
    if (s->mode != MODE_COOL)  return FSM_IDLE;
    switch (cur) {
    case FSM_COOLING:
        return (s->temp_x10 <= s->target_x10) ? FSM_IDLE : FSM_COOLING;
    case FSM_FAULT:
        return FSM_IDLE;
    case FSM_IDLE:
    default:
        return (s->temp_x10 >= s->target_x10 + HYSTERESIS_X10) ? FSM_COOLING : FSM_IDLE;
    }
}

static void apply_outputs(hvac_fsm_t fsm, const hvac_state_t *s)
{
    switch (fsm) {
    case FSM_COOLING:
        actuator_set_compressor(true);
        actuator_set_fan(s->fan_percent);
        break;
    case FSM_IDLE:
        actuator_set_compressor(false);
        actuator_set_fan(0);
        break;
    case FSM_FAULT:
        actuator_set_compressor(false);
        actuator_set_fan(FAN_SAFE_PCT);
        break;
    }
    actuator_set_alarm(s->alarm != ALARM_NONE);
}

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
    
    const hvac_state_t * s = app_state_get();
    hvac_fsm_t next = decide(s, s_fsm, sen_ok);
    if (next != s_fsm) {
        ESP_LOGI(TAG, "FSM %d -> %d", s_fsm, next);
        s_fsm = next;
    }
    apply_outputs(s_fsm, s);
    
    if (sen_ok) {
        app_state_set_measured(t);
        if (app_state_get()->alarm == ALARM_SENSOR) {
            app_state_set_alarm(ALARM_NONE);
        }
    } else {
        app_state_set_alarm(ALARM_SENSOR);
    }

    if (sen_ok != s_sensor_ok) {
        s_sensor_ok = sen_ok;
        if (sen_ok) ESP_LOGI(TAG, "sensor recovered");
        else    ESP_LOGW(TAG, "sensor invalid");
    }
}
