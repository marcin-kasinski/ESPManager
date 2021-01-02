
/*
#include "util.h"

#include "mqtt.h"

#include "led.h"

my9291 _my9291 = my9291(MY9291_DI_PIN, MY9291_DCKI_PIN, MY9291_COMMAND_DEFAULT, CHANNELS);

void rainbow(unsigned char index) {

  if (index < 85) {
    _my9291.setColor((my9291_color_t) {
      (unsigned int) index * 3, (unsigned int) 255 - index * 3, 0, 0, 0
    });
  } else if (index < 170) {
    index -= 85;
    _my9291.setColor((my9291_color_t) {
      (unsigned int) 255 - index * 3, 0, (unsigned int) index * 3, 0, 0
    });
  } else {
    index -= 170;
    _my9291.setColor((my9291_color_t) {
      0,
      (unsigned int) index * 3,
      (unsigned int) 255 - index * 3,
      0,
      0
    });
  }

}

# define RAINBOW_DELAY 20
static unsigned char count = 0;
static unsigned long last = millis();

void processRGBLed() {

}

void initRGBLed() {

  MQTTLogMessage(String("initRGBLed : state=") + conf.rgbled.state);

  if (conf.rgbled.enable == false) return;

  RGBLedSetState(conf.rgbled.state);
}

void RGBLedSetStatePermanently(int state) {

  //Serial.printf("RGBLedSetStatePermanently conf.rgbled.enable=%d, conf.rgbled.state=%d, instate=%d\n",conf.rgbled.enable,conf.rgbled.state,state);

  if (conf.rgbled.enable == false || conf.rgbled.state == state) return;

  RGBLedSetState(state);
  saveConfig();
}

void RGBLedSetColor(byte r, byte g, byte b, byte w, byte wa) {
  _my9291.setColor((my9291_color_t) {
    r,
    g,
    b,
    w,
    wa
  });
  _my9291.setState(true);

}

void RGBLedSetState(int state) {

  MQTTLogMessage(String("RGBLedSetState : state=") + state + " R " + conf.rgbled.r + " G " + conf.rgbled.g + " B " + conf.rgbled.b + " W " + conf.rgbled.w + " WA " + conf.rgbled.wa);

  if (state == 1) {
    //    white
    //   _my9291.setColor((my9291_color_t) { 0, 0, 0, 255, 0 }); // white
    //  _my9291.setColor((my9291_color_t) { 0, 0, 0, 255, 255 }); // white worm
    //  _my9291.setColor((my9291_color_t) { 255, 204, 40, 255, 0 }); // cieply
    _my9291.setColor((my9291_color_t) {
      conf.rgbled.r, conf.rgbled.g, conf.rgbled.b, conf.rgbled.w, conf.rgbled.wa
    }); // cieply
    //   _my9291.setColor((my9291_color_t) { 102, 255, 51, 0, 0 }); // zielony jasny
    //   _my9291.setColor((my9291_color_t) { 0, 255, 0, 0, 0 }); // zielony
    _my9291.setState(true);
  } else if (state == 0) {
    _my9291.setState(false);

  }
  conf.rgbled.state = state;

}

*/
