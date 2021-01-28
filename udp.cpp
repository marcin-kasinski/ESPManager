#include <ArduinoJson.h>

#include <WiFiUDP.h>

#include <LinkedList.h>

#include <NtpClientLib.h>

#include "util.h"

#include "udp.h"

#include "mqtt.h"

#include "timer.h"

WiFiUDP UDP;
LinkedList < MDNSObject > MDNSObjects = LinkedList < MDNSObject > ();

void sendUDPMessageAsync(String messagetype) {
  //  MQTTLogMessage("sendUDPMessageAsync");

  if (conf.discoverable == false) return;

//  MQTTLogMessage("UDP add async message");
  runtime.udpmessages.add(messagetype);
}

// connect to UDP – returns true if successful or false if not
boolean initUDP() {

  boolean state = false;


  MQTTLogMessage("Connecting to UDP");

  int ret = UDP.begin(9991);

  if (ret) {

    MQTTLogMessage("UDP Connection successful");

    state = true;

    //timer1.addTask("SYSTEM",-1,"Send PING to cluster","Every 20","*", "*", "*","*");
    //sendUDPRefresh();

    sendUDPMessageAsync("REFRESH");
  } else {

    MQTTLogMessage("UDP Connection failed");
  }

  return state;
}

# define UDP_TX_PACKET_MAX_SIZE 1000

void sendUDPMessage(String messagetype) {
  if (conf.discoverable == false) return;
  String message = "";


  String relays = "\"relays\" : [";

  int pos = 0;
  for (int i = 0; i < 2; i++) {
    if (conf.relays[i].relay_pin < 0) continue;


    if (pos > 0)relays = relays + ",";

    relays = relays +  "{ \"id\":\"" + i + "\"" ;
    relays = relays +  ", \"internalName\":\"" + conf.relays[i].internalName + "\" " ;
    relays = relays +  ", \"state\":\"" + conf.relays[i].relay_state + "\" " ;
    relays = relays +  " }" ;

    pos++;
  }//for

  relays = relays + "]";



  message = message + "{\"runtime\" : {\n";
  message = message + relays + ",";

  message = message + "\"messagetype\":\"" + messagetype.c_str() + "\",\"hostName\":\"" + conf.hostName + "\"" + ",\"ip\":\"" + WiFi.localIP().toString() + "\"" +



            ",\"security_enable\":\"" + conf.security_enable + "\"" +

            ",\"firmware_version\":\"" + conf.firmware_version + "\"" +
            ",\"spiffs_version\":\"" + conf.spiffs_version + "\"" +
            ",\"timestamp\":\"" + NTP.getTimeDateString() + "\"" + ",\"sunset_update_time\":\"" + runtime.sunset_update_time + "\"" + ",\"sunset\":\"" + getHourString(runtime.sunset_hour, runtime.sunset_minute) + "\"" + ",\"sunrise\":\"" + getHourString(runtime.sunrise_hour, runtime.sunrise_minute) + "\"" + ",\"uptime\":\"" + NTP.getUptimeString() + "\"\n";

  message = message + String(" }}");

  //MQTTpublish(runtime.pingtopic, message);

  MQTTLogMessage("UDP sending message : " + message);
  //MQTTLogMessage("UDP broadcast IP : "+conf.broadcast );

  byte messageBytes[message.length() + 1];

  message.getBytes(messageBytes, message.length() + 1);

  int ret = UDP.beginPacket(conf.broadcast.c_str(), 9991);

  // jeśli był problem
  if (ret == 0) {
    MQTTLogMessage("UDP.beginPacket error");
    return;
  }
  UDP.write(messageBytes, sizeof(messageBytes) + 1);

  ret = UDP.endPacket();
  // jeśli był problem
  if (ret == 0) {
    MQTTLogMessage("UDP message sent error: UDP.endPacket");

    return;
  }

  //MQTTLogMessage("UDP message sent : "+message );
  MQTTLogMessage("UDP sending message END");

}

void sendUDPPing() {

  sendUDPMessageAsync("PING");

}

/*
  void sendUDPRefresh()
  {
  sendUDPMessage("REFRESH");

  }
*/

