/* 
LAB 1 - Intro ESP8266 e Serviços HTTP
Disciplina: Tópicos em Informática - Internet das Coisas 
Docente: Vinicius Fernandes Soares Mota 
Discente: Luiza Batista Laquini
*/


// INCLUDES
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <StreamString.h>
#include <locale.h>
#include <ArduinoJson.h> //json
#include <Wire.h>              // Wire library (required for I2C devices)
#include <Adafruit_BMP280.h>   // Adafruit BMP280 sensor library

// DEFINES and INITS
#define D2 4
#define D1 5
// define device I2C address: 0x76 or 0x77 (0x77 is library default address)
#define BMP280_I2C_ADDRESS  0x76
// initialize Adafruit BMP280 library
Adafruit_BMP280  bmp280;

// NETWORK CONFIG
#ifndef STASSID
#define STASSID "WDS MESANINO"
#define STAPSK "agoravai"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);

// FUNCTIONS
const int led = 13;

// Sends json doc with both temperature and pressure to server 
void handleRoot() {
  float temp     = bmp280.readTemperature();   // get temperature
  float pressure = bmp280.readPressure();      //get pressure
  server.sendHeader("Content-Type", "text/html; charset=utf-8");
  setlocale(LC_NUMERIC, "pt_BR");

  StaticJsonDocument<200> doc;
  doc["temperatura"] = temp;
  doc["pressao"] = pressure;

  String json;
  serializeJson(doc, json);

  server.sendHeader("Content-Type", "application/json");
  server.send(200, "application/json", json);
}

// Sends json doc with temperature to server 
void handleTemp() {
  float temp     = bmp280.readTemperature();   // get temperature
  server.sendHeader("Content-Type", "text/html; charset=utf-8");
  setlocale(LC_NUMERIC, "pt_BR");

 StaticJsonDocument<200> doc;
  doc["temperatura"] = temp;
  String json;
  serializeJson(doc, json);
  server.sendHeader("Content-Type", "application/json");
  server.send(200, "application/json", json);
}

//Sends json doc with pressure to server 
void handlePres() {
  float pressure = bmp280.readPressure();      //get pressure
  server.sendHeader("Content-Type", "text/html; charset=utf-8");
  setlocale(LC_NUMERIC, "pt_BR");

 StaticJsonDocument<200> doc;
  doc["pressao"] = pressure;

  String json;
  serializeJson(doc, json);

  server.sendHeader("Content-Type", "application/json");
  server.send(200, "application/json", json);
}

// Error treatment
void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; }

  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

// SETUP  
void setup(void) {
  //BMP280 SETUP
  Serial.begin(9600);
   // initialize the BMP280 sensor
  Wire.begin();  // set I2C pins [SDA = D2, SCL = D1], default clock is 100kHz
  if( bmp280.begin(BMP280_I2C_ADDRESS) == 0 ) {
    // connection error or device address wrong!
    Serial.println("Erro de conexao ou não reconhece o sensor");
    while(1);  // stay here
  }
  
  //WEBSERVER SETUP
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  //Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) { Serial.println("MDNS responder started"); }

  server.on("/", handleRoot); // to see both temperature and pressure
  server.on("/temperature", handleTemp); // to see temperature
  server.on("/pressure", handlePres); // to see pressure
  
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

// LOOP
void loop(void) {
  delay(1000);    // wait a second
  server.handleClient();
  MDNS.update();
}
