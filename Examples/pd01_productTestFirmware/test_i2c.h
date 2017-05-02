#pragma once
#include <Adafruit_BNO055.h>

bool bno055_init(void);
bool bno055_test_accel(void);
bool bno055_test_ahrs(void);
bool bno055_demo(void);
void bno055_reset(void);

extern Adafruit_BNO055 bno055;