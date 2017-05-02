// ex04_basic_PeriodicProcessing.ino

// Global Variable
unsigned long latestMillis;
unsigned long periodMillis = 1000; // unit : ms

// the setup function runs once when you press reset or power the board
void setup() {
    // Serial Initialize
    Serial.begin(115200);
}

// the loop function runs over and over again until power down or reset
void loop() {
    // Every 1 second 
    if ((millis() - latestMillis) > periodMillis)
    {
        latestMillis = millis();
        Serial.println("processing something every 1 second.");
        // something ...
    }
}
