#pragma once

enum E_LED {
    E_LED_OFF,
    E_LED_BLINK_100MS,
    E_LED_BLINK_500MS,
    E_LED_BLINK_1S,
    E_LED_TOGGLE,
    E_LED_ON
};

void led_on(void);
void led_off(void);
void led_toggle(void);
void led_set(E_LED state);
void led_init(void);
