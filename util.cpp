#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include <ArduinoOTA.h>

#include <ESP8266mDNS.h>

#include <ArduinoJson.h>
 //#include <EEPROM.h>
#include "FS.h"

#include "file.h"

#include "timer.h"

#include "util.h"

#include "util.h"

#include "udp.h"

#include "mqtt.h"

#include <NtpClientLib.h>

#include <LinkedList.h>

LinkedList < AppLog > appLogs = LinkedList < AppLog > ();

//unsigned long OSWATCH_RESET_TIME;

//volatile unsigned long last_loop;

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater(true);

void addAppLogMessage(String message) {

  //  Serial.printf("addAppLogMessage [%s]\n",message.c_str());

  AppLog appLogObject;

  appLogObject.datetime = NTP.getTimeDateString();
  appLogObject.message = message;

  int logsize = appLogs.size();
  //jesi pełna lista logow
  if (logsize == conf.max_applog_size) {

    //    Serial.printf("Max log size exeeded\n");
    appLogs.shift();

  } //if

  appLogs.add(appLogObject);

}

/*
  bool isWebAddressAvailable(String webPage)
  {
  Serial.printf("isWebAddressAvailable %s\n",webPage.c_str());

  bool ret=false;

  HTTPClient client;
  client.begin(webPage);
  //client.begin("http://admin:admin@192.168.1.151:8080/json.htm?type=devices&used=true&filter=all&favorite=1");

  int httpRetCode = client.GET();

  Serial.printf("ret code [%d]\n",httpRetCode );

  //if(httpRetCode == HTTP_CODE_OK) ret=true;

  if(httpRetCode !=-1) ret=true;

  return ret;
  }

*/

void LogMessage(const char * message) {
  Serial.printf("Log [%s]\n", message);
}

String getHourString(byte hour, byte minute) {
  //  MQTTLogMessage(String("getHourString ")+ hour +":"+ minute);
  String ret = "";

  if (hour < 10) ret = ret + "0";
  ret = ret + hour;
  ret = ret + ":";

  if (minute < 10) ret = ret + "0";
  ret = ret + minute;

  if (ret.equals("00:00")) ret = "";

  //  MQTTLogMessage(String("getHourString END ")+ ret);
  return ret;
}

bool isupdateSunriseSunsetUpToDate() {
  bool ret = false;

  String current_date = NTP.getDateStr();

  if (runtime.sunset_update_time.indexOf(current_date) > 0) {
    //    Serial.printf("sunset time is up to date\n");
    //    MQTTLogMessage("Sunset time is up to date. Update time: " + runtime.sunset_update_time);
    //MQTTLogMessage("Sunset time is up to date");
    ret = true;
  }
  return ret;
}

void updateSunriseSunsetInfo(String sunset_update_time, String sunrise, String sunset)

{

  //Serial.printf("updateSunriseSunsetInfo sunrise [%s] sunset [%s]\n",sunrise.c_str(),sunset.c_str());

  //MQTTLogMessage("updateSunriseSunsetInfo START");

  //Serial.printf("updateSunriseSunsetInfo current_date [%s]\n",current_date.c_str());
  //Serial.printf("updateSunriseSunsetInfo runtime.sunset_update_time [%s]\n",runtime.sunset_update_time.c_str());

  if (isupdateSunriseSunsetUpToDate() == true) return;

  runtime.sunset_hour = atoi(sunset.substring(0, 2).c_str());
  runtime.sunset_minute = atoi(sunset.substring(3, 5).c_str());
  runtime.sunrise_hour = atoi(sunrise.substring(0, 2).c_str());
  runtime.sunrise_minute = atoi(sunrise.substring(3, 5).c_str());

  // runtime.sunset_update_time = NTP.getTimeDateString();
  runtime.sunset_update_time = sunset_update_time;
  //      MQTTLogMessage("U1");

  String message = "sunset_update_time updated " + getHourString(runtime.sunrise_hour, runtime.sunrise_minute) + " / sunset " + getHourString(runtime.sunset_hour, runtime.sunset_minute);

  //MQTTLogMessage(message);
  //      MQTTLogMessage("U2");
  addAppLogMessage(message);
  //          MQTTLogMessage("U3");

  //if (conf.discoverable==true)   sendUDPMessageAsync("PING");
  //        MQTTLogMessage("U4");

  //MQTTLogMessage("updateSunriseSunsetInfo END");

  //      MQTTLogMessage("U5");

}