void processUDP() {
  //------------------- najpierw sprawdza, czy nie ma czegoś do wysłąnia -------------------
  int listSize = runtime.udpmessages.size();

  //    MQTTLogMessage(String("UDP sending async messages:")+listSize );

  if (listSize > 0) MQTTLogMessage("UDP sending async messages");

  for (int i = 0; i < listSize; i++) {
    sendUDPMessage(runtime.udpmessages.get(i));
  }

  runtime.udpmessages.clear();

  //------------------- najpierw sprawdza, czy nie ma czegoś do wysłąnia -------------------

}

void processUDPMQTTCallback() {

  char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
  int packetSize = UDP.parsePacket();

  String buffer = String(packetBuffer);

  if (!packetSize) return;

  UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);

  packetBuffer[packetSize] = 0;
  buffer = String(packetBuffer);

  MQTTLogMessage("UDP received :" + buffer);

  // TU NIE RESTARTUJE SIE
  if (isValidJson(buffer.c_str()) == false) return;

  // TU NIE RESTARTUJE SIE

  //char inchars[runtime.json_buffer_length];
  char inchars[packetSize + 1];

  // addMDNSObjectToList("XXXXXXXXXXXXXXXX" , "192.168.1.211","10.00", "xxxx","   0 days 00:00:11");
  //  return;
  strcpy(inchars, buffer.c_str());

  /// TU SIE RESTARTUJE

  DynamicJsonDocument doc(packetSize + 1);

  deserializeJson(doc, inchars);

  //JsonObject root = doc.to<JsonObject>();

  /////////////////TU JAK DALEM RETURN TO DZIAŁAŁO/XXXXXXXXXXXXXXX

  JsonObject runtime = doc["runtime"];

  const char * messagetype_char = runtime["messagetype"];

  String messagetype = String(messagetype_char);
  // MQTTLogMessage("runtime[\"messagetype\"] :");
  // MQTTLogMessage(runtime["messagetype"]);
  // MQTTLogMessage("messagetype :"+messagetype);

  //Serial.printf("messagetype %s\n", messagetype.c_str());

  if (messagetype.equals("PING") || messagetype.equals("REFRESH")) {

    //    MQTTLogMessage("UDP received 2");

    //char hostName_char[20];
    //strcpy (hostName_char,"XXXXXXXXXXXXXXXX");
    const char * hostName_char = runtime["hostName"];
//    MQTTLogMessage(String("UDP received :") +messagetype +" / "+   hostName_char );

    const char * ip_char = runtime["ip"];
    const char * firmware_version_char = runtime["firmware_version"];
    const char * spiffs_version_char = runtime["spiffs_version"];
    const char * uptime_char = runtime["uptime"];

    String sunset_from_packet = String("");

    if (strcmp(runtime["sunset"], "") != 0) {

      const char * sunset_from_packet_char = runtime["sunset"];
      sunset_from_packet = String(sunset_from_packet_char);
    }

    //  MQTTLogMessage("runtime[\"sunset\"]");
    //  MQTTLogMessage(runtime["sunset"]);

    if (runtime["sunrise"] == NULL) MQTTLogMessage("runtime[\"sunrise\"]=NULL");

    String sunrise_from_packet = String("");
    //jesli NIE pusty sunrise
    if (strcmp(runtime["sunrise"], "") != 0) {
      const char * sunrise_from_packet_char = runtime["sunrise"];
      sunrise_from_packet = String(sunrise_from_packet_char);

    }

    //  MQTTLogMessage("runtime[\"sunrise\"]");
    //  MQTTLogMessage(runtime["sunrise"]);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /////////////////TU JAK DALEM RETURN TO DZIAŁAŁO/XXXXXXXXXXXXXXX

    String sunset_update_time_from_packet = String("");

    // MQTTLogMessage(String(strcmp(runtime["sunset_update_time"],"")));

    if (strcmp(runtime["sunset_update_time"], "") != 0) {
      const char * sunset_update_time_from_packet_char = runtime["sunset_update_time"];
      sunset_update_time_from_packet = String(sunset_update_time_from_packet_char);
      //  MQTTLogMessage("sunset_update_time NOT empty");
    }

    // MQTTLogMessage("runtime[\"sunset_update_time\"]");
    //  MQTTLogMessage(runtime["sunset_update_time"]);

    //MDNSObject MDNSObjectUDP;

    const char * hostname_char = runtime["hostName"];

    //  MQTTLogMessage(runtime["hostName"]);
    //MDNSObjectUDP.hostName= String(hostname_char );

    //char hostName2[40];
    //strcpy(hostName2  ,hostname_char );
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //MQTTLogMessage(hostname_char);

    //char hostNamexxx[20];
    //strcpy(hostNamexxx,"XXXX");
    //strcpy(MDNSObjectUDP.hostName,hostname_char );

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX TU JAK DALEM RETURN TO DZIAŁAŁO/XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    //

    //strcpy(MDNSObjectUDP.hostName,hostname_char );

    //MDNSObjectUDP.ip= String(ip_char);
    //strcpy(MDNSObjectUDP.ip,ip_char);
    //strcpy(MDNSObjectUDP.ip,ip_char);

    //strcpy(MDNSObjectUDP.firmware_version,firmware_version_char);
    //strcpy(MDNSObjectUDP.firmware_version,firmware_version_char);

    //MQTTLogMessage("NTP.getTimeDateString().c_str()");
    //MQTTLogMessage("X");
    //MQTTLogMessage(firmware_version_char);

    //MQTTLogMessage(NTP.getTimeDateString().c_str());

    //strcpy(MDNSObjectUDP.timestamp,NTP.getTimeDateString().c_str());
    //strcpy(MDNSObjectUDP.timestamp,NTP.getTimeDateString().c_str());

    //if (uptime_char!=NULL)strcpy(MDNSObjectUDP.uptime,uptime_char );
    //else strcpy(MDNSObjectUDP.uptime,"");

    //if (uptime_char!=NULL)strcpy(MDNSObjectUDPNew.uptime,uptime_char );
    //else
    //strcpy(MDNSObjectUDP.uptime,"");

    //MQTTLogMessage(uptime_char);

    /////////////////TU JAK DALEM RETURN TO RESTART/XXXXXXXXXXXXXXX



    if (!WiFi.localIP().toString().equals(String(ip_char))) {

      MDNSObject MDNSObjectUDP;

      strcpy(MDNSObjectUDP.hostName, hostName_char);
      strcpy(MDNSObjectUDP.ip, ip_char);
      strcpy(MDNSObjectUDP.firmware_version, firmware_version_char);
      if (spiffs_version_char != NULL) strcpy(MDNSObjectUDP.spiffs_version, spiffs_version_char);
      else strcpy(MDNSObjectUDP.spiffs_version, "");
      strcpy(MDNSObjectUDP.timestamp, NTP.getTimeDateString().c_str());
      strcpy(MDNSObjectUDP.uptime, uptime_char);

      ///////////////////////
      //      MDNSObjectUDP.MDNSObjectRelays=MDNSObjectRelays;
     


     if (runtime.containsKey("relays"))

    {
 ///////////////////////relays///////////////////////

//      MQTTLogMessage("Relays exist");

      JsonArray relays = runtime["relays"];
      byte size = relays.size();

//           MQTTLogMessage(String(">>>>>>>>>>>>>>>>>>relays.size():") + relays.size());

      //LinkedList<MDNSObjectRelay> MDNSObjectRelays;
      //    MDNSObjectRelay MDNSObjectRelays[size];

      for (int i = 0; i < size; i++) {

        JsonObject relay = relays[i];

        //const char * relay_id = relay["id"];
        //const char * state = relay["state"];
        //        MQTTLogMessage(String(">>>>>>>>>>>>>>>>>>>>>Init"));

        //MDNSObjectRelay MDNSObjectRelayUDP;
        //MDNSObjectRelayUDP.id=relay["id"];
        //MDNSObjectRelayUDP.state=relay["state"];
        //        MQTTLogMessage(String(">>>>>>>>>>>>>>>>>>>>>Adding ")+MDNSObjectRelayUDP.id+""+"/"+MDNSObjectRelayUDP.state);
        //        strcpy(MDNSObjectUDP.MDNSObjectRelays[i].id, relay_id );
        //        strcpy(MDNSObjectUDP.MDNSObjectRelays[i].state, state);
        MDNSObjectUDP.MDNSObjectRelays[i].id = relay["id"];
        
        strcpy(MDNSObjectUDP.MDNSObjectRelays[i].internalName, relay["internalName"]);
//        MQTTLogMessage(String("IN internalName ")+MDNSObjectUDP.MDNSObjectRelays[i].internalName);


        MDNSObjectUDP.MDNSObjectRelays[i].state = relay["state"];
        //        MDNSObjectUDP.MDNSObjectRelays.add(MDNSObjectRelayUDP);


        //        MDNSObjectRelays[0] = MDNSObjectRelayUDP;
        //        MQTTLogMessage(String(">>>>>>>>>>>>>>>>>>>>>Added Relays"));



      }//  for (int i = 0; i < size; i++) {


      ///////////////////////replays///////////////////////

    }//if (runtime.containsKey("runtime"))
  //  else       MQTTLogMessage("Relays dont exist");




      addMDNSObjectToList(MDNSObjectUDP);
      //      MQTTLogMessage(String(">>>>>>>>>>>>>>>>>>>>>Added Object"));
    }

    //if (!WiFi.localIP().toString().equals(String(ip_char)   ) ) addMDNSObjectToList(doc);
    //if (!WiFi.localIP().toString().equals(String(ip_char)   ) ) addMDNSObjectToList(runtime);
    //if (!WiFi.localIP().toString().equals(String(ip_char)   ) ) addMDNSObjectToList(hostName_char , ip_char,firmware_version_char ,NTP.getTimeDateString().c_str(),uptime_char );

    // jeśli przyszła w danych aktualna data aktualizacji zachodu i wschodu slonca
    if (sunset_update_time_from_packet != NULL) {
      //    MQTTLogMessage("UDP received 5");

      //Serial.printf("sunset_update_time_from_packet [%s]\n", sunset_update_time_from_packet.c_str());
      //Serial.printf("sunset_from_packet [%s]\n", sunset_from_packet_char);
      //Serial.printf("sunset_from_packet [%s]\n", sunset_from_packet.c_str());
      if (strcmp(runtime["sunset_update_time"], "") != 0) updateSunriseSunsetInfo(sunset_update_time_from_packet, sunrise_from_packet, sunset_from_packet);
      //   MQTTLogMessage("UDP received 6");

    }

  } //if (messagetype.equals("PING"))

  if (messagetype.equals("REFRESH")) sendUDPPing();

  //    MQTTLogMessage("UDP received END");
  MQTTLogMessage("UDP received END");

}

