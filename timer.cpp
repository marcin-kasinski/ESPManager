#include <Arduino.h>

#include <NtpClientLib.h>

#include "timer.h"

#include "device.h"

#include "mqtt.h"

#include "util.h"

#include "temp.h"

#include "udp.h"

#include "led.h"

#include <ESP8266Ping.h>

ITZoneTimer timer1;

void timer_func_resetRelayIfNoInternetConnection(CronObject cronObject) {
  //Serial.printf("TIMER timer_func_resetRelayIfNoInternetConnection executed [%d] \n",device_id);

  IPAddress ip(8, 8, 8, 8); // The remote ip to ping
  //IPAddress ip (2, 2, 2, 2); // The remote ip to ping
  bool ret = Ping.ping(ip);
  //Serial.printf("TIMER timer_func_resetRelayIfNoInternetConnection executed ret [%d] \n",ret);

  if (ret == false && conf.relays[cronObject.device_id].relay_state == 1) {
    Serial.printf("TIMER timer_func_resetRelayIfNoInternetConnection Reseting [%d] \n", ret);
    setRelayState(0, 0);
    delay(3000);
    setRelayState(0, 1);
  }
  /*
    else if (ret==false && conf.relays[index].relay_state==0)
    {
    Serial.printf("TIMER timer_func_resetRelayIfNoInternetConnection Reseting [%d] \n",ret);
    setRelayState(0, 1);
    setRelayState(0, 0);
    }
  */
}

/*
  void timer_func_updateTemperature(CronObject cronObject) {
  //Serial.printf("TIMER timer_func_updateTemperature executed [%d] \n",device_id);
  MQTTLogMessage(String("TIMER timer_func_updateTemperature executed device_id =") + device_id);
  //processTemp();
  }
*/
void timer_func_generateAppToken(CronObject cronObject) {
  //Serial.printf("TIMER timer_func_generateAppToken executed [%d] \n",device_id);
  MQTTLogMessage(String("TIMER timer_func_generateAppToken executed device_id =") + cronObject.device_id);
  generateToken();
  MQTTLogMessage(String("TIMER timer_func_generateAppToken executed END "));

}

void timer_func_sendUDPpacket(CronObject cronObject) {
  MQTTLogMessage(String("TIMER timer_func_sendUDPpacket executed device_id =") + cronObject.device_id);
  //Serial.printf("TIMER timer_func_sendUDPpacket executed [%d] \n",cronObject.device_id);
  sendUDPPing();
  MQTTLogMessage(String("TIMER timer_func_sendUDPpacket executed END"));
}

void timer_func_processRelayLed(CronObject cronObject) {
  //Serial.printf("TIMER timer_func_processRelayLed executed [%d] \n",cronObject.device_id);
  MQTTLogMessage(String("TIMER processRelayLed executed device_id =") + cronObject.device_id);
  processRelayLed(cronObject.device_id);
  MQTTLogMessage(String("TIMER processRelayLed executed END"));
}

//Firmware Update Check
void timer_func_firmware_update_check(CronObject cronObject) {
  addAppLogMessage(String("TIMER Firmware Update Check executed [") + cronObject.device_id + "]");

  //Serial.printf("TIMER Firmware Update Check executed [%d] \n",device_id);
  MQTTLogMessage(String("TIMER Firmware Update Check executed"));
  updateFirmwareFromNet();
}

void timer_func_relayOn(CronObject cronObject) {
  MQTTLogMessage(String("TIMER relayOn executed device_id =") + cronObject.device_id);
  addAppLogMessage(String("TIMER relayOn executed [") + cronObject.device_id + "]");
  //Serial.printf("TIMER relayOn executed [%d] \n",cronObject.device_id);
  //conf.relays[device_id].relay_state=1;

  setRelayStatePermanently(cronObject.device_id, 1);
  //Serial.printf("TIMER relayOn executed END\n");
  MQTTLogMessage(String("TIMER relayOn executed END"));
}

