#include <Adafruit_BNO055.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define WIFI_SSID "*******"
#define WIFI_PASS "*******"
#define SERVER_IP "*******"
#define SERVER_PORT *******

WiFiClient client;

Adafruit_BNO055 bno055 = Adafruit_BNO055();

void setup() {
    Serial.begin(115200);
    WiFi.disconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("\nIP ADDRESS: ");
    Serial.println(WiFi.localIP());

    bno055.begin();
    
    if (!client.connect(SERVER_IP, SERVER_PORT)) {
      Serial.println("connection failed");
      return;
    }
}
  
void loop() {

    
    // get heading and pitch 
    imu::Vector<3> euler = bno055.getVector(Adafruit_BNO055::VECTOR_EULER);
  
    //send data
    String packet = "heading : ";
    packet += String(euler.x());
    packet += " pitch : ";
    packet += String(euler.y());
    packet += " roll : ";
    packet += String(euler.z());
    Serial.println(packet);
    client.println(packet);
    delay(100);
    
}