void getSunriseSunset(String key, String city) {

  //  String current_date = NTP.getDateStr();
  //  MQTTLogMessage(String("getSunset current_date:[") + current_date + "]");

  //getSunset [20:20:17 27/08/2017]
  //getSunset current_date [27/08/2017]

  //jeśli nie ma aktualnej daty, to wychodzi
  if (NTP.getTimeDateString().equals("Time not set")) {
    //    MQTTLogMessage("No NTP info. Exiting");

    return;
  }

  if (isupdateSunriseSunsetUpToDate() == true) return;

  //Serial.printf("getSunset [%s]\n",current_time.c_str());
  //Serial.printf("getSunset current_date [%s]\n",current_date.c_str());

  //int ret=0;
  //if (ret==0) {runtime.sunset_update_time=NTP.getTimeDateString(NTP.getFirstSync());return ;}

  bool found_data = false;
  //  Serial.printf("getSunset\n");
  /*
    const char* host = "api.apixu.com";

    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    String url = "/v1/forecast.json?key="+key+"&q="+city;

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }


    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.printf("%s\n",line.c_str());
    }

    Serial.println();
    Serial.println("closing connection");
  */

  //http://api.apixu.com/v1/forecast.json?key=key&q=Warsaw
  //http://api.weatherstack.com/current?access_key=key&query=Warsaw

  HTTPClient http;
  http.begin("http://api.weatherstack.com/forecast?access_key=" + key + "&query=" + city);

  //MQTTLogMessage(String("getSunset requesting"));

  int httpRetCode = http.GET();

  //  MQTTLogMessage(String("getSunset ret code ") + httpRetCode );

  //Serial.printf("ret code [%d]\n", httpRetCode );

  String payload;
  if (httpRetCode == HTTP_CODE_OK) {
    //  payload = client.getString();
    Serial.printf("getSunset payload");

    // get lenght of document (is -1 when Server sends no Content-Length header)
    int len = http.getSize();

    // create buffer for read
    uint8_t buff[512] = {
      0
    };
    //byte buff[128] = { 0 };

    // get tcp stream
    WiFiClient * stream = http.getStreamPtr();

    // read all data from server
    while (http.connected() && (len > 0 || len == -1)) {
      // get available data size
      size_t size = stream->available();

      if (size) {
        // read up to 128 byte
        int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

        // write it to Serial
        //                        Serial.write(buff, c);

        String line = String((const char * ) buff);
        //Serial.printf("read line %s\n", line.c_str());

        int pos_sunset = line.indexOf("sunset");
        int pos_sunrise = line.indexOf("sunrise");

        if (pos_sunset > 0) {
          found_data = true;

                    Serial.printf("Found data %s\n", line.c_str());
          String sunset = line.substring(pos_sunset + 9, pos_sunset + 14);
          //          Serial.printf("sunset: [%s]\n", sunset.c_str());
          //          Serial.printf("sunset_hour: [%s]\n", sunset.substring(0, 2).c_str());
          //          Serial.printf("sunset_minute: [%s]\n", sunset.substring(3, 5).c_str());

          runtime.sunset_hour = atoi(sunset.substring(0, 2).c_str()) + 12;
          runtime.sunset_minute = atoi(sunset.substring(3, 5).c_str());
          //                        byte sunrise_hour;
          //          Serial.printf("runtime.sunset_hour: [%d]\n", runtime.sunset_hour);
          //          Serial.printf("runtime.sunset_minute: [%d]\n", runtime.sunset_minute);

          //MQTTLogMessage(String("Found sanset in data ") + runtime.sunset_hour + ":" + runtime.sunset_minute);
          addAppLogMessage(String("Found sanset in data ") + runtime.sunset_hour + ":" + runtime.sunset_minute);

        }

        if (pos_sunrise > 0) {
          //          Serial.printf("%s\n", line.c_str());
          String sunrise = line.substring(pos_sunrise + 10, pos_sunrise + 15);
          //          Serial.printf("sunrise: [%s]\n", sunrise.c_str());
          //          Serial.printf("sunrise_hour: [%s]\n", sunrise.substring(0, 2).c_str());
          //          Serial.printf("sunrise_minute: [%s]\n", sunrise.substring(3, 5).c_str());

          runtime.sunrise_hour = atoi(sunrise.substring(0, 2).c_str());
          runtime.sunrise_minute = atoi(sunrise.substring(3, 5).c_str());
          //          Serial.printf("runtime.sunrise_hour: [%d]\n", runtime.sunrise_hour);
          //          Serial.printf("runtime.sunrise_minute: [%d]\n", runtime.sunrise_minute);
          //MQTTLogMessage(String("Found sunrise in data ") + runtime.sunrise_hour + ":" + runtime.sunrise_minute);
          addAppLogMessage(String("Found sunrise in data ") + runtime.sunrise_hour + ":" + runtime.sunrise_minute);

        }

        if (len > 0) {
          len -= c;
        }
      }
      delay(1);
    } //

    if (found_data == true)

    {
      //                  MQTTLogMessage("updating sunset_update_time ");

      //updateSunriseSunsetInfo(sunrise, sunset);
      runtime.sunset_update_time = NTP.getTimeDateString();

      if (conf.discoverable == true) sendUDPPing();

    }
    //    Serial.printf("runtime.sunset_update_time %s\n", runtime.sunset_update_time.c_str());
    //    MQTTLogMessage("sunset_update_time " + runtime.sunset_update_time);

    //    Serial.println();
    //    Serial.print("[HTTP] connection closed or file end.\n");

  } // if

  /*
    DynamicJsonDocument jsonBuffer(conf.json_max_length);

    JsonObject root = jsonBuffer.parseObject(payload.c_str());


    // Test if parsing succeeds.
    if (!root.success()) {
      Serial.println("parseObject() failed");
    }

    const char* sunset= root["location"]["forecast"]["astro"]["sunset"];
    Serial.printf("getSunset sunset %s\n", sunset);



    //payload.trim();
  */
  //  Serial.printf("getSunset end\n");

}

void generateToken() {

  //  runtime.token= "thisisSparta" ;

  const char *
    const tokens[] = {
      "0",
      "1",
      "2",
      "3",
      "4",
      "5",
      "6",
      "7",
      "8",
      "9",
      "A",
      "B",
      "C",
      "D",
      "E",
      "F"
    };

  randomSeed(millis());

  int size = sizeof(tokens) / sizeof(tokens[0]);

  runtime.token = String("");
  for (int i = 0; i < 30; i++) {
    //  runtime.token=random(0, 4294967295);
    runtime.token = runtime.token + String(tokens[random(0, size)]);

    //Serial.printf("App token generated [%d]\n",random(0, size));

  }

  //  Serial.printf("\n%s : App token generated [%s]\n", NTP.getTimeDateString().c_str(), runtime.token.c_str());

}

//zwraca string z liczbami oddzielonymi przecinkami od do ilosc
//String getRandomNumbers(int fromincluded, int toincluded, int count)
String getRandomNumbers(String input) {

  String ret = "";
  String inputlocal = input;
  int fromincluded, toincluded, count;

  //Serial.printf("getRandomNumbers1 [%s]\n",inputlocal.c_str());

  inputlocal.replace("Random ", "");
  //Serial.printf("getRandomNumbers2 [%s]\n",inputlocal.c_str());

  int pos = inputlocal.indexOf("/");
  //Serial.printf("pos [%d]\n",pos);

  //ret.remove(0,pos);
  //Serial.printf("getRandomNumbers 3 [%s]\n",ret.c_str());

  fromincluded = inputlocal.substring(0, pos).toInt();

  //Serial.printf("fromincluded [%d]\n",fromincluded);

  inputlocal.remove(0, pos + 1);

  //Serial.printf("getRandomNumbers 4 [%s]\n",inputlocal.c_str());
  pos = inputlocal.indexOf("/");

  toincluded = inputlocal.substring(0, pos).toInt();

  //Serial.printf("toincluded [%d]\n",toincluded);

  pos = inputlocal.indexOf("/");
  //Serial.printf("pos [%d]\n",pos);

  inputlocal.remove(0, pos + 1);
  //Serial.printf("getRandomNumbers 5 [%s]\n",ret.c_str());

  count = inputlocal.toInt();

  //  Serial.printf("%d / %d / %d \n", fromincluded, toincluded, count);

  //generuje liste możliwych wartości.

  LinkedList < int > posibblevalues = LinkedList < int > ();

  //int size = toincluded-fromincluded

  for (int i = fromincluded; i <= toincluded; i++) {
    posibblevalues.add(i);
  } //for

  //Serial.printf("Listuje \n");

  int size = posibblevalues.size();

  for (int i = 0; i < size; i++) {
    int value = posibblevalues.get(i);

    //Serial.printf("value [%d]\n",value);

  } //for

  //  Serial.printf("Losuje \n");

  for (int i = 0; i < count; i++) {
    int size = posibblevalues.size();

    int value;
    if (size > 0) {
      int randomindex = random(0, size);
      value = posibblevalues.get(randomindex);
      posibblevalues.remove(randomindex);
    } else value = random(fromincluded, toincluded + 1);

    //    Serial.printf("value [%d]\n", value);

    if (!ret.equals("")) ret = ret + "," + String(value);
    else ret = ret = String(value);

  } //for

  //  Serial.printf("ret [%s]\n", ret.c_str());

  return ret;
}

