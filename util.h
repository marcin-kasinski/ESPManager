#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ESP8266WebServer.h>
#include "ESP8266HTTPUpdateServer.h"
#include <LinkedList.h>


#define RELAY_SWITCH_PIN_PROCESS_METHOD_INTERRUPT 1
#define RELAY_SWITCH_PIN_PROCESS_METHOD_LOOP 0



struct AppLog {


  String datetime;
  String message;

};


extern LinkedList<AppLog> appLogs;

const byte RELAY_SWITCH_TYPE_SWITCH =1;
const byte RELAY_SWITCH_TYPE_BUTTON =0;

const char * const PHY_MODE_NAMES[]
{
    "",
    "PHY_MODE_11B",
    "PHY_MODE_11G",
    "PHY_MODE_11N"
};

const char * const FLASH_SIZE_MAP_NAMES[] =
{
    "FLASH_SIZE_4M_MAP_256_256",
    "FLASH_SIZE_2M",
    "FLASH_SIZE_8M_MAP_512_512",
    "FLASH_SIZE_16M_MAP_512_512",
    "FLASH_SIZE_32M_MAP_512_512",
    "FLASH_SIZE_16M_MAP_1024_1024",
    "FLASH_SIZE_32M_MAP_1024_1024"
};


const char BUTTON[] = "BUTTON"; 
const char SWITCH[] = "SWITCH"; 

const char * const SWITCH_TYPES[] =
//char * const SWITCH_TYPES[] =
{
    BUTTON,
    SWITCH
};

//String SWITCH_TYPES22[] = { "BUTTON","SWITCH"};

static const uint8_t DD1   = 5;
static const uint8_t DD2   = 4;

#define CONF_FILE "/conf/conf.props" 

#define RUNTIME_MODE_OK 1
#define RUNTIME_MODE_WIFI_PROBLEM -2
#define RUNTIME_MODE_MQTT_PROBLEM -1

#define FIRMWARE_TYPE_FIRMWARE 1
#define FIRMWARE_TYPE_SPIFFS 2

#define RELAY_CONN_TYPE_NO 0
#define RELAY_CONN_TYPE_NC 1

#define LED_CONN_TYPE_HIGH_ON 1
#define LED_CONN_TYPE_LOW_ON 0

#define LED_ON_WHEN_RELAY_ON 1
#define LED_ON_WHEN_RELAY_OFF 0

extern unsigned long OSWATCH_RESET_TIME;
extern volatile unsigned long last_loop;

extern String savedConf;
extern bool confwassaved;


bool isValidJson(String in);
void updateSunriseSunsetInfo(String sunset_update_time, String sunrise, String sunset);

void addAppLogMessage(String message);
void getSunriseSunset(String key, String city);

String getHourString (byte hour, byte minute);

void processAPReboot();


void sendToDomoticz();
//void processFirmwareUpdateSheduler();
void updateFirmwareFromNet();
//char* string2char(String command);

bool isWebAddressAvailable(String webPage);
void reboot();
void initWatchdog();

//void initMDNS();

//String getUpTime();

String scanNetworks();

String getRandomNumbers(String input);
void generateToken();
void LogMessage(const char *message);
bool processConfig();
bool loadConfig();
//bool loadConfigFromEEPROM();
bool saveConfig();
void initOTA();
void processOTA();

//char * getConfigParam(char * paramName);

extern ESP8266WebServer httpServer;
//extern ESP8266HTTPUpdateServer httpUpdater;


struct RGBLed {

int state=0;
boolean enable=false;
byte r=0;
byte g=0;
byte b=0;
byte w=255;
byte wa=0;
};

struct Relay {

String internalName="";

int relay_pin=-1;// int 1         /// gpio 12 sonoff, gpio 13 sonoff led
int relay_switch_pin=-1;// D2 = 4, D4 =0 , sonoff 0 

bool relay_switch_pressed = false;

int relay_switch_pin_process_method=RELAY_SWITCH_PIN_PROCESS_METHOD_INTERRUPT;
//int relay_switch_pin_process_method=RELAY_SWITCH_PIN_PROCESS_METHOD_LOOP;

//const byte RELAY_SWITCH_TYPE_SWITCH =1;
//const byte RELAY_SWITCH_TYPE_BUTTON =0;

byte relay_switch_type=RELAY_SWITCH_TYPE_BUTTON;
//byte relay_switch_type=RELAY_SWITCH_TYPE_SWITCH;

int relay_state=0;
int relay_conn_type=RELAY_CONN_TYPE_NC;

int relay_led_pin=-1;
int relay_led_conn_type=LED_CONN_TYPE_HIGH_ON;
int relay_led_on_when=LED_ON_WHEN_RELAY_OFF;
int domoticz_device_idx=0;
  
};

struct CONF {

//int json_max_length=4096;
byte max_applog_size=250;

//char* web_username = "admin";

//char* web_password = "admin76";

volatile bool serialLogging=true;
volatile bool interruptProcess=false;

String hostName ; 
String broadcast ; 
float firmware_version = 15.28;
float spiffs_version = 0; 
String web_username;
String web_password;

Relay relays[2];

RGBLed rgbled ;
int temperatureSensor_tempsensor_pin=-2;//4;
int temperatureSensor_domoticz_device_idx=0;

String  wifi_ssid="";
String  wifi_password="";
String  wifi_AP_password="administrator";

int WIFI_STA_RECCONECT_COUNT=5;
//int WIFI_STA_RECCONECT_DELAY=2000;
//co tydzień
//unsigned long firmware_update_mins=10080;
char firmware_url[200] ;

unsigned int ntpUDPLocalPort = 2390;      // local port to listen for UDP packets
String ntpserver;

String OTA_password="admin76";
boolean security_enable=true;
boolean OTA_enable=false;
boolean discoverable=false;

boolean MQTT_enable=false;
String MQTT_hostName;
String Domoticz_hostName;
int Domoticz_port=8080;
String Domoticz_userName;
String Domoticz_password;

boolean network_update=false; // czy ma sprawdzać nową wersję po sieci?

int logTimerMessagesEveryXMinutes =10;

String sunsetAPIKey;
String sunsetAPICity;

};

extern CONF conf;
/*
class RUNTIME {
public:
char ip[16] = {0}; 
byte runtime_mode=-2000;
unsigned long start_milis=0;
unsigned long last_firmware_update=0;
// co ile minut ma sprawdzać w sieci, czy nie ma nowego firmware.

};
*/
struct RUNTIME {

int json_buffer_length=2100;
byte curr_hour;
byte curr_min;
byte curr_day;
String sunset_update_time;
byte sunset_minute;
byte sunset_hour;
byte sunrise_minute;
byte sunrise_hour;
byte curr_month;
String token="1";

String pingtopic="itzone/ping";

float temperature;
String temperature_measure_time;

boolean getSunriseSunsetRequest=false;

char ip[16] = {0};
byte runtime_mode=-2000;
unsigned long start_milis=0;
//unsigned long last_firmware_update=0;
//unsigned long last_temerature_get=-900000;
String mac_STA;
// co ile minut ma sprawdzać w sieci, czy nie ma nowego firmware.

boolean rainbow_state_enable=false;
byte state_before_rainbow;

LinkedList<String> udpmessages = LinkedList<String>();

};

extern RUNTIME runtime;

struct FirmwareVersionStruct {


float firmware_version;
float spiffs_version;
String firmware;
String spiffs;

};

#endif
