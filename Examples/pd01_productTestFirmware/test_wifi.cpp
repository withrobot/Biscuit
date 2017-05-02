#include "app_config.h"
#include "test_wifi.h"

#define DBG_PRINT
//#define DBG_PRINT Serial.print

const String g_strSSID = WIFI_SSID;
const String g_strPassword = WIFI_PASSWORD;
const String g_strUrl = WIFI_HTTP_URL_VERIFY;
const String g_strVerify = WIFI_STRING_VERIFY;

bool wifi_init(void)
{
    bool bRet = false;

    Serial.println("Wifi Connect : " + g_strSSID + ", " + g_strPassword);
    WiFi.mode(WIFI_STA);
    WiFi.begin(g_strSSID.c_str(), g_strPassword.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    bRet = true;

    return bRet;
}

bool wifi_test(void)
{
    bool bRet = false;
    uint8_t nRetry = 10;

    HTTPClient httpClient;
    int httpCode = 0;
    bool httpRet = false;
    String payload;

    do
    {
        DBG_PRINT("Connection Retry remaining : " + String(nRetry) + "\n");
        httpRet = httpClient.begin(g_strUrl);
    } while (!httpRet && nRetry--);

    if (!httpRet)
    {
        DBG_PRINT("Connection Error\n");
    }

    if (httpRet)
    {
        nRetry = 10;
        do
        {
            DBG_PRINT("Get Retry remaining : " + String(nRetry) + "\n");
            httpCode = httpClient.GET();

            if (httpCode > 0)
            {
                if (httpCode == HTTP_CODE_OK)
                {
                    payload = httpClient.getString();
                    DBG_PRINT(payload + "\n");
                    if (!g_strVerify.compareTo(payload))
                    {
                        bRet = true;
                        DBG_PRINT("compare ok.\n");
                    }
                }
            }
        } while (!bRet && nRetry--);
    }

    return bRet;
}
