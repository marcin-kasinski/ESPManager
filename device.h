
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DEVICE_H
#define DEVICE_H

/*
    LOW to trigger the interrupt whenever the pin is low,
    CHANGE to trigger the interrupt whenever the pin changes value
    RISING to trigger when the pin goes from low to high,
    FALLING for when the pin goes from high to low. 

The Due board allows also:
  
    HIGH to trigger the interrupt whenever the pin is high. 
 */

void setRelayStatePermanently(int index, byte instate);
void setRelayState(int index, byte instate);
void setLedState(int index,byte state);
void initDevices() ;
void processSwitch(int index) ;
void processSwitchInterrupt(int index);
void debounceInterruptBackend(int index) ;
void processRelayLed(int index) ;

#endif

