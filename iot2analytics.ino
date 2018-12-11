
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#define LED D0
//SSID and Password to your ESP Access Point
const char* ssid = "ESPWebServer";
const char* password = "12345678";

ESP8266WebServer server(80); //Server on port 80

//==============================================================
//     This rutine is exicuted when you open its IP in browser
//==============================================================
void handleRoot() {
  server.send(200, "application/json", "{connected:true}");
}
void handleNotFound() {
  Serial.print("handling");
  if (server.method() == HTTP_OPTIONS) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Max-Age", "10000");
    server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    server.send(204);
  } else {
    server.send(404, "text/plain", "Not Found");
  }
}
void getStatus() {
  Serial.print("getStatus\n");
  int currentStatus = digitalRead(LED_BUILTIN);
  Serial.print(currentStatus);
  if (currentStatus == 0)
    server.send(200, "application/json", "{\"status\":\"on\"}");
  else
    server.send(200, "application/json", "{\"status\":\"off\"}");

}

void setStatus() {
  Serial.print("Called");
  Serial.print(server.args());
  String argName = server.argName(0);
  String value = server.arg(0);
  if (argName != "state")
  {
    server.send(401, "application/json", "{\"err\":\"Undefined parameters\"}");
  } else {
    if (value == "on") {
      Serial.print("Turn on Device");
      digitalWrite(LED_BUILTIN, LOW);
      server.send(200, "application/json", "{\"status\":\"on\"}");
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.print("Turn off Device");
      server.send(200, "application/json", "{\"status\":\"off\"}");
    }

  }
}
// variable to be used
bool status;

//===============================================================
//                  SETUP
//===============================================================
void setup(void) {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("");
  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security

  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  Serial.print("HotSpt IP:");
  Serial.println(myIP);
  server.on("/getStatus", HTTP_GET, getStatus);
  server.on("/setStatus", HTTP_GET, setStatus);
  server.onNotFound(handleNotFound);
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//===============================================================
//                     LOOP
//===============================================================
void loop(void) {
  server.handleClient();          //Handle client requests
}


