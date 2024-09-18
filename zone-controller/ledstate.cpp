#include "ledstate.h"

int ledReadStatePins[MAX_ZONES] = {
  11,
  10,
  9,
  8,
  7,
  6,
};

void initilizeLedStateReader() {
  Log.traceln("Initializing LED state reader");
  for (int i = 0; i < MAX_ZONES; i++) {
    pinMode(ledReadStatePins[i], INPUT_PULLUP); 
  }
  Log.traceln("Initializing LED state reader completed");
}

ZoneState getLedState() {
  Log.traceln("Get LED state");
  ZoneState zoneState;
  for (int i = 0; i < MAX_ZONES; i++) {
    zoneState.enabled[i] = !digitalRead(ledReadStatePins[i]);
    Log.traceln("LED %d = %t", i, zoneState.enabled[i]);
  }
  Log.traceln("Get LED state completed");
  return zoneState;
}