void reboot() {

  //MQTTLogMessage("Rebooting...");

  SPIFFS.end();
  ESP.restart();
}

/*
  void ICACHE_RAM_ATTR osWatch(void) {
    unsigned long t = millis();
    unsigned long last_run = abs(t - last_loop);

  Serial.printf("checking wathdog last_loop \n");

  MQTTLogMessage("checking wathdog last_loop ");

  //if (!WiFi.isConnected()) reboot();

  //bool isWebAddressAvailableret= isWebAddressAvailable(String("http://")+ipAddressToString(WiFi.localIP()));

  //if (!isWebAddressAvailableret) reboot();

    if(last_run >= (OSWATCH_RESET_TIME * 1000)) {
      // save the hit here to eeprom or to rtc memory if needed
        reboot();  // normal reboot
        //ESP.reset();  // hard reset
    }
  }
*/
void initWatchdog() {
  /*
    OSWATCH_RESET_TIME=5*60; // 5 minutes
    last_loop = millis();
    tickerOSWatch.attach_ms(((OSWATCH_RESET_TIME) * 1000), osWatch);
  */
}

/*

  #define SECS_PER_MIN  (60UL)
  #define SECS_PER_HOUR (3600UL)
  #define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

  #define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)
  #define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
  #define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
  #define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)

  String getUpTime()
  {
  long val=millis() / 1000;
  int days = elapsedDays(val);
  int hours = numberOfHours(val);
  int minutes = numberOfMinutes(val);
  int seconds = numberOfSeconds(val);

  String ret=String (days)+"d "+String (hours)+"h "+String (minutes)+"m "+String (seconds)+"s";
  return ret;
  }
*/

FirmwareVersionStruct getFirmwareVersionFromNet() {

  HTTPClient client;

strcpy(conf.firmware_url,"http://itzone.pl/updateespmgr/");

char csrtmem[5];
itoa((int)(ESP.getFlashChipRealSize() / 1024.0 / 1024.0), csrtmem, 10);
strcat(conf.firmware_url,csrtmem);
strcat(conf.firmware_url,"MB/version.txt");

  //conf.firmware_url = String("http://itzone.pl/updateespmgr/4MB/version.txt");

  //MQTTLogMessage(conf.firmware_url);

  //    Serial.printf("(int)ESP.getFlashChipSize() : [%d]\n",(int)ESP.getFlashChipSize());

  //    Serial.printf("getFirmwareVersionFromNet : [%s]\n",conf.firmware_url.c_str() );

  //MQTTLogMessage(String("getFirmwareVersionFromNet : ")+conf.firmware_url );
  //MQTTLogMessage(String("getFirmwareVersionFromNet : ") + String((int)(ESP.getFlashChipRealSize() / 1024.0 / 1024.0)) + "MB");

  client.begin(conf.firmware_url); //HTTP

  //client.begin("http://api.apixu.com/v1/forecast.json?key=4fc4966df0e940bbb52171757172108&q=Warsaw");

//delay (2000);
  //MQTTLogMessage("After begin");
//delay (2000);
  int httpRetCode = client.GET();

 // MQTTLogMessage("After GET");
  FirmwareVersionStruct ret;

  //  Serial.printf("ret code [%d]\n", httpRetCode );

  if (httpRetCode == HTTP_CODE_OK) {
    String payload = client.getString();


    //    Serial.println(payload);

    //payload.trim();
    //String version;
    //String firmware;
    //String spiffs;



    if (isValidJson(payload) == false) return ret;

    char inchars[payload.length() + 1];
    strcpy(inchars, payload.c_str());

    DynamicJsonDocument root(payload.length() + 1);
    deserializeJson(root, inchars);
    //JsonObject root = doc.to < JsonObject > ();



//---------------------------    Tu dwa restarty i działa---------------------------


    float firmware_version = root["firmware_version"];
    float spiffs_version = root["spiffs_version"];
    const char * firmware = root["firmware"];
    const char * spiffs = root["spiffs"];


//---------------------------    Tu więcej restartów i działa---------------------------



    ret.firmware_version = firmware_version;
    ret.spiffs_version = spiffs_version;
    //    ret.firmware_version.trim();
    //    ret.spiffs_version.trim();

    ret.firmware = String(firmware);
    ret.spiffs = String(spiffs);
    ret.firmware.trim();
    ret.spiffs.trim();



    addAppLogMessage(String("Versions from net: firmware ") + ret.firmware_version + ", spiffs " + ret.spiffs_version);
    //MQTTLogMessage(String("Versions from net: firmware ") + ret.firmware_version + ", spiffs " + ret.spiffs_version);

    addAppLogMessage(String("Current versions: firmware ") + conf.firmware_version + ", spiffs " + conf.spiffs_version);
    //MQTTLogMessage(String("Current versions: firmware ") + conf.firmware_version + ", spiffs " + conf.spiffs_version);

    //            Serial.printf("ret.version: [%s]\n", ret.version.c_str());
    //            Serial.printf("ret.firmware: [%s]\n", ret.firmware.c_str());
    //            Serial.printf("ret.spiffs: [%s]\n", ret.spiffs.c_str());



  } else {
    Serial.printf("[HTTP GET failed: %s\n", client.errorToString(httpRetCode).c_str());
  }

  client.end();

  return ret;
}

t_httpUpdate_return updateFirmwareFromUrl(String url, byte firmware_type) {
  ESP8266WiFiMulti WiFiMulti;

  t_httpUpdate_return ret;
  // exit if not connected
  if ((WiFiMulti.run() != WL_CONNECTED)) {
    return HTTP_UPDATE_FAILED;
  }

  ESPhttpUpdate.rebootOnUpdate(false);

  /*
    #define FIRMWARE_TYPE_FIRMWARE 1
    #define FIRMWARE_TYPE_SPIFFS 2

  */

  if (firmware_type == FIRMWARE_TYPE_SPIFFS) {

    //    Serial.printf("Update SPIFFS: %s\n", url.c_str());
    ret = ESPhttpUpdate.updateSpiffs(url);
  } //if (firmware_type==FIRMWARE_TYPE_SPIFFS){
  else if (firmware_type == FIRMWARE_TYPE_FIRMWARE) {

    //    Serial.printf("Update Firmware: %s\n", url.c_str());

    ret = ESPhttpUpdate.update(url);

  } //else if (firmware_type==FIRMWARE_TYPE_FIRMWARE){

  switch (ret) {
  case HTTP_UPDATE_FAILED:
    MQTTLogMessage(String("HTTP_UPDATE_FAILD :") + ESPhttpUpdate.getLastErrorString().c_str());

    //      Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
    break;

  case HTTP_UPDATE_NO_UPDATES:
    //      Serial.println("HTTP_UPDATE_NO_UPDATES");
    break;

  case HTTP_UPDATE_OK:
    //      Serial.println("HTTP_UPDATE_OK");
    break;
  }

  return ret;

}

