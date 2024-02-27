#include <PMS7003.h>

GuL::PMS7003 pms(Serial2);

#define RX2 16
#define TX2 17

void startDust() {
  // start second serial port for dust sensor
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);
  pms.setToPassiveReporting();

  pms.sleep();
}

void readDust(int dusts[3]) {
  pms.poll();
  delay(20);
  pms.read();

  dusts[0] = pms.getPM1_ATM();
  dusts[1] = pms.getPM2_5_ATM();
  dusts[2] = pms.getPM10_ATM();

  pms.sleep();
}