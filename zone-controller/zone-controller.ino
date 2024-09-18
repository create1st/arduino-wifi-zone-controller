#include <Hashtable.h>

#include <ArduinoLog.h>
#include "logging.h"
#include "wifi.h"
#include "webserver.h"
#include "api.h"
#include "ledstate.h"
#include "buttonstate.h"

RequestHandler handlers[] = {
  { GET, "/zone/state", &handleGetZoneState },
  { POST, "/zone/state", &handleSetZoneState },
};

void setup() {
  connectSerial();
  connectWiFi();  
  showNetworkConnectionDetails();
  initilizeLedStateReader();
  initilizeButtonStateWriter();
  const size_t num = sizeof(handlers) / sizeof(handlers[0]);
  startServer(handlers, num);
}

void loop() {
  serverLoop();
}
