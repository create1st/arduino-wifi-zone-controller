#include "webserver.h"

WiFiServer server(serverPort);
WiFiUDP udp;
MDNS mdns(udp);
RequestHandler *requestHandlers;
int handlerCount;

void startServer(RequestHandler *handlers, size_t num) {
  server.begin();
  mdns.begin(WiFi.localIP(), mdnsName);
  mdns.addServiceRecord("Aircone zone control._aircone-zone", serverPort, MDNSServiceTCP);
  // mdns.addServiceRecord("aircone-zone, page 2._aircone-zone", servePort, MDNSServiceTCP, "\x7path=/2");
  requestHandlers = handlers;
  handlerCount = num;
  Log.traceln("Initializing %d handlers", handlerCount);
  for (int i = 0; i < handlerCount; i++) {
    RequestHandler handler = requestHandlers[i];
    Log.traceln("Found handler %s %s", handler.method.c_str(), handler.pathPattern.c_str());
  }
}

void serverLoop() {
  mdns.run();
  handleRequest();
}

void handleRequest() {
  WiFiClient client = server.available();
  if (client) {
    String request = "";
    while (client.connected() && client.available()) {
      char c = client.read();
      request += c;
    }
    Log.traceln("Incoming request");
    int bodySeparatorIndex = request.indexOf("\r\n\r\n");
    int bodyIndex = bodySeparatorIndex + 4;
    HttpRequest httpRequest;
    httpRequest.body = request.substring(bodyIndex);
    String headers = request.substring(0, bodySeparatorIndex + 2);
    int headerIndex = 0;
    while (headers.length() > 0) {
      int endHeaderIndex = headers.indexOf("\r\n");
      String header = headers.substring(0, endHeaderIndex);      
      int valueIndex = header.indexOf(":");
      if (valueIndex == -1) {
        int pathIndex = header.indexOf(" ");
        httpRequest.method = header.substring(0, pathIndex);
        Log.traceln("method: [%s]", httpRequest.method.c_str());
        int httpVersionIndex = header.indexOf(" HTTP/");
        httpRequest.path = header.substring(pathIndex + 1, httpVersionIndex);
        // TODO Strip query parameters
        Log.traceln("path: [%s]", httpRequest.path.c_str());
      } else if (headerIndex < MAX_HTTP_HEADERS) {        
        HttpHeader httpHeader;
        httpHeader.headerName = header.substring(0, valueIndex);
        httpHeader.headerValue = header.substring(valueIndex + 1);
        httpHeader.headerValue.trim();
        httpRequest.headers[headerIndex++] = httpHeader;
        Log.traceln("header: [%s=%s]", httpHeader.headerName.c_str(), httpHeader.headerValue.c_str());
      }      
      headers = headers.substring(endHeaderIndex + 2);
    }
    Log.traceln("body: \n[%s]", httpRequest.body.c_str());
    handleResponse(client, httpRequest);
    delay(disconnectClientDelay);
    client.stop();
  }
}

void handleResponse(WiFiClient client, HttpRequest httpRequest) {
  Log.traceln("handleResponse");
  if (!client.connected()) return;
  function<HttpResponse(WiFiClient, HttpRequest)> handlerFunction = NULL;
  for (int i = 0; i < handlerCount; i++) {
    RequestHandler handler = requestHandlers[i];
    if (handler.method == httpRequest.method && handler.pathPattern == httpRequest.path) {
      Log.traceln("Matching handler found: %s %s", handler.method.c_str(), handler.pathPattern.c_str());
      handlerFunction = handler.handlerFunction;
    }
    
  }
  if (handlerFunction == NULL) {
    Log.traceln("No handler found matching %s %s", httpRequest.method.c_str(), httpRequest.path.c_str());
  } 
  HttpResponse httpResponse = handlerFunction != NULL
    ? handlerFunction(client, httpRequest)
    : NOT_FOUND;
  String statusLine = "HTTP/1.1 " + String(httpResponse.statusCode.code) + " " + httpResponse.statusCode.message;
  String contentTypeHeader = "Content-Type: " + httpResponse.contentType;
  client.println(statusLine.c_str());
  client.println(contentTypeHeader.c_str());
  client.println();
  client.println(httpResponse.body);
}