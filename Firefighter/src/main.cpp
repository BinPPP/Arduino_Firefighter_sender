#include <Arduino.h>
#include "sender.h"

GasValues gas_m;

void setup()
{
  gas_m = {1.36f, 200.4665f, 3.0f, 4069.4069f}; //ch4 structure init
  initSender(&gas_m);
}

void loop()
{
  //toggleListen(500);
  readInput();
  sendPing(1000);
  listen();
  //connectionCheck(10000);
}
