#include <stdlib.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

#define MQTT_MAX_PACKET_SIZE 2560

#include <PubSubClient.h>

#include <ArduinoJson.h>

#include <NtpClientLib.h>

#include "util.h"

#include "udp.h"

#include "device.h"

#include "mqtt.h"

WiFiClient espClient;
PubSubClient mqttclient(espClient);

bool MQTTpublish(String topic, String message) {

  if (!WiFi.isConnected() || conf.MQTT_enable == false) return true;

  if (conf.interruptProcess == false) mqttclient.publish(topic.c_str(), message.c_str());

  return true;
}

bool MQTTpublish(String message) {

  //  mqttclient.publish("domoticz/in", message.c_str());

  return MQTTpublish("domoticz/in", message);
}

bool MQTTLogMessage(String message)
//bool MQTTLogMessage(const char *message)
{
  if (conf.interruptProcess == false) Serial.printf("MQTTLog [%s]\n", message.c_str());
  //Serial.printf("MQTTLog [%s]\n",message); 
  return true;
  //  return MQTTpublish("itzone/device/"+conf.hostName+"/log", NTP.getTimeDateString()+":"+message);
}

bool getDevices(String url) {
  HTTPClient httpclient;

  //String url =String("http://")+conf.Domoticz_userName+":"+conf.Domoticz_password+"@"+conf.Domoticz_hostName+":"+ conf.Domoticz_port+"/json.htm?type=devices&filter=all&used=true&order=Name";

  //  Serial.printf("Executing [%s]\n",url.c_str() ); 

  //MQTTLogMessage("getDevices");

  httpclient.begin(url); //HTTP

  int httpRetCode = httpclient.GET();
  String payload;
  //  Serial.printf("ret code [%d]\n",httpRetCode ); 

  if (httpRetCode == HTTP_CODE_OK) {

    int size = httpclient.getSize();

    httpServer.setContentLength(size);
    httpServer.send(200, "text/html", "");

    ///////////////////////////////////////////////////////////
    /*
                    // get lenght of document (is -1 when Server sends no Content-Length header)
                    int len = httpclient.getSize();

                    // create buffer for read
                    uint8_t buff[128] = { 0 };

                    // get tcp stream
                    WiFiClient * stream = httpclient.getStreamPtr();

                    // read all data from server
                    while(httpclient.connected() && (len > 0 || len == -1)) {
                        // get available data size
                        size_t size = stream->available();

                        if(size) {
                            // read up to 128 byte
                            int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

                            // write it to Serial
                            Serial.write(buff, c);
                              httpServer.client().write((uint8_t *)buff, c);
      
                            if(len > 0) {
                                len -= c;
                            }
                        }
                        delay(1);
                    }
    */
    //////////////////////////////////////////////////////////

    payload = httpclient.getString();

    Serial.printf("payload %s\n", payload.c_str());

    httpServer.sendContent(payload);

    httpclient.end();
    //MQTTLogMessage("getDevices end");

    return true;

  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", httpclient.errorToString(httpRetCode).c_str());

    MQTTLogMessage(String("[HTTP] GET... failed, error: ") + httpclient.errorToString(httpRetCode));

    httpclient.end();
    return false;
  }

}

void callback(char * topic, byte * payload, unsigned int length) {

  //  Serial.print("MQTT message arrived [");
  //  Serial.print(topic);
  //  Serial.print("] ");

  char json[length];

  int i = 0;
  for (i = 0; i < length; i++) {
    //    Serial.print((char)payload[i]);
    json[i] = (char) payload[i];
  }
  //  Serial.println();

  json[i] = '\n';

  //Serial.printf("callback json");

  Serial.printf("callback json %s\n", json);

  String payloadString = String(json);

  //if (strcmp(topic, runtime.pingtopic.c_str()) ==0 ){processUDPMQTTCallback(payloadString );return;}

  //  StaticJsonBuffer<1000> jsonBuffer;

  if (isValidJson(payloadString) == false) return;

  DynamicJsonDocument doc(length);
  deserializeJson(doc, json);
  JsonObject root = doc.to < JsonObject > ();

  /*
  DynamicJsonDocument jsonBuffer(conf.json_max_length); 

  JsonObject& root = jsonBuffer.parseObject(json);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
*/
  const char * idx = root["idx"];
  const char * nvalue = root["nvalue"];

  int idx_int = atoi(idx);

  //  Serial.printf("status idx %d nvalue [%s]\n", idx_int,nvalue );

  int nvalue_int = atoi(nvalue);

  for (i = 0; i < 2; i++) {

    if (idx_int == conf.relays[i].domoticz_device_idx && nvalue_int != conf.relays[i].relay_state) {

      MQTTLogMessage(String("new mqtt message for relay ") + i + " domoticz idx " + idx_int + " value " + nvalue_int);

      conf.relays[i].relay_state = nvalue_int;

      setRelayStatePermanently(i, nvalue_int);
      saveConfig();
    } //if
  } //for

  /*
  else if (idx_int==conf.relays[1].domoticz_device_idx) 
  {
    int nvalue_int= atoi(nvalue );

    MQTTLogMessage(String("new mqtt message for relay 1 domoticz idx ")+idx_int+" value "+nvalue_int);

    conf.relays[1].relay_state =nvalue_int;
    
    setRelayState(1,nvalue_int);
    saveConfig();
  }

  */

  //getDevices();
  /*
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
      digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
      // but actually the LED is on; this is because
      // it is acive low on the ESP-01)
    } else {
      digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    }
  */
}

void initMQTT() {
  Serial.printf("initMQTT\n");

  mqttclient.setServer(conf.MQTT_hostName.c_str(), 1883);
  mqttclient.setCallback(callback);
  MQTTConnect();
  sendUDPMessageAsync("REFRESH");

}

long MQTTlastReconnectAttempt = -300000;
void MQTTProcess() {

  //  return;
  //    Serial.printf("processMQTT...\n");

  if (!mqttclient.connected()) {

    long now = millis();

    if (abs((long)(millis() - MQTTlastReconnectAttempt)) < 120000) return;

    //    Serial.printf("processMQTT... MQTTlastReconnectAttempt[%d][%d]\n",abs((long)(millis() - MQTTlastReconnectAttempt)) ,MQTTlastReconnectAttempt);

    MQTTlastReconnectAttempt = now;
    MQTTConnect();
  }

  mqttclient.loop();

}

void MQTTConnect() {

  Serial.printf("MQTT connecting\n");
  //    Serial.printf(".");

  int mqttconnected = mqttclient.connect(conf.hostName.c_str());
  if (!mqttconnected) {
    return;
  } //if (!mqttconnected )

  //if (conf.serialLogging==true)Serial.printf("MQTT connected\n");

  MQTTLogMessage("Device started");
  //MQTTLogMessage(String("conf.rgbled.enable loaded ")+String(conf.rgble.enabled) );
  //MQTTLogMessage(String("conf.rgbled.state loaded ")+String(conf.rgbled.state) );

  // Once connected, publish an announcement...
  mqttclient.subscribe("domoticz/out");
  mqttclient.subscribe(runtime.pingtopic.c_str());

  if (conf.relays[0].domoticz_device_idx > 0) {
    String command1 = String("{\"command\": \"getdeviceinfo\", \"idx\": ") + conf.relays[0].domoticz_device_idx + " }";

    MQTTpublish(command1.c_str());
  } //if 

  if (conf.relays[1].domoticz_device_idx > 0) {
    String command2 = String("{\"command\": \"getdeviceinfo\", \"idx\": ") + conf.relays[1].domoticz_device_idx + " }";

    MQTTpublish(command2.c_str());
  } //if 

}
