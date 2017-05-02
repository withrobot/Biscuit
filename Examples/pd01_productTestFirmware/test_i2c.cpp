#include "test_i2c.h"
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

void bno055_reset(void)
{
    digitalWrite(PIN_BNO055_RESET, LOW); delay(100);
    digitalWrite(PIN_BNO055_RESET, HIGH); delay(100);
}

bool bno055_init(void)
{
    bool bRet = false;
    uint8_t nRetryCnt = 0;
    pinMode(PIN_BNO055_RESET, OUTPUT);

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

void handler_bno055_demo(void)
{
    imu::Vector<3> accel = bno055.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu::Vector<3> gyro = bno055.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Vector<3> mag = bno055.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
    imu::Vector<3> hrp = bno055.getVector(Adafruit_BNO055::VECTOR_EULER);
    uint8_t auto_system, auto_gyro, auto_accel, auto_mag;
    bno055.getCalibration(&auto_system, &auto_gyro, &auto_accel, &auto_mag);

    static double accel_x, accel_y, accel_z;
    static double gyro_x, gyro_y, gyro_z;
    static double mag_x, mag_y, mag_z;
    static double hrp_h, hrp_r, hrp_p;

    accel_x = accel.x()/9.8; accel_y = accel.y()/9.8; accel_z = accel.z()/9.8;
    gyro_x = gyro.x(); gyro_y = gyro.y(); gyro_z = gyro.z();
    mag_x = mag.x(); mag_y = mag.y(); mag_z = mag.z();
    hrp_h = hrp.x(); hrp_r = hrp.y(); hrp_p = hrp.z();

    Serial.print("[Acc]");
    Serial.print(String(accel_x)); Serial.print(", ");
    Serial.print(String(accel_y)); Serial.print(", ");
    Serial.print(String(accel_z)); Serial.print("\t");

    Serial.print("[Gyr]");
    Serial.print(String(gyro_x)); Serial.print(", ");
    Serial.print(String(gyro_y)); Serial.print(", ");
    Serial.print(String(gyro_z)); Serial.print("\t");

    Serial.print("[Mag]");
    Serial.print(String(mag_x)); Serial.print(", ");
    Serial.print(String(mag_y)); Serial.print(", ");
    Serial.print(String(mag_z)); Serial.print("\t");

    Serial.print("[HPR]");
    Serial.print(String(hrp_h)); Serial.print(", ");
    Serial.print(String(hrp_p)); Serial.print(", ");
    Serial.print(String(hrp_r)); Serial.print("\t");

    Serial.print("[Cal]");
    Serial.print(String(auto_system)); Serial.print(", ");
    Serial.print(String(auto_accel)); Serial.print(", ");
    Serial.print(String(auto_gyro)); Serial.print(", ");
    Serial.print(String(auto_mag)); Serial.print("\t");

    Serial.println();
}

void handler_bno055_accel(void)
{
    if (g_bIsRunning)
    {
        imu::Vector<3> accel = bno055.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
        double accel_x = accel.x() / 9.8;
        double accel_y = accel.y() / 9.8;
        double accel_z = accel.z() / 9.8;

        double norm = sqrt(pow(accel_x, 2) + pow(accel_y, 2) + pow(accel_z, 2));
        DBG_PRINT("NORM : " + String(norm) + "\n");
        if (norm > 3.0)
        {
            g_bAccel = true;
            g_bIsRunning = false;
        }
        else
            bno055Tick.once(0.01, handler_bno055_accel);
    }

}

void handler_bno055_ahrs(void)
{
    if (g_bIsRunning)
    {
        imu::Vector<3> hrp = bno055.getVector(Adafruit_BNO055::VECTOR_EULER);
        volatile double hrp_p;
        hrp_p = hrp.z();

        DBG_PRINT("Pitch : " + String(hrp_p) + "\n");
        if ((hrp_p < -165) || (hrp_p > 165))
        {
            g_bAHRS = true;
            g_bIsRunning = false;
        }
        else
            bno055Tick.once(0.01, handler_bno055_ahrs);
    }
}

bool bno055_demo(void)
{
    if (g_bIsRunning == false)
    {
        DBG_PRINT("bno055 Run : Demo\n");
        g_bIsRunning = true;
        bno055Tick.attach(0.1, handler_bno055_demo);
    }
    return true;
}

bool bno055_test_accel(void)
{
    if (g_bIsRunning == false)
    {
        g_bIsRunning = true;

        DBG_PRINT("bno055 Run : Accel\n");
        bno055Tick.once(0.01, handler_bno055_accel);
    }
    if (g_bAccel)
    {
        g_bIsRunning = false;

        DBG_PRINT("bno055 STOP : Accel\n");
        bno055Tick.detach();
        delay(100);
    }
    return g_bAccel;
}

bool bno055_test_ahrs(void)
{
    if (g_bIsRunning == false)
    {
        g_bIsRunning = true;

        DBG_PRINT("bno055 Run : AHRS\n");
        bno055Tick.once(0.01, handler_bno055_ahrs);
    }
    if (g_bAHRS)
    {
        g_bIsRunning = false;

        DBG_PRINT("bno055 STOP : AHRS\n");
        bno055Tick.detach();
        delay(100);
    }
    return g_bAHRS;
}
