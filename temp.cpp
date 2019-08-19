/*
#include <ESP8266HTTPClient.h>


#include <TimeLib.h>
#include <TimeAlarms.h>

#include <OneWire.h>
#include "temp.h"

#include <TimeLib.h>
#include <NtpClientLib.h>

#include "ntp.h"
#include "mqtt.h"

#include "util.h"

//Timer t;



float getTemp(void) {

      MQTTLogMessage(String("getTemp" ) );


OneWire  ds(conf.temperatureSensor_tempsensor_pin);   // D2 =4




  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(6000);
    return -1;
  }
  
  Serial.print("ROM =");
  for( i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1;
  }
  Serial.println();
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      MQTTLogMessage(String("Chip = DS18S20" ) ); // or old DS1820
      type_s = 1;
      break;
    case 0x28:
    MQTTLogMessage(String("Chip = DS18B20" ) );
      type_s = 0;
      break;
    case 0x22:
    MQTTLogMessage(String("Chip = DS1822" ) );
      type_s = 0;
      break;
    default:
        MQTTLogMessage(String("Device is not a DS18x20 family device." ) );
      return -1;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  Serial.print("  Data = ");
  Serial.print(present,HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  // convert the data to actual temperature

  unsigned int raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // count remain gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw << 3;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw << 2; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw << 1; // 11 bit res, 375 ms
    // default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");

  return celsius;

}

void sendTempValue()
{
  // int status = WL_IDLE_STATUS;     // the Wifi radio's status
//WiFiClient client;
HTTPClient client;


 int port = 8080; //Domoticz port
 int idx = 2; //IDX for this virtual sensor, found in Setup -> Devices

 float humidity = 0;
 float temp = getTemp() ;

if (temp==-1)
{
return;
}
  MQTTLogMessage("sending temp"); 


//Retrieve status of specific device


//http://admin:admin@192.168.1.151:8080/json.htm?type=devices&rid=3
//http://admin:admin@192.168.1.151:8080/json.htm?type=devices&used=true&filter=all&favorite=1
//http://admin:admin@192.168.1.151:8080/json.htm?type=devices&filter=all&used=true&order=Name
String url ="http://admin:admin@192.168.1.151:8080/json.htm?type=command&param=udevice&idx="+String(idx)+"&nvalue=0&svalue="+String(temp)+";";

     client.begin(url); //HTTP

  int httpRetCode = client.GET();

  Serial.printf("ret code [%d]\n",httpRetCode ); 

   if(httpRetCode == HTTP_CODE_OK) {
                String payload = client.getString();
                Serial.println(payload);

        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", client.errorToString(httpRetCode).c_str());
        }

        client.end();


    Serial.println("Temp sent"); 
}

void processTemp()
{
  
if (conf.temperatureSensor_tempsensor_pin<0) return;

  Serial.println("processTemp"); 

  float temp=getTemp();  

  if (conf.MQTT_enable && conf.temperatureSensor_domoticz_device_idx>0)
  {
    MQTTpublish(String("{ \"idx\": ")+conf.temperatureSensor_domoticz_device_idx+", \"nvalue\" : 0, \"svalue\": \""+temp+"\"  }");
  }

  runtime.temperature=temp;
  runtime.temperature_measure_time=NTP.getTimeDateString() ;

} 

void initTempSensor()
{

    Serial.println("initTempSensor");

  Alarm.timerRepeat(5, processTemp);      // timer for every 2 seconds

//   int tickEvent = t.every(2000, getTemp);
}
*/
