#include "temp_sensor.h"
#include "temp_sensor_mock.h"

static int16_t s_temp_x10 = 270;
static bool    s_fault    = false;

esp_err_t temp_sensor_init(void) { return ESP_OK; }

esp_err_t temp_sensor_read_x10(int16_t * out)
{
    if (out == NULL) return ESP_ERR_INVALID_ARG;
    if (s_fault)     return ESP_FAIL;          // sensor "broken"
    *out = s_temp_x10;
    return ESP_OK;
}

void temp_sensor_mock_set(int16_t t)  { s_temp_x10 = t; }
void temp_sensor_mock_set_fault(bool f) { s_fault = f; }
