#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <WString.h>

#include "file.h"

#include "FS.h"

#include "mqtt.h"

#include "ntp.h"

#include "udp.h"

#include "view.h"

#include "device.h"

#include "timer.h"

#include "led.h"

#include <ESP8266WebServer.h>

#include <NtpClientLib.h>

#ifndef UTIL_H

#include "util.h"

#endif

/*
String getPrepareInitValuesString()
{


String content ="\n<script>\n";

content =content + "setValue('devicetitle' ,'"+ conf.hostName +" ( ver. "+ conf.firmware_version +" )',''); \n";

content =content + "</script>";

  return content ;
}
*/

bool processToken() {

  String inToken = httpServer.arg("token");
  //Serial.printf("Input token :%s\n",inToken.c_str());

  if (!inToken.equals(runtime.token)) {
    String content = "Wrong application token :" + inToken;

    httpServer.setContentLength(content.length());
    //httpServer.setContentLength(5000);
    httpServer.send(400, "text/html", "");
    httpServer.sendContent(content.c_str());

    return false;

  }

  return true;

}
/*
void handleRestAPI() {

  //Serial.printf("handleRestAPI :%s\n");

  if (!processAuth()) return;

  int index = -1;
  if (httpServer.hasArg("index")) index = httpServer.arg("index").toInt();
  String mode = httpServer.arg("mode");

  Serial.printf("mode: [%s]\n", mode.c_str());
  Serial.printf("index: [%s]\n", index);

  String content = "sdsdlfjsldflsdfj";
  httpServer.setContentLength(content.length());
  httpServer.send(200, "text/html", "");
  httpServer.sendContent(content);

}
*/
void handlePage(String page) {

  if (!processAuth()) return;

  //String content="<script>var token='"+runtime.token+"';\nalert('token '+token);\n</script>";
  String content = "<script>\nvar token='" + runtime.token + "';\n</script>";


//int s1=getWebFileSize("/html/before.php") ;
//int s2=getWebFileSize("/html/after.php") ;
//int s3=getWebFileSize((char *) page.c_str()) ;


  int size = getFileSize("/html/before.php") + getFileSize("/html/after.php") + content.length() + getFileSize((char * ) page.c_str());
  httpServer.setContentLength(size);
  httpServer.send(200, "text/html", "");

  //int ret4= httpServerSendFileContent( "/html/processticket.php");

  httpServer.sendContent(content);
  //int ret= 
  httpServerSendFileContent("/html/before.php");
  //int ret2= 
  httpServerSendFileContent((char * ) page.c_str());
  //int ret3= 
  httpServerSendFileContent("/html/after.php");

  //if (ret==-1) httpServer.send(200, "text/html", "error....");

}

void handleFirmwareGet() {
  //    //Serial.printf("handleFirmwareGet\n");

  handlePage("/html/firmware.php");
}

void handleRoot() {

  handlePage("/html/status.php");

  return;
/*
  if (!processAuth()) return;

  int size = getFileSize("/html/before.php") + getFileSize("/html/after.php") + getFileSize("/html/status.php");

  httpServer.setContentLength(size);
  httpServer.send(200, "text/html", "");

  //int ret= 
  httpServerSendFileContent("/html/before.php");
  //int ret2= 
  httpServerSendFileContent("/html/status.php");
  //int ret3= 
  httpServerSendFileContent("/html/after.php");
  //httpServer.sendContent(content);
  //if (ret==-1) httpServer.send(200, "text/html", "error....");
*/
}

void handleJSONSwitchChange() {
  //Serial.printf("handleJSONSwitchChange()\n");

  if (!processToken()) return;

  int index = -2;
  if (httpServer.hasArg("index")) index = httpServer.arg("index").toInt();

  String content = "";
  content = content + "{\"runtime\" : [\n";
  String switchcmd = "On";

  //    MQTTpublish(String("switch index=")+index);
  //Serial.printf("switch index=%d\n",index);

  // jesli led
  if (index == -1) {
//    RGBLedSetStatePermanently(!conf.rgbled.state);

//    content = content + "{" + "\"n\":\"rgbled.state\"," + "\"v\":\"" + conf.rgbled.state + "\"" + ",\"h\":\"\"}\n";

  }
  //jesli relay
  else if (index >= 0) {
    setRelayStatePermanently(index, !conf.relays[index].relay_state);

    if (conf.relays[index].relay_state == 0) switchcmd = "Off";

    if (conf.MQTT_enable && conf.relays[index].domoticz_device_idx > 0) {
      MQTTpublish(String("{\"command\": \"switchlight\", \"idx\": ") + conf.relays[index].domoticz_device_idx + ", \"switchcmd\": \"" + switchcmd.c_str() + "\"  }");
    }

    content = content + "{" + "\"n\":\"relay_state" + index + "\"," + "\"v\":\"" + conf.relays[index].relay_state + "\"" + ",\"h\":\"\"}\n";

  }

  content = content + " ]}";
  //conf.relays[index].relay_state=!conf.relays[index].relay_state;
  //    saveConfig();

  Serial.println(content.c_str());

  int size = content.length();

  httpServer.setContentLength(size);
  httpServer.send(200, "text/html", "");
  httpServer.sendContent(content.c_str());
  //Serial.printf("handleJSONSwitchChange() END\n");

}

String processCronInputValues(String keyid, String valueid) {
  Serial.printf("processCronInputValues %s %s \n", keyid.c_str(), valueid.c_str());

  String key = httpServer.arg(keyid);
  String value = httpServer.arg(valueid);

  Serial.printf("processCronInputValues %s %s \n", key.c_str(), value.c_str());

  String ret = "";

  if (key.equals("Every")) ret = String("Every ") + value;

  //if (key.equals("EveryTime")) ret="*";

  else if (key.equals("Even") || key.equals("Odd")) ret = key;
  else if (key.equals("Random")) ret = String("Random ") + httpServer.arg(valueid) + "/" + httpServer.arg(valueid + '2') + "/" + httpServer.arg(valueid + '3');
  else if (key.equals("Range")) ret = String("Range ") + httpServer.arg(valueid) + "-" + httpServer.arg(valueid + '2');
  else if (key.equals("At")) {

    int args_size = httpServer.args();

    ret = "";
    for (int i = 0; i < args_size; i++) {
      String namearg = httpServer.argName(i);
      String valuearg = httpServer.arg(i);

      if (namearg.equals(valueid) && !ret.equals("")) ret = ret + "," + valuearg;
      else if (namearg.equals(valueid) && ret.equals("")) ret = ret + valuearg;
    }

  } else ret = "*";

  //Serial.printf("processCronInputValues return %s\n",ret.c_str());

  return ret;
}