void updateFirmwareFromNet() {
  t_httpUpdate_return ret;

  //  Serial.printf("updateFirmwareFromNet\n");
  //  MQTTLogMessage(String ("updateFirmwareFromNet"));

  FirmwareVersionStruct firmwareVersionFromNet = getFirmwareVersionFromNet();
  //  Serial.printf("current firmware version :%s\n", conf.firmware_version.c_str());
  //  Serial.printf("current spiffs version :%s\n", conf.spiffs_version.c_str());

  //  Serial.printf("firmware version from net :%s\n", firmwareVersionFromNet.firmware_version.c_str());
  //  Serial.printf("spiffs version from net :%s\n", firmwareVersionFromNet.spiffs_version.c_str());

  //  MQTTLogMessage(String ("current firmware version : ")+conf.firmware_version);
  //  MQTTLogMessage(String ("current spiffs version : ")+conf.spiffs_version);
  //  MQTTLogMessage(String ("firmware version from net : ")+firmwareVersionFromNet.firmware_version);
  //  MQTTLogMessage(String ("spiffs version from net : ")+firmwareVersionFromNet.spiffs_version);

  if (firmwareVersionFromNet.firmware_version <= conf.firmware_version && firmwareVersionFromNet.spiffs_version <= conf.spiffs_version)

  {
    //    Serial.println("No new version from net");
    //MQTTLogMessage(String("Firmware is up to date"));

    return;
  }

  //  Serial.println("updating data from net");

  //MQTTLogMessage(String("updating data from net"));

  //Serial.printf("1.11 cmp 1.12 %d \n", strcmp( "1.11" ,"1.12"));
  //Serial.printf("1.12 cmp 1.11 %d \n", strcmp( "1.12" ,"1.11"));

  //  Serial.printf("updating data from net firmware ...[%d] [%s] %d \n", firmwareVersionFromNet.firmware_version.c_str() , conf.firmware_version.c_str(), strcmp( firmwareVersionFromNet.firmware_version.c_str() , conf.firmware_version.c_str() ));
  //  Serial.printf("updating data from net spiffs...[%s] [%s] %d \n", firmwareVersionFromNet.spiffs_version.c_str() , conf.spiffs_version.c_str(), strcmp( firmwareVersionFromNet.spiffs_version.c_str() , conf.spiffs_version.c_str() ));

  // jesli rózna jest wersja firmware
  //                if (!firmwareVersionFromNet.firmware_version.equals(conf.firmware_version) )

  if (firmwareVersionFromNet.firmware_version > conf.firmware_version)

  {

    MQTTLogMessage(String("updating firmware"));

    ret = updateFirmwareFromUrl(firmwareVersionFromNet.firmware, FIRMWARE_TYPE_FIRMWARE);

  } //                if (!firmwareVersionFromNet.firmware_version.equals(conf.firmware_version) )

  if (ret == HTTP_UPDATE_OK) {
    //        Serial.printf("Saving config... \n");
    //        saveConfig();

  }

  // jesli rózna jest wersja spiffs
  //                if (!firmwareVersionFromNet.spiffs_version.equals(conf.spiffs_version) )
  if (firmwareVersionFromNet.spiffs_version > conf.spiffs_version) {

    MQTTLogMessage(String("updating spiffs"));

    ret = updateFirmwareFromUrl(firmwareVersionFromNet.spiffs, FIRMWARE_TYPE_SPIFFS);

    // jeśli udało się zrobić update spiffs
    if (ret == HTTP_UPDATE_OK) {

      saveConfig();

      //      Serial.printf("Restarting... \n");

    } //if(ret==HTTP_UPDATE_OK)

  }

  reboot();

}

/*
  void processFirmwareUpdateSheduler()
  {
  //runtime.start_milis=0;
  //runtime.last_firmware_update=0;

  unsigned long current_milis=millis();

  unsigned long mins_interval=abs(current_milis-runtime.last_firmware_update)/1000/60;

  //  Serial.printf("mins_interval [%d], current_milis [%d], runtime.start_milis [%d], runtime.last_firmware_update [%d]\n",mins_interval,current_milis,runtime.start_milis,runtime.last_firmware_update);

  if (mins_interval>=conf.firmware_update_mins)
  {
  updateFirmwareFromNet();
  runtime.last_firmware_update  =current_milis;
  }

  //delay(3000);

  }
*/
/*
  char* string2char(String instr){

  int size =instr.length();

  char *ret= (char *)malloc(sizeof(char)*size+1);

  int i=0;
  for (;i<size;i++)
  {

  ret[i]=instr.charAt(i);

  }
  ret[i]='\0';

  return ret;

  }
*/

void processAPReboot() {

  unsigned long current_milis = millis();

  unsigned long mins_interval = abs(current_milis - runtime.start_milis) / 1000 / 60;

  //WIFI_AP_STA
  int getMode = WiFi.getMode();

  //if (mins_interval>10 && WiFi.status()!=WL_CONNECTED && !conf.wifi_ssid.equals(""))   {reboot();}
  //if (mins_interval>10 && getMode== WIFI_AP_STA )   {reboot();}
  if (mins_interval > 10 && getMode != WIFI_STA) {

    //MQTTLogMessage("mins_interval=" + mins_interval);
    //MQTTLogMessage("getMode=" + getMode);
    //MQTTLogMessage("WIFI_STA=" + WIFI_STA);

    reboot();

  }

}

/*
char * getConfigParam(char * paramName)
{

//  Serial.print("getConfigParam in ");
//  Serial.println(paramName);

  return "xxx";

}
*/
/*
void writeEEPROM( String in)  {

  Serial.println();

  Serial.println("writeEEPROM");

  int length = in.length();
  int i = 0;
  for (; i < length; i++)
  {

    char value = in.c_str()[i];
    Serial.print(value);
    EEPROM.write( i, value);
  }//for

  EEPROM.write( i, '\0');

  EEPROM.commit( );

  Serial.println();

}
*/
/*
String readEEPROM( )  {
  //  Serial.println();

  Serial.println("readEEPROM");
  int i = 0;
  String str;
  char value;
  while (  true)  {
    value = EEPROM.read( i );
    // Serial.print(value);
    str += (char)(value);

    if (i > 300 || value == '\0') break;

    i++;

  }
  //  Serial.println();

  //Serial.printf("[%s][%d]",str.c_str(),str.length());
  //  Serial.println();
  return str;
}
*/