void timer_func_relayOff(CronObject cronObject) {
  MQTTLogMessage(String("TIMER relayOff executed device_id =") + cronObject.device_id);
  addAppLogMessage(String("TIMER relayOff executed [") + cronObject.device_id + "]");
  //Serial.printf("TIMER relayOff executed [%d] \n",cronObject.device_id);
  //conf.relays[device_id].relay_state=0;
  setRelayStatePermanently(cronObject.device_id, 0);
  //Serial.printf("TIMER relayOff executed END\n");
  MQTTLogMessage(String("TIMER relayOff executed END"));
}

/*
  void timer_func_RGBLedOn(byte device_id) {
  MQTTLogMessage(String("TIMER RGBLedOn executed device_id =") + device_id);
  addAppLogMessage(String("TIMER RGBLedOn executed [") + device_id + "]");

  RGBLedSetStatePermanently(1);

  }
*/
/*
  void timer_func_RGBLedOff(byte device_id) {
  MQTTLogMessage(String("TIMER RGBLedOff executed device_id =") + device_id);
  addAppLogMessage(String("TIMER RGBLedOff executed [") + device_id + "]");

  RGBLedSetStatePermanently(0);

  }
*/
void timer_func_getSunriseSunsetTime(CronObject cronObject) {
  //if (runtime.curr_hour<4) return;

  //Serial.printf("TIMER getSunsetTime executed [%d] \n",device_id);
  MQTTLogMessage(String("TIMER getSunsetTime executed device_id =") + cronObject.device_id);
  getSunriseSunset(conf.sunsetAPIKey, conf.sunsetAPICity);
  MQTTLogMessage(String("TIMER getSunsetTime executed END"));
}


void set_hour_and_time_with_shift(byte &hour_after_shift, byte &min_after_shift, String shift_value) {



  if (shift_value.equals("")) return;
//  MQTTLogMessage(String ("shift_value =") + shift_value);

  //minus, czy plus
  byte shift_mode = shift_value.charAt(0);
  byte space_location = shift_value.indexOf(" ");
  byte h_location = shift_value.indexOf("h");
  byte m_location = shift_value.indexOf("m");
  byte shift_hour = 0;
  byte shift_min = 0;


  //jeśli jest minuta i godzina
  if (space_location != 255)
  {

    shift_hour = shift_value.substring(1, h_location).toInt();
    shift_min = shift_value.substring(space_location, shift_value.length() - 1).toInt();
  }
  // jest tylko godzina
  else if (h_location != 255)
  {

    shift_hour = shift_value.substring(1, shift_value.length()).toInt();

  }
  // jest tylko minuta
  else if (m_location != 255)
  {

    shift_min = shift_value.substring(1, shift_value.length()).toInt();

  }

  //  MQTTLogMessage("###############################################");


  if (shift_mode == '-')
  {
    MQTTLogMessage(String ("MINUS"));

    if (shift_min > min_after_shift)
    {
      hour_after_shift = hour_after_shift - 1;

      min_after_shift = 60 - (shift_min - min_after_shift);
    }//  if (shift_min<min_after_shift)
    else  min_after_shift = min_after_shift - shift_min;

    hour_after_shift = hour_after_shift - shift_hour;
  }

  else if (shift_mode == '+')
  {
    MQTTLogMessage(String ("PLUS"));
    hour_after_shift = hour_after_shift + shift_hour;

    if ((shift_min + min_after_shift) > 60)
    {
      hour_after_shift = hour_after_shift + shift_hour;
      min_after_shift = min_after_shift - (60 - shift_min) ;

    }
    else  min_after_shift = min_after_shift + shift_min;


  }

  //  MQTTLogMessage(String ("shift_min =") + shift_min);
  //  MQTTLogMessage(String ("shift_hour =") + shift_hour);
  MQTTLogMessage(String ("hour_after_shift =") + hour_after_shift);
  MQTTLogMessage(String ("min_after_shift =") + min_after_shift);
  //  MQTTLogMessage(String ("shift_value =") + shift_value);
//  MQTTLogMessage(String ("shift_mode =") + shift_mode);




  //min_after_shift=111;
}


