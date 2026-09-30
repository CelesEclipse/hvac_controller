#include "app_state.h"

static hvac_state_t s_state = {
    .target_x10 = 240,
    .temp_x10 = 274,
    .fan_percent = 36,
    .mode = MODE_OFF,
    .alarm = ALARM_NONE,
};

const hvac_state_t * app_state_get(void)
{   
    return &s_state;
}

bool app_state_set_fan(uint8_t percent)
{
    if (percent > 100) return false;
    s_state.fan_percent = percent;
    return true;
}
