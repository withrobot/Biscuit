#include "test_led.h"
#include "app_config.h"
#include <arduino.h>
#include <Ticker.h>

//#define __USE_TIMER0__ // not working
//#define __USE_TIMER1__ // not working
#define __USE_TICKER__

#if defined(__USE_TIMER0__)
#if defined(__USE_CPUCLOCK_160__)
#define CYCLE_MS (160000)
#else
#define CYCLE_MS (80000)
#endif
#define CYCLE_100HZ (ESP.getCycleCount() + (CYCLE_MS*10))
#elif defined(__USE_TIMER1__)
#define TIMER1_TICKS_PER_US (APB_CLK_FREQ / 1000000L)
uint32_t usToTicks(uint32_t us)
{
    return (TIMER1_TICKS_PER_US / 16 * us);
}
#elif defined(__USE_TICKER__)
Ticker ledTick;
#endif

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

#if defined(__USE_TIMER0__)
    timer0_write(CYCLE_100HZ);
#endif
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
#if defined(__USE_TIMER0__) || defined(__USE_TIMER1__)
    noInterrupts();
#if defined(__USE_TIMER0__)
    timer0_isr_init();
    timer0_attachInterrupt(handler_led);
    timer0_write(CYCLE_100HZ);
#elif defined(__USE_TIMER1__)
    timer1_isr_init();
    timer1_attachInterrupt(handler_led);
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
    timer1_write(usToTicks(10*1000));
#endif
    interrupts();
#elif defined(__USE_TICKER__)
    ledTick.attach(0.01, handler_led);
#endif
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

