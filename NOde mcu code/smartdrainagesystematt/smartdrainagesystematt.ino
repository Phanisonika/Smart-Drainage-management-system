
#define CBOR

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Define http and mqtt endpoints
#define http "api.allthingstalk.io"  // API endpoint
#define mqtt "api.allthingstalk.io"  // broker

#include <ATT_IOT.h>
#include <SPI.h>  // required to have support for signed/unsigned long type.

void callback(char* topic, byte* payload, unsigned int length);
WiFiClient espClient;
PubSubClient pubSub(mqtt, 1883, callback, espClient);

char deviceId[ ] = "X8Q8TLnudxN7cAZCMPhphKq9";
char token[] = "maker:4SzOEk966Lyj80Nw7yC4Qcznz77X6P2GoKcI9aF4";
ATTDevice device(deviceId, token);
// ATTDevice device;

#ifdef CBOR
  #include <CborBuilder.h>
  CborBuilder payload(device);
#endif

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the onboard LED off
  
  Serial.begin(9600);  // Init serial link for debugging
  
  // Enter your WiFi credentials here!
  setupWiFi("sonikav", "sonika1304");
  //
  
  while(!device.connect(&espClient, http))  // Connect to AllThingsTalk
    Serial.println("retrying");

  // Create device assets
  device.addAsset("overflow_status", "overflow_status", "counting up", "sensor", "{\"type\": \"string\"}");
  device.addAsset("block_status", "block_status", "counting up", "sensor", "{\"type\": \"string\"}");
  
  while(!device.subscribe(pubSub))  // Subscribe to mqtt
    Serial.println("retrying"); 
}

void setupWiFi(const char* ssid, const char* password)
{
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
}

char j;
int counter = 0;
void loop()
{
 
  if(Serial.available()>0)
  {
    while(Serial.available()==0);
    j=Serial.read();
    check();
    }
  device.process(); 
}
void check()
{
  if(j=='1')
  device.send("Drainage empty","overflow_status");
  else if(j=='2')
  device.send("Drainage halffilled","overflow_status");
  else if(j=='3')
  device.send("Drainage Overflow","overflow_status");
 
  if(j=='4')
  device.send("Pipe is free","block_status");
  else if(j=='5')
  device.send("Pipe Blocked","block_status");
}

/****
 * Callback function
 * Handle messages that were sent from the AllThingsTalk cloud to this device
 */
void callback(char* topic, byte* payload, unsigned int length) 
{ 
  
}
