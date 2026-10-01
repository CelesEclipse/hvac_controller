#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <stdint.h>
#include "esp_err.h"

#define SENSOR_MIN_C  (-10)
#define SENSOR_MAX_C  60

esp_err_t temp_sensor_init(void);
esp_err_t temp_sensor_read_x10(int16_t *out);   // tenths of a degree C

#endif
