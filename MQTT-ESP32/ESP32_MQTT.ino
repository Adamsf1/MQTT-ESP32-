#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "MQTT_Controller.h"

#define DHTTYPE    DHT11     // DHT 11
#define DHTPIN 17     // Digital pin connected to the DHT sensor 

const char* ssid = "ADAMSF 7582";
const char* password =  "83407805";
const char* mqttServer = "m14.cloudmqtt.com";
const int mqttPort = 15293;
const char* mqttUser = "qcxvyaqb";
const char* mqttPassword = "rfbdC0o9SCCu";
const char* ClientID = "ESP32Client";
WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(DHTPIN, DHTTYPE);


void setup() {

  Serial.begin(115200);
  Serial.println();

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect(ClientID, mqttUser, mqttPassword )) {

      Serial.println("connected");

    } else {

     Serial.print("failed with state ");
     Serial.print(client.state());
             

                   
     delay(2000);
    }
  }
    Serial.println(xPortGetCoreID());
   
   xTaskCreatePinnedToCore(  MQTT_Publish,       /* Task function. */
                 "Publish to MQTT",      /* String with name of task. */
                 10000,             /* Stack size in words. */
                 NULL,              /* Parameter passed as input of the task */
                 1,                 /* Priority of the task. */
                 NULL,               /* Task handle. */
                 0);            
                 
   xTaskCreatePinnedToCore(  MQTT_Subscribe,       /* Task function. */
                 "subscribe from MQTT",     /* String with name of task. */
                 10000,             /* Stack size in words. */
                 NULL,              /* Parameter passed as input of the task */
                 1,                 /* Priority of the task. */
                 NULL,               /* Task handle. */
                 1); 
  client.subscribe("esp/control");
  dht.begin();
  Serial.println(F("DHT11  Sensor Setup!"));


}

void loop() {
  
}


void MQTT_Publish(void * parameter)
{
  for(;;)
  {
    Serial.println(xPortGetCoreID());
   if(!client.connected())
    {
      MQTT_Reconnect(client,ClientID, mqttUser, mqttPassword);
     }
    
    float humi = dht.readHumidity();
    float temp = dht.readTemperature();
    float f = dht.readTemperature(true);
  
    StaticJsonBuffer<300> JSONbuffer;
    JsonObject& JSONencoder = JSONbuffer.createObject();
  
    JSONencoder["device"] = "ESP32";
    JSONencoder["sensorType"] = "DHT11";
    JsonArray& temp_values = JSONencoder.createNestedArray("temp");
    JsonArray& humi_values = JSONencoder.createNestedArray("humi");
  
    if (isnan(humi) || isnan(f) || isnan(temp)) 
    {
      Serial.println(F("Error reading temperature!"));
    }
    else 
    {
      Serial.print(F("Temperature: "));
      Serial.print(temp);
      Serial.println(F("°C"));
      Serial.print(F("Humidity: "));
      Serial.print(humi);
      Serial.println(F("%"));
    }
  
    temp_values.add(temp);  //温度将数据添加到Json中
    humi_values.add(humi);  //将湿度数据添加到Json中
  
    char JSONmessageBuffer[100];
    JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println("Sending message to MQTT topic..");
    Serial.println(JSONmessageBuffer);
  
    if (client.publish("esp/test", JSONmessageBuffer) == true) 
    {
      Serial.println("Success sending message");
    }
    else 
    {
      Serial.println("Error sending message");
      client.disconnect ();
       MQTT_Reconnect(client,ClientID, mqttUser, mqttPassword);
    }
  
   // client.loop();
  
    Serial.println("---------------------------------------------");
  
    delay(2000);
   
   
   }
  

}


void MQTT_Subscribe(void * parameter)
{
  
  for(;;)
  {
    
    if(!client.connected())
    {
       MQTT_Reconnect(client,ClientID, mqttUser, mqttPassword);
     }
     
    
    
    client.loop();  
    delay(2000);
   }
  
}
