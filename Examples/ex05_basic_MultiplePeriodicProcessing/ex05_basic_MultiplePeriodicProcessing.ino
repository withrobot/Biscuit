// ex05_basic_MultiplePeriodicProcessing.ino

// Pin definition.
#define PIN_LED (12)

// Global Variable
unsigned long firstJob_latestMillis;
unsigned long firstJob_periodMillis = 100; // unit : ms
unsigned long secondJob_latestMillis;
unsigned long secondJob_periodMillis = 500; // unit : ms
unsigned long thirdJob_latestMillis;
unsigned long thirdJob_periodMillis = 1000; // unit : ms

// the setup function runs once when you press reset or power the board
void setup() {
    // Serial Initialize
    Serial.begin(115200);

    // Pin Configuration for LED
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, HIGH); // LED OFF
}

// the loop function runs over and over again until power down or reset
void loop() {
    // Every 100 milliseconds
    if ((millis() - firstJob_latestMillis) > firstJob_periodMillis)
    {
        firstJob_latestMillis = millis();
        Serial.println("first job");
    }
    // Every 500 milliseconds
    if ((millis() - secondJob_latestMillis) > secondJob_periodMillis)
    {
        secondJob_latestMillis = millis();
        Serial.println("second job");
        digitalWrite(PIN_LED, !digitalRead(PIN_LED)); // toggle LED
    }
    // Every 1000 milliseconds
    if ((millis() - thirdJob_latestMillis) > thirdJob_periodMillis)
    {
        thirdJob_latestMillis = millis();
        Serial.println("third job");
    }
}
