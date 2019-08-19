#include "Button.h"

#include <Arduino.h>

#include "mqtt.h"

#include "button.h"

#include "util.h"

extern
const byte RELAY_SWITCH_TYPE_SWITCH;
extern
const byte RELAY_SWITCH_TYPE_BUTTON;

Button::Button() {}

void Button::begin(int pin, byte in_switch_type) {
  //    lastButtonState = state;
  //    thisButtonState = state;

  switch_type = in_switch_type;
  buttonPin = pin;

  buttonState = iddle_state;

  if (switch_type == RELAY_SWITCH_TYPE_SWITCH) buttonState = iddle_state = digitalRead(buttonPin);

  // the current and previous readings from the input pin
  thisButtonState = iddle_state;
  lastButtonState = iddle_state;

  pinMode(buttonPin, INPUT);

  //Serial.printf("BEGIN initButton %d\n",pin);
  //Serial.printf("BEGIN buttonState %d\n",buttonState);
  //Serial.printf("BEGIN lastButtonState %d\n",lastButtonState);
  //Serial.printf("BEGIN thisButtonState %d\n",thisButtonState);
}

// 
// public methods
// 

// has the button gone from on -> off
bool Button::process() {
  //Serial.printf("process\n");

  // the buttonPin is read multiple times and the value must remain the same for debounceDelay millis to toggle the LED

  // read button state, HIGH when pressed, LOW when not
  thisButtonState = digitalRead(buttonPin);

  //        Serial.printf("thisButtonState %d\n",thisButtonState );

  /*
    // if the current state does not match the previous state
    // the button was just pressed/released, or is transition noise
    if (thisButtonState != lastButtonState) {
      // reset the timer
      lastDebounceTime = millis();
    }
  */
  // once delay millis have elapsed, if the state remains the same, register the press
  //  if ((millis() - lastDebounceTime) > debounceDelay) {

  // if the button state has changed
  if (thisButtonState != buttonState) {

    /*
            Serial.printf("toggle \n");
            Serial.printf("thisButtonState %d\n",thisButtonState);
            Serial.printf("lastButtonState %d\n",lastButtonState);
            Serial.printf("buttonState %d\n",buttonState );
    */
    buttonState = thisButtonState;

    // only toggle the LED if the buttonState has switched from LOW to HIGH
    if (switch_type == RELAY_SWITCH_TYPE_BUTTON && buttonState != iddle_state) {
      fn_Callback();
      //        Serial.printf("Button pressed (BUTTON) , thisButtonState %d\n", thisButtonState);

      MQTTLogMessage(String("Button pressed (BUTTON) ") + ", thisButtonState " + thisButtonState + ", lastButtonState " + lastButtonState + ", buttonState " + buttonState);

    } else if (switch_type == RELAY_SWITCH_TYPE_SWITCH) {
      fn_Callback();
      //      Serial.printf("Button pressed (SWITCH) , thisButtonState %d\n", thisButtonState);

      MQTTLogMessage(String("Button pressed (SWITCH) ") + ", thisButtonState " + thisButtonState + ", lastButtonState " + lastButtonState + ", buttonState " + buttonState);

    }

  } //    if (thisButtonState != buttonState) {

  //  }//  if ((millis() - lastDebounceTime) > debounceDelay) {

  // persist for next loop iteration

  if (lastButtonState != thisButtonState) lastButtonState = thisButtonState;

}

void Button::setcallbackFunction(THandlerFunction_Callback fn) {

  fn_Callback = fn;
}
