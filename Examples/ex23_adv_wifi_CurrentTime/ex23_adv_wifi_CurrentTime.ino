// ex23_adv_CurrentTime.ino
#include <ESP8266WiFi.h>
#include <time.h>

// Global Variable
int timezone = 9;
int dst = 0;
const char* ssid = "yourApSsid";
const char* password = "yourApPassword";

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(115200);

    // Connect to WiFi AP
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting to WiFi");
    // Wait for connected
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    // set up internal time using NTP(Network Time Protocol) server
    Serial.println("\nWaiting for time");
    configTime(timezone * 3600, dst, "pool.ntp.org", "time.nist.gov");

    while (!time(nullptr)) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("");
    WiFi.disconnect();
}

// the loop function runs over and over again until power down or reset
void loop() {
    // get time as the number of second since the Epoch(a.k.a. Unix time), 1970-01-01 00:00:00 + timezone offset
    time_t now = time(nullptr);

    // convert to human readable time. method 1.
    int h = (now / 3600) % 24;
    int m = (now / 60) % 60;
    int s = (now % 60);

    Serial.print("hour : ");
    Serial.print(h);
    Serial.print(", min : ");
    Serial.print(m);
    Serial.print(", sec : ");
    Serial.println(s);


    // convert to human readable time. method 2.
    tm* source = localtime(&now);

    Serial.print("hour : ");
    Serial.print(source->tm_hour);
    Serial.print(", min : ");
    Serial.print(source->tm_min);
    Serial.print(", sec : ");
    Serial.println(source->tm_sec);

    Serial.println();

    delay(1000);
}
