#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define PIN_LED 12
#define PIN_SWITCH 2 

#define SERVER_IP "192.168.11.31"
#define SERVER_PORT 1980
#define input_time 5000

int loading = 1;
int storage = 0;
int current_time;
int ssid_addr = 0;
int pass_addr = 32;

uint8_t  WIFI_SSID[32];
uint8_t  WIFI_PASS[32];

uint8_t ssid[32];
uint8_t pass[32];

WiFiClient client;

void sw_init(void)
{
  pinMode(PIN_SWITCH, INPUT_PULLUP);
}

bool sw_read(void)
{
  return digitalRead(PIN_SWITCH)?false:true;
}

bool sw_onehit()
{
  static bool prevSw = false;
  bool ret = false;

  if(sw_read())
  {
    if(prevSw == false)
    {
      ret = true;
      prevSw = true;
    }
  }
  else
  {
    prevSw = false;
  }

  return ret;
}

void eeprom_init(void)
{
  EEPROM.begin(512);
}

void eeprom_read(int addr, uint8_t* buf, int len)
{
  int eeprom_read_ptr = addr;
  for (int i = 0; i < len; i++)
  {
    *(buf + i) = EEPROM.read(addr+i);
  }
}

void eeprom_write(int addr, uint8_t* buf, int len, bool commit)
{
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addr+i, *(buf + i));
  }
  if (commit)
    EEPROM.commit();
}

void setup(){
  
  Serial.begin(115200);
  //led setup
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED,HIGH);
  
  //start eeprom
  eeprom_init();

  //start switch
  sw_init();

  current_time = millis();
  while((millis() - current_time) <= input_time) //For 5 seconds
  {
    if(sw_onehit())//When the switch is pressed
    {
      storage = 1;
      digitalWrite(PIN_LED, LOW);
      Serial.println("Please enter your SSID");
      Serial.print("SSID : ");
      while(1)
      {
        if(Serial.available())
        {
          Serial.readBytesUntil('\n', WIFI_SSID, sizeof(WIFI_SSID));
          Serial.println((char*)WIFI_SSID);
          break;
        
        }
      }
      
      Serial.println("Please enter your PASSWORD");
      Serial.print("PASSWORD : ");
      while(1)
      {
        if(Serial.available())
        {
          Serial.readBytesUntil('\n',WIFI_PASS,sizeof(WIFI_PASS));
          Serial.println((char*)WIFI_PASS);
          break;
        }
      } 
      break;
    }
    delay(10); //also available yield() 
  }
  
  if(storage)
  { //eeprom write 
    eeprom_write(ssid_addr,WIFI_SSID,sizeof(WIFI_SSID),true);
    eeprom_write(pass_addr,WIFI_PASS,sizeof(WIFI_PASS),true);
  }
  
  if((EEPROM.read(ssid_addr) == '\0') || (EEPROM.read(pass_addr) == '\0'))
  {
    Serial.println("\n\nThere is no saved data. Please restart and enter the input mode.");
    loading = 0;
  }
  else
    Serial.println("\n\nAccess using saved data");

  if(loading)
  { //eeprom read
    eeprom_read(ssid_addr,ssid,sizeof(WIFI_SSID));
    for(int i = 0; i < sizeof(WIFI_SSID); i++)
    {
      Serial.print(char(ssid[i]));
    }
    Serial.println();
    
    eeprom_read(pass_addr,pass,sizeof(WIFI_PASS));
    for(int i = 0; i < sizeof(WIFI_PASS); i++)
    {
      Serial.print(char(pass[i]));
    }
    Serial.println();
  }

  if(!(EEPROM.read(ssid_addr) == '\0') && !(EEPROM.read(pass_addr) == '\0'))//if eeprom has data
  {
    //connect wifi
    WiFi.begin(String((char*)ssid), String((char*)pass));
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(500);
    }
    Serial.println("");
    Serial.println("\nWiFi connected");
    Serial.print("\nIP ADDRESS: ");
    Serial.println(WiFi.localIP());
  
    if (!client.connect(SERVER_IP, SERVER_PORT)) 
    {
      Serial.println("connection failed");
      return;
    }
    else  
      Serial.println("connection done");
  }
}

void loop(){
  client.println("withrobot");
  delay(100);
}
