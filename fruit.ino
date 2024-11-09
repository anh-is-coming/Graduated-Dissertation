// Library
#include <HardwareSerial.h>
#include "ThingSpeak.h"
#include <WiFi.h>

// DHT
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 15
#define DHTTYPE    DHT22
DHT_Unified dht(DHTPIN, DHTTYPE);

// CO2_Etilen
HardwareSerial SerialPort2 (2);
HardwareSerial SerialPort0 (0);

// WIFI
const char* ssid = "Vo tuyen 217";      // your network SSID (name) 
const char* password = "votuyen217@";   // your network password
WiFiClient  client;

// ThingSpeak Channel
// Hop 1
unsigned long myChannelNumber = 2360610;
const char * myWriteAPIKey = "WXG157TW49QRHRO3";

// Hop 2
// unsigned long myChannelNumber = 2492918;
// const char * myWriteAPIKey = "H8F1JOF8HXEY7Q3L";

// Prototype function
void getCO2(int *co2);
void getEthylene(float *etilen);
void connectWiFi();

// Setup
void setup() 
{
  // DHT
  dht.begin();
  sensor_t sensor;
  // CO2_Etilen
  SerialPort2.begin(9600, SERIAL_8N1, 16, 17);
  SerialPort2.setTimeout(5000);
  SerialPort0.begin(9600, SERIAL_8N1, 3, 1);
  SerialPort0.setTimeout(5000);
  btStop();
  Serial.begin(9600);
  delay(2000);

  //WIFI
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  delay(2000);
}


void loop() 
{
  // put your main code here, to run repeatedly:
    
  // Connect or reconnect to WiFi
  connectWiFi();

  // Get CO2 data and print the concentration
  int *co2;
  co2 = (int*)malloc(sizeof(int));
  getCO2(co2);
  if((*co2 != -1) && (*co2 < 5000))
  {
    Serial.print("Giá trị CO2: ");
    Serial.print(*co2);
    Serial.println(" ppm");
    ThingSpeak.setField(3, *co2);
  }
  delay(500);

  // Get Ethylene data and print the concentration
  float *etilen;
  etilen = (float*)malloc(sizeof(float));
  getEthylene(etilen);
  if((*etilen != -1) && (*etilen < 120))
  {
    Serial.print("Giá trị Etilen: ");
    Serial.print(*etilen);
    Serial.println(" ppm");
    ThingSpeak.setField(4, *etilen);
  }
  delay(500);
  
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if(isnan(event.temperature))
  {
  }
  else
  {
    Serial.print("Giá trị nhiệt độ: ");
    Serial.print(event.temperature);
    Serial.println(" độ C");
    ThingSpeak.setField(1, event.temperature);
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) 
  {
  }
  else 
  {
    Serial.print("Giá trị độ ẩm: ");
    Serial.print(event.relative_humidity - 5);
    Serial.println(" %RH");
    ThingSpeak.setField(2, event.relative_humidity - 5);
  }

  Serial.println();
  // Upload to ThingSpeak
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  // Free memory
  free(co2);
  free(etilen);

  // Delay
  delay(15000);
}

// Get CO2 concentration using UART protocol
void getCO2(int *co2)
{
  int i;
  int data[16] = {0};

  if(SerialPort2.available())
  {
    if(SerialPort2.peek() == 66)
    {
      for(i = 0; i < 16; i++)
      {
        data[i] = SerialPort2.read();
      }
    }
  }
  SerialPort2.flush();

  if(data[0] == 66 && (0 <= data[6] <= 20) && data[6] != (-1) && data[7] != (-1))
  {
    *co2 =  data[6]*256 + data[7];
  }
  else
  { 
    *co2 = -1;
  }

  if(SerialPort2.available() > 0)
  {
    SerialPort2.read();
  }
}

// Get Ethylene concentration using UART protocol
void getEthylene(float *etilen)
{
  int j;
  int dat[9] = {0};

  if(SerialPort0.available())
  {
    if(SerialPort0.peek() == 255)
    {
      for(j = 0; j < 9; j++)
      {
        dat[j] = SerialPort0.read();
      }
    }
  }
  SerialPort0.flush();

  if(dat[0] == 255 && dat[2] != -1 && dat[3] != -1)
  {
    *etilen = (dat[2]*256 + dat[3])*0.1;
  }
  else
  {
    *etilen = -1;
  }

  if(SerialPort0.available() > 0)
  {
    SerialPort0.read();
  }
}

// Connect Wifi
void connectWiFi()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    while(WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, password); 
      delay(5000);     
    } 
  } 
}