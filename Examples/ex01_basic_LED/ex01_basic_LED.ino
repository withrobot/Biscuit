// ex01_basic_LED.ino

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

}
