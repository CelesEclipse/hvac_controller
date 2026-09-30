#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include <stdint.h>

void bsp_gpio_init_led(void);
void bsp_gpio_set_led_rgb(uint8_t r, uint8_t g, uint8_t b);
void bsp_gpio_init_button(void);

#endif
