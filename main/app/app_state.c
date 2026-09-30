#include "app_state.h"
#include <ctype.h>

static hvac_state_t s_state;

const hvac_state_t * app_state_get(void)
{
    s_state.target_x10 = 240;
    s_state.temp_x10 = 274;
    s_state.fan_percent = 36;
    s_state.mode = MODE_OFF;
    s_state.alarm = ALARM_NONE;
    
    return &s_state;
}

bool app_state_set_fan(uint8_t percent)
{
    if (!isdigit(percent)) return false;
    s_state.fan_percent = percent;
    return true;
}
