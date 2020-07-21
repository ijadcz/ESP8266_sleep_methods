#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiUdp.h>

#include <PubSubClient.h>
//#define DEBUG 0

// Witty Cloud Board specifc pins
const int LDR = A0;
const int BUTTON = 4;
const int RED = 15;
const int GREEN = 12;
const int BLUE = 13;
int counter = 0;
const int sleepTimeS= 10;
unsigned long currentTime=0;


const char* mqttServer = "";
const char* ssid = "";
const char* password =  "";
const int mqttPort =  1883;


WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  #ifdef DEBUG
    Serial.begin(115200);
  #endif  

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
      delay(500);
      Serial.print("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");

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

    client.subscribe("esp/test");
  #endif
  String pubString = String(counter); 
  char message_buff[pubString.length()+1];
  pubString.toCharArray(message_buff, pubString.length()+1); 


   client.publish("esp/test",message_buff);  
    delay(2);

 ESP.deepSleep(sleepTimeS * 1000000);
 

 
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
void loop(){

  
}
