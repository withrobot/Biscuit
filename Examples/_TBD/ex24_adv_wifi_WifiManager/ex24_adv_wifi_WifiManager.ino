// ex24_adv_wifi_WifiManager.ino
#if 0
#include <ESP8266WiFi.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>

// You need WiFiManager Library. Install WiFiManager Library via Arduino Library Manager.
#include <WiFiManager.h>
#include <Ticker.h>

#define PIN_LED (12)
#define PIN_SWITCH (2)


// Global Variable
//WiFiManager wifiManager;
Ticker tickLED;

void configModeCallback(WiFiManager *myWiFiManager) {
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());
    //if you used auto generated SSID, print it
    Serial.println(myWiFiManager->getConfigPortalSSID());
    //entered config mode, make led toggle faster
    tickLED.attach(0.2, toggleLED);
}

void toggleLED() {
    digitalWrite(PIN_LED, !digitalRead(PIN_LED));
}
#endif

// the setup function runs once when you press reset or power the board
void setup() {
#if 0
    Serial.begin(115200);
    Serial.println();

    pinMode(PIN_SWITCH, INPUT_PULLUP);

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    tickLED.attach(0.5, toggleLED);

    WiFiManager wifiManager;
    wifiManager.setAPCallback(configModeCallback);
    //wifiManager.setTimeout(180);
    wifiManager.setConnectTimeout(1);

    if (!wifiManager.autoConnect("biscuitApSetup", "1234567890"))
    {
        Serial.println("failed to connect and hit timeout");
        //reset and try again, or maybe put it to deep sleep
        ESP.reset();
        delay(1000);
    }

    Serial.println("Wifi Connected.");
    tickLED.detach();
    //keep LED on
    digitalWrite(PIN_LED, LOW);

    if (digitalRead(PIN_SWITCH) == LOW)
        wifiManager.resetSettings();
#endif

}

// the loop function runs over and over again until power down or reset
void loop() {

}
