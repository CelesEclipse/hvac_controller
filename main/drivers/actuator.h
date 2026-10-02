#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"

esp_err_t actuator_init(void);
esp_err_t actuator_set_fan(uint8_t percent);
esp_err_t actuator_set_compressor(bool on);
esp_err_t actuator_set_alarm(bool on);

uint8_t actuator_get_fan(void);
bool    actuator_get_compressor(void);
bool    actuator_get_alarm(void);

#endif
