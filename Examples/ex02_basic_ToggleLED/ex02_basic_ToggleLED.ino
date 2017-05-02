// ex02_basic_ToggleLED.ino

// Pin definition.
#define PIN_LED (12)

// the setup function runs once when you press reset or power the board
void setup() {
    // Serial Initialize
    Serial.begin(115200);

    // Pin Configuration for LED
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW); // Output LOW meaning is Turn on the LED
}

// the loop function runs over and over again until power down or reset
void loop() {
    int ledStatus;

    ledStatus = digitalRead(PIN_LED);

    if (ledStatus == LOW) // meanning LED on
    {
        digitalWrite(PIN_LED, HIGH); // Turn off the LED
    }
    else // ledStatus == HIGH // meanning LED off
    {
        digitalWrite(PIN_LED, LOW); // Turn on the LED
    }

    // waitting 500 milliseconds.
    delay(500);

}
