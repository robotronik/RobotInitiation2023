#include "webSiteLoader.h"
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <string.h>
#include <vector> 


String webSiteLoader::webSiteFileDirectory;
bool webSiteLoader::webSiteLoaded = false;


void webSiteLoader::load(String indexDirectory, AsyncWebServer* server){
  Serial.println();
  if(webSiteLoaded!=true){
    webSiteLoader::webSiteFileDirectory = indexDirectory;
    webSiteLoader::webSiteLoaded = true;
    webSiteLoader::loadWebSite(server);
  } 
  else{
    Serial.println("error : server web already loaded");
  }
  Serial.println();
}

AsyncWebServer* webSiteLoader::load(String indexDirectory){
  Serial.println();
  AsyncWebServer* server = new AsyncWebServer(80);
  if(webSiteLoaded!=true){
    webSiteLoader::webSiteFileDirectory = indexDirectory;
    webSiteLoader::webSiteLoaded = true;
    webSiteLoader::loadWebSite(server);
    server->begin();
  } 
  else{
    Serial.println("error : server web already loaded");
  }
  Serial.println();
  return server;
}

void webSiteLoader::loadWebSite(AsyncWebServer* server){
  SPIFFS.begin();
  

  int iNumFile = 0;
  std::vector<File> WebFile(1); 
  WebFile[0] = SPIFFS.open(webSiteFileDirectory,"r");

  while(WebFile[0])
  {
    WebFile.push_back(WebFile[iNumFile].openNextFile());
    iNumFile++;
    
    if(!(WebFile[iNumFile].isDirectory())){     
      String filename = WebFile[iNumFile].name();
      Serial.println(filename);
      filename = filename.substring(webSiteFileDirectory.length(),filename.length());
      if(filename.length()==0){
        Serial.println(filename);
        WebFile[iNumFile].close();//close file
        WebFile.pop_back();
        iNumFile--;
        WebFile[iNumFile].close();//close folder
        WebFile.pop_back();
        iNumFile--;
      }
      else{
        Serial.println(filename);//file load
        server->on(filename.c_str(), HTTP_GET, [](AsyncWebServerRequest *request)
        {
          String fileUrl = webSiteFileDirectory + request->url();
          Serial.println(request->url() + " -> " + fileUrl);//file upload
          request->send(SPIFFS, fileUrl, webSiteLoader::contentTypeSelect(fileUrl));
        });
        
        WebFile[iNumFile].close(); //fclose file
        WebFile.pop_back();
        iNumFile--;
      }
    }
  }
  //First Page
  Serial.println("First page setup");
  server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    //request->redirect("http://192.168.4.1/index.html");
    Serial.println("First page");
    request->send(SPIFFS, webSiteLoader::webSiteFileDirectory + "/index.html", webSiteLoader::contentTypeSelect(webSiteLoader::webSiteFileDirectory + "/index.html"));
  });

  server->onNotFound([](AsyncWebServerRequest *request){
    if (!request->host().equals(WiFi.softAPIP().toString())) {
      String message = "http://"  + WiFi.softAPIP().toString() + ("/index.html");
      request->redirect(message);
      return;
    }
    // Votre code de gestion de la page de portail captif ici
    request->send(SPIFFS, webSiteLoader::webSiteFileDirectory + "/index.html", webSiteLoader::contentTypeSelect(webSiteLoader::webSiteFileDirectory + "/index.html"));
  });

}



String webSiteLoader::contentTypeSelect(String fileDirectory){
  String fileType;
  String retContentType;
  int inumDetect = fileDirectory.length()-1;
  while (inumDetect >=0 && fileDirectory[inumDetect] != '.')
  {
    inumDetect--;
  }
  fileType = fileDirectory.substring(inumDetect,fileDirectory.length());
  if(fileType == ".html"){
    retContentType = "text/html";
  }
  else if(fileType == ".css"){
    retContentType = "text/css";
  }
  else if(fileType == ".xml"){
    retContentType = "text/xml";
  }
  else if(fileType == ".wml"){
    retContentType = "text/vnd.wap.wml";
  }
  else if(fileType == ".wmls"){
    retContentType = "text/vnd.wap.wmlscript";
  }
  else if(fileType == ".mpeg" || fileType == ".mpg" || fileType == ".mpe"){
    retContentType = "video/mpeg";
  }
  else if(fileType == ".gif"){
    retContentType = "image/gif";
  }
  else if(fileType == ".jpeg" || fileType == ".jpg" || fileType == ".jpe"){
    retContentType = "image/jpeg";
  }
  else if(fileType == ".png"){
    retContentType = "image/png";
  }
  else if(fileType == ".svg"){
    retContentType = "image/svg+xml";
  }
  else if(fileType == ".wbmp"){
    retContentType = "image/vnd.wap.wbmp";
  }
  else if(fileType == ".wrl" || fileType == ".vrml"){
    retContentType = "model/vrml";
  }
  else if(fileType == ".mid" || fileType == ".midi" || fileType == ".Kar"){
    retContentType = "audio/midi";
  }
  else if(fileType == ".mpga" || fileType == ".mp2" || fileType == ".mp3"){
    retContentType = "audio/mpeg";
  }
  else if(fileType == ".doc"){
    retContentType = "application/msword";
  }
  else if(fileType == ".bin" || fileType == ".dms" || fileType == ".lha" || fileType == ".lzh" || fileType == ".exe" || fileType == ".class"){
    retContentType = "application/octet-stream";
  }
  else if(fileType == ".pdf"){
    retContentType = "application/pdf";
  }
  else if(fileType == ".js"){
    retContentType = "application/x-javascript";
  }
  else if(fileType == ".swf"){
    retContentType = "application/x-shockwave-flash";
  }
  else if(fileType == ".zip"){
    retContentType = "application/zip";
  }
  else if(fileType == ".wmlc"){
    retContentType = "application/vnd.wap.wmlc";
  }
  else if(fileType == ".json"){
    retContentType = "application/json";
  }
  else{
    retContentType = "text/plain";
  } 
  return retContentType;
}