#ifndef APP_APPSTATE_H
#define APP_APPSTATE_H

#include <stdint.h>
#include <stdbool.h>

#define TARGET_MIN_C   16     // degrees
#define TARGET_MAX_C   30

#define FAN_MIN_PCT    0      // percent
#define FAN_MAX_PCT    100

typedef enum {MODE_OFF, MODE_COOL, MODE_HEAT, MODE_FAN} hvac_mode_t;
typedef enum {ALARM_NONE, ALARM_SENSOR, ALARM_OVERTEMP} hvac_alarm_t;

typedef struct
{
    int16_t      temp_x10;     // 274 = 27.4 C
    int16_t      target_x10;   // 240 = 24.0 C
    uint8_t      fan_percent;  // 0..100
    hvac_mode_t  mode;
    hvac_alarm_t alarm;
} hvac_state_t;

const hvac_state_t * app_state_get(void);
bool app_state_set_fan(uint8_t percent);
bool app_state_set_target(int16_t temp);
bool app_state_set_mode(hvac_mode_t mode);
void app_state_set_measured(int16_t temp_x10);
void app_state_set_alarm(hvac_alarm_t alarm);

#endif
