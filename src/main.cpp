#include <Arduino.h>
#include "main.h"
#include "sensor.h"
#include "wifi.h"
#include "http.h"

void setup()
{
  Serial.begin(115200);

  wifi_initAP();
  http_init();

  sensor_init(declinationAngle);
  sensor_checkSettings();
}

void loop()
{
  wifi_loop();
  // sensor_printPlotter();
  // delay(100);
}