void handleDeleteTask() {

  ////Serial.printf("handleDeleteTask\n");

  if (!processToken()) return;

  int index = httpServer.arg("index").toInt();

  //Serial.printf("index %d\n",index);

  timer1.cronObjects.remove(index);
  saveConfig();
  handleJSONListSheduledTasks();
}

void handleAddTask() {

  //Serial.printf("handleAddTask\n");

  //MQTTLogMessage("handleAddTask");

  if (!processToken()) return;

  /*
  int args_size=httpServer.args();

     Serial.printf("Args\n");

  for (int i=0;i<args_size;i++)
      {
        String name=httpServer.argName(i);
        String value=httpServer.arg(i);

     Serial.printf("%s : %s\n",name.c_str(),value.c_str());
        }
  */

  String minutein = processCronInputValues("minutetype", "minutevalue");
  String hourin = processCronInputValues("hourtype", "hourvalue");
  String dayin = processCronInputValues("daytype", "dayvalue");
  String monthin = processCronInputValues("monthtype", "monthvalue");
  String dayofweekin = processCronInputValues("dayofweektype", "dayofweekvalue");

  String device_id = httpServer.arg("devuce_id");
  String function_name = httpServer.arg("function_name");
  String function_name_parameter = httpServer.arg("function_name_parameter");

//  MQTTLogMessage(String("function_name_parameter: ")+function_name_parameter );

  timer1.addTask("USER", device_id.toInt(), function_name,function_name_parameter, minutein, hourin, dayin, monthin, dayofweekin);

  saveConfig();

  httpServer.sendHeader("Location", "/tasks");
  httpServer.sendHeader("Cache-Control", "no-cache");
  httpServer.send(302);

}

void handleTasks() {
  ////Serial.printf("handleTasks\n");

  if (!processAuth()) return;

  handlePage("/html/tasks.php");

}

void handleLogs() {
  ////Serial.printf("handleLogs\n");

  if (!processAuth()) return;

  handlePage("/html/logs.php");

}

void handleJSONListLogs() {

  String content = "";
  content = content + "{ \"Logs\" : [\n";

  int listSize = appLogs.size();

  //  Serial.printf("listSize %d\n",listSize );

  for (int i = 0; i < listSize; i++)

  {

    AppLog appLogObject = appLogs.get(i);

    if (i > 0) content = content + ",";

    content = content + "{" + "\"datetime\":\"" + appLogObject.datetime + "\"," +
      "\"message\":\"" + appLogObject.message + "\"}\n";

  } //for

  content = content + " ]}";

  //  Serial.printf("%s\n",content.c_str());

  int size = content.length();

  httpServer.setContentLength(size);
  httpServer.send(200, "text/html", "");
  httpServer.sendContent(content.c_str());

}

void handleJSONListSheduledTasks() {

  String content = "";
  content = content + "{        \"extra_params\" : {              \"sunrise\" :  \""+runtime.sunrise_hour+":"+runtime.sunrise_minute+"\"     ,   \"sunset\" :  \""+runtime.sunset_hour+":"+runtime.sunset_minute+"\"   },                                                     \"Tasks\" : [\n";

  int listSize = timer1.cronObjects.size();

  for (int i = 0; i < listSize; i++)

  {

    CronObject cronObjects = timer1.cronObjects.get(i);

    if (i > 0) content = content + ",";

    content = content + "{" + "\"device_id\":\"" + cronObjects.device_id + "\"," +
      "\"minute\":\"" + cronObjects.minute + "\"," + "\"minute_logical\":\"" + cronObjects.minute_logical + "\"," +
//      "\"type\":\"" + cronObjects.type +       "\", minute\":\"" + cronObjects.minute + "\"," + "\"minute_logical\":\"" + cronObjects.minute_logical + "\"," +
      "\"hour\":\"" + cronObjects.hour + "\"" + "," + "\"hour_logical\":\"" + cronObjects.hour_logical + "\"" + "," +
      "\"day\":\"" + cronObjects.day + "\"" + "," + "\"day_logical\":\"" + cronObjects.day_logical + "\"" + "," +
      "\"month\":\"" + cronObjects.month + "\"" + "," + "\"month_logical\":\"" + cronObjects.month_logical + "\"" + "," +
      "\"weekday\":\"" + cronObjects.weekday + "\"" + "," + "\"weekday_logical\":\"" + cronObjects.weekday_logical + "\"" + "," +
      "\"function_name_parameter\":\"" + cronObjects.function_name_parameter  + "\"" + "," +
      "\"function_name\":\"" + cronObjects.function_name + "\"}\n";

  } //for

  content = content + " ]}";

  //  Serial.printf("%s\n",content.c_str());

  int size = content.length();

  httpServer.setContentLength(size);
  httpServer.send(200, "text/html", "");
  httpServer.sendContent(content.c_str());

}


void handleDeleteOtherEspDevice() {

  ////Serial.printf("handleDeleteTask\n");

  if (!processToken()) return;

  int index = httpServer.arg("index").toInt();

  //Serial.printf("index %d\n",index);
  MDNSObjects.remove(index);
  handleJSONListOtherDevices();
}

void handleJSONListOtherDevices() {
  //    //Serial.printf("handleJSONListOtherDevices()\n");
  String content = getDNSDevices();

  int size = content.length();

  httpServer.setContentLength(size);
  httpServer.send(200, "text/html", "");
  httpServer.sendContent(content.c_str());

}

