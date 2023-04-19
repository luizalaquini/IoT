/************************************************************************
 * 
 * ESP8266 NodeMCU Interface with ST7789 TFT display (240x240 pixel)
 *   and BMP280 barometric pressure & temperature sensor.
 * This is a free software with NO WARRANTY.
 * https://simple-circuit.com/
 *
 ************************************************************************/

#include <Wire.h>              // Wire library (required for I2C devices)
#include <Adafruit_BMP280.h>   // Adafruit BMP280 sensor library

#define D2 4
#define D1 5
// define device I2C address: 0x76 or 0x77 (0x77 is library default address)
#define BMP280_I2C_ADDRESS  0x76
// initialize Adafruit BMP280 library
Adafruit_BMP280  bmp280;

void setup(void) {
  Serial.begin(9600);
   // initialize the BMP280 sensor
  Wire.begin();  // set I2C pins [SDA = D2, SCL = D1], default clock is 100kHz
  if( bmp280.begin(BMP280_I2C_ADDRESS) == 0 ) {
    // connection error or device address wrong!
    Serial.println("Nao conseguiu reconhecer o Sensor");
    while(1);  // stay here
  }


}

// main loop
void loop() {

  delay(1000);    // wait a second

  // read temperature and pressure from BMP280 sensor
  float temp     = bmp280.readTemperature();   // get temperature
  float pressure = bmp280.readPressure();      // get pressure
 

  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.println(" C");

  Serial.print("Pressao: ");
  Serial.print(pressure );
  Serial.println(" hPa");



}

// end of code.
