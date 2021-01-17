#ifndef ITZoneTimer_H
#define ITZoneTimer_H
#include <LinkedList.h>
#include <functional>

const char * const WEEKDAYS[] =
{
    "MON",
    "TUE",
    "WED",
    "THU",
    "FRI",
    "SAT",
    "SUN"
};

/*
struct TimerOBject {

  byte runtime_hour=-1;
  byte runtime_min=-1;
  byte device_id;
  String function_name;

  THandlerFunction_Callback fn_Callback;

};
*/

struct CronObject {

  String type;
  String minute="*";
  String minute_logical="*";
  String hour="*";
  String hour_logical="*";
  String day="*";
  String day_logical="*";
  String month="*";
  String month_logical="*";
  String weekday="*";
  String weekday_logical="*";

  byte device_id;
  byte xxx;
  String function_name;
  String function_name_parameter;

  //THandlerFunction_Callback fn_Callback;

};

typedef std::function<void(CronObject)> THandlerFunction_Callback;


class ITZoneTimer
{
  public:
    ITZoneTimer();
    void begin();
    uint8_t getDayOfWeek();
    void process();
    void processCronObject(CronObject cronObject);
    bool isElementInList(String element, String list);
    bool isEvery(String instr, byte in_value);
    bool isRange(String instr, byte in_value);
    bool isEven(String instr, byte in_value);
    bool isOdd(String instr, byte in_value);
//    THandlerFunction_Callback fn_Callback;
//    void setcallbackFunction(THandlerFunction_Callback fn);
    byte getCurrentSec();
    byte getCurrentMin();
    byte getCurrentHour();
    byte getCurrentDay();
    byte getCurrentMonth();
    int getCurrentYear();
//    void setRunTime(byte index, byte hour, byte min,THandlerFunction_Callback fn);
    void addTask(String type,byte device_id , String function_name, String function_name_parameter, String minute , String  hour , String  day , String  month , String weekday);

//  TimerOBject timerObjects[10];
  LinkedList<CronObject> cronObjects = LinkedList<CronObject>();
  
  private:

  byte last_red_hour=-1;
  byte last_red_min=-1;
  byte last_red_wday=-1;

};

extern ITZoneTimer timer1;


#endif
