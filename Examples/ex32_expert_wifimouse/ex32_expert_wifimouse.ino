// ex99_wifimouse.ino

#include <Adafruit_BNO055.h>
#include <math.h>

#define __USE_NETWORK__
#define __USE_WINDOWS_SERVER__
#define __USE_ODROID_SERVER__

#if defined(__USE_NETWORK__)
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define WIFI_SSID "yourApSsid"
#define WIFI_PASS "yourApPassword"

#if defined(__USE_ODROID_SERVER__)
#define ODROID_SERVER_IP "192.168.0.25"
#endif

#if defined(__USE_WINDOWS_SERVER__)
#define WINDOWS_SERVER_IP "192.168.0.96"
#endif

#define SERVER_PORT 1980
WiFiUDP client;

unsigned long commLatestMillis;
unsigned long commPeriodMillis = 20; // unit : ms
#endif

unsigned long bno055LatestMillis;
unsigned long bno055PeriodMillis = 10; // unit : ms

unsigned long ledLatestMillis;
unsigned long ledPeriodMillis = 1000; // unit : ms

#define ODROID_DISPLAY_WIDTH (1280)
#define ODROID_DISPLAY_HEIGHT (720)
#define WINDOWS_DISPLAY_WIDTH (1920)
#define WINDOWS_DISPLAY_HEIGHT (1080)

#define PIN_LED (12)
#define PIN_SWITCH  (2)
#define PIN_CUSTOM_SW_1 (16)
#define PIN_BNO055_RESET (13)

#define SW_LEFT (PIN_SWITCH)
#define SW_RIGHT (PIN_CUSTOM_SW_1)

Adafruit_BNO055 bno055 = Adafruit_BNO055();

int odroid_axis_x = 0;
int odroid_axis_y = 0;
int windows_axis_x = 0;
int windows_axis_y = 0;

float offset_h = 0.0;
float offset_p = 0.0;

int prev_btn_l, curr_btn_l;
int prev_btn_r, curr_btn_r;

char packetValue[128];

// the setup function runs once when you press reset or power the board
void setup() {
    // Serial Initialize
    Serial.begin(115200);

    // Biscuit Pin Configuration
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_SWITCH, INPUT_PULLUP);
    pinMode(PIN_CUSTOM_SW_1, INPUT_PULLUP);
    pinMode(PIN_BNO055_RESET, OUTPUT);

    curr_btn_l = digitalRead(SW_LEFT);
    prev_btn_l = curr_btn_l;
    curr_btn_r = digitalRead(SW_RIGHT);
    prev_btn_r = curr_btn_r;

    // BNO055 Initialize
    do
    {
        Serial.println("Initialize BNO055");
        digitalWrite(PIN_BNO055_RESET, LOW); delay(100);
        digitalWrite(PIN_BNO055_RESET, HIGH); delay(100);
    } while (!bno055.begin());

#if defined(__USE_NETWORK__)
    // Connect to AP
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.print("\nIP ADDRESS: ");
    Serial.println(WiFi.localIP());

    // use UDP protocol
    client.begin(4210);
#endif
}

#if defined(__USE_NETWORK__)
// send mouse event packet via UDP network.
void packetMouseSend(char* serverIP, int serverPort, const char* value)
{
    String packet;

    char length[5];
    sprintf(length, "%3d", strlen(value));

    packet = "mos";
    packet += String(length);
    packet += String(value);

    client.beginPacket(serverIP, serverPort);
    client.write(packet.c_str(), packet.length());
    client.endPacket();
}
#endif


// calculate the coordinate on screen using heading and pitch. based on screen resolution.
void calcCoord(int* coord_x, int* coord_y, int screen_width, int screen_height, float h, float p)
{
    float sensitivity_x = 2.5;
    float sensitivity_y = 2;
    int direction_x = 1;
    int direction_y = -1;

    int axis_x = (int)(screen_width / 2. * direction_x * sin(h * PI / 180.));
    int axis_y = (int)(screen_height/ 2. * direction_y * sin(p * PI / 90.));

    axis_x *= sensitivity_x;
    axis_y *= sensitivity_y;

    axis_x += (int)(screen_width / 2.);
    axis_y += (int)(screen_height/ 2.);

    axis_x = constrain(axis_x, 0, screen_width);
    axis_y = constrain(axis_y, 0, screen_height);

    *coord_x = axis_x;
    *coord_y = axis_y;
}

