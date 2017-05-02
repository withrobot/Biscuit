// ex00_welcome_HelloBiscuit.ino

// the setup function runs once when you press reset or power the board
void setup() {
    // Serial Initialize
    Serial.begin(115200);
}

// the loop function runs over and over again until power down or reset
void loop() {
    // print String to serial
    Serial.println("Hello, Biscuit!");

    // waiting 1 second.
    delay(1000);
}
