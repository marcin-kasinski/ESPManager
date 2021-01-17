#ifndef UDP_H
#define UDP_H
#include <LinkedList.h>
#include <ArduinoJson.h>

struct MDNSObject {

  char hostName[20];
  char ip[16];
  char firmware_version[10];
  char spiffs_version[10];
  char timestamp[20];
  char uptime[20];
};




extern LinkedList<MDNSObject> MDNSObjects;

String getDNSDevices();

boolean initUDP();
void sendUDPPing();
void sendUDPMessageAsync(String messagetype);
//void sendUDPRefresh();

void processUDP();
void processUDPMQTTCallback( );

void addMDNSObjectToList(MDNSObject MDNSObjectUDP );
//void addMDNSObjectToList(const char* hostName,const char* ip ,const char* firmware_version ,const char* timestamp ,const char* uptime);
//void addMDNSObjectToList(JsonObject runtime);
//void addMDNSObjectToList(DynamicJsonDocument doc);



#endif
