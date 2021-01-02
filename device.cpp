#include <stdlib.h>

#include <string.h>

#include <ESP8266WiFi.h>

#include "util.h"

#include "device.h"

#include "Button.h"

#include "mqtt.h"

#include "timer.h"

#include <NtpClientLib.h>


long debouncing_time = 120; //Debouncing Time in Milliseconds
volatile long last_micros[2];
Button switchs[2]; // Connect your button between pin 2 and GND

//byte current_hour_for_led=-1;

void processRelayLed(int index) {
  /*
  String datetime=NTP.getTimeDateString();

  if (datetime.equals("Time not set")) return;

  byte current_hour_local=atoi(datetime.substring(0,2).c_str());

  if (current_hour_local==current_hour_for_led) return;

  //jeśli zmieniła sie godzina 

  current_hour_for_led=current_hour_local;

  if (conf.relays[index].relay_state==1) return;
  */

  if (conf.relays[index].relay_led_pin < 0) return;

  if (conf.relays[index].relay_state == 1) return;

  byte current_hour_for_led = timer1.getCurrentHour();

  if (current_hour_for_led >= 23 || current_hour_for_led <= 16) {
    //if (conf.serialLogging==true)   Serial.printf("disable led by time : datetime [%d]\n",current_hour_for_led);

    //MQTTLogMessage(String(F("disable pin by time : datetime ")) + current_hour_for_led);

    setLedState(index, 0);

  } //if (current_hour_for_led>=23 ||current_hour_for_led<=17 && conf.relays[index].relay_state==0) 
  else setLedState(index, 1);

}

// 1 on , 0 off
void setLedState(int index, byte state) {

  if (conf.relays[index].relay_led_pin < 0) return;

  //MQTTLogMessage(String(F("setLedState state pin ")) + conf.relays[index].relay_led_pin + " state " + state);

  if (conf.relays[index].relay_led_conn_type == LED_CONN_TYPE_LOW_ON) state = !state;

  //if (conf.serialLogging==true) Serial.printf("digitalWrite pin %d state %d \n",conf.relays[index].relay_led_pin,state);

  //MQTTLogMessage(String("digitalWrite pin ") + conf.relays[index].relay_led_pin + " state " + state);

  digitalWrite(conf.relays[index].relay_led_pin, state); // turn on /off relay 

}

// 1 on , 0 off
void setRelayState(int index, byte instate) {

  byte state = instate;

  if (conf.relays[index].relay_pin < 0) return;

  //MQTTLogMessage(String("setRelayState state index : ") + index + " state " + state);
  addAppLogMessage(String("setRelayState state index : ") + index + " state " + state);

  if (conf.relays[index].relay_conn_type == RELAY_CONN_TYPE_NC) state = !state;

  //MQTTLogMessage(String("digitalWrite pin ") + conf.relays[index].relay_pin + " state " + state);

  digitalWrite(conf.relays[index].relay_pin, state); // turn on /off relay 

  //MQTTLogMessage(String("digitalWrite end pin ") + conf.relays[index].relay_pin + " state " + state);

  if (conf.relays[index].relay_led_pin >= 0 && conf.relays[index].relay_led_on_when == LED_ON_WHEN_RELAY_ON) setLedState(index, instate);
  else if (conf.relays[index].relay_led_pin >= 0 && conf.relays[index].relay_led_on_when == LED_ON_WHEN_RELAY_OFF) setLedState(index, !instate);

  //current_hour_for_led=-1;
  if (conf.relays[index].relay_led_pin >= 0 && instate == 0) processRelayLed(index);

}