void handleJSONRuntimeInfo() {
  //    //Serial.printf("handleJSONRuntimeInfo()\n");

  //  if (!processAuth())    return;

  bool processTokenRet = false;

  String inToken = httpServer.arg("token");
  if (inToken.equals(runtime.token)) processTokenRet = true;

  //String inToken= httpServer.arg("token");

  String ip = WiFi.localIP().toString();
  String netmask = WiFi.subnetMask().toString();
  String gateway = WiFi.gatewayIP().toString();

  String dns = WiFi.dnsIP().toString();
  //String dns1= WiFi.dnsIP(1).toString();
  //String dns2= WiFi.dnsIP(2).toString();
  //String dns3= WiFi.dnsIP(3).toString();
  //String dns4= WiFi.dnsIP(4).toString();
  //String uptime=getUpTime;

  FSInfo fs_info;
  SPIFFS.info(fs_info);

  float fileTotalKB = (float) fs_info.totalBytes / 1024.0;
  float fileUsedKB = (float) fs_info.usedBytes / 1024.0;
  float totalmemforProgram = (ESP.getFlashChipRealSize() - (float) fs_info.totalBytes) / 1024.0;

  String content = "";
  String content2 = "";
  String contentGeneral = "";
  String contentMEM = "";
  String contentCONF_Relays = "";
  String contentCONF_RGBLed = "";
  String contentCONF_MQTT = "";
  String contentCONF_Advanced = "";
  content = content + "{\"runtime\" : [\n";

  content = content + "{" + "\"n\":\"uptime\"," + "\"v\":\"" + NTP.getUptimeString() + "\"" + ",\"h\":\"\"},\n";

  content = content + "{" + "\"n\":\"devicetitle\"," + "\"v\":\"" + conf.hostName.c_str() + "\"" + ",\"h\":\"\"},\n";
  content = content + "{" + "\"n\":\"htmltitle\"," + "\"v\":\"" + conf.hostName.c_str() + "\"" + ",\"h\":\"\"},\n";
  content = content + "{" + "\"n\":\"firmware_version\"," + "\"v\":\"" + conf.firmware_version + "\"" + ",\"h\":\"\"},\n";
  content = content + "{" + "\"n\":\"spiffs_version\"," + "\"v\":\"" + conf.spiffs_version + "\"" + ",\"h\":\"\"},\n";
  //    content2=content2+"{"+"\"n\":\"firmware_version_full\","+"\"v\":\""+conf.firmware_version.c_str()+" ( SPIFFS "+conf.spiffs_version.c_str()+" )"+"\""+",\"h\":\"\"},\n";

  if (processTokenRet) {
    if (WiFi.getMode() == WIFI_STA) content = content + "{" + "\"n\":\"hostname\"," + "\"v\":\"" + WiFi.hostname() + "\"" + ",\"h\":\"\"},\n";
    else if (WiFi.getMode() == WIFI_AP) content = content + "{" + "\"n\":\"hostname\"," + "\"v\":\"" + conf.hostName.c_str() + "\"" + ",\"h\":\"\"},\n";

    String sunrise = getHourString(runtime.sunrise_hour, runtime.sunrise_minute);
    String sunset = getHourString(runtime.sunset_hour, runtime.sunset_minute);

    content = content + "{" + "\"n\":\"sunset\"," + "\"v\":\"" + sunset + "\"" + ",\"h\":\"\"},\n";
    content = content + "{" + "\"n\":\"sunrise\"," + "\"v\":\"" + sunrise + "\"" + ",\"h\":\"\"},\n";
    content = content + "{" + "\"n\":\"sunset_update_time\"," + "\"v\":\"" + runtime.sunset_update_time + "\"" + ",\"h\":\"\"},\n";

    contentGeneral = contentGeneral + "{" + "\"n\":\"cbid.network.lan.wifissid\"," + "\"v\":\"" + conf.wifi_ssid.c_str() + "\"" + ",\"h\":\"0\"},\n";
    contentGeneral = contentGeneral + "{" + "\"n\":\"cbid.network.lan.wifipassword\"," + "\"v\":\"" + conf.wifi_password.c_str() + "\"" + ",\"h\":\"0\"},\n";
    contentGeneral = contentGeneral + "{" + "\"n\":\"cbid.network.lan.hostname\"," + "\"v\":\"" + conf.hostName.c_str() + "\"" + ",\"h\":\"\"},\n";
    contentGeneral = contentGeneral + "{" + "\"n\":\"cbid.network.lan.broadcast\"," + "\"v\":\"" + conf.broadcast + "\"" + ",\"h\":\"_\"},\n";

    contentMEM = contentMEM + "{" + "\"n\":\"localtime\"," + "\"v\":\"" + NTP.getTimeDateString() + "\"" + ",\"h\":\"\"},\n";
    contentMEM = contentMEM + "{" + "\"n\":\"totalmem\"," + "\"v\":\"" + totalmemforProgram + "\"" + ",\"h\":\"\"},\n";
    contentMEM = contentMEM + "{" + "\"n\":\"sketchsize\"," + "\"v\":\"" + ESP.getSketchSize() / 1024.0 + "\"" + ",\"h\":\"\"},\n";
    contentMEM = contentMEM + "{" + "\"n\":\"totalmemSPIFFS\"," + "\"v\":\"" + fileTotalKB + "\"" + ",\"h\":\"\"},\n";
    contentMEM = contentMEM + "{" + "\"n\":\"usedSPIFFS\"," + "\"v\":\"" + fileUsedKB + "\"" + ",\"h\":\"\"},\n";

    //    content=content+"{"+"\"n\":\"freemem\","+"\"v\":\""+ESP.getFreeSketchSpace()+"\""+    ",\"h\":\"\"},\n";

    contentCONF_Advanced = contentCONF_Advanced + "{" + "\"n\":\"adv.security_enable\"," + "\"v\":\"" + conf.security_enable + "\"" + ",\"h\":\"\"},\n";
    contentCONF_Advanced = contentCONF_Advanced + "{" + "\"n\":\"adv.security_enable_checkbox\"," + "\"v\":\"" + conf.security_enable + "\"" + ",\"h\":\"\"},\n";
    contentCONF_Advanced = contentCONF_Advanced + "{" + "\"n\":\"adv.OTA_enable\"," + "\"v\":\"" + conf.OTA_enable + "\"" + ",\"h\":\"\"},\n";
    contentCONF_Advanced = contentCONF_Advanced + "{" + "\"n\":\"adv.OTA_enable_checkbox\"," + "\"v\":\"" + conf.OTA_enable + "\"" + ",\"h\":\"\"},\n";
    contentCONF_Advanced = contentCONF_Advanced + "{" + "\"n\":\"adv.discoverable\"," + "\"v\":\"" + conf.discoverable + "\"" + ",\"h\":\"\"},\n";
    contentCONF_Advanced = contentCONF_Advanced + "{" + "\"n\":\"adv.discoverable_checkbox\"," + "\"v\":\"" + conf.discoverable + "\"" + ",\"h\":\"\"},\n";
    contentCONF_Advanced = contentCONF_Advanced + "{" + "\"n\":\"adv.network_update\"," + "\"v\":\"" + conf.network_update + "\"" + ",\"h\":\"\"},\n";
    contentCONF_Advanced = contentCONF_Advanced + "{" + "\"n\":\"adv.network_update_checkbox\"," + "\"v\":\"" + conf.network_update + "\"" + ",\"h\":\"\"},\n";
    contentCONF_Advanced = contentCONF_Advanced + "{" + "\"n\":\"adv.sunsetAPIKey\"," + "\"v\":\"" + conf.sunsetAPIKey.c_str() + "\"" + ",\"h\":\"_\"},\n";
    contentCONF_Advanced = contentCONF_Advanced + "{" + "\"n\":\"adv.sunsetAPICity\"," + "\"v\":\"" + conf.sunsetAPICity.c_str() + "\"" + ",\"h\":\"_\"},\n";
    contentCONF_Advanced = contentCONF_Advanced + "{" + "\"n\":\"adv.ntpserver\"," + "\"v\":\"" + conf.ntpserver + "\"" + ",\"h\":\"_\"},\n";
    contentCONF_Advanced = contentCONF_Advanced + "{" + "\"n\":\"adv.web_password\"," + "\"v\":\"" + conf.web_password + "\"" + ",\"h\":\"0\"},\n";

    /*
        if (conf.relays[1].relay_pin>=0)
        {
        content=content+"{"+"\"n\":\"adv.enablerelay2\","+"\"v\":\""+conf.network_update+"\""+    ",\"h\":\"\"},\n";
        content=content+"{"+"\"n\":\"adv.enablerelay2_checkbox\","+"\"v\":\""+conf.network_update+"\""+    ",\"h\":\"\"},\n";
        }
    */

    contentCONF_MQTT = contentCONF_MQTT + "{" + "\"n\":\"mqtt.MQTT_enable\"," + "\"v\":\"" + conf.MQTT_enable + "\"" + ",\"h\":\"\"},\n";
    contentCONF_MQTT = contentCONF_MQTT + "{" + "\"n\":\"mqtt.MQTT_enable_checkbox\"," + "\"v\":\"" + conf.MQTT_enable + "\"" + ",\"h\":\"\"},\n";

    contentCONF_MQTT = contentCONF_MQTT + "{" + "\"n\":\"mqtt.hostname\"," + "\"v\":\"" + conf.MQTT_hostName.c_str() + "\"" + ",\"h\":\"_\"},\n";
    contentCONF_MQTT = contentCONF_MQTT + "{" + "\"n\":\"mqtt.domoticzhostname\"," + "\"v\":\"" + conf.Domoticz_hostName.c_str() + "\"" + ",\"h\":\"_\"},\n";
    contentCONF_MQTT = contentCONF_MQTT + "{" + "\"n\":\"mqtt.domoticzport\"," + "\"v\":\"" + conf.Domoticz_port + "\"" + ",\"h\":\"_\"},\n";
    contentCONF_MQTT = contentCONF_MQTT + "{" + "\"n\":\"mqtt.domoticzusername\"," + "\"v\":\"" + conf.Domoticz_userName.c_str() + "\"" + ",\"h\":\"_\"},\n";
    contentCONF_MQTT = contentCONF_MQTT + "{" + "\"n\":\"mqtt.domoticzuserpassword\"," + "\"v\":\"" + conf.Domoticz_password.c_str() + "\"" + ",\"h\":\"_\"},\n";

    content2 = content2 + "{" + "\"n\":\"ip\"," + "\"v\":\"" + ip + "\"" + ",\"h\":\"\"},\n";
    content2 = content2 + "{" + "\"n\":\"mac\"," + "\"v\":\"" + runtime.mac_STA + "\"" + ",\"h\":\"\"},\n";
    content2 = content2 + "{" + "\"n\":\"netmask\"," + "\"v\":\"" + netmask + "\"" + ",\"h\":\"\"},\n";
    content2 = content2 + "{" + "\"n\":\"gateway\"," + "\"v\":\"" + gateway + "\"" + ",\"h\":\"\"},\n";
    content2 = content2 + "{" + "\"n\":\"dns\"," + "\"v\":\"" + dns + "\"" + ",\"h\":\"\"},\n";
    //    content2=content2+"{"+"\"n\":\"dns1\","+"\"v\":\""+dns1+"\""+",\"h\":\"0.0.0.0\"},\n";
    //    content2=content2+"{"+"\"n\":\"dns2\","+"\"v\":\""+dns2+"\""+",\"h\":\"0.0.0.0\"},\n";
    //    content2=content2+"{"+"\"n\":\"dns3\","+"\"v\":\""+dns3+"\""+",\"h\":\"0.0.0.0\"},\n";
    //    content2=content2+"{"+"\"n\":\"dns4\","+"\"v\":\""+dns4+"\""+",\"h\":\"0.0.0.0\"},\n";

    contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay_state0\"," + "\"v\":\"" + conf.relays[0].relay_state + "\"" + ",\"h\":\"\"},\n";

//    Serial.printf("\n\n----------------------- Przeczytałem relay state 2 %d na indeksie %d \n\n",conf.relays[0].relay_state , 0);

    contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay0.internalName\"," + "\"v\":\"" + conf.relays[0].internalName + "\"" + ",\"h\":\"_\"},\n";
    contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay0.relay_conn_type\"," + "\"v\":\"" + conf.relays[0].relay_conn_type + "\"" + ",\"h\":\"\"},\n";
    contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay0.relay_switch_pin\"," + "\"v\":\"" + conf.relays[0].relay_switch_pin + "\"" + ",\"h\":\"\"},\n";
    contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay0.relay_led_conn_type\"," + "\"v\":\"" + conf.relays[0].relay_led_conn_type + "\"" + ",\"h\":\"\"},\n";
    contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay0.relay_led_pin\"," + "\"v\":\"" + conf.relays[0].relay_led_pin + "\"" + ",\"h\":\"\"},\n";
    contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay0.domoticzidx\"," + "\"v\":\"" + conf.relays[0].domoticz_device_idx + "\"" + ",\"h\":\"_\"},\n";
    contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay0.relay_switch_type\"," + "\"v\":\"" + conf.relays[0].relay_switch_type + "\"" + ",\"h\":\"_\"},\n";
    contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay0.relay_pin\"," + "\"v\":\"" + conf.relays[0].relay_pin + "\"" + ",\"h\":\"\"},\n";

    contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay1.relay_pin\"," + "\"v\":\"" + conf.relays[1].relay_pin + "\"" + ",\"h\":\"\"},\n";
    if (conf.relays[1].relay_pin > 0) {
      contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay_state1\"," + "\"v\":\"" + conf.relays[1].relay_state + "\"" + ",\"h\":\"\"},\n";
      contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay1.internalName\"," + "\"v\":\"" + conf.relays[1].internalName + "\"" + ",\"h\":\"_\"},\n";
      contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay1.relay_conn_type\"," + "\"v\":\"" + conf.relays[1].relay_conn_type + "\"" + ",\"h\":\"\"},\n";
      contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay1.relay_switch_pin\"," + "\"v\":\"" + conf.relays[1].relay_switch_pin + "\"" + ",\"h\":\"\"},\n";
      contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay1.relay_led_conn_type\"," + "\"v\":\"" + conf.relays[1].relay_led_conn_type + "\"" + ",\"h\":\"\"},\n";
      contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay1.relay_led_pin\"," + "\"v\":\"" + conf.relays[1].relay_led_pin + "\"" + ",\"h\":\"\"},\n";
      contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay1.domoticzidx\"," + "\"v\":\"" + conf.relays[1].domoticz_device_idx + "\"" + ",\"h\":\"_\"},\n";
      contentCONF_Relays = contentCONF_Relays + "{" + "\"n\":\"relay1.relay_switch_type\"," + "\"v\":\"" + conf.relays[1].relay_switch_type + "\"" + ",\"h\":\"_\"},\n";
    }

    content2 = content2 + "{" + "\"n\":\"cbid.network.temperaturesensor.pin\"," + "\"v\":\"" + conf.temperatureSensor_tempsensor_pin + "\"" + ",\"h\":\"\"},\n";
    content2 = content2 + "{" + "\"n\":\"cbid.network.temperaturesensor.domoticzidx\"," + "\"v\":\"" + conf.temperatureSensor_domoticz_device_idx + "\"" + ",\"h\":\"\"},\n";

    if (conf.temperatureSensor_tempsensor_pin > 0) {
      content2 = content2 + "{" + "\"n\":\"temperature\"," + "\"v\":\"" + runtime.temperature + "\"" + ",\"h\":\"\"},\n";
      content2 = content2 + "{" + "\"n\":\"temperature_measure_time\"," + "\"v\":\"" + runtime.temperature_measure_time + "\"" + ",\"h\":\"\"},\n";
    }

    contentCONF_RGBLed = contentCONF_RGBLed + "{" + "\"n\":\"rgbled.enable\"," + "\"v\":\"" + conf.rgbled.enable + "\"" + ",\"h\":\"\"},\n";
    contentCONF_RGBLed = contentCONF_RGBLed + "{" + "\"n\":\"rgbled.enable_checkbox\"," + "\"v\":\"" + conf.rgbled.enable + "\"" + ",\"h\":\"\"},\n";
    contentCONF_RGBLed = contentCONF_RGBLed + "{" + "\"n\":\"rgbled.state\"," + "\"v\":\"" + conf.rgbled.state + "\"" + ",\"h\":\"\"},\n";
    contentCONF_RGBLed = contentCONF_RGBLed + "{" + "\"n\":\"rgbled.r\"," + "\"v\":\"" + conf.rgbled.r + "\"" + ",\"h\":\"\"},\n";
    contentCONF_RGBLed = contentCONF_RGBLed + "{" + "\"n\":\"rgbled.g\"," + "\"v\":\"" + conf.rgbled.g + "\"" + ",\"h\":\"\"},\n";
    contentCONF_RGBLed = contentCONF_RGBLed + "{" + "\"n\":\"rgbled.b\"," + "\"v\":\"" + conf.rgbled.b + "\"" + ",\"h\":\"\"},\n";
    contentCONF_RGBLed = contentCONF_RGBLed + "{" + "\"n\":\"rgbled.w\"," + "\"v\":\"" + conf.rgbled.w + "\"" + ",\"h\":\"\"},\n";
    contentCONF_RGBLed = contentCONF_RGBLed + "{" + "\"n\":\"rgbled.wa\"," + "\"v\":\"" + conf.rgbled.wa + "\"" + ",\"h\":\"\"},\n";

    //    content=content+"{"+"\"n\":\"mqtt.MQTT_enable\","+"\"v\":\""+conf.MQTT_enable+"\""+    ",\"h\":\"\"},\n";
    //    content=content+"{"+"\"n\":\"mqtt.MQTT_enable_checkbox\","+"\"v\":\""+conf.MQTT_enable+"\""+    ",\"h\":\"\"},\n";

  } //  if (processTokenRet)

  content2 = content2 + "{" + "\"n\":\"timeDate\"," + "\"v\":\"" + NTP.getTimeDateString() + "\"" + ",\"h\":\"\"}\n";

  content2 = content2 + " ]}";

  /*
    Serial.printf("content.length %d\n",content.length()); 
    Serial.printf("contentGeneral.length %d\n",contentGeneral.length()); 
    Serial.printf("contentMEM.length %d\n",contentMEM.length()); 
    Serial.printf("contentCONF_Relays.length %d\n",contentCONF_Relays.length()); 
    Serial.printf("contentCONF_RGBLed.length %d\n",contentCONF_RGBLed.length()); 
    Serial.printf("contentCONF_MQTT.length %d\n",contentCONF_MQTT.length()); 
    Serial.printf("contentCONF_Advanced.length %d\n",contentCONF_Advanced.length()); 
    Serial.printf("content2.length %d\n",content2.length()); 
  */

  int size = content.length() + contentGeneral.length() + contentMEM.length() + contentCONF_Relays.length() + contentCONF_RGBLed.length() + contentCONF_MQTT.length() + contentCONF_Advanced.length() + content2.length();
  httpServer.setContentLength(size);
  //httpServer.setContentLength(5000);
  httpServer.send(200, "text/html", "");
  httpServer.sendContent(content.c_str());
  httpServer.sendContent(contentGeneral.c_str());
  httpServer.sendContent(contentMEM.c_str());
  httpServer.sendContent(contentCONF_Relays.c_str());
  httpServer.sendContent(contentCONF_RGBLed.c_str());
  httpServer.sendContent(contentCONF_MQTT.c_str());
  httpServer.sendContent(contentCONF_Advanced.c_str());
  httpServer.sendContent(content2.c_str());

  //for (int i=size;i<5000;i++)httpServer.sendContent(" ");

}