void timer_func_relayOnAtSunset(CronObject cronObject) {

  byte hour_after_shift = runtime.curr_hour;
  byte min_after_shift = runtime.curr_min;

  set_hour_and_time_with_shift(hour_after_shift, min_after_shift, cronObject.function_name_parameter) ;


  MQTTLogMessage(String ("TIMER timer_func_relayOnAtSunset device_id =") + cronObject.device_id);
  MQTTLogMessage(String ("hour_after_shift =") + hour_after_shift);
  MQTTLogMessage(String ("min_after_shift =") + min_after_shift);



  if (hour_after_shift == runtime.sunset_hour && min_after_shift == runtime.sunset_minute) {
    //  MQTTLogMessage(String ("TIMER relayOnAtSunset This is sunset device_id =")+device_id);
    //conf.relays[device_id].relay_state=1;
    setRelayStatePermanently(cronObject.device_id, 1);
  }
}

void timer_func_relayOffAtSunrise(CronObject cronObject) {
  MQTTLogMessage(String ("TIMER timer_func_relayOffAtSunrise device_id =") + cronObject.device_id);

  byte hour_after_shift = runtime.curr_hour;
  byte min_after_shift = runtime.curr_min;

  set_hour_and_time_with_shift(hour_after_shift, min_after_shift, cronObject.function_name_parameter) ;


  //Serial.printf("timer_func_relayOffAtSunrise runtime.curr_hour [%d] runtime.sunrise_hour [%d] runtime.curr_min [%d] runtime.sunrise_minute [%d] \n", runtime.curr_hour ,runtime.sunrise_hour,runtime.curr_min ,runtime.sunrise_minute);


  if (hour_after_shift  == runtime.sunrise_hour && min_after_shift== runtime.sunrise_minute) {
    setRelayStatePermanently(cronObject.device_id, 0);
  }
}

/*
  void timer_func_RGBLedOnAtSunset(byte device_id) {
  if (runtime.curr_hour == runtime.sunset_hour && runtime.curr_min == runtime.sunset_minute) RGBLedSetStatePermanently(1);

  }

  void timer_func_RGBLedOffAtSunrise(byte device_id) {
  if (runtime.curr_hour == runtime.sunrise_hour && runtime.curr_min == runtime.sunrise_minute) RGBLedSetStatePermanently(0);

  }
*/
ITZoneTimer::ITZoneTimer() {}

void ITZoneTimer::begin() {
  //Serial.printf("ITZoneTimer::begin \n");
}

byte ITZoneTimer::getCurrentSec() {
  byte ret = -1;
  String str = NTP.getTimeDateString();
  //Serial.printf("getCurrentSec [%s]\n",str.c_str());

  if (str.equals("Time not set") || str.endsWith("01/01/1970")) return -1;

  str = str.substring(6, 8);
  //Serial.printf("getCurrentSec [%s]\n",str.c_str());

  ret = str.toInt();
  //Serial.printf("getCurrentSec ret [%d]\n",ret);

  return ret;
}

byte ITZoneTimer::getCurrentMin() {
  byte ret = -1;
  String str = NTP.getTimeDateString();

  if (str.equals("Time not set") || str.endsWith("01/01/1970")) return -1;

  str = str.substring(3, 5);

  ret = str.toInt();
  //Serial.printf("getCurrentMin [%s]\n",str.c_str());

  return ret;
}

byte ITZoneTimer::getCurrentHour() {
  byte ret = -1;
  String str = NTP.getTimeDateString();

  if (str.equals("Time not set") || str.endsWith("01/01/1970")) return -1;

  str = str.substring(0, 2);

  ret = str.toInt();
  //Serial.printf("getCurrentHour [%s]\n",str.c_str());

  return ret;
}

byte ITZoneTimer::getCurrentDay() {
  byte ret = -1;
  String str = NTP.getDateStr();

  if (str.equals("Time not set") || str.endsWith("01/01/1970")) return -1;
  //Serial.printf("getCurrentDay [%s]\n",str.c_str());

  str = str.substring(0, 2);

  ret = str.toInt();
  //Serial.printf("getCurrentDay [%s]\n",str.c_str());

  return ret;
}

byte ITZoneTimer::getCurrentMonth() {
  byte ret = -1;
  String str = NTP.getDateStr();
  //Serial.printf("getCurrentMonth [%s]\n",str.c_str());

  if (str.equals("Time not set") || str.endsWith("01/01/1970")) return -1;

  str = str.substring(3, 5);

  ret = str.toInt();
  //Serial.printf("getCurrentMonth [%s]\n",str.c_str());

  return ret;
}

