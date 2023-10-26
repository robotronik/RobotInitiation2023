#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <string.h>
#include <vector> 
#include <DNSServer.h>
#include "webSiteLoader.h"
#include "MotorControl.h"




const char *ssid = "Robot1";
const char *password = "azertyui";

AsyncWebServer server(80);
DNSServer dnsServer;

const String WebDirectory = "/web";

int valueX = 0;
int valueY = 0;

int minmax(int value, int born){
  if(value>born){
    return value = born;
  }
  else if (value<-born){
    return value = -born;
  }
  return value;
}

void setup(){
  //----------------------------------------------------Serial
  Serial.begin(115200);

  //----------------------------------------------------GPIO
  initPortMotor();

  //----------------------------------------------------Connect WIFI
  // WiFi.begin(ssid, password);
	// Serial.print("Tentative de connexion...");
	
	// while(WiFi.status() != WL_CONNECTED)
	// {
	// 	Serial.print(".");
	// 	delay(100);
	// }
	
	// Serial.println("\n");
	// Serial.println("Connexion etablie!");
	// Serial.print("Adresse IP: ");
	// Serial.println(WiFi.localIP());

  //----------------------------------------------------generate WIFI
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid,password);
  Serial.println(WiFi.softAPIP());

  //----------------------------------------------------SERVER
  webSiteLoader::load("/web", &server); 
  

  server.on("/positionX", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("positionX");
     if (request->hasParam("value")) {
        String valueStr = request->getParam("value")->value();
        valueX = valueStr.toInt(); // Convertit la valeur en entier si nécessaire
        request->send(200);
    } else {
        request->send(400); // Code d'erreur 400 si le paramètre "value" est manquant
    }
  });

    server.on("/positionY", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("positionY");
     if (request->hasParam("value")) {
        String valueStr = request->getParam("value")->value();
        valueY = valueStr.toInt(); // Convertit la valeur en entier si nécessaire
        int motorvalue1 = valueY*2.55 + valueX;
        int motorvalue2 = -valueY*2.55 + valueX;
        
        motorControl1(minmax(motorvalue1,255));
        motorControl3(minmax(motorvalue2,255));
        request->send(200);
    } else {
        request->send(400); // Code d'erreur 400 si le paramètre "value" est manquant
    }
  });

  server.on("/REV", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("backward");
    motorControl1(0,255);
    motorControl3(1,255);
    request->send(200);
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("off");
    motorControl1(0,0);
    motorControl3(0,0);
    request->send(200);
  });


  dnsServer.start(53, "*", WiFi.softAPIP());
  server.begin();
  Serial.println("Serveur actif!");
  Serial.println();
   

}


void loop(){
  dnsServer.processNextRequest();
}




