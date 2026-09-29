#include "led.h"
#include "bsp/bsp_gpio.h"

static bool s_led_state = false;

void led_init(void)
{
    bsp_gpio_init_led();
}

void led_set_color(uint8_t red, uint8_t green, uint8_t blue)
{
    bsp_gpio_set_led_rgb(red, green, blue);
}

void led_toggle(void)
{
    s_led_state = !s_led_state;
    if (s_led_state) {
        led_set_color(36, 36, 0);
    } else {
        led_set_color(0, 0, 0);
    }
}
