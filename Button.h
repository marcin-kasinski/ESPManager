#include <Arduino.h>
#include <functional>

#ifndef Button_h
#define Button_h

class Button
{
	public:
		Button();		
		void begin(int pin, byte in_switch_type);
    bool process();

    typedef std::function<void(void)> THandlerFunction_Callback;

    THandlerFunction_Callback fn_Callback;
    void setcallbackFunction(THandlerFunction_Callback fn);

int buttonPin = 0;//D3;

int buttonState ;

// the current and previous readings from the input pin
int thisButtonState;
int lastButtonState;

// time is measured in milliseconds and will quickly exceed limitations of an integer, so we use a long for these two
unsigned long lastDebounceTime = 0;  // the time the button state last switched
unsigned long debounceDelay = 50;    // the state must remain the same for this many millis to register the button press
  
	private:

  int iddle_state=1;
  byte switch_type;
};

#endif
