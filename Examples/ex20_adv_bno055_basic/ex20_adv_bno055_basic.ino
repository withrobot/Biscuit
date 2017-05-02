// ex20_adv_BNO055Basic.ino
#include <Adafruit_BNO055.h>

// Pin definition
#define PIN_LED (12)
#define PIN_BNO055_RESET (13)

// Global Variable
Adafruit_BNO055 bno055 = Adafruit_BNO055();
unsigned long bno055LatestMillis;
unsigned long bno055PeriodMillis = 10; // unit : ms
unsigned long ledLatestMillis;
unsigned long ledPeriodMillis = 1000; // unit : ms

// the setup function runs once when you press reset or power the board
void setup() {
    // Serial Initialize
    Serial.begin(115200);

    // Biscuit Pin Configuration
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_BNO055_RESET, OUTPUT);

    // BNO055 Initialize
    do
    {
        Serial.println("Initialize BNO055");
        // reset BNO055
        digitalWrite(PIN_BNO055_RESET, LOW); delay(100);
        digitalWrite(PIN_BNO055_RESET, HIGH); delay(100);
    } while (!bno055.begin());

}

// the loop function runs over and over again until power down or reset
void loop() {
    // Every 0.01 second
    if ((millis() - bno055LatestMillis) > bno055PeriodMillis)
    {
        bno055LatestMillis = millis();

        // Get Temperature Value
        int8_t temperature = bno055.getTemp();
        Serial.print("[Temp ] ");
        Serial.print(temperature);
        Serial.println("C");

        // Get Accelometer Values
        imu::Vector<3> accel = bno055.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER); // Unit : m/s^2
        Serial.print("[Accel] ");
        Serial.print(accel.x());
        Serial.print(", ");
        Serial.print(accel.y());
        Serial.print(", ");
        Serial.println(accel.z());

        // Get Gyroscope Values
        imu::Vector<3> gyro = bno055.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE); // Unit : rps
        Serial.print("[Gyro ] ");
        Serial.print(gyro.x());
        Serial.print(", ");
        Serial.print(gyro.y());
        Serial.print(", ");
        Serial.println(gyro.z());

        // Get Magnetometer Values
        imu::Vector<3> mag = bno055.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER); // Unit : ut
        Serial.print("[ Mag ] ");
        Serial.print(mag.x());
        Serial.print(", ");
        Serial.print(mag.y());
        Serial.print(", ");
        Serial.println(mag.z());

        // Get Euler Angle Values
        imu::Vector<3> euler = bno055.getVector(Adafruit_BNO055::VECTOR_EULER); // Unit : Degree
        Serial.print("[Euler] ");
        Serial.print(euler.x());    // Heading
        Serial.print(", ");
        Serial.print(euler.y());    // Pitch
        Serial.print(", ");
        Serial.println(euler.z());  // Roll
        Serial.println();
    }

    // Every 1 second
    if ((millis() - ledLatestMillis) > ledPeriodMillis)
    {
        ledLatestMillis = millis();

        // Toggle LED
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
    }
}
