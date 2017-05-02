// ex03_basic_Switch.ino

// Pin definition.
#define PIN_LED (12)
#define PIN_SWITCH (2)

// the setup function runs once when you press reset or power the board
void setup() {
    // Serial Initialize
    Serial.begin(115200);

    // Pin configuration for LED
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW); // Output LOW meaning is Turn on the LED

    // Pin Configuration for SWITCH
    pinMode(PIN_SWITCH, INPUT_PULLUP);

}

// the loop function runs over and over again until power down or reset
void loop() {
    int switchStatus = digitalRead(PIN_SWITCH);

    if (switchStatus == LOW) // The switch is pressed.
    {
        digitalWrite(PIN_LED, LOW); // turn on the LED
    }
    else // The switch is released.
    {
        digitalWrite(PIN_LED, HIGH); // turn off the LED
    }

    // waiting 100 milliseconds.
    delay(100);
}
