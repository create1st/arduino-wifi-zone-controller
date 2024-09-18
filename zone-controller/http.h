#include <Arduino.h>
#include <WiFiNINA.h>
#include <functional>
#define MAX_HTTP_HEADERS 10

using namespace std;

#ifndef HTTP_H_
#define HTTP_H_

struct StatusCode {
  int code;
  String message;
};

struct HttpHeader {
  String headerName;
  String headerValue;
};

struct HttpRequest { 
  String method;
  String path;
  String body;
  HttpHeader headers[MAX_HTTP_HEADERS];
};

struct HttpResponse {
  StatusCode statusCode;
  String contentType;
  String body;
};

struct RequestHandler {
  String method;
  String pathPattern;
  function<HttpResponse(WiFiClient, HttpRequest)> handlerFunction;
};

const String GET = "GET";
const String POST = "POST";
const String PUT = "PUT";
const String PATCH = "PATCH";
const String DELETE = "DELETE";

const String CONTENT_TYPE_PLAIN_TEST = "text/plain";
const String CONTENT_TYPE_APPLICATION_JSON = "application/json";

const StatusCode HTTP_OK = { 200, "OK" };
const StatusCode HTTP_CREATED = { 201, "Created" };
const StatusCode HTTP_ACCEPTED = { 202, "Accepted" };
const StatusCode HTTP_NO_CONTENT = { 204, "No Content" };
const StatusCode HTTP_BAD_REQUEST = { 400, "Bad Request" };
const StatusCode HTTP_NOT_FOUND = { 404, "Not Found" };
const StatusCode HTTP_UNSUPPORTED_MEDIA_TYPE = { 415, "Unsupported Media Type" };

const HttpResponse NOT_FOUND = { HTTP_NOT_FOUND, CONTENT_TYPE_PLAIN_TEST, "" };

#endif