#ifndef BSP_TIMER_H
#define BSP_TIMER_H

#include <stdint.h>
#include "esp_err.h"

esp_err_t bsp_timer_init_tick(void);   // starts a 1 ms tick
uint32_t  bsp_timer_ticks(void);       // ms since init (wraps after ~49 days)

#endif
