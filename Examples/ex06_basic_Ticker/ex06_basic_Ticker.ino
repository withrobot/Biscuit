// ex06_basic_Ticker.ino
#include <Ticker.h>

// Global Variable
Ticker tickMyTickerOnce;
Ticker tickMyTickerPeriodic;

void myFunctionOnce() {
    Serial.println("ticker once");
}

void myFunctionPeriodic() {
    Serial.println("ticker Periodic");
}

// the setup function runs once when you press reset or power the board
void setup() {
    // Serial Initialize
    Serial.begin(115200);
    delay(1000);
    Serial.println();

    // start Ticker calling function "myFunctionOnce" only once.
    tickMyTickerOnce.once(0.1, myFunctionOnce);
    // start Ticker calling function "myFuctionPeriodic" every 0.5 second
    tickMyTickerPeriodic.attach(0.5, myFunctionPeriodic);
}

// the loop function runs over and over again until power down or reset
void loop() {

}
