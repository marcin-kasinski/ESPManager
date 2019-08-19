
#ifndef MQTT_H
#define MQTT_MAX_PACKET_SIZE 2560
#define MQTT_H

//extern WiFiClient espClient;
//extern PubSubClient client;

void initMQTT() ;
void MQTTProcess() ;
void MQTTConnect();
bool MQTTpublish(String message);
bool MQTTpublish(String topic, String message);
bool MQTTLogMessage(String message);
//bool MQTTLogMessage(const char *message);



bool getDevices(String url);

#endif
