#ifndef TEMP_SENSOR_MOCK_H
#define TEMP_SENSOR_MOCK_H

#include <stdint.h>
#include <stdbool.h>

void temp_sensor_mock_set(int16_t temp_x10);
void temp_sensor_mock_set_fault(bool fault);

#endif
