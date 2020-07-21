#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiUdp.h>

#include <PubSubClient.h>
//#define DEBUG 0

const int LDR = A0;
const int BUTTON = 4;
const int RED = 15;
const int GREEN = 12;
const int BLUE = 13;
int counter = 0;
unsigned long interval= 180000;
unsigned long currentTime=0;


const char* mqttServer = "192.168.0.103";
const char* ssid = "*****";
const char* password =  "*****";
const int mqttPort =  1883;


WiFiClient espClient;
PubSubClient client(espClient);
extern "C" {
  #include "user_interface.h"
}

void setup()
{
  #ifdef DEBUG
    Serial.begin(115200);
  #endif  
  wifi_set_sleep_type(LIGHT_SLEEP_T);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    #ifdef DEBUG
      Serial.print("Connecting to WiFi..");
    #endif
  }
  #ifdef DEBUG
    Serial.println("Connected to the WiFi network");
  #endif
  client.setServer(mqttServer, mqttPort);
  #ifdef DEBUG
    client.setCallback(callback);
  #endif  
  while (!client.connected()) 
  {
  #ifdef DEBUG
    Serial.println("Connecting to MQTT...");
  #endif  
    if (client.connect("ESP8266Client" )) 
    {
    #ifdef DEBUG
      Serial.println("connected");  
    #endif   
    } else 
    {
      #ifdef DEBUG
        Serial.print("failed with state ");
        Serial.print(client.state());
      #endif   
      delay(2000);
    }
  }
  #ifdef DEBUG
    client.publish("esp/test", "Hello from ESP8266");
    client.subscribe("esp/test");
  #endif   
}
#ifdef DEBUG
  void callback(char* topic, byte* payload, unsigned int length) 
  {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}
#endif  

void loop() 
{
  client.loop();
  delay(2);
  if (WiFi.status() != WL_CONNECTED) 
  {
  WiFi.begin(ssid, password);
  }

  while (WiFi.status() != WL_CONNECTED) 
  {
  delay(500);
  #ifdef DEBUG
    Serial.print("Connecting to WiFi..");
  #endif
  }

 

 String pubString = String(counter); 
 char message_buff[pubString.length()+1];
  pubString.toCharArray(message_buff, pubString.length()+1); 

  
  if (!client.connected()) 
  {
    client.setServer(mqttServer, mqttPort);
    #ifdef DEBUG
      client.setCallback(callback);
    #endif  
    while (!client.connected()) 
    {
      #ifdef DEBUG
        Serial.println("Connecting to MQTT...");
      #endif  
      if (client.connect("ESP8266Client" )) 
      {
        #ifdef DEBUG
          Serial.println("connected");  
        #endif   
      } else 
      {
        #ifdef DEBUG
          Serial.print("failed with state ");
          Serial.print(client.state());
        #endif   
        delay(2000);
      }
    }
  }

  client.publish("esp/test",message_buff);  
  delay(2);
  counter=counter+1;
  currentTime=millis();

  delay(interval);
}
  

  
