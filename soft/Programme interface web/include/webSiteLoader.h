#ifndef _WEBSITELOADER_H_
#define _WEBSITELOADER_H_

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <string.h>
#include <vector> 

class webSiteLoader
{
public:
    static String webSiteFileDirectory;
    static bool webSiteLoaded;
    static void load(String indexDirectory, AsyncWebServer* server);//don't forget to start the server: .begin()
    static AsyncWebServer* load(String indexDirectory);
private:
    static void loadWebSite(AsyncWebServer* server);
    static String contentTypeSelect(String fileDirectory);
    int test =0;
};


#endif