bool isValidJson(String in ) {

  //    Serial.println("isValidJson0");
  //    Serial.println(in.length());
  //    Serial.println(in.c_str());

  char inchars[ in .length() + 1];
  strcpy(inchars, in .c_str());

  DynamicJsonDocument doc( in .length() + 1);
  auto error = deserializeJson(doc, inchars);

  if (error) {
    MQTTLogMessage(String("Failed to parse string"));
    return false;
  }
  JsonObject json = doc.to < JsonObject > ();

  return true;
}

/*
JsonObject getJsonObjectFromString(String in)
{
 char inchars[runtime.json_buffer_length];
  strcpy (inchars, in.c_str());

 DynamicJsonDocument jsonBuffer(conf.json_max_length);
    Serial.printf("getJsonObjectFromString\n[%s]\n",inchars );
 // json = jsonBuffer.parseObject(inchars);
  
  return &jsonBuffer.parseObject(inchars);
}
*/

bool loadConfigFromString(String in ) {

  //    Serial.printf("loadConfigFromString...size [%d]\n", in.length());

  if (isValidJson( in ) == false) return false;

  //  Serial.printf("loadConfigFromString 2\n");

  // char inchars[runtime.json_buffer_length];
  char inchars[ in .length() + 1];
  strcpy(inchars, in .c_str());

  //    Serial.printf("loadConfigFromString...ichars [%s]\n", inchars);

  DynamicJsonDocument json( in .length() + 1);
  auto error = deserializeJson(json, inchars);

  if (error) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    return false;
  }

  //JsonObject json = doc.to<JsonObject>();  

  //  Serial.printf("loadConfigFromString len [%d]\n", measureJson(json));

  //char srt1[2000];

  //serializeJson(json, srt1);
  //  Serial.printf("loadConfigFromString srt1 [%s]\n", srt1);
  //  Serial.printf("loadConfigFromString len2 [%d]\n", measureJson(json));
  //serializeJson(json, Serial);

  //  Serial.printf("loadConfigFromString 3\n");

  /*
    DynamicJsonDocument jsonBuffer(conf.json_max_length);

    //  StaticJsonBuffer<1400> jsonBuffer;

    JsonObject json = jsonBuffer.parseObject(in);

    if (!json.success()) {
      Serial.printf("Failed to parse config %s\n", in.c_str());
      return false;
    }
  */
  /*
    for (JsonObject::iterator it=json.begin(); it!=json.end(); ++it)
    {
    Serial.printf("[%s] : [%s]\n",it->key,it->value.asString());
    }

    /*
    if (json.containsKey("hostName"))
    {
    Serial.printf("Jest hostname %s\n");

    }
    else   Serial.printf("Nie ma hostname %s\n");
  */

  const char * hostName = json["hostName"];

  //MQTTLogMessage(doc["hostName"]);

  String hostName_String = hostName;

  if (!hostName_String.equals(""))

  {
    conf.hostName = hostName;
  } else {
    char chip[6];
    sprintf(chip, "%06X", ESP.getChipId());
    conf.hostName = String("DEVICE_") + chip;
  }

  //  const char* firmware_version = json["firmware_version"];
  //if (firmware_version !=NULL) conf.firmware_version= firmware_version;

  //  const char* spiffs_version= json["spiffs_version"];

  //if (spiffs_version!=NULL)  conf.spiffs_version= spiffs_version;

  const char * web_username = json["web_username"];
  if (web_username != NULL) conf.web_username = String(web_username);

  const char * broadcast = json["broadcast"];
  if (broadcast != NULL) conf.broadcast = broadcast;

  const char * web_password = json["web_password"];

  if (web_password != NULL) conf.web_password = String(web_password);

  const char * sunsetAPIKey = json["sunsetAPIKey"];
  if (sunsetAPIKey != NULL) conf.sunsetAPIKey = String(sunsetAPIKey);
  const char * sunsetAPICity = json["sunsetAPICity"];
  if (sunsetAPICity != NULL) conf.sunsetAPICity = String(sunsetAPICity);

  const char * wifi_ssid = json["wifi_ssid"];
  String wifi_ssid_String = wifi_ssid;

  if (!wifi_ssid_String.equals("")) {

    conf.wifi_ssid = wifi_ssid;

    //  Serial.printf("Przeczytalem SID: [%s][%s]\n",conf.wifi_ssid.c_str(), wifi_ssid_String.c_str());

  }

  const char * wifi_password = json["wifi_password"];
  String wifi_password_String = wifi_password;

  if (!wifi_password_String.equals("")) {
    conf.wifi_password = wifi_password;
  }

  JsonArray relays = json["relays"];

  //Serial.printf("\n\n\n\n----------------------- relays.size() %d \n\n\n\n\n",relays.size()  );

  //Serial.printf("MQTTLogXXXXXXXXXXX [%s]\n",String(json["wifi_ssid"]).c_str()); 

  // Serial.printf("json[\"wifi_ssid\"] %s\n",json["wifi_ssid"] );

  byte size = relays.size();
  for (int i = 0; i < size; i++) {

    JsonObject relay = relays[i];

    const char * relay_internalName = relay["internalName"];
    String relay_internalName_String = relay_internalName;

    if (relay_internalName_String.equals("")) conf.relays[i].internalName = "Relay " + String(i);
    else conf.relays[i].internalName = relay_internalName_String;

    conf.relays[i].relay_pin = relay["relay_pin"];
    conf.relays[i].relay_switch_pin = relay["relay_switch_pin"];
    conf.relays[i].relay_led_pin = relay["relay_led_pin"];
    conf.relays[i].relay_conn_type = relay["relay_conn_type"];
    conf.relays[i].relay_led_conn_type = relay["relay_led_conn_type"];
    conf.relays[i].relay_state = relay["relay_state"];
    conf.relays[i].domoticz_device_idx = relay["domoticz_device_idx"];


//    Serial.printf("\n\n--load config Przeczytałem relay state = %d na indeksie %d \n\n",conf.relays[i].relay_state , i);


    //delay(4000);

    String relay_switch_type;

    const char * relay_switch_type_char = relay["relay_switch_type"];
    relay_switch_type = relay_switch_type_char;

    if (relay_switch_type.equals("")) relay_switch_type = SWITCH_TYPES[conf.relays[i].relay_switch_type];

    //Serial.printf("ZZZZZZZZZZZZZZZ %s ZZZZZZZZZZZZZZZZZZZZ\n",relay_switch_type.c_str() );

    if (relay_switch_type.equals(SWITCH_TYPES[i])) conf.relays[i].relay_switch_type = 0;
    else if (relay_switch_type.equals(SWITCH_TYPES[1])) conf.relays[i].relay_switch_type = 1;

    //Serial.printf("relay_switch_type  %s   conf.relays[i].relay_switch_type %d\n",relay_switch_type .c_str(),conf.relays[i].relay_switch_type);

  }

  /*

    JsonObject relay0 = relays[0];


    const char* relay_internalName= relay0["internalName"];
    String relay_internalName_String = relay_internalName;

    if (relay_internalName_String .equals("") ) conf.relays[0].internalName="Relay";

    conf.relays[0].relay_pin=relay0["relay_pin"];
    conf.relays[0].relay_switch_pin=relay0["relay_switch_pin"];
    conf.relays[0].relay_led_pin=relay0["relay_led_pin"];
    conf.relays[0].relay_conn_type=relay0["relay_conn_type"];
    conf.relays[0].relay_led_conn_type=relay0["relay_led_conn_type"];
    conf.relays[0].relay_state=relay0["relay_state"];
    conf.relays[0].domoticz_device_idx=relay0["domoticz_device_idx"] ;


    //Serial.printf("\n\n\n\n----------------------- Przeczytałem relay state %d \n\n\n\n\n",conf.relays[0].relay_state );

    //delay(4000);

    String relay_switch_type0 ;

    const char* relay_switch_type_char0= relay0["relay_switch_type"];
    relay_switch_type0  = relay_switch_type_char0;

    if (relay_switch_type0.equals("")) relay_switch_type0 =SWITCH_TYPES[conf.relays[0].relay_switch_type];

    //Serial.printf("ZZZZZZZZZZZZZZZ %s ZZZZZZZZZZZZZZZZZZZZ\n",relay_switch_type.c_str() );

    if (relay_switch_type0.equals(SWITCH_TYPES[0]))conf.relays[0].relay_switch_type=0;
    else if (relay_switch_type0.equals(SWITCH_TYPES[1]))conf.relays[0].relay_switch_type=1;

    //Serial.printf("relay_switch_type  %s   conf.relays[0].relay_switch_type %d\n",relay_switch_type .c_str(),conf.relays[0].relay_switch_type);

    JsonObject relay1 = relays[1];

    conf.relays[1].relay_pin=relay1["relay_pin"];
    conf.relays[1].relay_switch_pin=relay1["relay_switch_pin"];
    conf.relays[1].relay_led_pin=relay1["relay_led_pin"];
    conf.relays[1].relay_conn_type=relay1["relay_conn_type"];
    conf.relays[1].relay_led_conn_type=relay1["relay_led_conn_type"];
    conf.relays[1].relay_state=relay1["relay_state"];
    conf.relays[1].domoticz_device_idx=relay1["domoticz_device_idx"] ;

    String relay_switch_type1;

    const char* relay_switch_type_char1= relay1["relay_switch_type"];
    relay_switch_type1  = relay_switch_type_char1;

    if (relay_switch_type1.equals("")) relay_switch_type1 =SWITCH_TYPES[conf.relays[1].relay_switch_type];

    if (relay_switch_type1.equals(SWITCH_TYPES[0]))conf.relays[1].relay_switch_type=0;
    else if (relay_switch_type1.equals(SWITCH_TYPES[1]))conf.relays[1].relay_switch_type=1;

    //Serial.printf("relay_switch_type  %s   conf.relays[0].relay_switch_type %d\n",relay_switch_type .c_str(),conf.relays[0].relay_switch_type);
  */
  JsonObject temp = json["temperaturesensor"];

  if (temp["tempsensor_pin"] != NULL) conf.temperatureSensor_tempsensor_pin = temp["tempsensor_pin"];
  conf.temperatureSensor_domoticz_device_idx = temp["domoticz_device_idx"];

  JsonObject rgbled = json["rgbled"];
  conf.rgbled.state = rgbled["state"];
  conf.rgbled.enable = rgbled["enable"];
  conf.rgbled.r = rgbled["r"];
  conf.rgbled.g = rgbled["g"];
  conf.rgbled.b = rgbled["b"];
  conf.rgbled.w = rgbled["w"];
  conf.rgbled.wa = rgbled["wa"];

  //MQTTLogMessage(String("conf.rgbled.enable loaded ")+String(conf.rgbled.enable) );
  //Serial.printf("conf.rgbled.enable loaded  %d\n",conf.rgbled.enable);

  if (json["WIFI_STA_RECCONECT_COUNT"] != NULL) conf.WIFI_STA_RECCONECT_COUNT = json["WIFI_STA_RECCONECT_COUNT"];

  //  conf.firmware_update_mins = json["firmware_update_mins"];
  conf.ntpUDPLocalPort = json["ntpUDPLocalPort"];

  const char * ntpserver = json["ntpserver"];

  if (ntpserver != NULL) conf.ntpserver = ntpserver;

  const char * OTA_password = json["OTA_password"];
  if (OTA_password != NULL) conf.OTA_password = OTA_password;

  //const char * OTA_enable = json["OTA_enable"];
  if (json["OTA_enable"] != NULL) conf.OTA_enable = json["OTA_enable"];

  //const char * discoverable = json["discoverable"];
  if (json["discoverable"] != NULL) conf.discoverable = json["discoverable"];

