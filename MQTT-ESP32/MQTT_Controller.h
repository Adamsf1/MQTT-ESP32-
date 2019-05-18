#ifndef _MQTT_CONTROLLER_H
#define _MQTT_CONTROLLER_H
#include <WiFi.h>
#include <PubSubClient.h>
void MQTT_Reconnect(PubSubClient Client,const char *ClientID,const char *mqttUser,const char *mqttPassword);
void callback(char* topic, byte* payload, unsigned int length);

#endif
