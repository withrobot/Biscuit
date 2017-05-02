// pd01_productTestFirmware.ino

#include <elapsedMillis.h>
#include "test_i2c.h"
#include "test_wifi.h"
#include "test_switch.h"
#include "test_led.h"
#include "test_eeprom.h"

bool g_bTestedI2C = false;
bool g_bTestedWIFI = false;
bool g_bTestedSerial = false;
bool g_bTestedBNO055_1 = false;
bool g_bTestedBNO055_2 = false;
bool g_bTestedSwitch = false;
bool g_bTestedUserFlash = false;

const String g_strRetVal[] = {
    "OK",
    "FAIL"
};

const String g_strState[] = {
    "None",
    "I2C",
    "WIFI",
    "SERIAL",
    "BNO055 ACCELOMETER",
    "BNO055 AHRS",
    "SWITCH",
    "FLASH WRITE",
    "Test Finished",
    "Demonstration",
    "TheEnd"
};
const String g_strPass = "PASS";

enum E_TEST {
    E_TEST_NONE = 0x00,
    E_TEST_I2C,
    E_TEST_WIFI,
    E_TEST_SERIAL,
    E_TEST_BNO055_1,
    E_TEST_BNO055_2,
    E_TEST_SW,
    E_TEST_USERFLASH,
    E_TEST_FINISH,
    E_TEST_DEMONSTRATION,
    E_TEST_END
};

int g_eTestState = E_TEST_NONE;


void printState(int eState)
{
    static int ePrevState = E_TEST_END;

    if (ePrevState != eState)
    {
        Serial.print(g_strState[eState] + String(" : "));
        ePrevState = eState;
    }
}

bool checkPass(void)
{
    unsigned long lastTime;
    bool bRet = false;
    bool waitkey = true;
    bool keyStatus = false;

    Serial.println("if you want clear test result, press the button.");
    lastTime = millis();
    while (waitkey)
    {
        if (sw_hit() || (millis() - lastTime) > 2000)
        {
            if (sw_hit())
                keyStatus = true;
            waitkey = false;
        }
    }

    if (keyStatus)
    {
        int clear = 0;
        eeprom_write(0, (uint8_t*)&clear, sizeof(clear));
        Serial.println("Clear");
    }
    else
    {
        char buf[5];
        memset(buf, 0x00, sizeof(buf));
        // READ USERFLASH
        eeprom_read(0, (uint8_t*)buf, g_strPass.length());
        Serial.println(g_strPass + " vs " + String(buf));
        if (g_strPass == String(buf))
            bRet = true;
    }

    return bRet;
}

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(115200);
    // WATCHDOG DISABLE
    // LED initialize
    led_init();
    led_set(E_LED_OFF);

    // SW initialize
    sw_init();

    // EEPROM Initialize
    eeprom_init();
    if (checkPass())
    {
        Serial.println("START DEMO");
        g_eTestState = E_TEST_DEMONSTRATION;
        led_set(E_LED_OFF);
    }
    else
    {
        Serial.println("START TEST");
        g_eTestState++;
        // WIFI Initialize
        wifi_init();
    }
    // I2C Initialize
    g_bTestedI2C = bno055_init();
}

// the loop function runs over and over again forever
void loop() {
    static uint32_t loop_cnt = 0;
    bool bSuccess = false;

    printState(g_eTestState);

    if (g_eTestState == E_TEST_I2C)
    {
        if (g_bTestedI2C)
            bSuccess = true;
    }
    else if (g_eTestState == E_TEST_WIFI)
    {
        g_bTestedWIFI = wifi_test();
        if (g_bTestedWIFI)
        {
            led_set(E_LED_ON);
            bSuccess = true;
        }
    }
    else if (g_eTestState == E_TEST_SERIAL)
    {
#if defined(__USE_TEST_SERIAL__)
        // SERIAL INPUT/OUTPUT
        if (Serial.available())
        {
            char key = Serial.read();
            if (key == ' ')
                g_bTestedSerial = true;
        }
        if (g_bTestedSerial)
            bSuccess = true;
#else
        bSuccess = true;
#endif
    }
    else if (g_eTestState == E_TEST_BNO055_1)
    {
#if defined(__USE_TEST_BNO055_ACCEL__)
        // BNO055 ACCEL
        g_bTestedBNO055_1 = bno055_test_accel();
        if (g_bTestedBNO055_1)
        {
            led_set(E_LED_BLINK_100MS);
            bSuccess = true;
        }
#else
        bSuccess = true;
#endif
    }
    else if (g_eTestState == E_TEST_BNO055_2)
    {
        // BNO055 EULER 
        g_bTestedBNO055_2 = bno055_test_ahrs();
        if (g_bTestedBNO055_2)
        {
            led_set(E_LED_BLINK_500MS);
            bSuccess = true;
        }
    }
    else if (g_eTestState == E_TEST_SW)
    {
        // BNO055 SW TEST
        if (sw_onehit())
            g_bTestedSwitch = true;

        if (g_bTestedSwitch)
        {
            led_set(E_LED_ON);
            bSuccess = true;
        }
    }
    else if (g_eTestState == E_TEST_USERFLASH)
    {
        g_bTestedUserFlash = eeprom_test();
        if (g_bTestedUserFlash)
            bSuccess = true;
    }
    else if (g_eTestState == E_TEST_FINISH)
    {
        Serial.println("\n\nALL PASS\n\n");
        eeprom_write(0, (uint8_t *)g_strPass.c_str(), g_strPass.length());

        delay(1000);
        while (1);
    }
    else if (g_eTestState == E_TEST_DEMONSTRATION)
    {
        if (sw_onehit())
        {
            led_set(E_LED_TOGGLE);
        }
        bno055_demo();
    }

    if (bSuccess)
    {
        Serial.println(" PASS");
        g_eTestState++;
    }
    else
    {
        delay(50);
    }
}
