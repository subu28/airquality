#include <Wire.h>
#include <SensirionI2CScd4x.h>

SensirionI2CScd4x scd4x;

void startCO2 () {
  Wire.begin();
  uint16_t error;
  char errorMessage[256];
  scd4x.begin(Wire);
  // stop potentially previously started measurement
  error = scd4x.stopPeriodicMeasurement();
  if (error) {
    Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }


  // Start Measurement
  error = scd4x.startLowPowerPeriodicMeasurement();
  if (error) {
    Serial.print("Error trying to execute startPeriodicMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }
}

void readCO2 (uint16_t& co2, float& temperature, float& humidity) {

  uint16_t error;
  char errorMessage[256];

  bool isDataReady = false;
  error = scd4x.getDataReadyFlag(isDataReady);
  if (error) {
    Serial.print("Error trying to execute getDataReadyFlag(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
    return;
  }
  if (!isDataReady) {
    Serial.println("Data not ready");
    return;
  }
  error = scd4x.readMeasurement(co2, temperature, humidity);
  if (error) {
    Serial.print("Error trying to execute readMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  } else if (co2 == 0) {
    Serial.println("Invalid sample detected, skipping.");
  }
}