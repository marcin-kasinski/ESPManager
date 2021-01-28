#define MQTT_MAX_PACKET_SIZE 1024

//void ICACHE_RAM_ATTR handleInterrupt();

#include <NtpClientLib.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include "util.h"

//#include <EEPROM.h>
//#include <ESP8266WiFi.h>
//#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
 //#include <ESP8266mDNS.h>
//#include <ArduinoOTA.h>
#include "view.h"

#include "mqtt.h"

//#include "temp.h"

#include "udp.h"

#include "file.h"

#include "FS.h"

#include "ntp.h"

#include "firmware.h"

#include "device.h"

#include "timer.h"

#include "wifi.h"

//#include "led.h"

char * update_path = "/firmware";

void setup() {

  delay(1000);
  MQTTLogMessage("setup START");
  addAppLogMessage("Device started");

  static WiFiEventHandler e1, e2;

  runtime.start_milis = millis();

  Serial.begin(115200);

  SPIFFS.begin();
  bool processConfigret = processConfig();


  WiFi.hostname(conf.hostName);

  readSpiffsVersion();



  int retsetWIFIClient = 0;

  //if (processConfigret==true) 
  retsetWIFIClient = setWIFIClient();



  if (conf.OTA_enable == true) initOTA();
  if (conf.MQTT_enable == true) initMQTT();


  //Serial.printf("retsetWIFIClient [%d]",retsetWIFIClient); 

  if (!retsetWIFIClient) setWIFIAP();
  if (retsetWIFIClient) initNTPClient();

  httpServer.on("/", handleRoot);
  httpServer.on("/conf", handleConf);
  httpServer.on("/saveconf", handleSaveConf);

  httpServer.on("/reboot", handleReboot);
  httpServer.on("/tasks", handleTasks);
  httpServer.on("/logs", handleLogs);
  httpServer.on("/addtask", handleAddTask);
  httpServer.on("/deletetask", handleDeleteTask);
  httpServer.on("/deleteotherespdevice", handleDeleteOtherEspDevice);

  httpServer.on("/rebootask", handleRebootASK);
  httpServer.on("/listwifinetworks", handleJSONListWifiNetworks);
  httpServer.on("/handleJSONRuntimeInfo", handleJSONRuntimeInfo);
  httpServer.on("/handleJSONListDomoticzDevices", handleJSONListDomoticzDevices);
  httpServer.on("/handleJSONListOtherDevices", handleJSONListOtherDevices);
  httpServer.on("/handleJSONListLogs", handleJSONListLogs);
  httpServer.on("/handleJSONListSheduledTasks", handleJSONListSheduledTasks);

  httpServer.on("/getConfFile", handleGetConfFile);
  httpServer.on("/factoryReset", handleFactoryReset);

  httpServer.on("/handleJSONSwitchChange", handleJSONSwitchChange);


  httpServer.on("/firmware", HTTP_GET, handleFirmwareGet);
  httpServer.on("/firmware", HTTP_POST, handleFirmwarePostAfterFinished, handleFirmwarePost);
  httpServer.on("/firmwareSPIFFS", HTTP_POST, handleFirmwarePostAfterFinished, handleFirmwarePost);
  httpServer.on("/confupload", HTTP_POST, handleConfUploadAfterFinished, handleConfUpload);


  httpServer.on("/login", handleLogin);
  httpServer.onNotFound(handleNotFound); //called when handler is not assigned



  const char * headerkeys[] = {
    "User-Agent",
    "Cookie"
  };
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char * );
  //ask server to track these 
  httpServer.collectHeaders(headerkeys, headerkeyssize);

   MQTTLogMessage("httpServer.begin");

  httpServer.begin();



//  if (conf.rgbled.enable == true) initRGBLed();

  timer1.begin();