void handleJSONListWifiNetworks() {

  //    //Serial.printf("handleJSONListWifiNetworks()\n");

  String content = scanNetworks();

  int size = content.length();

  httpServer.setContentLength(size);
  httpServer.send(200, "text/html", "");
  httpServer.sendContent(content.c_str());

}

bool processAuth() {
  //    Serial.printf("processAuth \n");

  if (conf.security_enable== false) return true;


  if (!is_authentified()) {
    httpServer.sendHeader("Location", "/login");
    httpServer.sendHeader("Cache-Control", "no-cache");
    httpServer.send(301);
    return false;
  }
  return true;
}

void handleReboot() {

  //Serial.printf("handleReboot() \n");

  if (!processAuth()) return;

  if (!processToken()) return;

  //    httpServer.sendHeader("Location","/");
  httpServer.sendHeader("Cache-Control", "no-cache");
  httpServer.send(200);

  reboot();
}

void handleSaveConf() {

  //   //Serial.printf("handleSaveConf()\n");

  //MQTTLogMessage("handleSaveConf");

  int args_size = httpServer.args();

  /*  bool hasArg(String name);       // check if argument exists
    String arg(String name);        // get request argument value by name
    String arg(int i);              // get request argument value by number
    String argName(int i);          // get request argument name by number
  */

  /*
  String content=String("");

  for (int i=0;i<args_size;i++)
      {
        String name=httpServer.argName(i);
        String value=httpServer.arg(i);

        content=content+name+" : "+value+"<br />";
     Serial.printf("%s : %s\n",name.c_str(),value.c_str());
        }
  */
  if (!processAuth()) return;

  if (!processToken()) return;

  //////////////////////////////

  if (httpServer.hasArg("cbid.network.lan.hostname")) conf.hostName = httpServer.arg("cbid.network.lan.hostname");
  if (httpServer.hasArg("cbid.network.lan.broadcast")) conf.broadcast = httpServer.arg("cbid.network.lan.broadcast");

  if (httpServer.hasArg("cbid.network.lan.wifissid")) {

    String wifissid = httpServer.arg("cbid.network.lan.wifissid");

    if (!wifissid.equals("")) {
      //     Serial.printf("setting new wifissid %s \n", wifissid.c_str());
      conf.wifi_ssid = httpServer.arg("cbid.network.lan.wifissid");

      //WiFi.begin(conf.wifi_ssid.c_str(), wifipassword.c_str());
    } //if (!wifipassword.equals(""))  

  }

  //if (httpServer.hasArg("cbid.network.lan.wifipassword")) 
  //{
  //  conf.wifi_password=httpServer.arg("cbid.network.lan.wifipassword");
  String wifipassword = httpServer.arg("cbid.network.lan.wifipassword");
  String web_password = httpServer.arg("adv.web_password");
  String ntpserver = httpServer.arg("adv.ntpserver");

  String sunsetAPIKey = httpServer.arg("adv.sunsetAPIKey");
  String sunsetAPICity = httpServer.arg("adv.sunsetAPICity");

  if (!wifipassword.equals("")) conf.wifi_password = httpServer.arg("cbid.network.lan.wifipassword");
  if (!web_password.equals("")) conf.web_password = httpServer.arg("adv.web_password");
  if (!ntpserver.equals("")) conf.ntpserver = httpServer.arg("adv.ntpserver");
  if (!sunsetAPIKey.equals("")) conf.sunsetAPIKey = httpServer.arg("adv.sunsetAPIKey");
  if (!sunsetAPICity.equals("")) conf.sunsetAPICity = httpServer.arg("adv.sunsetAPICity");

  //Serial.printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX %s \n", sunsetAPICity.c_str());
  //Serial.printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX %s \n", conf.sunsetAPIKey.c_str());

  //MQTTLogMessage("handleSaveConf2");

  //{
  //     Serial.printf("setting new wifipassword %s \n", wifipassword.c_str());

  //WiFi.begin(conf.wifi_ssid.c_str(), wifipassword.c_str());
  //}//if (!wifipassword.equals(""))  

  //}

  if (httpServer.hasArg("mqtt.MQTT_enable") && httpServer.arg("mqtt.MQTT_enable").equals("true")) conf.MQTT_enable = true;
  else if (httpServer.hasArg("mqtt.MQTT_enable") && httpServer.arg("mqtt.MQTT_enable").equals("false")) conf.MQTT_enable = false;

  if (httpServer.hasArg("adv.security_enable") && httpServer.arg("adv.security_enable").equals("true")) conf.security_enable = true;
  else if (httpServer.hasArg("adv.security_enable") && httpServer.arg("adv.security_enable").equals("false")) conf.security_enable = false;

  if (httpServer.hasArg("adv.OTA_enable") && httpServer.arg("adv.OTA_enable").equals("true")) conf.OTA_enable = true;
  else if (httpServer.hasArg("adv.OTA_enable") && httpServer.arg("adv.OTA_enable").equals("false")) conf.OTA_enable = false;

  if (httpServer.hasArg("rgbled.enable") && httpServer.arg("rgbled.enable").equals("true")) conf.rgbled.enable = true;
  else if (httpServer.hasArg("rgbled.enable") && httpServer.arg("rgbled.enable").equals("false")) conf.rgbled.enable = false;

  if (httpServer.hasArg("rgbled.r")) conf.rgbled.r = atoi(httpServer.arg("rgbled.r").c_str());
  if (httpServer.hasArg("rgbled.g")) conf.rgbled.g = atoi(httpServer.arg("rgbled.g").c_str());
  if (httpServer.hasArg("rgbled.b")) conf.rgbled.b = atoi(httpServer.arg("rgbled.b").c_str());
  if (httpServer.hasArg("rgbled.w")) conf.rgbled.w = atoi(httpServer.arg("rgbled.w").c_str());
  if (httpServer.hasArg("rgbled.wa")) conf.rgbled.wa = atoi(httpServer.arg("rgbled.wa").c_str());

  if (httpServer.hasArg("adv.discoverable") && httpServer.arg("adv.discoverable").equals("true")) conf.discoverable = true;
  else if (httpServer.hasArg("adv.discoverable") && httpServer.arg("adv.discoverable").equals("false")) conf.discoverable = false;

  if (httpServer.hasArg("adv.network_update") && httpServer.arg("adv.network_update").equals("true")) conf.network_update = true;
  else if (httpServer.hasArg("adv.network_update") && httpServer.arg("adv.network_update").equals("false")) conf.network_update = false;

  if (httpServer.hasArg("mqtt.hostname")) conf.MQTT_hostName = httpServer.arg("mqtt.hostname");
  if (httpServer.hasArg("mqtt.domoticzhostname")) conf.Domoticz_hostName = httpServer.arg("mqtt.domoticzhostname");

  if (httpServer.hasArg("mqtt.domoticzport")) conf.Domoticz_port = atoi(httpServer.arg("mqtt.domoticzport").c_str());
  if (httpServer.hasArg("mqtt.domoticzusername")) conf.Domoticz_userName = httpServer.arg("mqtt.domoticzusername");
  if (httpServer.hasArg("mqtt.domoticzuserpassword")) conf.Domoticz_password = httpServer.arg("mqtt.domoticzuserpassword");

  if (httpServer.hasArg("relay0.internalName")) conf.relays[0].internalName = httpServer.arg("relay0.internalName").c_str();
  if (httpServer.hasArg("relay0.relay_pin")) conf.relays[0].relay_pin = atoi(httpServer.arg("relay0.relay_pin").c_str());
  if (httpServer.hasArg("relay0.relay_conn_type")) conf.relays[0].relay_conn_type = atoi(httpServer.arg("relay0.relay_conn_type").c_str());
  if (httpServer.hasArg("relay0.relay_led_pin")) conf.relays[0].relay_led_pin = atoi(httpServer.arg("relay0.relay_led_pin").c_str());
  if (httpServer.hasArg("relay0.relay_led_conn_type")) conf.relays[0].relay_led_conn_type = atoi(httpServer.arg("relay0.relay_led_conn_type").c_str());
  if (httpServer.hasArg("relay0.relay_switch_type")) conf.relays[0].relay_switch_type = atoi(httpServer.arg("relay0.relay_switch_type").c_str());
  if (httpServer.hasArg("relay0.relay_switch_pin")) conf.relays[0].relay_switch_pin = atoi(httpServer.arg("relay0.relay_switch_pin").c_str());
  if (httpServer.hasArg("relay0.domoticzidx")) conf.relays[0].domoticz_device_idx = atoi(httpServer.arg("relay0.domoticzidx").c_str());

  //MQTTLogMessage("handleSaveConf3");

  if (httpServer.hasArg("relay1.internalName")) conf.relays[1].internalName = httpServer.arg("relay1.internalName").c_str();
  if (httpServer.hasArg("relay1.relay_pin")) conf.relays[1].relay_pin = atoi(httpServer.arg("relay1.relay_pin").c_str());
  else conf.relays[1].relay_pin = -2;

  if (httpServer.hasArg("relay1.relay_conn_type")) conf.relays[1].relay_conn_type = atoi(httpServer.arg("relay1.relay_conn_type").c_str());
  if (httpServer.hasArg("relay1.relay_led_pin")) conf.relays[1].relay_led_pin = atoi(httpServer.arg("relay1.relay_led_pin").c_str());
  if (httpServer.hasArg("relay1.relay_led_conn_type")) conf.relays[1].relay_led_conn_type = atoi(httpServer.arg("relay1.relay_led_conn_type").c_str());
  if (httpServer.hasArg("relay1.relay_switch_type")) conf.relays[1].relay_switch_type = atoi(httpServer.arg("relay1.relay_switch_type").c_str());
  if (httpServer.hasArg("relay1.relay_switch_pin")) conf.relays[1].relay_switch_pin = atoi(httpServer.arg("relay1.relay_switch_pin").c_str());
  if (httpServer.hasArg("relay1.domoticzidx")) conf.relays[1].domoticz_device_idx = atoi(httpServer.arg("relay1.domoticzidx").c_str());

  if (httpServer.hasArg("cbid.network.temperaturesensor.pin")) conf.temperatureSensor_tempsensor_pin = atoi(httpServer.arg("cbid.network.temperaturesensor.pin").c_str());
  else conf.temperatureSensor_tempsensor_pin = -2;
  if (httpServer.hasArg("cbid.network.temperaturesensor.domoticzidx")) conf.temperatureSensor_domoticz_device_idx = atoi(httpServer.arg("cbid.network.temperaturesensor.domoticzidx").c_str());

  //MQTTLogMessage("handleSaveConf4");

  saveConfig();

  httpServer.sendHeader("Location", "/");
  //    httpServer.sendHeader("Cache-Control","no-cache");
  httpServer.send(301);
  reboot();
}