int ITZoneTimer::getCurrentYear() {
  int ret = -1;
  String str = NTP.getDateStr();

  if (str.equals("Time not set") || str.endsWith("01/01/1970")) return -1;
  //Serial.printf("getCurrentYear [%s]\n",str.c_str());

  str = str.substring(6, 10);
  //Serial.printf("getCurrentYear [%s]\n",str.c_str());

  ret = str.toInt();
  //Serial.printf("getCurrentYear ret [%d]\n",ret);

  return ret;
}

bool ITZoneTimer::isElementInList(String element, String list) {
  bool ret = false;

  //Serial.printf("isElementInList element [%s] list = [%s] \n",element.c_str(),list.c_str());

  char str[list.length()];

  strcpy(str, list.c_str());

  char * pch = strtok(str, " ,.-");
  while (pch != NULL) {

    int cmp = strcmp(pch, element.c_str());
    //    Serial.printf ("%s %d\n",pch,cmp);

    // lesli jest na liście
    if (strcmp(pch, element.c_str()) == 0) {
      ret = true;
      break;
    }

    pch = strtok(NULL, " ,.-");
  }

  //  Serial.printf("isElementInList ret [%d]\n",ret);

  return ret;
}

bool ITZoneTimer::isEven(String instr, byte in_value) {
  bool ret = false;
  //  Serial.printf("isEven [%s] [%d]\n",instr.c_str(), in_value);

  // jesli to nie jest even wychodzi
  if (!instr.equals("Even")) return false;

  if (in_value % 2 == 0) ret = true;

  // Serial.printf("isEven ret [%d]\n",ret);

  return ret;
}

bool ITZoneTimer::isOdd(String instr, byte in_value) {
  bool ret = false;

  //  Serial.printf("isOdd [%s] [%d]\n",instr.c_str(), in_value);

  // jesli to nie jest Odd wychodzi
  if (!instr.equals("Odd")) return false;

  if (in_value & 1) ret = true;

  // Serial.printf("isOdd ret [%d]\n",ret);

  return ret;
}

bool ITZoneTimer::isRange(String instr, byte in_value) {
  bool ret = false;

  //Serial.printf("isRange instr [%s] in_value = [%d] \n",instr.c_str(),in_value);
  //Serial.printf("XXX [%s] \n",instr.substring(0,6).c_str());

  // jesli to nie jest Range wychodzi
  if (!instr.substring(0, 6).equals("Range ")) return false;

  String msg = instr.substring(6, instr.length());

  //Serial.printf("isRange msg [%s] \n",msg.c_str());
  byte pos = msg.indexOf("-");

  //Serial.printf("pos [%d] \n",pos);

  //Serial.printf("isRange instr [%s] \n",instr.substring(2,instr.length()).c_str());

  int from_value = msg.substring(0, pos).toInt();
  int to_value = msg.substring(pos + 1, msg.length()).toInt();
  //Serial.printf("isRange from_value [%d]  to_value [%d] \n",from_value,to_value);

  if (in_value >= from_value && in_value <= to_value) ret = true;

  // Serial.printf("isRange ret [%d]\n",ret);

  return ret;

}

bool ITZoneTimer::isEvery(String instr, byte in_value) {
  bool ret = false;

  //Serial.printf("isEvery instr [%s] in_value = [%d] \n",instr.c_str(),in_value);
  //Serial.printf("XXX [%s] \n",instr.substring(0,6).c_str());

  // jesli to nie jest every wychodzi
  if (!instr.substring(0, 6).equals("Every ")) return false;

  //Serial.printf("isEvery instr [%s] \n",instr.substring(2,instr.length()).c_str());

  int every_value = instr.substring(6, instr.length()).toInt();
  //Serial.printf("isEvery every_value [%d] \n",every_value);

  byte modulo = in_value % every_value;
  //Serial.printf("modulo [%d] \n",modulo);

  // jeśli mieści się w every
  if (modulo == 0) ret = true;

  //  Serial.printf("isEvery ret [%d]\n",ret);

  return ret;
}

