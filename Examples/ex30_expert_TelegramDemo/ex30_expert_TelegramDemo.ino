#include <ESP8266TelegramBOT.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <Ticker.h>

#include "app_config.h"
#include "led.h"
#include "bno055.h"

const String ssid = WIFI_SSID;
const String password = WIFI_PASSWORD;

const String BOTtoken = BOT_TOKEN;
const String BOTname = BOT_NAME;
const String BOTusername = BOT_USERNAME;

TelegramBOT bot(BOTtoken, BOTname, BOTusername);
Ticker sensorTick;

bool g_bPrevBtn = HIGH;
bool g_bPressedBtn = false;
int eeprom_pointer = 0;

// Variables for telegramBot
unsigned int g_iBotPeriod = 1000; //mean time between scan messages
unsigned int g_iBotLastTime; //last time messages' scan has been done
long g_lLoop = 0;
String g_strUserName = "";
String g_strUserID = "";

struct {
    uint8_t hasid;
    uint8_t userid_len;
    char    userid[32];
    uint8_t username_len;
    char    username[128];
}g_sUserData;

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
String makeString(String body)
{
    String strBody;
    strBody = "[" + String(g_lLoop) + "] " + body;

    return strBody;
}
void eeprom_read(int addr, uint8_t* buf, int len)
{
    for (int i = 0; i < len; i++)
        *(buf+i) = EEPROM.read(addr+i);

}
void eeprom_write(int addr, uint8_t* buf, int len)
{
    for (int i = 0; i < len; i++)
        EEPROM.write(addr+i, *(buf+i));
    EEPROM.commit();
}

void Bot_SendProc() {
    if (g_sUserData.hasid)
    {
        if (g_bPressedBtn)
        {
            g_bPressedBtn = false;
            bot.sendMessage(g_strUserID, makeString("button Pressed"), "");
        }

        if (bno055_get_impact())
        {
            bot.sendMessage(g_strUserID, makeString("impact detected!"), "");
        }
    }
}

void Bot_RecvProc() {
    for (int i = 1; i < bot.message[0][0].toInt() + 1; i++) {
        g_strUserID = bot.message[i][4];
        Serial.println("Full: " + bot.message[i][5]);
        String strCmd = getValue(bot.message[i][5],' ', 0);
        Serial.println("Cmd : " + strCmd);
        String strArg = bot.message[i][5].substring(strCmd.length()+1, bot.message[i][5].length());
        Serial.println("Arg : " + strArg);

        if (strCmd == "/led_on") {
            led_set(E_LED_ON);
            bot.sendMessage(g_strUserID, makeString("Led is ON"), "");
        }
        if (strCmd == "/led_off") {
            led_set(E_LED_OFF);
            bot.sendMessage(g_strUserID, makeString("Led is OFF"), "");
        }
        if (strCmd == "/led_toggle") {
            led_set(E_LED_BLINK_500MS);
            bot.sendMessage(g_strUserID, makeString("Led is TOGGLE"), "");
        }
        if (strCmd == "/impact_low") {
            g_dThresholdNorm = 1.2;
            bot.sendMessage(g_strUserID, makeString("impact LOW"), "");
        }
        if (strCmd == "/impact_medium") {
            g_dThresholdNorm = 1.5;
            bot.sendMessage(g_strUserID, makeString("impact MEDIUM"), "");
        }
        if (strCmd == "/impact_high") {
            g_dThresholdNorm = 2.;
            bot.sendMessage(g_strUserID, makeString("impact HIGH"), "");
        }
        if (strCmd == "/impact") {
            String strSubCmd = getValue(strArg, ' ', 0);
            double impactValue = strSubCmd.toFloat();
            if (impactValue >= 1.0 && impactValue <= 8)
            {
                g_dThresholdNorm = impactValue;
                bot.sendMessage(g_strUserID, makeString("impact set to : " + String(impactValue)), "");
            }
        }
        if (strCmd == "/rememberme") {
            memset(&g_sUserData, 0x00, sizeof(g_sUserData));
            eeprom_write(0, (uint8_t*)&g_sUserData, sizeof(g_sUserData));

            g_sUserData.hasid = true;
            g_sUserData.userid_len = g_strUserID.length();
            memcpy(g_sUserData.userid, g_strUserID.c_str(), g_strUserID.length());

            Serial.println(String(g_sUserData.hasid) + ", " + String(g_sUserData.userid_len) + ", " + String(g_sUserData.userid));

            eeprom_write(0, (uint8_t*)&g_sUserData, sizeof(g_sUserData));
            String strBody = "UserID : " + String(g_sUserData.userid);
            bot.sendMessage(g_strUserID, makeString(strBody), "");
        }
        if (strCmd == "/start") {
            String wellcome = "Welcome~ Biscuit!";
            bot.sendMessage(g_strUserID, makeString(wellcome), "");
        }
    }

    bot.message[0][0] = "";   // All messages have been replied - reset new messages
}

void handler_sensor()
{
    if (g_bPrevBtn != digitalRead(PIN_SWITCH))
    {
        g_bPrevBtn = digitalRead(PIN_SWITCH);
        g_bPressedBtn = true;
    }
    sensorTick.once_ms(10, handler_sensor);
}

void setup() {
    Serial.begin(115200);
    while (!Serial);

    EEPROM.begin(512);
    eeprom_read(0, (uint8_t*)&g_sUserData, sizeof(g_sUserData));
    g_strUserID = String(g_sUserData.userid);
    if (g_sUserData.hasid == true)
        Serial.println("\n\nHASID : " + String(g_sUserData.hasid) + ", IDLEN : " + String(g_sUserData.userid_len) + ", ID : " + g_strUserID + "\n\n");


    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nWiFi connected :");
    Serial.println(WiFi.localIP());

    pinMode(PIN_SWITCH, INPUT_PULLUP);
    led_init();
    led_set(E_LED_ON);

    bno055_init();
    bno055_run_impact();

    bot.begin();
    sensorTick.once_ms(10, handler_sensor);

}

// the loop function runs over and over again forever
void loop() {
    if (millis() > g_iBotLastTime + g_iBotPeriod)
    {
        g_iBotLastTime = millis();
        bot.getUpdates(bot.message[0][1]);   // launch API GetUpdates up to xxx message
        Bot_RecvProc();   // reply to message with Echo
        g_lLoop++;
    }
    Bot_SendProc();   // reply to message with Echo
}
