#include <ArduinoJson.h>
#include "http.h"
#include "ledstate.h"
#include "buttonstate.h"
#include "zonestate.h"

#ifndef API_H_
#define API_H_

HttpResponse handleGetZoneState(WiFiClient client, HttpRequest request);
HttpResponse handleSetZoneState(WiFiClient client, HttpRequest request);

#endif