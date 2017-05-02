#include "test_eeprom.h"
#include <EEPROM.h>

#define DBG_PRINT
//#define DBG_PRINT Serial.print

void eeprom_read(int addr, uint8_t* buf, int len)
{
    int eeprom_read_ptr = addr;
    for (int i = 0; i < len; i++)
    {
        *(buf + i) = EEPROM.read(addr+i);
    }

}

void eeprom_write(int addr, uint8_t* buf, int len, bool commit)
{
    for (int i = 0; i < len; i++)
    {
        EEPROM.write(addr+i, *(buf + i));
    }
    if (commit)
        EEPROM.commit();
}

void eeprom_init(void)
{
    EEPROM.begin(512);
}

bool eeprom_test(void)
{
    uint8_t srcBuf[16];
    uint8_t dstBuf[16];
    bool bRet = false;
    int iErr = 0;

    for (int i = 0; i < sizeof(srcBuf); i++)
    {
        srcBuf[i] = (uint8_t)i;
        eeprom_write(i, &srcBuf[i], sizeof(uint8_t), false);
        DBG_PRINT("src : " + String(srcBuf[i]) + "\n");
    }
    EEPROM.commit();

    for (int i = 0; i < sizeof(dstBuf); i++)
    {
        eeprom_read(i, &dstBuf[i], sizeof(uint8_t));
        DBG_PRINT("dst : " + String(dstBuf[i]) + "\n");
    }

    for (int i = 0; i < sizeof(dstBuf); i++)
    {
        if (srcBuf[i] != dstBuf[i])
        {
            DBG_PRINT("error\r\n");
            iErr++;
        }
    }

    if (!iErr)
        bRet = true;

    return bRet;

}