void ITZoneTimer::processCronObject(CronObject cronObject) {

  /*
    byte curr_hour= getCurrentHour();
    byte curr_min= getCurrentMin();
    byte curr_day=getCurrentDay();
    byte curr_month=getCurrentMonth();
  */

  byte curr_min = runtime.curr_min;

  byte curr_hour = runtime.curr_hour;

  byte curr_day = runtime.curr_day;
  byte curr_month = runtime.curr_month;

  //if (curr_min%conf.logTimerMessagesEveryXMinutes==0) MQTTLogMessage(String ("processCronObjects Alert datetime hour =")+cronObject.hour.c_str()+" min= "+cronObject.minute.c_str()+" day= "+cronObject.day.c_str()+" month= "+cronObject.month.c_str()+" ( func "+cronObject.function_name+")");
  //if (curr_min%conf.logTimerMessagesEveryXMinutes==0) Serial.printf("processCronObjects Alert datetime hour =%s min = %s day %s  month %s weekday = %s \n",cronObject.hour.c_str(),cronObject.minute.c_str(),cronObject.day.c_str(),cronObject.month.c_str(),cronObject.weekday.c_str());

  //Serial.printf("curr_day= %d \n",curr_day);
  //Serial.printf("curr_month= %d \n",curr_month);

  //if (curr_min%conf.logTimerMessagesEveryXMinutes==0) Serial.printf("CURR datetime hour = %d min = %d day %d  month %d weekday = %d \n",curr_hour,curr_min,curr_day,curr_month,last_red_wday);
  if (

    (cronObject.minute.equals("*") || isElementInList(String(curr_min), cronObject.minute) || isEvery(cronObject.minute, curr_min) || isRange(cronObject.minute, curr_min) || isEven(cronObject.minute, curr_min) || isOdd(cronObject.minute, curr_min)) &&
    (cronObject.hour.equals("*") || isElementInList(String(curr_hour), cronObject.hour) || isEvery(cronObject.hour, curr_hour) || isRange(cronObject.hour, curr_hour)) &&
    (cronObject.day.equals("*") || isElementInList(String(curr_day), cronObject.day) || isEvery(cronObject.day, curr_day) || isRange(cronObject.day, curr_day)) &&
    (cronObject.month.equals("*") || isElementInList(String(curr_month), cronObject.month) || isEvery(cronObject.month, curr_month) || isRange(cronObject.month, curr_month)) &&
    (cronObject.weekday.equals("*") || isElementInList(String(last_red_wday), cronObject.weekday))

  )
  {
    //Serial.printf("EXECUTING TIMER START [%s] [%s] [%s] [%d]\n",cronObject.type.c_str(), cronObject.function_name.c_str(),cronObject.minute.c_str(), curr_min);


    THandlerFunction_Callback fn_Callback;




    if (cronObject.function_name.equals("Relay on")) fn_Callback = timer_func_relayOn;
    else if (cronObject.function_name.equals("Relay off")) fn_Callback = timer_func_relayOff;

    //  else if (cronObject.function_name.equals("RGBLed on")) fn_Callback = timer_func_RGBLedOn;
    //  else if (cronObject.function_name.equals("RGBLed off")) fn_Callback = timer_func_RGBLedOff;
    //  else if (cronObject.function_name.equals("RGBLed off at sunrise")) fn_Callback = timer_func_RGBLedOffAtSunrise;
    //  else if (cronObject.function_name.equals("RGBLed on at sunset")) fn_Callback = timer_func_RGBLedOnAtSunset;

    else if (cronObject.function_name.equals("Relay off at sunrise")) fn_Callback = timer_func_relayOffAtSunrise;
    else if (cronObject.function_name.equals("Relay on at sunset")) fn_Callback = timer_func_relayOnAtSunset;
    else if (cronObject.function_name.equals("Get sunrise and sunset time")) fn_Callback = timer_func_getSunriseSunsetTime;
    else if (cronObject.function_name.equals("Send PING to cluster")) fn_Callback = timer_func_sendUDPpacket;
    else if (cronObject.function_name.equals("Generate App token")) fn_Callback = timer_func_generateAppToken;
    else if (cronObject.function_name.equals("Reset relay if no internet connection")) fn_Callback = timer_func_resetRelayIfNoInternetConnection;
    else if (cronObject.function_name.equals("Firmware update check")) fn_Callback = timer_func_firmware_update_check;
    else if (cronObject.function_name.equals("Process relay led")) fn_Callback = timer_func_processRelayLed;
    //  else if (cronObject.function_name.equals("Update temperature")) fn_Callback = timer_func_updateTemperature;

    fn_Callback(cronObject);

    //Serial.printf("EXECUTING TIMER END [%s] [%s] [%s] [%d]\n",cronObject.type.c_str(), cronObject.function_name.c_str(),cronObject.minute.c_str(), curr_min);
  }
}