void setRelayStatePermanently(int index, byte value) {
  //jesli stan sie nie zmienia nic nie rob

  //    if (conf.relays[index].relay_state==value) return;

  conf.relays[index].relay_state = value;

  String switchcmd = "On";
  if (conf.relays[index].relay_state == 0) switchcmd = "Off";

  addAppLogMessage(String("setRelayStatePermanently :index [") + index + "] " + switchcmd + ", previous state: " + conf.relays[index].relay_state);

  setRelayState(index, value);
  if (conf.MQTT_enable && conf.relays[index].domoticz_device_idx > 0) {

    MQTTpublish(String("{\"command\": \"switchlight\", \"idx\": ") + conf.relays[index].domoticz_device_idx + ", \"switchcmd\": \"" + switchcmd.c_str() + "\"  }");
  } //  if (conf.MQTT_enable && conf.relays[index].domoticz_device_idx>0)

  saveConfig();

}

void InterruptSwitch(int index) {

  //  int pin_state = digitalRead(conf.button_pin);

  setRelayStatePermanently(index, !conf.relays[index].relay_state);

}

// wywoływane w funkcji loop po tym jak wcisnieto przycisk
void processSwitchInterrupt(int index) {

  if (conf.relays[index].relay_switch_pressed == true) {
   conf.relays[index].relay_switch_pressed = false;
 
    //MQTTLogMessage(String("Relay switch pressed index ") + index );
    //MQTTLogMessage(String("Relay state ") + conf.relays[index].relay_state );

    debounceInterruptBackend(index);

    if (conf.MQTT_enable && conf.relays[index].domoticz_device_idx > 0) {
      String switchcmd = "On";
      if (conf.relays[index].relay_state == 0) switchcmd = "Off";
      MQTTpublish(String("{\"command\": \"switchlight\", \"idx\": ") + conf.relays[index].domoticz_device_idx + ", \"switchcmd\": \"" + switchcmd.c_str() + "\"  }");
    } //  if (conf.MQTT_enable && conf.relays[index].domoticz_device_idx>0)

  } //  if (conf.relays[index].relay_switch_pressed == true)

}

void processSwitch(int index) {
  switchs[index].process();

  /*
    if (switchs[index].pressed())
    {
      Serial.printf("Button %d pressed\n", index);
      InterruptSwitch(index) ;
    }
    */
}

void debounceInterruptBackend(int index) {

  //  int buttonstate = digitalRead(conf.relays[index].relay_switch_pin);

  //Serial.printf("buttonstate %d\n" ,buttonstate );

  //  if((long)(micros() - last_micros[index]) >= debouncing_time * 1000) {
  InterruptSwitch(index);

  //MQTTLogMessage(String("debounceInterruptBackend  ") + index);

  if (conf.MQTT_enable && conf.relays[index].domoticz_device_idx > 0) {
    String switchcmd = "On";
    if (conf.relays[index].relay_state == 0) switchcmd = "Off";
    MQTTpublish(String("{\"command\": \"switchlight\", \"idx\": ") + conf.relays[index].domoticz_device_idx + ", \"switchcmd\": \"" + switchcmd.c_str() + "\"  }");
  } //  if (conf.MQTT_enable && conf.relays[index].domoticz_device_idx>0)

  //    last_micros[index] = micros();
  //  }//  if((long)(micros() - last_micros[index]) >= debouncing_time * 1000) {
}

void ICACHE_RAM_ATTR  debounceInterrupt0() {

  if (abs((long)(micros() - last_micros[0])) >= debouncing_time * 1000) {
    //Serial.printf("debounceInterrupt0 START\n" );
    conf.serialLogging = false;
    conf.interruptProcess = true;

    //Serial.printf("debounceInterrupt0\n" );

    //debounceInterruptBackend(0);
    conf.relays[0].relay_switch_pressed = true;

    last_micros[0] = micros();

    conf.serialLogging = true;
    conf.interruptProcess = false;
    //Serial.printf("debounceInterrupt0 END\n" );

  } // if(abs((long)(micros() - last_micros[0])) >= debouncing_time * 1000) {

}

