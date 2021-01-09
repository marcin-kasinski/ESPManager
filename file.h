
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ESP8266WebServer.h>

#ifndef FILEUTIL_H
#define FILEUTIL_H

extern ESP8266WebServer httpServer;

void readSpiffsVersion();


void prepareHeader( int code, const char* content_type, size_t contentLength);
int getFileSize(char *path);
int getWebFileSize(char * path) ;

//int httpServerSendFileContent(int code,const char* content_type,char *path);
int httpServerSendFileContent(char *path);

int readFullFile(char *path, String &content);

int readFullFile(char *path, char *data);

bool saveFile(String path,String indata) ;
#endif
