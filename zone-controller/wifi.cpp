#include "credentials.h"
#include "wifi.h"


const int connectionWaitTime = 5 * 1000; // msec

int status = WL_IDLE_STATUS;     // the Wifi radio's status

void connectWiFi() {
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Log.traceln("Please upgrade the firmware");
  }
  Log.traceln("WiFi firmware version: %s", fv.c_str());
  while (status != WL_CONNECTED) {
    Log.traceln("Connecting to WiFi: %s", ssid);
    status = WiFi.begin(ssid, pass);
    if (status == WL_CONNECTED) break;
    delay(connectionWaitTime);
  }
  Log.traceln("Connected");
}

void showNetworkConnectionDetails() {
  Log.traceln("Board Information:");
  IPAddress ip = WiFi.localIP();
  Log.traceln("IP Address: %p", ip);
  Log.traceln("Network Information:");
  Log.traceln("SSID: %s", WiFi.SSID());
  long rssi = WiFi.RSSI();
  Log.traceln("signal strength (RSSI): %d dBm", rssi);
  byte encryption = WiFi.encryptionType();
  Log.traceln("Encryption Type: %X", encryption);
  byte mac[6];
  WiFi.macAddress(mac);
  String macString = macToString(mac);
  Log.traceln("MAC: %s", macString.c_str());
}

String macToString(byte mac[]) {
  String macString = "";
  char buffer[3];
  for (int i = 5; i >= 0; i--) {
    sprintf(buffer, "%02X", mac[i]);
    macString += buffer;
    if (i > 0) {
      macString += ":";
    }
  }
  return macString;
}
