#include <led_strip.h>
#include <stdint.h>
#include "bsp_gpio.h"
#include "esp_log.h"
#include "led_strip_rmt.h"
#include "led_strip_types.h"
#include "soc/clk_tree_defs.h"

#define BLINK_GPIO  48

static const char * TAG = "BSP_GPIO";
static led_strip_handle_t led_hdl = NULL;

void bsp_gpio_init(void)
{
    led_strip_config_t strip_cfg = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 1,
        .led_model = LED_MODEL_WS2812
    };
    led_strip_rmt_config_t strip_rmtcfg = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000,
        .flags.with_dma = false
    };

    esp_err_t ret = led_strip_new_rmt_device(&strip_cfg, &strip_rmtcfg, &led_hdl);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize RMT for led");
    }
    led_strip_clear(led_hdl);
}

void bsp_gpio_set_led_rgb(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t idx = 0;
    esp_err_t ret = led_strip_set_pixel(led_hdl, idx, r, g, b);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set color to pixel %d", idx);
        return;
    }

    if ((ret = led_strip_refresh(led_hdl)) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to refresh led color");
    }
}
