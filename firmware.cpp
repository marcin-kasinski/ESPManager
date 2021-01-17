
#include <Arduino.h>

#include <WiFiClient.h>

#include <WiFiServer.h>

#include <ESP8266WebServer.h>

#include <WiFiUdp.h>
 //#include "FS.h"

#include "util.h"

#include "mqtt.h"

#include "file.h"

#include "view.h"


const char * _updatefailedResponse = R"(Update Failed!)";
const char * _updatesuccessResponse = "<META http-equiv=\"refresh\" content=\"15;URL=/\">Update Success! Rebooting...";

void handleFirmwarePostAfterFinished() {

  MQTTLogMessage(String("handleFirmwarePostAfterFinished :") + httpServer.uri());

  if (!processToken()) return;

  httpServer.send(200, "text/html", Update.hasError() ? _updatefailedResponse : _updatesuccessResponse);

  // jeśli robiliśmy update SPIFFS zapisujemy konfig

  if (httpServer.uri().equals("/firmwareSPIFFS")) saveConfig();

  //robimy inny crash
  //OneWire  ds(6); 

  reboot();
  // robimy crash
  //*((int*) 0) = 0;

}

void handleFirmwarePost() {

  //    Serial.printf("handleFirmwarePost\n");

  String inToken = httpServer.arg("token");

  //  Serial.printf("handleFirmwarePost red token [%s]\n",inToken.c_str());

  if (!inToken.equals(runtime.token)) {
    //    Serial.printf("Wrong token\n");
    //    return;  
  }

  //if (!processToken()) return;

  // handler for the file upload, get's the sketch bytes, and writes
  // them through the Update object
  HTTPUpload upload = httpServer.upload();
  if (upload.status == UPLOAD_FILE_START) {

    //    SPIFFS.format();
    MQTTLogMessage(String("UPLOAD_FILE_START :") + httpServer.uri());

    Serial.setDebugOutput(true);

    if (!processAuth()) {

      MQTTLogMessage(String("Unauthenticated Update"));
      return;
    }

    WiFiUDP::stopAll();

    int mode = U_FLASH;

    if (httpServer.uri().equals("/firmware")) {

      MQTTLogMessage(String("Update Firmware: ") + upload.filename);
      mode = U_FLASH;
    }

    if (httpServer.uri().equals("/firmwareSPIFFS")) {

      MQTTLogMessage(String("Update SPIFFS: ") + upload.filename);

      //mode = U_SPIFFS;
      mode = U_FS;
    }

    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;

      MQTTLogMessage(String("Update maxSketchSpace: ") + maxSketchSpace);


    if (!Update.begin(maxSketchSpace, mode)) { //start with max available size
      Update.printError(Serial);
    }
  } else if (processAuth() && upload.status == UPLOAD_FILE_WRITE) {
    Serial.printf(".");
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Update.printError(Serial);

    }
  } else if (processAuth() && upload.status == UPLOAD_FILE_END) {
    if (Update.end(true)) { //true to set the size to the current progress

      MQTTLogMessage(String("Update Success. Total size: ") + upload.totalSize);
      MQTTLogMessage(String("Rebooting"));

    } else {
      Update.printError(Serial);
    }
    Serial.setDebugOutput(false);
  } else if (processAuth() && upload.status == UPLOAD_FILE_ABORTED) {
    Update.end();

    MQTTLogMessage(String("Update was aborted"));

  }
  delay(0);

}