void addMDNSObjectToList(MDNSObject MDNSObjectUDP)



//void addMDNSObjectToList(const char* hostName,const char* ip ,const char* firmware_version ,const char* timestamp ,const char* uptime)
//void addMDNSObjectToList(JsonObject runtime)
//void addMDNSObjectToList(DynamicJsonDocument doc)
{

//  MQTTLogMessage(String("addMDNSObjectToList START: ")+MDNSObjectUDP.hostName);


/*
    byte relays_size=2;

    for (int ii = 0; ii < relays_size ; ii++) {

      MQTTLogMessage(String("INNNN id: ")+MDNSObjectUDP.MDNSObjectRelays[ii].id );
      MQTTLogMessage(String("INNNN internalName: ")+MDNSObjectUDP.MDNSObjectRelays[ii].internalName );
    }//for
*/


  /*
      Serial.printf("addMDNSObjectToList\n");
    //      Serial.printf("hostName [%s] ip [%s] firmware_version [%s] timestamp [%s] uptime [%s]\n",hostName, ip,firmware_version,timestamp, uptime);
      Serial.printf("hostName [%s] \n",hostName);
      Serial.printf("ip [%s]\n",ip);
      Serial.printf("firmware_version [%s]\n",firmware_version);
      Serial.printf("uptime [%s]\n",uptime);
      Serial.printf("timestamp [%s]\n",timestamp);

      Serial.printf("addMDNSObjectToList 2\n");
  */
  /*
        serializeJson(doc, Serial);
        Serial.printf("\n");
        serializeJson(doc["runtime"], Serial);
        Serial.printf("\n");

    //JsonObject runtime= doc["runtime"]   ;

    JsonObject runtime = doc.as<JsonObject>();

    //iterate
    // using C++11 syntax (preferred):
    for (JsonPair kv : runtime) {
      Serial.println(kv.key().c_str());
      Serial.println(kv.value().as<char*>());
    }

      Serial.println("hostName");

      Serial.printf("%s",runtime["hostName"]);


  */

  /*

    {"runtime":{"messagetype":"PING","hostName":"wemosxxx","ip":"192.168.1.211","firmware_version":null}}

    {"messagetype":"PING","hostName":"wemosxxx","ip":"192.168.1.211","firmware_version":null}

  */
  /*


        serializeJson(runtime, Serial);

         Serial.printf("\n\n");

    bool has = runtime.containsKey("hostName"); // true
        Serial.printf("has %d\n",has);

        Serial.printf("messagetype %s\n",runtime["messagetype"]);
        Serial.printf("hostName %s\n",runtime["hostName"]);
  */

  //LogMessage("hostName");
  //  MQTTLogMessage(runtime["hostName"]);
  //     Serial.printf("%s\n",MDNSObjectUDP.hostName.c_str());
  //    Serial.printf("%s\n",MDNSObjectUDP.ip.c_str());
  //   Serial.printf("%s\n",MDNSObjectUDP.firmware_version.c_str());
  //  Serial.printf("%s\n",MDNSObjectUDP.timestamp.c_str());

  //     Serial.printf("listing list\n");

  int size = MDNSObjects.size();

  for (int i = 0; i < size; i++) {
    MDNSObject MDNSObjectI = MDNSObjects.get(i);

    //  Serial.printf("%s\n",MDNSObjectI.hostName.c_str());

    //  if ( MDNSObjectI.ip.equals(MDNSObjectUDP.ip) && MDNSObjectI.hostName.equals(MDNSObjectUDP.hostName)    )
    if (
      strcmp(MDNSObjectI.ip, MDNSObjectUDP.ip) == 0 && strcmp(MDNSObjectI.hostName, MDNSObjectUDP.hostName) == 0
    )

    {
      MDNSObjects.set(i, MDNSObjectUDP);
      return;
    } //if

  } //for

  MDNSObjects.add(MDNSObjectUDP);
  //MQTTLogMessage(String("addMDNSObjectToList END: ")+MDNSObjectUDP.hostName);

}