void ICACHE_RAM_ATTR  debounceInterrupt1() {

  if (abs((long)(micros() - last_micros[1])) >= debouncing_time * 1000) {
    //Serial.printf("debounceInterrupt1 START\n" );

    conf.serialLogging = false;
    conf.interruptProcess = true;

    //Serial.printf("debounceInterrupt1\n" );

    //debounceInterruptBackend(1);
    conf.relays[1].relay_switch_pressed = true;

    last_micros[1] = micros();

    conf.serialLogging = true;
    conf.interruptProcess = false;

    //Serial.printf("debounceInterrupt1 END\n" );

  } //if(abs((long)(micros() - last_micros[1])) >= debouncing_time * 1000) {

}

void initRelaySwitchInterrupt(int index) {

  //Serial.printf("initRelaySwitch INTERRUPT idnex %d\n", index);
  pinMode(conf.relays[index].relay_switch_pin, INPUT); // set Pin as Input (default)

  // jeśli to przycisk
  if (conf.relays[index].relay_switch_type == RELAY_SWITCH_TYPE_BUTTON) {
    if (index == 0) attachInterrupt(conf.relays[index].relay_switch_pin, debounceInterrupt0, FALLING);
    else if (index == 1) attachInterrupt(conf.relays[index].relay_switch_pin, debounceInterrupt1, FALLING);
  } // jeśli to przycisk

  // jeśli to przełącznik
  else if (conf.relays[index].relay_switch_type == RELAY_SWITCH_TYPE_SWITCH) {
    if (index == 0) attachInterrupt(conf.relays[index].relay_switch_pin, debounceInterrupt0, CHANGE);
    else if (index == 1) attachInterrupt(conf.relays[index].relay_switch_pin, debounceInterrupt1, CHANGE);
  } // jeśli to przełącznik

}

void initRelaySwitch(int index) {
  if (conf.relays[index].relay_switch_pin < 0) return;

  //Serial.printf("initRelaySwitch %d (%s)\n" ,conf.relays[index].relay_switch_pin,SWITCH_TYPES[conf.relays[index].relay_switch_type]);

  if (conf.relays[index].relay_switch_pin_process_method == RELAY_SWITCH_PIN_PROCESS_METHOD_LOOP)

  {
    //  Serial.printf("initRelaySwitch LOOP\n");

    switchs[index].begin(conf.relays[index].relay_switch_pin, conf.relays[index].relay_switch_type);

    if (index == 0)
      switchs[index].setcallbackFunction([]() {
        debounceInterruptBackend(0);

      });
    else
      switchs[index].setcallbackFunction([]() {
        debounceInterruptBackend(1);

      });
    return;
  } else initRelaySwitchInterrupt(index);

}

void initRelay(int index) {
  //Serial.printf("initRelay\n");

  if (conf.relays[index].relay_pin < 0) return;
  //Serial.printf("initRelay [relay_pin=%d] [relay_state=%d]\n",conf.relays[index].relay_pin,conf.relays[index].relay_state);

  addAppLogMessage(String("initRelay [relay_pin=") + conf.relays[index].relay_pin + " [relay_state=" + conf.relays[index].relay_state);
  //Serial.printf("initRelay [relay_pin=%d] [relay_state=%d]\n",conf.relays[index].relay_pin,conf.relays[index].relay_state);
  //MQTTLogMessage(String("initRelay [relay_pin=") + conf.relays[index].relay_pin + " [relay_state=" + conf.relays[index].relay_state);

  pinMode(conf.relays[index].relay_pin, OUTPUT);
  setRelayState(index, conf.relays[index].relay_state);
}

void initLed(int index) {
  if (conf.relays[index].relay_led_pin < 0) return;

  //Serial.printf("initLed [led_pin=%d]\n",conf.relays[index].relay_led_pin);
  //MQTTLogMessage(String("initLed [led_pin=") + conf.relays[index].relay_led_pin + "]");

  pinMode(conf.relays[index].relay_led_pin, OUTPUT);
}

void initDevices() {

  //Serial.printf("initDevice\n");
  //MQTTLogMessage(String("initDevice"));

  initLed(0);
  initLed(1);
  initRelay(0);
  initRelay(1);
  initRelaySwitch(0);
  initRelaySwitch(1);
  //MQTTLogMessage(String("initDevice END"));

}