void handleRebootASK() {

  //Serial.printf("handleRebootASK()\n");
  if (!processAuth()) return;

  if (!processToken()) return;

  handlePage("/html/reboot.php");

  /*
  if (!processAuth())    return;

      //Serial.printf("handleRebootASK() 2\n");

  int size = getFileSize("/html/before.php") +getFileSize("/html/after.php")+getFileSize("/html/reboot.php");


  //    httpServer.sendHeader("Cache-Control","no-cache");
    httpServer.setContentLength(size);
    httpServer.send(200, "text/html", "");

  int ret= httpServerSendFileContent( "/html/before.php");
  int ret2= httpServerSendFileContent( "/html/reboot.php");
  int ret3= httpServerSendFileContent( "/html/after.php");


  if (ret==-1) httpServer.send(200, "text/html", "error....");
  */
}

void handleConf() {

  //
  handlePage("/html/conf.php");

  /*
   if (!processAuth())    return;

  int size = getFileSize("/html/before.php") +getFileSize("/html/after.php")+getFileSize("/html/conf.php");


  //    httpServer.sendHeader("Cache-Control","no-cache");
    httpServer.setContentLength(size);
    httpServer.send(200, "text/html", "");

  int ret= httpServerSendFileContent( "/html/before.php");
  int ret2= httpServerSendFileContent( "/html/conf.php");
  int ret3= httpServerSendFileContent( "/html/after.php");


  if (ret==-1) httpServer.send(200, "text/html", "error....");
  */
}