uint8_t ITZoneTimer::getDayOfWeek() {
  //Got NTP time: 11:17:30 20/02/2017
  //Serial.printf("getTimeDateString()1 = %s\n",NTP.getTimeDateString().c_str());

  //int year = getCurrentYear();
  //int month  = getCurrentMonth();
  //int day =  getCurrentDay();
  //int hour =  getCurrentHour();
  //int min =  getCurrentMin();

  time_t nowTime = NTP.getTime();
  //Serial.printf("getTimeDateString()2 = %s\n",NTP.getTimeDateString().c_str());

  uint32_t time = (uint32_t) nowTime;
  time /= 60; // now it is minutes
  time /= 60; // now it is hours
  time /= 24; // now it is days

  //Serial.printf("getTimeDateString()3 = %s\n",NTP.getTimeDateString().c_str());
  //uint8_t wday= ((time + 4) % 7) + 1; // Sunday is day 1
  uint8_t wday = ((time + 4) % 7); // Monday is day 1

  //Serial.printf("getTimeDateString()4 = %s\n",NTP.getTimeDateString().c_str());

  return wday;

}

void ITZoneTimer::process() {
  //delay (1000);
  //XXXXXXXXXXXXXX
  //Serial.printf("process START\n");

  byte curr_sec = getCurrentSec();

  if (curr_sec == -1) return;
  //if (curr_sec==0 ||curr_sec==255 ) return;

  byte curr_min = getCurrentMin();
  if (curr_min == last_red_min) return;

  byte curr_hour = getCurrentHour();

  //  if (curr_hour == last_red_hour && curr_min == last_red_min) return;

  byte curr_day = getCurrentDay();
  byte curr_month = getCurrentMonth();

  runtime.curr_hour = curr_hour;
  runtime.curr_min = curr_min;
  runtime.curr_day = curr_day;
  runtime.curr_month = curr_month;

  //Serial.printf("getTimeDateString() = %s\n",NTP.getTimeDateString().c_str());

  //Serial.printf("runtime.curr_hour  = %d runtime.curr_min = %d \n",runtime.curr_hour ,runtime.curr_min);


  //  byte last_red_hour=-1;
  //  byte last_red_min=-1;

  byte modulo = curr_min % 1;

  //Serial.printf("TMP modulo = %d\n",modulo);

  if (modulo != 0) return;

  //Serial.printf("getTimeDateString() = %s\n",NTP.getTimeDateString().c_str());

  //if (curr_min%conf.logTimerMessagesEveryXMinutes ==0) MQTTLogMessage("hour = %d min = %d sec = %d\n",curr_hour,curr_min,curr_sec);

  //Serial.printf("year = [%d]\n",getCurrentYear());

  if ((curr_hour == 0 && curr_min == 0) || last_red_wday == 255) {

    uint8_t wday = getDayOfWeek();

    last_red_wday = wday;
  }

  //loguj do mqtt tylko co 5 minut
  if (curr_min % conf.logTimerMessagesEveryXMinutes == 0) MQTTLogMessage(String("process Timer hour =") + curr_hour + " min= " + curr_min + " day= " + curr_day + " month= " + curr_month + " dayofweek= " + last_red_wday);

  //if (curr_min%conf.logTimerMessagesEveryXMinutes ==0) Serial.printf("day of week = %d \n",last_red_wday);

  // tm.Wday = ((time + 4) % 7) + 1; // Sunday is day 1

  int listSize = cronObjects.size();
  for (int i = 0; i < listSize; i++) processCronObject(cronObjects.get(i));

  //if (curr_hour==timerObjects[i].runtime_hour && curr_min==timerObjects[i].runtime_min)        timerObjects[i].fn_Callback(timerObjects[i].device_id);

  last_red_hour = curr_hour;
  last_red_min = curr_min;
  //Serial.printf("process END\n");

}

