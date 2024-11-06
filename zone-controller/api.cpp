#include "api.h"

HttpResponse handleGetZoneState(WiFiClient client, HttpRequest request) {
  ZoneState zoneState = getLedState();
  JsonDocument doc;
  for (int i = 0; i < MAX_ZONES; i++) {
    doc["zone" + String(i)] = zoneState.enabled[i] ? "enabled" : "disabled";
  }
  String body;
  serializeJsonPretty(doc, body);
  return { HTTP_OK, CONTENT_TYPE_APPLICATION_JSON, body };
}

HttpResponse handleSetZoneState(WiFiClient client, HttpRequest request) {
  JsonDocument doc;
  deserializeJson(doc, request.body);
  ZoneState zoneState = getLedState();
  ZoneState buttonState;
  for (int i = 0; i < MAX_ZONES; i++) {
    buttonState.enabled[i] = (doc["zone" + String(i)] == "enabled") != zoneState.enabled[i];
  }
  setButtonState(buttonState);
  HttpResponse response = handleGetZoneState(client, request);
  return { HTTP_ACCEPTED, CONTENT_TYPE_PLAIN_TEST, response.body };
}

