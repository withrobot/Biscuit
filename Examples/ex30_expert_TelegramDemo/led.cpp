#include "led.h"
#include <arduino.h>
#include <Ticker.h>
#include "app_config.h"

#define __USE_TICKER__

Ticker ledTick;
E_LED g_eLedState = E_LED_OFF;

inline void led_on(void)
{
    digitalWrite(PIN_LED, LOW);
}

inline void led_off(void)
{
    digitalWrite(PIN_LED, HIGH);
}

inline void led_toggle(void)
{
    digitalWrite(PIN_LED, !digitalRead(PIN_LED));

}

void handler_led(void)
{
    static uint8_t cnt = 0;

    cnt++;
    if ( ((g_eLedState == E_LED_BLINK_100MS) && (cnt > 10))
        || ((g_eLedState == E_LED_BLINK_500MS) && (cnt > 50))
        || ((g_eLedState == E_LED_BLINK_1S) && (cnt > 100)) )
    {
        led_toggle();
        cnt = 0;
    }
}

void led_init(void)
{
    pinMode(PIN_LED, OUTPUT);
    ledTick.attach(0.01, handler_led);
}

void led_handle(E_LED state)
{
    switch (state)
    {
    case E_LED_ON:
        led_on();
        break;
    case E_LED_OFF:
        led_off();
        break;
    case E_LED_TOGGLE:
        led_toggle();
        break;
    case E_LED_BLINK_1S:
    case E_LED_BLINK_500MS:
    case E_LED_BLINK_100MS:
        break;
    }
}

void led_set(E_LED state)
{
    g_eLedState = state;
    led_handle(g_eLedState);
}

