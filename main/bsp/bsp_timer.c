#include <driver/gptimer.h>
#include <stdint.h>
#include "bsp_timer.h"
#include "driver/gptimer_types.h"
#include "esp_attr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "hal/timer_types.h"
#include "soc/clk_tree_defs.h"

#define TICK_RESOLUTION_HZ  1000000U
#define TICK_PERIOD_US      1000U

static const char * TAG = "BSP_TIMER";
static gptimer_handle_t s_timer = NULL;
static volatile uint32_t s_ticks = 0;

static bool IRAM_ATTR tick_alarm_isr_callback(
            gptimer_handle_t timer,
            const gptimer_alarm_event_data_t * ev,
            void * user_ctx)
{
    s_ticks++;
    return false;
}

uint32_t bsp_timer_ticks(void)
{
    return s_ticks;
}

esp_err_t bsp_timer_init_tick(void)
{
    gptimer_config_t timer_cfg = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .resolution_hz = TICK_RESOLUTION_HZ,
        .direction = GPTIMER_COUNT_UP
    };

    esp_err_t ret = gptimer_new_timer(&timer_cfg, &s_timer);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "new_timer: %s", esp_err_to_name(ret));
        return ret;
    }

    gptimer_event_callbacks_t cbs = {.on_alarm = tick_alarm_isr_callback};
    ret = gptimer_register_event_callbacks(s_timer, &cbs, NULL);
    if (ret != ESP_OK) return ret;

    ret = gptimer_enable(s_timer);
    if (ret != ESP_OK) return ret;

    gptimer_alarm_config_t alarm_cfg = {
        .alarm_count = TICK_PERIOD_US,
        .reload_count = 0,
        .flags.auto_reload_on_alarm = true
    };
    ret = gptimer_set_alarm_action(s_timer, &alarm_cfg);
    if (ret != ESP_OK) return ret;

    return gptimer_start(s_timer);
}