// the loop function runs over and over again until power down or reset
void loop() {
    // Every 1 second
    if ((millis() - ledLatestMillis) > ledPeriodMillis)
    {
        ledLatestMillis = millis();
        // toggle LED
        digitalWrite(PIN_LED, !digitalRead(PIN_LED));
    }

    // Every 0.01 second
    if ((millis() - bno055LatestMillis) > bno055PeriodMillis)
    {
        bno055LatestMillis = millis();

        // get heading and pitch
        imu::Vector<3> euler = bno055.getVector(Adafruit_BNO055::VECTOR_EULER); // Unit : rps
        float h = euler.x() - offset_h;
        float p = euler.y() - offset_p;

        // -180 ~ 180 to 0 ~ 360
        if (h < 0.0)
            h += 360.0;

        calcCoord(&windows_axis_x, &windows_axis_y, WINDOWS_DISPLAY_WIDTH, WINDOWS_DISPLAY_HEIGHT, h, p);
        calcCoord(&odroid_axis_x, &odroid_axis_y, ODROID_DISPLAY_WIDTH, ODROID_DISPLAY_HEIGHT, h, p);
    }

#if defined(__USE_NETWORK__)
    if ((millis() - commLatestMillis) > commPeriodMillis)
    {
        commLatestMillis = millis();

#if defined(__USE_ODROID_SERVER__)
        if (odroid_axis_x != 0 || odroid_axis_y != 0)
        {
            // absolute position event
            sprintf(packetValue, "p %d %d", odroid_axis_x, odroid_axis_y);
            odroid_axis_x = 0; odroid_axis_y = 0;

            packetMouseSend(ODROID_SERVER_IP, SERVER_PORT, packetValue);
        }
#endif

#if defined(__USE_WINDOWS_SERVER__)
        if (windows_axis_x != 0 || windows_axis_y != 0)
        {
            // absolute position event
            sprintf(packetValue, "p %d %d", windows_axis_x, windows_axis_y);
            windows_axis_x = 0; windows_axis_y = 0;

            packetMouseSend(WINDOWS_SERVER_IP, SERVER_PORT, packetValue);
        }
#endif
    }

    // position reset event
    if ((digitalRead(SW_LEFT) != HIGH) && (digitalRead(SW_RIGHT) != HIGH))
    {
        Serial.println("Send Reset Position");
        sprintf(packetValue, "r");
#if defined(__USE_ODROID_SERVER__)
        packetMouseSend(ODROID_SERVER_IP, SERVER_PORT, packetValue);
#endif
#if defined(__USE_WINDOWS_SERVER__)
        packetMouseSend(WINDOWS_SERVER_IP, SERVER_PORT, packetValue);
#endif

        // make offset value on current attitude
        imu::Vector<3> euler = bno055.getVector(Adafruit_BNO055::VECTOR_EULER); // Unit : rps
        offset_h = euler.x();
        offset_p = euler.y();

        delay(100);
        return;
    }

    // left button event
    curr_btn_l = digitalRead(SW_LEFT);
    if (prev_btn_l != curr_btn_l)
    {
        prev_btn_l = curr_btn_l;

        if (curr_btn_l == HIGH)
            sprintf(packetValue, "R l u");
        if (curr_btn_l == LOW)
            sprintf(packetValue, "R l d");

#if defined(__USE_ODROID_SERVER__)
        packetMouseSend(ODROID_SERVER_IP, SERVER_PORT, packetValue);
#endif
#if defined(__USE_WINDOWS_SERVER__)
        packetMouseSend(WINDOWS_SERVER_IP, SERVER_PORT, packetValue);
#endif
    }

    // right button event
    curr_btn_r = digitalRead(SW_RIGHT);
    if (prev_btn_r != curr_btn_r)
    {
        prev_btn_r = curr_btn_r;

        if (curr_btn_r == HIGH)
            sprintf(packetValue, "R r u");
        if (curr_btn_r == LOW)
            sprintf(packetValue, "R r d");

#if defined(__USE_ODROID_SERVER__)
        packetMouseSend(ODROID_SERVER_IP, SERVER_PORT, packetValue);
#endif
#if defined(__USE_WINDOWS_SERVER__)
        packetMouseSend(WINDOWS_SERVER_IP, SERVER_PORT, packetValue);
#endif
    }
#endif
}
