#include "util.h"

#include "wifi.h"
#include "mqtt.h"

void setWIFIAP() {

  WiFi.softAPdisconnect(true);

  WiFi.disconnect();

  delay(1000);

  WiFi.mode(WIFI_AP_STA);
  // WiFi.mode(WIFI_AP);
  delay(1000);

  WiFi.softAP(conf.hostName.c_str(), conf.wifi_AP_password.c_str());

  IPAddress myIP(192, 168, 1, 1);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.softAPConfig(myIP, gateway, subnet);

  delay(1000);

  //  Serial.printf("AP IP address: %s",WiFi.softAPIP().c_str());

  //  Serial.print("Host name: ");
  //  Serial.println(myIP);
  //  Serial.println(WiFi.hostname());

  //  Serial.print("AP mac Address: ");
  //  Serial.println(WiFi.macAddress());

}

/*
WiFiUDP Udp;

WiFiUDP Udp8;
const byte DNS_PORT = 53;


void processWIFIRepeater()
{
  byte packetBuffer[512]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP

  int noBytes = Udp.parsePacket();
  if ( noBytes ) {
        Serial.print(millis() / 1000);
        Serial.print(":Packet of ");
        Serial.print(noBytes);
        Serial.print(" received from ");
        Serial.print(Udp.remoteIP());
        Serial.print(":");
        Serial.println(Udp.remotePort());
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, noBytes); // read the packet into the buffer

    // display the packet contents in HEX
    //    for (int i = 1; i <= noBytes; i++) {
    //      Serial.print(packetBuffer[i - 1], HEX);
    //      if (i % 32 == 0) {
    //        Serial.println();
    //      }
    //      else Serial.print(' ');
    //    } // end for
    //    Serial.println();

    IPAddress ip8(8, 8, 8, 8);
    Udp8.beginPacket(ip8, DNS_PORT); //NTP requests are to port 123
    Udp8.write(packetBuffer, noBytes);
    Udp8.endPacket();
    delay(100);

    int cb = Udp8.parsePacket();
    if (!cb) {
     // Serial.println("no packet yet");
    }
    else {
            Serial.print("packet received, length=");
            Serial.println(cb);
      byte packetBuffer8[cb];
      Udp8.read(packetBuffer8, cb);
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(packetBuffer8, cb);
      Udp.endPacket();
    }

} // end if
}


boolean setWIFIRepeater()
{

 WiFi.mode(WIFI_AP);
 


  WiFi.softAP(conf.hostName.c_str(), conf.wifi_AP_password.c_str());

 IPAddress      ip(192, 168, 1, 100); 
 IPAddress gateway(192, 168, 1, 1); 
 IPAddress subnet(255, 255, 255, 0);
 WiFi.begin();
 WiFi.config(ip, gateway, subnet);



  //Wifi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("dnsIP address: ");
  Serial.println(WiFi.dnsIP());
  Serial.print("gatewayIP address: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("subnetMask address: ");
  Serial.println(WiFi.subnetMask());

  Serial.println("");
  Serial.println("Configuring access point...");
  // You can remove the password parameter if you want the AP to be open. 
//WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  WiFi.softAP("MK Repeter", "1qaz0okm");

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP Repeter address: ");
  Serial.println(myIP);


  Udp.begin(DNS_PORT);
Udp8.begin(DNS_PORT);  

return true;

}

*/
boolean setWIFIClient() {
  /*
  if (conf.wifi_ssid==NULL) 
  {

      Serial.printf("No sid in conf\n");
  //conf.wifi_ssid="";
  //  return false;
  }
  */
//  Serial.printf("Connecting WIFI STA... SID: [%s] mac Address: %s\n", conf.wifi_ssid.c_str(), WiFi.macAddress().c_str());
    MQTTLogMessage(String("Connecting WIFI STA... SID: ") + String(conf.wifi_ssid.c_str()) + " mac Address:"+ String(WiFi.macAddress().c_str()) );


  WiFi.mode(WIFI_STA);

  //  WiFi.begin(conf.wifi_ssid.c_str(), conf.wifi_password.c_str());

  /*
    while(WiFi.waitForConnectResult() != WL_CONNECTED){
      WiFi.begin(ssid, password);
      Serial.println("WiFi failed, retrying.");
    }
  */

  int reconnect_count = 0;
  while (reconnect_count < conf.WIFI_STA_RECCONECT_COUNT) {
    WiFi.begin(conf.wifi_ssid.c_str(), conf.wifi_password.c_str());

    if (WiFi.waitForConnectResult() == WL_CONNECTED) {
      //runtime.runtime_mode = RUNTIME_MODE_OK;

      break;
    }

  //  Serial.printf("WiFi failed, retrying... AP mac Address: %s\n", WiFi.macAddress().c_str());

      MQTTLogMessage(String("WiFi failed, retrying... AP mac Address: ") + WiFi.macAddress().c_str());


    addAppLogMessage("WiFi failed, retrying");

    runtime.mac_STA = WiFi.macAddress();

    reconnect_count++;
  }

  // jeśli nie połącz
  if (reconnect_count == conf.WIFI_STA_RECCONECT_COUNT) {
    //runtime.runtime_mode = RUNTIME_MODE_WIFI_PROBLEM;

    return false;
  }
  //  int WIFI_STA_RECCONECT_COUNT=5;
  //int WIFI_STA_RECCONECT_DELAY=2000;

  //    addAppLogMessage("WiFi connect");

//  Serial.print("localIP: ");
//  Serial.println(WiFi.localIP());


      MQTTLogMessage(String("localIP: ") + WiFi.localIP().toString() );

  
  runtime.mac_STA = WiFi.macAddress();

  /*
      Serial.print(F("wifi_get_phy_mode(): "));
      Serial.println(PHY_MODE_NAMES[wifi_get_phy_mode()]);

      Serial.print(F("system_get_flash_size_map(): "));
      Serial.println(FLASH_SIZE_MAP_NAMES[system_get_flash_size_map()]);

      uint32 flashid =spi_flash_get_id();
      Serial.printf("spi_flash_get_id(void): %u\n",flashid);

      Serial.print(F("system_get_chip_id(): 0x"));
      Serial.println(system_get_chip_id(), HEX);

      Serial.print(F("system_get_sdk_version(): "));
      Serial.println(system_get_sdk_version());

      Serial.print(F("system_get_boot_version(): "));
      Serial.println(system_get_boot_version());

      Serial.print(F("system_get_userbin_addr(): 0x"));
      Serial.println(system_get_userbin_addr(), HEX);

      Serial.print(F("system_get_cpu_freq(): "));
      Serial.println(system_get_cpu_freq());
  */
  //    Serial.print(F("ESP.getFreeHeap: "));
  //    Serial.println(ESP.getFreeHeap());

  uint32_t a = ESP.getSketchSize();
  uint32_t b = ESP.getFreeSketchSpace();

  //    Serial.printf("ESP.getSketchSize(): %d\n",a);
  //    Serial.printf("ESP.getFreeSketchSpace(): %d\n",b);

  uint32_t realSize = ESP.getFlashChipRealSize();
  uint32_t ideSize = ESP.getFlashChipSize();
  FlashMode_t ideMode = ESP.getFlashChipMode();

  //    Serial.printf("Flash real id:   %08X\n", ESP.getFlashChipId());
  //    Serial.printf("ESP.getFlashChipRealSize(): %u\n\n", realSize);

  //    Serial.printf("ESP.getFlashChipSize(): %u\n", ideSize);
  //    Serial.printf("Flash ide speed: %u\n", ESP.getFlashChipSpeed());
  //    Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));

  if (ideSize != realSize) {
    //        Serial.println("Flash Chip configuration wrong!\n");
  } else {
    //        Serial.println("Flash Chip configuration ok.\n");
  }

  //String ip= String(WiFi.localIP()[0])+String(WiFi.localIP()[1])+String(WiFi.localIP()[2])+String(WiFi.localIP()[3]);

    MQTTLogMessage("setWIFIClient END");


  //strcpy(runtime.ip,WiFi.localIP()[0]);

  return true;
}
