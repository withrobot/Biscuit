#include "test_switch.h"
#include "app_config.h"
#include <elapsedMillis.h>

void sw_init(void)
{
    pinMode(PIN_SWITCH, INPUT_PULLUP);
}

bool sw_read(void)
{
    return digitalRead(PIN_SWITCH)?false:true;
}

bool sw_onehit()
{
    static bool prevSw = false;
    bool ret = false;

    if (sw_read())
    {
        if (prevSw == false)
        {
            ret = true;
            prevSw = true;
        }
    }
    else
    {
        prevSw = false;
    }

    return ret;
}

int sw_hit()
{
    static elapsedMillis elapsedSwitch;

    static int prevSwState = E_SW_RELEASED;
    int currSwState = prevSwState;

    if (sw_read())
    {
        if (prevSwState == E_SW_RELEASED)
        {
            elapsedSwitch = 0;
            currSwState = E_SW_PRESSED;
        }
        else if (prevSwState == E_SW_PRESSED && elapsedSwitch > 2000) // && time && 2sec
        {
            currSwState = E_SW_PRESSED_LONG;
        }
        else if (prevSwState == E_SW_PRESSED_LONG && elapsedSwitch > 10000) // && time 10sec
        {
            currSwState = E_SW_PRESSED_LONGLONG;
        }
        else if (prevSwState == E_SW_PRESSED_LONGLONG)
        {

        }
    }
    else
    {
        currSwState = E_SW_RELEASED;
    }
    prevSwState = currSwState;

    return currSwState;
}
