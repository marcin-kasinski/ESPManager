//#include <TimeLib.h>

#include <NtpClientLib.h>

//#include <WiFiUdp.h>

#include "ntp.h"

#include "util.h"

#include "mqtt.h"

#include "udp.h"

byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packe
// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

bool initNTPClient() {

  NTP.onNTPSyncEvent([](NTPSyncEvent_t ntpEvent) {
    if (ntpEvent) {

    } else {

      //      MQTTLogMessage(String("Got NTP time: " ) + NTP.getTimeDateString(NTP.getLastNTPSync()));
      if (!NTP.getTimeDateString().equals("Time not set")) getSunriseSunset(conf.sunsetAPIKey, conf.sunsetAPICity);

    }
  });

  //    Serial.printf("conf.ntpServerName [%s]\n",conf.ntpServerName.c_str());

  NTP.begin(conf.ntpserver.c_str(), 1, true);
  NTP.setInterval(3600);

  //delay(5000);

  return true;

}
