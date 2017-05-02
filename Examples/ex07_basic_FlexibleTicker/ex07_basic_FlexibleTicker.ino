// ex07_basic_FlexibleTicker.ino
#include <Ticker.h>

// Pin definition
#define PIN_LED (12)
#define PIN_SWITCH (2)

// Global Variable
Ticker tickLED;
int prevSwitchStatus;


void toggleLED() {
    Serial.println("ticker toggle led");
    digitalWrite(PIN_LED, !digitalRead(PIN_LED));
}

// the setup function runs once when you press reset or power the board
void setup() {
    // Serial Initialize
    Serial.begin(115200);

    // Pin Configuration
    pinMode(PIN_SWITCH, INPUT_PULLUP);
    prevSwitchStatus = digitalRead(PIN_SWITCH);

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, HIGH);
}

// the loop function runs over and over again until power down or reset
void loop() {
    // get current status of the switch
    int currSwitchStatus = digitalRead(PIN_SWITCH);

    // switch down/up event (NOT PRESSED/RELEASED)
    if (currSwitchStatus != prevSwitchStatus)
    {
        prevSwitchStatus = currSwitchStatus;

        if (currSwitchStatus == LOW)
        {
            Serial.println("attach ticker");
            tickLED.attach(0.2, toggleLED);
        }
        else // currSwitchStatus == HIGH
        {
            Serial.println("detach ticker");
            tickLED.detach();
        }
    }
}
