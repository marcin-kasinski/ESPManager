#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "FS.h"

#include "util.h"

#include "file.h"

#include "mqtt.h"

#define HTTP_DOWNLOAD_UNIT_SIZE 1460

void prepareHeader(int code,
                   const char * content_type, size_t contentLength) {

  String response;

  response = "HTTP/1.0 ";
  response += String(code);
  response += " ";
  response += F("OK");;
  response += "\r\n";

  httpServer.sendContent(response);

  if (!content_type)
    content_type = "text/html";

  response = "Content-Type: " + String(content_type) + "\r\n";

  httpServer.sendContent(response);

  //    httpServer.sendContent("Content-Type", content_type, true);

  //        httpServer.sendHeader("Content-Length", String(contentLength));

  response = "Content-Length: " + String(contentLength) + "\r\n";

  httpServer.sendContent(response);

  //    httpServer.sendHeader("Access-Control-Allow-Origin", "*");

  response = "Access-Control-Allow-Origin: " + String("*") + "\r\n";

  httpServer.sendContent(response);

  httpServer.sendContent("\r\n");

}


/*
  int getWebFileSize(char * path) {

  MQTTLogMessage(String("getWebFileSize ") + path);

  int size = 0;


  HTTPClient http; //must be declared after WiFiClient for correct destruction order, because used by http.begin(client,...)

  MQTTLogMessage("[HTTP] begin...");
  // configure server and url
  http.begin(String("http://itzone.com.pl/updateespmgr/data") + path);


  MQTTLogMessage("[HTTP] GET...");
  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    MQTTLogMessage("[HTTP] NO HTTP_CODE_OK...");
    return -1;
  }



  int len = http.getSize();

     // create buffer for read
        uint8_t buff[512] = { 0 };


  // get tcp stream
        //WiFiClient * stream = &client;
        WiFiClient * stream = http.getStreamPtr();

        // read all data from server
        while (http.connected() && (len > 0 || len == -1)) {
          // read up to 128 byte
          int c = stream->readBytes(buff, std::min((size_t)len, sizeof(buff)));
          size=size+c;
          //Serial.printf("readBytes: %d\n", c);
          if (!c) {
            Serial.println("read timeout");
          }

          // write it to Serial
          //Serial.write(buff, c);

          if (len > 0) {
            len -= c;
          }
        }




  //  MQTTLogMessage(String ("[HTTP] size...") + len);
  MQTTLogMessage(String ("[HTTP] size...") + size);



  http.end();


  return len;
  }

*/

int getFileSize(char * path) {

  //    Serial.printf("getFileSize [%s]\n",path);

  //    Serial.printf("getFileSize 2[%s]\n",path);

  File f = SPIFFS.open(path, "r");
  if (!f) {
    MQTTLogMessage(String("error reading ") + path);
    return -1;
  }

  int size = f.size();
  //Serial.printf("size [%d]\n",size);
  f.close();
  //Serial.printf("file closed\n");

  return size;
}

//int httpServerSendFileContent(int code,const char* content_type,char *path)
int httpServerSendFileContent(char * path) {

  //    MQTTLogMessage(String("httpServerSendFileContent ")+path);

  //    Serial.printf("httpServerSendFileContent [%s]\n",path);

  File f = SPIFFS.open(path, "r");

  if (!f) {
    MQTTLogMessage(String("error reading ") + path);
    return -1;
  }

  //////////////
  //    size_t sent = httpServer.streamFile(f, content_type);

  httpServer.client().write(f, HTTP_DOWNLOAD_UNIT_SIZE);

  f.close();

  return 0;
  /////////////
}

void readSpiffsVersion() {

  MQTTLogMessage("readSpiffsVersion START");
  //      Serial.println("readSpiffsVersion()" );
  //    MQTTLogMessage(String("error reading ")+path);

  String version = "";

  int retfnc = readFullFile("/conf/version.txt", version);

  version.trim();

  conf.spiffs_version = version.toFloat();
  //MQTTLogMessage(String("SPIFFS version:" )+conf.spiffs_version);
  //addAppLogMessage("SPIFFS version: "+conf.spiffs_version);
  MQTTLogMessage("readSpiffsVersion END");

}

