#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <ArduinoMDNS.h>
#include <ArduinoLog.h>
#include "http.h"

#ifndef WEB_SERVER_H_
#define WEB_SERVER_H_

const int serverPort = 80;
const int disconnectClientDelay = 1; // msec
const char mdnsName[] = "aircon-zone";

void startServer(RequestHandler *requestHandlers, size_t num);
void serverLoop();
void handleRequest();
void handleResponse(WiFiClient client, HttpRequest httpRequest);

#endif