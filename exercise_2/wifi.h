#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

void connectToWiFi(const char * ssid, const char * password);
void printWifiStatus();

#endif