//login page, also called for disconnect
void handleLogin() {

  if (httpServer.hasHeader("Cookie")) {
    String cookie = httpServer.header("Cookie");
    //    Serial.printf("Found cookie: %s\n",cookie.c_str());

  }
  if (httpServer.hasArg("DISCONNECT")) {
    //    Serial.println("Disconnection");
    httpServer.sendHeader("Location", "/login");
    httpServer.sendHeader("Cache-Control", "no-cache");
    httpServer.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    httpServer.send(301);
    return;
  }

  if (httpServer.hasArg("USERNAME") && httpServer.hasArg("PASSWORD")) {

    //   Serial.println("Jest login i haslo: ");

    //char *inusername=string2char(httpServer.arg("USERNAME")) ;
    //char *inpassword=string2char(httpServer.arg("PASSWORD"));

    //char *inusername=httpServer.arg("USERNAME").c_str() ;
    //char *inpassword=httpServer.arg("PASSWORD").c_str() ;

    //char *inusername=httpServer.arg("USERNAME").toCharArray();
    //char *inpassword=httpServer.arg("PASSWORD").toCharArray();

    //    Serial.printf("IN username [%s]\n",inusername);
    //    Serial.printf("IN password [%s]\n",inpassword);

    //int cmp1=strcmp("abv","anc");

    //    Serial.printf("cmp1 [%d]\n",cmp1);

    //int cmp2=strcmp(inpassword,conf.web_password);

    //    Serial.printf("cmp2 [%d]\n",cmp2);

    //char* md5strinu =get_md5(inusername);
    //char* md5strinp =get_md5(inpassword);

    //    Serial.print("HASH IN");

    //    Serial.printf("HASH IN username [%s]\n",md5strinu);
    //    Serial.printf("HASH IN password [%s]\n",md5strinp );

    //porownanie

    /*
        Serial.print("Username [");
        Serial.print(httpServer.arg("USERNAME"));
        Serial.println("]");
        Serial.print("Password [");
        Serial.print(httpServer.arg("PASSWORD"));
        Serial.println("]");
    */
    /*
        Serial.print("Password in conf[");
        Serial.print(String(conf.web_password).c_str());
        Serial.println("]");
        Serial.print("Password in conf2[");
        Serial.print(conf.web_password);
        Serial.println("]");
      */
    //Serial.printf("11 [%d]\n",httpServer.arg("ADMIN")=="admin");
    //Serial.printf("22 [%d]\n",httpServer.arg("ADMIN").equals("admin"));

    //Serial.printf("1 [%d]\n",httpServer.arg("PASSWORD")=="admin76");
    //Serial.printf("2 [%d]\n",httpServer.arg("PASSWORD").equals("admin76"));

    if (httpServer.arg("USERNAME") == conf.web_username && httpServer.arg("PASSWORD") == conf.web_password) {
      //      Serial.println("Ustawia cookie");
      httpServer.sendHeader("Location", "/");
      httpServer.sendHeader("Cache-Control", "no-cache");
      httpServer.sendHeader("Set-Cookie", "ESPSESSIONID=" + runtime.token);
      httpServer.send(301);
      //      Serial.println("Log in Successful");
      return;
    }

    //  Serial.println("Log in Failed");
  }
  /*
    
    String content = "<html><body><form action='/login' method='POST'>To log in, please use : admin/admin<br>";
    content += "User:<input type='text' name='USERNAME' placeholder='user name'><br>";
    content += "Password:<input type='password' name='PASSWORD' placeholder='password'><br>";
    content += "<input type='submit' name='SUBMIT' value='Submit'></form>" + msg + "<br>";
    content += "You also can go <a href='/inline'>here</a></body></html>";
    httpServer.send(200, "text/html", content);
    */

  int size = getFileSize("/html/before.php") + getFileSize("/html/loginform.php") + getFileSize("/html/after.php");

  httpServer.setContentLength(size);

  httpServer.send(200, "text/html", "");

  int ret1 = httpServerSendFileContent("/html/before.php");
  int ret2 = httpServerSendFileContent("/html/loginform.php");
  int ret3 = httpServerSendFileContent("/html/after.php");

}

