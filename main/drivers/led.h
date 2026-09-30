#ifndef HVAC_LED_H
#define HVAC_LED_H

#include <stdint.h>
void led_init(void);
void led_set_color(uint8_t red, uint8_t green, uint8_t blue);
void led_toggle(void);

#endif
