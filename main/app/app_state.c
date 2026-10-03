#include "app_state.h"
#include <stdint.h>

static hvac_state_t s_state = {
    .target_x10 = 240,
    .temp_x10 = 274,
    .fan_percent = 36,
    .mode = MODE_OFF,
    .alarm = ALARM_NONE,
    .fsm = FSM_IDLE
};

const hvac_state_t * app_state_get(void)
{   
    return &s_state;
}

bool app_state_set_fan(uint8_t percent)
{
    if (percent > FAN_MAX_PCT) return false;
    s_state.fan_percent = percent;
    return true;
}

bool app_state_set_target(int16_t target_x10)
{
    if (target_x10 < TARGET_MIN_C * 10 || target_x10 > TARGET_MAX_C * 10) return false;
    s_state.target_x10 = target_x10;
    return true;
}
bool app_state_set_mode(hvac_mode_t mode)
{
    switch ((int)mode) {
        case MODE_OFF:
        case MODE_COOL:
        case MODE_HEAT:
        case MODE_FAN:
            s_state.mode = mode;
            break;
        default: return false;
    }
    return true;
}

void app_state_set_measured(int16_t temp_x10)
{
    s_state.temp_x10 = temp_x10;
}

void app_state_set_alarm(hvac_alarm_t alarm)
{
    if ((unsigned)alarm > ALARM_OVERTEMP) return;
    s_state.alarm = alarm;
}

void app_state_set_fsm(hvac_fsm_t fsm)
{
    if ((unsigned)fsm > FSM_FAULT) return;
    s_state.fsm = fsm;
}