void handleGetConfFile() {

  ////Serial.printf("handleGetConfFile\n");

  if (!processAuth()) return;
  if (!processToken()) return;

  int size = getFileSize(CONF_FILE);

  httpServer.sendHeader("Content-Disposition", String("  attachment; filename=\"" + conf.hostName + ".json\" "));
  httpServer.setContentLength(size);
  httpServer.send(200, "application/octet-stream", "");

  int ret = httpServerSendFileContent(CONF_FILE);

}

String getContentType(String filename) {
  if (httpServer.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path) {

  //  Serial.println("handleFileRead: " + path);
  //  if(path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists("/html" + pathWithGz) || SPIFFS.exists("/html" + path)) {
    if (SPIFFS.exists("/html" + pathWithGz))
      path += ".gz";
    File file = SPIFFS.open("/html" + path, "r");
    //  Serial.println("streaming: " + path);
    size_t sent = httpServer.streamFile(file, contentType);
    file.close();

    return true;
  }

  return false;
}

void handleNotFound() {

  //  Serial.print("NOT FOUND ");
  //  Serial.println(httpServer.uri());

  //  prepareHeader(200, "text/html", 4);
  //  httpServer.sendContent("XXXX");

  if (!handleFileRead(httpServer.uri())) httpServer.send(404, "text/plain", "FileNotFound");

}

