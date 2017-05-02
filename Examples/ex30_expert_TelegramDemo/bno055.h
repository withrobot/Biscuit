#pragma once
#include <Adafruit_BNO055.h>

extern Adafruit_BNO055 bno055;
extern double g_dThresholdNorm;

bool bno055_init(void);
bool bno055_run_impact(void);
bool bno055_get_impact(void);