String getDNSDevices() {

  String content = "";
  content = content + "{ \"MDNSDevices\" : [\n";
  /*
    Serial.println("Sending mDNS query");
    int n = MDNS.queryService("esp", "tcp"); // Send out query for esp tcp services
    Serial.println("mDNS query done");
    if (n == 0) {
      Serial.println("no services found");
    }

      Serial.print(n);
      Serial.println(" service(s) found");
  */

  int size = MDNSObjects.size();

  for (int i = 0; i < size; ++i) {



    MDNSObject MDNSObjecti = MDNSObjects.get(i);



    ////////////////////////////////////////////////
    String relays = ", \"relays\" : [";

    byte relays_size = 4;//MDNSObjecti.MDNSObjectRelays.size();
    byte pos = 0;
    for (int ii = 0; ii < relays_size ; ii++) {
      if (MDNSObjecti.MDNSObjectRelays[ii].id < 0) continue;

      if (pos > 0)relays = relays + ",";

      relays = relays +  "{ \"id\":\"" + MDNSObjecti.MDNSObjectRelays[ii].id + "\"" ;
      relays = relays +  ", \"internalName\":\"" + MDNSObjecti.MDNSObjectRelays[ii].internalName + "\" " ;
      relays = relays +  ", \"state\":\"" + MDNSObjecti.MDNSObjectRelays[ii].state + "\" " ;
      relays = relays +  " }" ;

      pos++;
    }//for

    relays = relays + "]";

    //MQTTLogMessage(String("relayString: ")+MDNSObjecti.hostName+":"+ relays);
    //MQTTLogMessage(String("pos: ")+pos);
    //if (pos==0)relays="";
    //MQTTLogMessage(String("relayStringNEW: ")+relays);

    ///////////////////////////////////////////////




    //    MDNSTxt mdnsTxt=

    if (i > 0) content = content + ",";

    content = content + "{" + "\"hostname\":\"" + MDNSObjecti.hostName + "\"," + "\"IP\":\"" + MDNSObjecti.ip + "\"" +
              ",\"firmware_version\":\"" + MDNSObjecti.firmware_version + "\"" +
              ",\"spiffs_version\":\"" + MDNSObjecti.spiffs_version + "\"" +

              "  ,\"uptime\":\"" + MDNSObjecti.uptime + "\",\"timestamp\":\"" + MDNSObjecti.timestamp + "\"     " + relays + "   }\n";

    /*
          Serial.print(i + 1);
          Serial.print(": ");
          Serial.print(MDNS.hostname(i));
          Serial.print(" (");
          Serial.print(MDNS.IP(i));
          Serial.print(":");
          Serial.print(MDNS.port(i));
          Serial.println(")");
    */

    //      content = content +relays;

  } //for

  content = content + " ]}";

  // Serial.printf("%s\n",content.c_str());

  return content;

}