//Check if header is present and correct
bool is_authentified() {

  //  return true;

  //  Serial.println("Enter is_authentified");
  if (httpServer.hasHeader("Cookie")) {
    //   Serial.print("Found cookie: ");
    String cookie = httpServer.header("Cookie");
    //    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=" + runtime.token) != -1) {
      //      Serial.println("Authentification Successful");
      return true;
    }
  }
  //  Serial.println("Authentification Failed");
  return false;
}

void handleJSONListDomoticzDevices() {
  //    //Serial.printf("handleJSONListDomoticzDevices()\n");

  String url = httpServer.arg("url");

  //int size = getFileSize("/html/before.php") +getFileSize("/html/after.php")+getFileSize("/html/status.php")+3;

  //  httpServer.setContentLength(size);

  if (!getDevices(url)) {

    httpServer.send(500);

  }
  //int ret= httpServerSendFileContent( "/html/before.php");
  //httpServer.sendContent("xxx");

  //int ret2= httpServerSendFileContent( "/html/after.php");

}

//////////////////////////////////////////////////////////////////////

//File fsUploadFileconf;

void handleConfUpload() {

  //Serial.printf("handleConfUpload :%s\n",httpServer.uri().c_str());

  int args_size = httpServer.args();

  //Serial.printf("args_size %d\n",args_size);

  /*  bool hasArg(String name);       // check if argument exists
    String arg(String name);        // get request argument value by name
    String arg(int i);              // get request argument value by number
    String argName(int i);          // get request argument name by number
  */

  //String content=String("");

  for (int i = 0; i < args_size; i++) {
    String name = httpServer.argName(i);
    String value = httpServer.arg(i);

    //    Serial.printf("%s : %s\n",name.c_str(),value.c_str());
  }

  //if (!processToken()) return;

  // handler for the file upload, get's the sketch bytes, and writes
  // them through the Update object
  HTTPUpload upload = httpServer.upload();

  if (upload.status == UPLOAD_FILE_START) {

    /*
        Serial.printf("UPLOAD_FILE_START\n");

        Serial.printf("uri: %s \n",httpServer.uri().c_str());
                    savedConf ="";
    */
    /*
    //    fsUploadFileconf= SPIFFS.open(CONF_FILE, "w");
    //       Serial.printf("3\n");
        if(!fsUploadFileconf)
        {
              Serial.printf("open file failed %s\n" ,CONF_FILE);
              return;
        }
    */
  } else if (processAuth() && upload.status == UPLOAD_FILE_WRITE) {

    //       Serial.printf("UPLOAD_FILE_WRITE\n");

    //       Serial.printf("Writing upload.currentSize %d \n",upload.currentSize );
    //       Serial.printf("Writing buf %s \n",upload.buf);

    savedConf = savedConf + String((char * ) upload.buf);
    //       Serial.printf("savedConf  %s\n",savedConf.c_str() );
    //    confwassaved=true;
    //       Serial.printf("2\n");

    //      fsUploadFileconf.write((const uint8_t*)buff, 3);
    //holds the current upload

    //      fsUploadFileconf.write(upload.buf, upload.currentSize);
    //             Serial.printf("5\n");

    //          fsUploadFileconf.close();
    //       Serial.printf("6\n");

  } else if (processAuth() && upload.status == UPLOAD_FILE_END) {

    //         Serial.printf("UPLOAD_FILE_END\n" );
    //          fsUploadFileconf.close();

    //yield();
    //saveFile(CONF_FILE ,savedConf) ;
    //          Serial.printf("saved\n" );
    //yield();
    //bool savefileret=saveFile(CONF_FILE ,savedConf) ;

  } else if (processAuth() && upload.status == UPLOAD_FILE_ABORTED) {

    Serial.println("UPLOAD_FILE_ABORTED");
  }
  delay(0);

}

void handleConfUploadAfterFinished() {
  //         Serial.println("handleConfUploadAfterFinished");

  saveFile(CONF_FILE, savedConf);

  httpServer.sendHeader("Location", "/");
  httpServer.sendHeader("Cache-Control", "no-cache");
  httpServer.send(302);
  reboot();

}

void handleFactoryReset() {
  Serial.println("handleFactoryReset");

  if (!processToken()) return;

  SPIFFS.remove(CONF_FILE);

  httpServer.sendHeader("Location", "/");
  httpServer.sendHeader("Cache-Control", "no-cache");
  httpServer.send(302);

  reboot();
}
