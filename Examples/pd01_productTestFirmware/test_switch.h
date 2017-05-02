#pragma once

enum E_SW {
    E_SW_RELEASED,
    E_SW_PRESSED,
    E_SW_PRESSED_LONG,
    E_SW_PRESSED_LONGLONG
};

void sw_init(void);
bool sw_read(void);
bool sw_onehit();
int sw_hit();
