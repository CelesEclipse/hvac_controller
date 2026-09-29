#include <led_strip.h>
#include <driver/gpio.h>
#include <stdint.h>
#include "bsp_gpio.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "hal/gpio_types.h"
#include "led_strip_rmt.h"
#include "led_strip_types.h"
#include "soc/clk_tree_defs.h"

#define BLINK_GPIO      48
#define BUTTON_DEBOUNCE_US  50000
#define BUTTON_GPIO         GPIO_NUM_0

static const char * TAG = "BSP_GPIO";
static led_strip_handle_t led_hdl = NULL;

volatile bool g_btn_pressed = false;

static void IRAM_ATTR button_isr_handler(void * arg)
{
    // A simple debouncer I mean ?
    static uint64_t last_intr_time = 0;
    uint64_t now = esp_timer_get_time();

    if (gpio_get_level(BUTTON_GPIO) == 0 &&               // falling edge only
        (now - last_intr_time) > BUTTON_DEBOUNCE_US) {
        g_btn_pressed = true;
    }
    last_intr_time = now;                                    // every edge, rising too
}

void bsp_gpio_init_led(void)
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

void bsp_gpio_init_button(void) {
    gpio_config_t io_cfg = {
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE  // NEGEDGE might never update last_intr_time in ISR
    };
    
    if (gpio_config(&io_cfg) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize gpio config");
        return;
    }
    gpio_install_isr_service(0);    // Treat this as OK

    if (gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, NULL) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add this ISR handler for GPIO PIN : %d", BUTTON_GPIO);
        return;
    }
}