if (conf.security_enable== true)
{
  generateToken();

  timer1.addTask("SYSTEM", -1, "Generate App token","", "15", "Every 2", "*", "*", "*");
}

  if (conf.network_update == true) {


    timer1.addTask("SYSTEM", -1, "Firmware update check","", "0", "4", "*", "*", "*");


    updateFirmwareFromNet();
  }



  if (conf.relays[0].relay_led_pin >= 0) timer1.addTask("SYSTEM", 0, "Process relay led","", "0", "*", "*", "*", "*");
  if (conf.relays[1].relay_led_pin >= 0) timer1.addTask("SYSTEM", 1, "Process relay led","", "1", "*", "*", "*", "*");

  //timer1.addTask("SYSTEM",1,"XXX","Random 1/10/4","Random 1/10/4", "Random 1/10/4", "Random 1/10/4","*");
  //timer1.addTask("SYSTEM",1,"XXX","Every 10","Random 1/10/4", "*", "*","*");

  if (conf.temperatureSensor_tempsensor_pin >= 0) {
    //  processTemp();
    timer1.addTask("SYSTEM", -1, "Update temperature","", "Every 10", "*", "", "*", "*");

  }



  timer1.addTask("SYSTEM", -1, "Get sunrise and sunset time","", "Random 0/59/1", "Range 0-21", "*", "*", "*");
  //timer1.addTask("SYSTEM", -1, "Get sunrise and sunset time", "*", "*", "*", "*", "*");
  if (conf.discoverable == true) timer1.addTask("SYSTEM", -1, "Send PING to cluster","", "Every 10", "*", "*", "*", "*");



  initUDP();
  initDevices();

MQTTLogMessage("SETUP END");
  
}

void loop() {
//  processRGBLed();

  //processWIFIRepeater();
  httpServer.handleClient();

  if (conf.discoverable == true) {

    processUDP();
    processUDPMQTTCallback();

  }

  processAPReboot();

  /*
  if (abs(millis()- last_loop) >OSWATCH_RESET_TIME*1000/2)
  {
  //Serial.printf("updating last_loop %d %d\n",(millis()- last_loop),OSWATCH_RESET_TIME*1000 );

  last_loop = millis();
  }
  */

  //if (conf.relays[0].relay_pin>=0 && conf.relays[0].relay_switch_pin>=0 && conf.relays[0].relay_switch_pin_process_method==RELAY_SWITCH_PIN_PROCESS_METHOD_LOOP) processSwitch(0);
  if (conf.relays[0].relay_pin >= 0 && conf.relays[0].relay_switch_pin >= 0 && conf.relays[0].relay_switch_pin_process_method == RELAY_SWITCH_PIN_PROCESS_METHOD_INTERRUPT) processSwitchInterrupt(0);
  if (conf.relays[1].relay_pin >= 0 && conf.relays[1].relay_switch_pin >= 0 && conf.relays[1].relay_switch_pin_process_method == RELAY_SWITCH_PIN_PROCESS_METHOD_INTERRUPT) processSwitchInterrupt(1);
//  if (conf.relays[0].relay_pin>=0 && conf.relays[0].relay_led_pin>=0)  processLed(0) ;
//  if (conf.relays[1].relay_pin>=0 && conf.relays[1].relay_led_pin>=0)  processLed(1) ;

  if (conf.OTA_enable == true) processOTA();

  //if (conf.network_update==true && WiFi.status() == WL_CONNECTED) processFirmwareUpdateSheduler();

  if (conf.MQTT_enable == true) MQTTProcess();

  if (WiFi.getMode() == WIFI_STA) timer1.process();

  //if (conf.relays[1].relay_pin<0) return;

  //if (conf.relays[1].relay_pin>=0 && conf.relays[1].relay_switch_pin>=0 && conf.relays[1].relay_switch_pin_process_method==RELAY_SWITCH_PIN_PROCESS_METHOD_LOOP) processSwitch(1);
  //if (conf.relays[1].relay_pin >= 0 && conf.relays[1].relay_switch_pin >= 0 && conf.relays[1].relay_switch_pin_process_method == RELAY_SWITCH_PIN_PROCESS_METHOD_INTERRUPT) processSwitchInterrupt(1);
  //if (conf.relays[1].relay_pin>=0 && conf.relays[1].relay_led_pin>=0)  processLed(1) ;

  //yield();


      if (runtime.getSunriseSunsetRequest==true) 
      {
        getSunriseSunset(conf.sunsetAPIKey, conf.sunsetAPICity);
        runtime.getSunriseSunsetRequest=false;
      }



  

}
