#include "MQTT_Controller.h"


void MQTT_Reconnect(PubSubClient Client,const char *ClientID,const char *mqttUser,const char *mqttPassword) 
{
               // 若目前沒有和伺服器相連，則反覆執行直到連結成功…
  while (!Client.connected()) {
                // 指定用戶端ID並連結MQTT伺服器
    if (Client.connect(ClientID,mqttUser,mqttPassword)) {
               // 若連結成功，在序列埠監控視窗顯示「已連線」。
      Serial.println("connected");
    } else {
               // 若連線不成功，則顯示錯誤訊息
      Serial.print("failed, rc=");
      Serial.print(Client.state());
      Serial.println(" try again in 5 seconds");
              // 等候5秒，再重新嘗試連線。
      delay(5000);
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("*************************************");
}
