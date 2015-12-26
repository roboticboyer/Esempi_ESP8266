/* 
= SPIFFS
upload the contents of the data folder with MkSPIFFS Tool ("ESP8266 Sketch Data Upload" in Tools menu in Arduino IDE)
https://github.com/esp8266/Arduino/blob/master/doc/filesystem.md#uploading-files-to-file-system

= DS1820
Usare 1.8K per resistneza di pull up
Created by Igor Jarc
See http://iot-playground.com for details
Code based on https://github.com/DennisSc/easyIoT-ESPduino/blob/master/sketches/ds18b20.ino
https://github.com/milesburton/Arduino-Temperature-Control-Library

= Javascript GAUGE
Javascript Gauge
http://www.esp8266.com/viewtopic.php?p=36089
http://newmaan.com/gauge.min.js
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
#include <FS.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define DBG_OUTPUT_PORT Serial

const char* ssid = "slip";
const char* password = "";

//const char* host = "esp8266fs";
ESP8266WebServer server(80);
//holds the current upload
//File fsUploadFile;

// DS1820 Setup
#define ONE_WIRE_BUS 2  // DS18B20 pin GPIO2 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
#define INTERVAL 5 // in sec
float temp=20; 
float webtemp=20; //temperatura per web
long temp_time;

//------------------------------------------------------------------ 
String XML;
void buildXML(){
  XML="<?xml version='1.0'?>";
  XML+="<response>";
  //XML+=random(-10,100);
  XML+=webtemp;
  XML+="</response>";
  }
//------------------------------------------------------------------ 
void handleXML(){
  buildXML();
  server.send(200,"text/xml",XML);
  DBG_OUTPUT_PORT.println("XML Data sent");
  }  
//----------------------------------------------------------------------
//format bytes
String formatBytes(size_t bytes){
  if (bytes < 1024){
    return String(bytes)+"B";
  } else if(bytes < (1024 * 1024)){
    return String(bytes/1024.0)+"KB";
  } else if(bytes < (1024 * 1024 * 1024)){
    return String(bytes/1024.0/1024.0)+"MB";
  } else {
    return String(bytes/1024.0/1024.0/1024.0)+"GB";
  }
}
//----------------------------------------------------------------------
String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
//----------------------------------------------------------------------
bool handleFileRead(String path){
  DBG_OUTPUT_PORT.println("handleFileRead: " + path);
  if(path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
    if(SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}
//----------------------------------------------------------------------
void setup(void){
  DBG_OUTPUT_PORT.begin(115200);
  DBG_OUTPUT_PORT.print("\n");
  DBG_OUTPUT_PORT.setDebugOutput(true);
  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {    
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      DBG_OUTPUT_PORT.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    DBG_OUTPUT_PORT.printf("\n");
  }
    //WIFI INIT
  DBG_OUTPUT_PORT.printf("Connecting to %s\n", ssid);
  if (String(WiFi.SSID()) != String(ssid)) {
    WiFi.begin(ssid, password);
    }
  WiFi.config(IPAddress(192,168,1,220), IPAddress(192,168,1,1),IPAddress(255,255,255,0));
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DBG_OUTPUT_PORT.print(".");
  }
  DBG_OUTPUT_PORT.println("");
  DBG_OUTPUT_PORT.print("Connected! IP address: ");
  DBG_OUTPUT_PORT.println(WiFi.localIP());

  //MDNS.begin(host);
//  DBG_OUTPUT_PORT.print("Open http://");
//  DBG_OUTPUT_PORT.print(host);
//  DBG_OUTPUT_PORT.println(".local/edit to see the file browser");
  
  
  //SERVER INIT
  //called when the url is not defined here
  //use it to load content from SPIFFS
  server.on("/xml",handleXML);
  server.onNotFound([](){
    if(!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  server.begin();
  DBG_OUTPUT_PORT.println("HTTP server started");

}
//---------------------------------------------------------------------- 
void loop(void){
  server.handleClient();
  yield();
  misura_temp();
}
//----------------------------------------------------------------------
void misura_temp(void){
  //Usato millis per non bloccare il web server ESP 
  long time=millis();
    if(time>temp_time+INTERVAL){
	  do {
		DS18B20.requestTemperatures(); 
		temp = DS18B20.getTempCByIndex(0);
		//DBG_OUTPUT_PORT.print("Temperature: ");
		//DBG_OUTPUT_PORT.println(temp);
		//delay(500);
	  } while (temp == 85.0 || temp == (-127.0));
	temp_time=millis();
}
  
  if (temp != webtemp)
  {
    webtemp = temp; // aggiorna il valore di temperatura da mastrare precedentemente registrato
    DBG_OUTPUT_PORT.println("Temperatura cambiata");
    DBG_OUTPUT_PORT.print("Temperature: ");
	DBG_OUTPUT_PORT.println(temp);
  }
  }