void ITZoneTimer::addTask(String type, byte device_id, String function_name, String function_name_parameter, String minute, String hour, String day, String month, String weekday)

//void ITZoneTimer::setRunTime(byte index, byte hour, byte min,THandlerFunction_Callback fn)
{

  //String hourstr=time.substring(0,2);
  //String minstr=time.substring(3,5);

  //byte hour=hourstr.toInt();
  //byte min=minstr.toInt();

  //Serial.printf("TIMER SET RUNTIME device %d function %s hour %s min %s day %s month %s weekday %s\n", device_id , function_name.c_str(), hour.c_str(), minute.c_str(), day.c_str(), month.c_str(), weekday.c_str() );

  //MQTTLogMessage("TIMER set func "+function_name+" hour "+hour+" minute "+minute);

  CronObject cronObject;

  cronObject.type = type;

  cronObject.minute = minute;
  cronObject.minute_logical = minute;
  cronObject.hour = hour;
  cronObject.hour_logical = hour;
  cronObject.day = day;
  cronObject.day_logical = day;
  cronObject.month = month;
  cronObject.month_logical = month;
  cronObject.weekday = weekday;
  cronObject.weekday_logical = weekday;

  if (minute.startsWith("Random")) cronObject.minute = getRandomNumbers(minute);
  if (hour.startsWith("Random")) cronObject.hour = getRandomNumbers(hour);
  if (day.startsWith("Random")) cronObject.day = getRandomNumbers(day);
  if (month.startsWith("Random")) cronObject.month = getRandomNumbers(month);
  if (weekday.startsWith("Random")) cronObject.weekday = getRandomNumbers(weekday);

  cronObject.function_name = function_name;
  cronObject.function_name_parameter = function_name_parameter;
  cronObject.device_id = device_id;

  //  if (cronObject.function_name.equals("Relay on")) cronObject.fn_Callback = timer_func_relayOn;
  //  else if (cronObject.function_name.equals("Relay off")) cronObject.fn_Callback = timer_func_relayOff;

  //  else if (cronObject.function_name.equals("RGBLed on")) cronObject.fn_Callback = timer_func_RGBLedOn;
  //  else if (cronObject.function_name.equals("RGBLed off")) cronObject.fn_Callback = timer_func_RGBLedOff;
  //  else if (cronObject.function_name.equals("RGBLed off at sunrise")) cronObject.fn_Callback = timer_func_RGBLedOffAtSunrise;
  //  else if (cronObject.function_name.equals("RGBLed on at sunset")) cronObject.fn_Callback = timer_func_RGBLedOnAtSunset;

  //  else if (cronObject.function_name.equals("Relay off at sunrise")) cronObject.fn_Callback = timer_func_relayOffAtSunrise;
  //  else if (cronObject.function_name.equals("Relay on at sunset")) cronObject.fn_Callback = timer_func_relayOnAtSunset;
  //  else if (cronObject.function_name.equals("Get sunrise and sunset time")) cronObject.fn_Callback = timer_func_getSunriseSunsetTime;
  //  else if (cronObject.function_name.equals("Send PING to cluster")) cronObject.fn_Callback = timer_func_sendUDPpacket;
  //  else if (cronObject.function_name.equals("Generate App token")) cronObject.fn_Callback = timer_func_generateAppToken;
  //  else if (cronObject.function_name.equals("Reset relay if no internet connection")) cronObject.fn_Callback = timer_func_resetRelayIfNoInternetConnection;
  //  else if (cronObject.function_name.equals("Firmware update check")) cronObject.fn_Callback = timer_func_firmware_update_check;
  //  else if (cronObject.function_name.equals("Process relay led")) cronObject.fn_Callback = timer_func_processRelayLed;
  //  else if (cronObject.function_name.equals("Update temperature")) cronObject.fn_Callback = timer_func_updateTemperature;

  cronObjects.add(cronObject);

}

/*
  void ITZoneTimer::setcallbackFunction(THandlerFunction_Callback fn)
  {

  }

*/
