#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiUdp.h>

#include <PubSubClient.h>
//#define DEBUG 0
extern "C" {
  #include "user_interface.h"
}

int counter = 0;
unsigned long interval= 180000;
unsigned long currentTime=0;


const char* mqttServer = "";
const char* ssid = "*****";
const char* password =  "***-**";

const int mqttPort =  1883;

WiFiClient espClient;
PubSubClient client(espClient);




void setup() {
  

  Serial.begin(115200);
  wifi_status_led_uninstall();
  WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
  delay(500);

client.setServer(mqttServer, mqttPort);
#ifdef DEBUG
  client.setCallback(callback);
#endif  
while (!client.connected()) {
  #ifdef DEBUG
    Serial.println("Connecting to MQTT...");
  #endif  
    if (client.connect("ESP8266Client" )) {
    #ifdef DEBUG
    Serial.println("connected");  
    #endif   
    } else {
    #ifdef DEBUG
    Serial.print("failed with state ");
    Serial.print(client.state());
    #endif   
    delay(2000);
    }
}


}
}

void wakeup_cb() {
  wifi_fpm_close();
  Serial.printf("wakeup_cb\n");
  Serial.flush();
}

#ifdef DEBUG
void callback(char* topic, byte* payload, unsigned int length) {
 
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

   WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
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
while (!client.connected()) {
  #ifdef DEBUG
    Serial.println("Connecting to MQTT...");
  #endif  
    if (client.connect("ESP8266Client" )) {
    #ifdef DEBUG
    Serial.println("connected");  
    #endif   
    } else {
    #ifdef DEBUG
    Serial.print("failed with state ");
    Serial.print(client.state());
    #endif   
    delay(2000);
    }
}
  String pubString = String(counter); 
 char message_buff[pubString.length()+1];
pubString.toCharArray(message_buff, pubString.length()+1); 

  #ifdef DEBUG
  Serial.print(counter);
  #endif  

 
 client.publish("esp/test",message_buff);  

 delay(50);
 counter=counter+1;

  

  extern os_timer_t *timer_list;
  while(timer_list != 0) {
 
    timer_list = timer_list->timer_next;
  }
  Serial.flush();
  WiFi.forceSleepBegin();
delay (100);
  wifi_set_opmode_current(NULL_MODE);
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
  wifi_fpm_open();
  wifi_fpm_set_wakeup_cb(wakeup_cb);
 
  wifi_fpm_do_sleep(interval * 1000);
  delay (interval+1);

 
}
