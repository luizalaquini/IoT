//André Louis Souza Ribeiro
//Api de temperatura e de pressão
//rotas
// "/" - envia todos as infos
// "/temp" - envia apenas temperatura
// "/pressure" - envia apenas pressao

//INCLUDES E INFOS
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <StreamString.h>
#include <locale.h>
//para enviar como json
#include <ArduinoJson.h> 
//credenciais da rede
#ifndef STASSID
#define STASSID "Louis house"
#define STAPSK "marimed3"
#endif

#include <Wire.h>              // Wire library (required for I2C devices)
#include <Adafruit_BMP280.h>   // Adafruit BMP280 sensor library

#define D2 4
#define D1 5
// define device I2C address: 0x76 or 0x77 (0x77 is library default address)
#define BMP280_I2C_ADDRESS  0x76
// initialize Adafruit BMP280 library
Adafruit_BMP280  bmp280;


const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);

const int led = 13;

//funcao raiz, vai enviar temperatura e pressao
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

//funcao de enviar apenas temperatura
void handletemp() {
  float temp     = bmp280.readTemperature();   // get temperature
  server.sendHeader("Content-Type", "text/html; charset=utf-8");
  setlocale(LC_NUMERIC, "pt_BR");

  
//  String html = "<html><head><title>Temperatura</title></head><body>";
//  html += "<h1>Temperatura</h1>";
//  html += "<p>Temperatura: " + String(temp, 2) + " °C</p>";
//  html += "</body></html>";
//
//  server.send(200, "text/html", html);

 StaticJsonDocument<200> doc;
  doc["temperatura"] = temp;
  String json;
  serializeJson(doc, json);
  server.sendHeader("Content-Type", "application/json");
  server.send(200, "application/json", json);
}

//funcao de mostrar enviar apenas pressao
void handlepressure() {
  float pressure = bmp280.readPressure();      //get pressure
  server.sendHeader("Content-Type", "text/html; charset=utf-8");
  setlocale(LC_NUMERIC, "pt_BR");

//  String html = "<html><head><title>Temperatura e Pressão</title></head><body>";
//  html += "<h1>Temperatura e Pressão</h1>";
//  html += "<p>Pressão: " + String(pressure, 2) + " Pa</p>";
//  html += "</body></html>";
//
//  server.send(200, "text/html", html);

 StaticJsonDocument<200> doc;
  doc["pressao"] = pressure;

  String json;
  serializeJson(doc, json);

  server.sendHeader("Content-Type", "application/json");
  server.send(200, "application/json", json);
}


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


void setup(void) {
  //SETUP DO MEDIDOR
  Serial.begin(9600);
   // initialize the BMP280 sensor
  Wire.begin();  // set I2C pins [SDA = D2, SCL = D1], default clock is 100kHz
  if( bmp280.begin(BMP280_I2C_ADDRESS) == 0 ) {
    // connection error or device address wrong!
    Serial.println("Nao conseguiu reconhecer o Sensor");
    while(1);  // stay here
  }
  

  //SETUP DO WEBSERVER
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

  server.on("/", handleRoot); //api raiz - mostra todas informacoes
  server.on("/temp", handletemp); //rota de mostrar apenas temperatura
  server.on("/pressure", handlepressure); //rota de mostrar apenas pressao
  
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  delay(1000);    // wait a second
 
  //FUNCOES WEBSERVER
  server.handleClient();
  MDNS.update();
}
