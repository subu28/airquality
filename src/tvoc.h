#include <Adafruit_AGS02MA.h>
#include <Wire.h>

#define SDA_2 33
#define SCL_2 32

Adafruit_AGS02MA ags;

void startTvoc () {
  Wire1.begin(SDA_2, SCL_2);

  if (! ags.begin(&Wire1, 0x1A)) {
  //if (! ags.begin(&Wire1, 0x1A)) { // or use Wire1 instead!
    Serial.println("Couldn't find AGS20MA sensor, check your wiring and pullup resistors!");
    while (1) yield();
  }
  if (ags.getFirmwareVersion() == 0) {
    Serial.println(F("Could not read firmware, I2C communications issue?"));
    while (1) yield();
  }
}

uint32_t readTvoc () {
  return ags.getTVOC();
}