//  const char * network_update = json["network_update"];
//  if (network_update != NULL) conf.network_update = json["network_update"];
  if (json["network_update"] != NULL) conf.network_update = json["network_update"];
// if (strcmp(json["network_update"], "true")==0 ) conf.network_update = 1;
//XXXXXXXXXXXXXXXXXXXXXXXXX
//MQTTLogMessage(json["network_update"]);
//MQTTLogMessage(String("network_update 2 ")+String(network_update) );
//MQTTLogMessage(String("network_update 3 ")+String(conf.network_update) );
 


  const char * MQTT_enable = json["MQTT_enable"];

  if (MQTT_enable != NULL) conf.MQTT_enable = json["MQTT_enable"];

  const char * MQTT_hostName = json["MQTT_hostName"];
  if (MQTT_hostName != NULL) conf.MQTT_hostName = MQTT_hostName;

  const char * Domoticz_hostName = json["Domoticz_hostName"];
  if (Domoticz_hostName != NULL) conf.Domoticz_hostName = Domoticz_hostName;

  const char * Domoticz_port = json["Domoticz_port"];
  if (Domoticz_port != NULL) conf.Domoticz_port = json["Domoticz_port"];

  const char * Domoticz_userName = json["Domoticz_userName"];
  if (Domoticz_userName != NULL) conf.Domoticz_userName = Domoticz_userName;

  const char * Domoticz_password = json["Domoticz_password"];
  if (Domoticz_password != NULL) conf.Domoticz_password = Domoticz_password;

  JsonArray timers = json["timers"];

  byte timers_size = timers.size();

  //Serial.printf("timers_size %d\n",timers_size);

  for (int i = 0; i < timers_size; i++) {
    JsonObject timer = timers[i];
    //{"device_id":0,"function_name":"relay_on","hour":15,"min":15},

    String minute = "";

    const char * minutechar = timer["minute"];
    if (minutechar != NULL) minute = minutechar;

    String hour = timer["hour"];
    String day = timer["day"];
    String month = timer["month"];
    String weekday = timer["weekday"];

    timer1.addTask("USER", timer["device_id"], timer["function_name"], minute, hour, day, month, weekday);
  }

  //JsonObject relay0 = relays[0];

  //conf.relays[0].relay_pin=relay0["relay_pin"];

  return true;
}
/*
  bool loadConfigFromEEPROM(String &content)
  {
  Serial.println("loadConfigFromEEPROM");
  content=readEEPROM( );
  Serial.printf("EEPROM Config [%s]\n",content.c_str());

  //    writeEEPROM( "Marcin Kasinski");

  bool loadConfigRet=loadConfigFromString(content) ;

  if (!loadConfigRet)
  {

    Serial.printf("Nie zaladowal configa \n");
    return false;
  }

  return true;
  }
*/

