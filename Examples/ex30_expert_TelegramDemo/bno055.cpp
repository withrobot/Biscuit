#include "bno055.h"
#include "app_config.h"
#include <Ticker.h>
#include <math.h>

#define DBG_PRINT
//#define DBG_PRINT Serial.print

Adafruit_BNO055 bno055 = Adafruit_BNO055();

#define BNO055_RETRY_COUNT (10)
#define BNO055_RETRY_DELAY (500) //ms

Ticker bno055Tick;
bool g_bIsRunning = false;
bool g_bAccel = false;
bool g_bAHRS = false;
static double accel_x, accel_y, accel_z;
//static double gyro_x, gyro_y, gyro_z;
//static double mag_x, mag_y, mag_z;
//static double hrp_h, hrp_r, hrp_p;
bool g_bImpact = false;
double g_dThresholdNorm = 1.2;


void bno055_reset(void)
{
    digitalWrite(PIN_BNO055_RESET, LOW); delay(100);
    digitalWrite(PIN_BNO055_RESET, HIGH); delay(100);
}

bool bno055_init(void)
{
    bool bRet = false;
    uint8_t nRetryCnt = 0;

    do
    {
        bno055_reset();
        if (bno055.begin())
        {
            bRet = true;
            break;
        }
        nRetryCnt++;
        delay(BNO055_RETRY_DELAY);
    } while (nRetryCnt < BNO055_RETRY_COUNT);

    return bRet;
}

void handler_bno055_impact(void)
{
    imu::Vector<3> accel = bno055.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Vector<3> gyro = bno055.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Vector<3> mag = bno055.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
    imu::Vector<3> hrp = bno055.getVector(Adafruit_BNO055::VECTOR_EULER);

    accel_x = accel.x(); accel_y = accel.y(); accel_z = accel.z();
    //gyro_x = gyro.x(); gyro_y = gyro.y(); gyro_z = gyro.z();
    //mag_x = mag.x(); mag_y = mag.y(); mag_z = mag.z();
    //hrp_h = hrp.x(); hrp_r = hrp.y(); hrp_p = hrp.z();

    double norm = sqrt(pow(accel_x/9.8, 2) + pow(accel_y/9.8, 2) + pow(accel_z/9.8, 2));
    DBG_PRINT("NORM : " + String(norm) + "\n");
    if (norm > g_dThresholdNorm)
        g_bImpact = true;

}

bool bno055_get_impact(void)
{
    bool bRet = false;
    if (g_bImpact)
    {
        g_bImpact = false;
        bRet = true;
    }

    return bRet;
}

bool bno055_run_impact(void)
{
    if (g_bIsRunning == false)
    {
        DBG_PRINT("bno055 Run : impact\n");
        g_bIsRunning = true;
        bno055Tick.attach(0.1, handler_bno055_impact);
    }
    return true;
}
