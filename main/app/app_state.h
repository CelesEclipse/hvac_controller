#ifndef APP_APPSTATE_H
#define APP_APPSTATE_H

#include <stdint.h>
#include <stdbool.h>

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
bool app_state_set_temp(int16_t temp);
bool app_state_set_mode(hvac_mode_t mode);

#endif