int readFullFile(char * path, String & content) {

  content=String("");

  MQTTLogMessage(String("readFullFile START :" ) + path);

  long milisstart = millis();

  int filesize = getFileSize(path);

  //Serial.printf("filesize [%d]\n",filesize);

  MQTTLogMessage(String("readFullFile filesize :" ) + filesize );

  if (filesize == -1) return -1;

  //      Serial.printf("Reading file size [%d]\n",filesize);

  //      Serial.printf("Reading [%s]\n",path);

  File f = SPIFFS.open(path, "r");

  if (!f) {

    MQTTLogMessage(String("error reading  :") + path);

    return -1;
  }

  //  Serial.print("f.size() ");
  //  Serial.println(f.size());

  //String content=String ("");

  //f.read(content, sizeof(char)*f.size());

  //  Serial.print("content ");
  int index = 1;
  int sum = 0;

  String s;
  //while (s!=NULL)

//  MQTTLogMessage("readFullFile before loop" );


  //while (content.length() <= filesize) {
  while (f.available()) {
//  MQTTLogMessage("readFullFile in loop 1" +content +" / "+ index);

//  if (index>3) break;

    char c = f.read();
    //s = f.readStringUntil('\n');
    content.concat(c);
//  MQTTLogMessage("readFullFile before loop 2" +content);
    //s=f.readStringUntil('---');

    //char data[1000];

    //Serial.printf("s [%s]\n",s.c_str());
    //Serial.printf("s [%s]\n",data);

  //  sum = sum + s.length();

//  MQTTLogMessage("readFullFile before loop 3" +content);
//    content = content + s + '\n';
//  MQTTLogMessage("readFullFile before loop 4" +content);

    //Serial.printf("[index=%d][suma znakow %d]dlugosc [suma znakow calosc  %d/suma znakow w linii %d]],filesize [%d],content.length()[%d] , f.position() [%d]\n",index,sum,content.length(),s.length(), filesize, content.length(), f.position());
    //Serial.print("przeczytano linie [");

    //Serial.print(s);
    //Serial.println("]");
    //  Serial.println(s);

    index = index + 1;

  } //while

  f.close();

  //Serial.printf("File closed");

  //delay(1000);

  long milisend = millis();

  long exec_time = milisend - milisstart;

  //Serial.printf("Czas działania [%d] milis\n",exec_time);
  //Serial.printf("Content [%s] milis\n",content.c_str());

  //Serial.printf("readFullFile end\n");

  MQTTLogMessage("readFullFile END");

  return 1;

}
/*
  int readFullFile(char *path, char *data)
  {


  File f =SPIFFS.open(path, "r");

  if (!f) {
    Serial.println("Failed to open file ");
    Serial.println(path);

    return -2;
  }

  Serial.printf("Reading %s, size  %u\n",path, f.size());

  int index=0;

  //data= (char *)malloc(sizeof(char)*f.size()+1);

  //f.read((uint8_t*)content,f.size() );

  while (f.available())
  {
    data[index]=f.read();
    index++;
  }

    data[index]='\0';

  //Serial.printf("%s\n",content);

    f.close();

  return 0;
  }
*/
bool saveFile(char *path, String indata) {

  MQTTLogMessage(String("saveFile [")+path+"] START");
//  MQTTLogMessage(String("saveFile : ") + indata);

  //Serial.printf("--------------saveFile [%s]\n[%s]\n",path.c_str(), indata.c_str());

  //Serial.printf("indata.length()=%d\n",indata.length() );
  //Serial.printf("isValidJson(indata)=%d\n",isValidJson(indata) );

  if (indata.length() < 800 || isValidJson(indata) == false) return false;

  //if (conf.serialLogging==true)     Serial.printf("saveFile %s\n[%s]\n",path.c_str(), indata.c_str());

  //String indatapart=indata.substring(0,300) ;

  //    MQTTLogMessage(String("saveFile : ")+path+     String(", length : ")+indata.length()+   String(" , leading: ")+indata.substring(0,300));
  //MQTTLogMessage(String("saveFile : ") + path + String(", length : ") + indata.length() + String(" , data: ") + indata);
  //    addAppLogMessage(String("saveFile : ")+indata );


  File f = SPIFFS.open(path, "w");


  if (!f) {
    //    Serial.printf("Save file error [%s]\n",path.c_str() );
    MQTTLogMessage(String("Save file error : ") + path);

    return false;
  }



  f.println(indata.c_str());


  //if (conf.serialLogging==true) Serial.printf("saved %s \n",indata.c_str());

  f.close();

  //    MQTTLogMessage(String("File saved : ")+path.c_str() );

  //    MQTTLogMessage(String("saveFile END") );

  MQTTLogMessage(String("saveFile END"));
  return true;
}
