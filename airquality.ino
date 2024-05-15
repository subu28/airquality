#include "./src/upload.h"
#include "./src/screen.h"
#include "./src/dust.h"
#include "./src/tvoc.h"
#include "./src/co2.h"


void setup()
{

// start serial port for programmer communication and printing to terminal
  Serial.begin(9600);

  while (!Serial) {
    delay(100);
  }
  Serial.println("Starting");

// start co2 sensor
  startCO2();

// start dust sensor
  startDust();

// start tvoc sensor
  startTvoc();

// start display
  startScreen();
  show("Hello, World");
  delay(30000);

}

void loop()
{
  delay(30000);
  Serial.println("running...");
  
  uint16_t co2 = 0;
  float temperature = 0.0f;
  float humidity = 0.0f;
  readCO2(co2, temperature, humidity);

  int dusts[3];
  readDust(dusts);

  uint32_t tvoc = readTvoc();

  char Wifi = upload(temperature, humidity, co2, dusts[0], dusts[1], dusts[2], tvoc);

  String out = "T:" + String(temperature) + " CO2:" + String(co2) + " H:" + String(humidity) + " PM1:" + String(dusts[0]) + " PM2.5:" + String(dusts[1]) + " PM10:" + String(dusts[2]) + " tvoc:" + String(tvoc) + " W:" + String(Wifi);
  invert(false);
  show(out);
  delay(30000);
  invert(true);

}