bool loadConfig() {

  //delay(20000);
  //  Serial.printf("loadConfig [%s]\n", CONF_FILE);

  String content;

  int ret = readFullFile(CONF_FILE, content);

  if (ret == -1) {
    //  Serial.printf("Brak konfiguracji\n");
    char chip[6];
    sprintf(chip, "%06X", ESP.getChipId());
    conf.hostName = String("DEVICE_") + chip;

    return false;
  }

  //    content.replace(",",",\n");

  //Serial.println(content.c_str());

  bool loadConfigRet = loadConfigFromString(content);

  if (!loadConfigRet) {

    //Serial.printf("Nie zaladowal configa \n");
    char chip[6];
    sprintf(chip, "%06X", ESP.getChipId());
    conf.hostName = String("DEVICE_") + chip;
    return false;
  }

  return true;
}

bool saveConfig() {
  //
  MQTTLogMessage(String("saveConfig"));

  //  if (conf.serialLogging == true) Serial.printf("saveConfig() start %s\n", CONF_FILE );

  DynamicJsonDocument json(runtime.json_buffer_length);
  //StaticJsonBuffer<1500> jsonBuffer;

  // JsonObject json = jsonBuffer.createObject();
  json["hostName"] = conf.hostName;
  json["broadcast"] = conf.broadcast;
  //  json["firmware_version"] = conf.firmware_version.c_str();
  //  json["spiffs_version"] = conf.spiffs_version.c_str();
  json["web_username"] = conf.web_username;
  json["web_password"] = conf.web_password;

  json["sunsetAPIKey"] = conf.sunsetAPIKey;
  json["sunsetAPICity"] = conf.sunsetAPICity;

  json["wifi_ssid"] = conf.wifi_ssid;
  json["wifi_password"] = conf.wifi_password;

  //array

  JsonArray relays = json.createNestedArray("relays");

  for (int i = 0; i < 2; i++) {
    if (conf.relays[i].relay_pin < 0) continue;
    JsonObject relay = relays.createNestedObject();

    relay["internalName"] = conf.relays[i].internalName;
    relay["relay_pin"] = conf.relays[i].relay_pin;

    relay["relay_conn_type"] = conf.relays[i].relay_conn_type;
    relay["relay_switch_pin"] = conf.relays[i].relay_switch_pin;
    relay["relay_state"] = conf.relays[i].relay_state;

//    Serial.printf("\n\n--save config zapisałem relay state = %d na indeksie %d \n\n",conf.relays[i].relay_state , i);
//    Serial.printf("\n\n--save config zapisałem relay state = %d na indeksie %d \n\n",relay["relay_state"] , i);

    relay["relay_led_pin"] = conf.relays[i].relay_led_pin;
    relay["relay_led_conn_type"] = conf.relays[i].relay_led_conn_type;
    relay["domoticz_device_idx"] = conf.relays[i].domoticz_device_idx;

    char switch_type[10];

    strcpy(switch_type, SWITCH_TYPES[conf.relays[i].relay_switch_type]);

    // Serial.printf("--------------------  %s\n", switch_type );

    //    relay["relay_switch_type"]  = jsonBuffer.strdup(switch_type);
    relay["relay_switch_type"] = switch_type;

  }

  JsonObject temp = json.createNestedObject("temperaturesensor");

  temp["tempsensor_pin"] = conf.temperatureSensor_tempsensor_pin;
  temp["domoticz_device_idx"] = conf.temperatureSensor_domoticz_device_idx;

  // Serial.printf("Saving RGB %d\n",conf.rgbled.rgbled_state);

  JsonObject rgbled = json.createNestedObject("rgbled");

  rgbled["enable"] = conf.rgbled.enable;
  rgbled["state"] = conf.rgbled.state;
  rgbled["r"] = conf.rgbled.r;
  rgbled["g"] = conf.rgbled.g;
  rgbled["b"] = conf.rgbled.b;
  rgbled["w"] = conf.rgbled.w;
  rgbled["wa"] = conf.rgbled.wa;

  //MQTTLogMessage(String("conf.rgbled.enable saved ")+String(conf.rgbled.enable) );
  //MQTTLogMessage(String("conf.rgbled.state saved ")+String(conf.rgbled.state) );

  // Serial.printf("Saving RGB END %d\n",conf.rgbled.rgbled_state);

  json["WIFI_STA_RECCONECT_COUNT"] = conf.WIFI_STA_RECCONECT_COUNT;
  //  json["firmware_update_mins"] = conf.firmware_update_mins;
  json["ntpUDPLocalPort"] = conf.ntpUDPLocalPort;
  json["ntpserver"] = conf.ntpserver;

  json["OTA_password"] = conf.OTA_password;
  json["OTA_enable"] = conf.OTA_enable;

  json["discoverable"] = conf.discoverable;

  json["network_update"] = conf.network_update;
  json["MQTT_enable"] = conf.MQTT_enable;

  json["MQTT_hostName"] = conf.MQTT_hostName;

  json["Domoticz_hostName"] = conf.Domoticz_hostName;
  json["Domoticz_port"] = conf.Domoticz_port;
  json["Domoticz_userName"] = conf.Domoticz_userName;
  json["Domoticz_password"] = conf.Domoticz_password;

  JsonArray timers = json.createNestedArray("timers");

  int listSize = timer1.cronObjects.size();

  //  Serial.printf(" listSize %d\n",listSize );
  for (int i = 0; i < listSize; i++) {

    //if (timer1.timerObjects[i].function_name==NULL) break;
    if (timer1.cronObjects.get(i).function_name == NULL) break;
    if (timer1.cronObjects.get(i).type.equals("SYSTEM")) continue;

    JsonObject timer = timers.createNestedObject();

    CronObject cronObject = timer1.cronObjects.get(i);

    timer["device_id"] = timer1.cronObjects.get(i).device_id;
    timer["function_name"] = timer1.cronObjects.get(i).function_name;

    timer["minute"] = cronObject.minute_logical;
    timer["hour"] = cronObject.hour_logical;
    timer["day"] = cronObject.day_logical;
    timer["month"] = cronObject.month_logical;
    timer["weekday"] = cronObject.weekday;

  }

  //  Serial.printf("JSON timers.length()  %d\n", timers.size() );

  String str;
  //  json.printTo(configFile);

  ////int len= str.length();
  //MQTTLogMessage(String("Str len:"+len) );

  //  json.prettyPrintTo(str);
  serializeJsonPretty(json, str);

  //  Serial.printf("save in util--------------------  %s\n", str.c_str() );

  //  char data[1500];
  //  json.printTo(data,1500);

  //  MQTTLogMessage(String("Data ")+String(data) );

  //    Serial.printf("Saving \n%s\n",str.c_str());

  //SPIFFS.remove(CONF_FILE );

  bool savefileret = saveFile(CONF_FILE, str);

  if (savefileret == false) return false;

  MQTTLogMessage(String("Conf saved"));

  return true;
  //  return false;
}

