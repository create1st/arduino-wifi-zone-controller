#include <WiFiNINA.h>
#include <ArduinoLog.h>

#ifndef WIFI_H_
#define WIFI_H_

void connectWiFi();
void showNetworkConnectionDetails();
String macToString(byte mac[]);

#endif