bool processConfig() {

  //  Serial.printf("processConfig\n");

  bool loadConfigRet = loadConfig();

  int retsave;

  if (!loadConfigRet) retsave = saveConfig();

}

void sendToDomoticz() {
  // int status = WL_IDLE_STATUS;     // the Wifi radio's status
  WiFiClient client;

  // domoticz

  int port = 8080; //Domoticz port
  int idx = 14051; //IDX for this virtual sensor, found in Setup -> Devices

  float humidity = 0;
  float temp = 22.2;

  // Serial.println("sending temp");

  if (client.connect(conf.Domoticz_hostName.c_str(), port)) {

    client.print("GET /json.htm?type=command&param=udevice&idx=");
    client.print(idx);
    client.print("&nvalue=0&svalue=");
    client.print(temp);
    //    client.print(";");
    //      client.print(humidity);
    //        client.print(";0"); //Value for HUM_STAT. Can be one of: 0=Normal, 1=Comfortable, 2=Dry, 3=Wet
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.print(conf.Domoticz_hostName);
    client.print(":");
    client.println(port);
    client.println("User-Agent: Arduino-ethernet");
    client.println("Connection: close");
    client.println();

    client.stop();
  }

  //Serial.println("Temp sent");

}

void initWWW() {
  //ESP8266WebServer httpServer(80);
  //ESP8266HTTPUpdateServer httpUpdater(true);

}

String scanNetworks() {

  String ret = "";

  int n = WiFi.scanNetworks();
  //  Serial.println("scan done");

  if (n == 0) {
    //    Serial.println("no networks found");
    ret = "";
  } else {

    //    node_t ssids [n];

    //    Serial.print(n);
    //    Serial.println(" networks found");

    ret = ret + "{\"networks\":[";

    String auth;

    for (int i = 0; i < n; ++i) {

      if (i > 0) ret = ret + ",";

      if (WiFi.encryptionType(i) == ENC_TYPE_NONE) auth = "open";
      else if (WiFi.encryptionType(i) == ENC_TYPE_WEP) auth = "WEP";
      else if (WiFi.encryptionType(i) == ENC_TYPE_TKIP) auth = "WPA - PSK";
      else if (WiFi.encryptionType(i) == ENC_TYPE_CCMP) auth = "WPA2 - PSK";
      else if (WiFi.encryptionType(i) == ENC_TYPE_AUTO) auth = "mixed WPA/WPA2 - PSK";
      else auth = "*";

      ret = ret + "{" +
        "\"name\":\"" + WiFi.SSID(i) + "\"," +
        "\"bssid\":\"" + WiFi.BSSIDstr(i) + "\"," +
        "\"channel\":\"" + WiFi.channel(i) + "\"," +
        "\"encryptionType\":\"" + auth + "\"," +
        "\"rssi\":\"" + WiFi.RSSI(i) + "\"" +
        "}";

      //    ssids[i].SSID=WiFi.SSID(i);

      //    ssids[i].BSSID=WiFi.BSSIDstr(i);

      // Print SSID and RSSI for each network found
      /*
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print("  ");
      Serial.print(WiFi.BSSIDstr(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      */
      //      delay(10);
    }
  }

  ret = ret + "]}";

  Serial.println(ret.c_str());
  return ret;
}

String savedConf;
bool confwassaved;

CONF conf;
RUNTIME runtime;

void processOTA() {
  ArduinoOTA.handle();
}

/*
  void initMDNS()
  {
  Serial.printf("initMDNS\n");
  if (!MDNS.begin(conf.hostName.c_str())) {
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.printf("mDNS responder started %s\n",conf.hostName.c_str());

  //  MDNS.addServiceTxt("esp", "tcp", "host", conf.hostName.c_str());
  //  MDNS.setInstanceName(conf.hostName);

  if (conf.discoverable) MDNS.addService("esp", "tcp", 80);

  }
*/
void initOTA() {

  Serial.printf("initOTA\n");

  static int last_percent_prograss;

  ArduinoOTA.onStart([]() {

    MQTTLogMessage("OTA Start");

    last_percent_prograss = -1;
  });
  ArduinoOTA.onEnd([]() {

    MQTTLogMessage("OTA End");

    saveConfig();

  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {

    int percent_prograss = (progress / (total / 100));

    int mod = percent_prograss % 10; // x now contains 4

    if (mod == 0 && last_percent_prograss != percent_prograss)

    {
      //      Serial.printf("Progress: %u%%\r", percent_prograss);
      MQTTLogMessage(String("OTA End ") + percent_prograss + "%");

      last_percent_prograss = percent_prograss;
    }

  });
  ArduinoOTA.onError([](ota_error_t error) {
    MQTTLogMessage(String("ERROR ") + error);
    if (error == OTA_AUTH_ERROR) MQTTLogMessage("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) MQTTLogMessage("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) MQTTLogMessage("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) MQTTLogMessage("Receive Failed");
    else if (error == OTA_END_ERROR) MQTTLogMessage("End Failed");
  });

  ArduinoOTA.setPassword(conf.OTA_password.c_str());

  ArduinoOTA.setHostname(conf.hostName.c_str());

  ArduinoOTA.begin();

  //  Serial.printf("OTA server started\n");

}
