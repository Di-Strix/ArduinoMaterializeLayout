#if UNIT_TEST

#pragma once

#include <Arduino.h>
#include <functional>

enum AwsEventType {
  WS_EVT_CONNECT,
  WS_EVT_DISCONNECT,
  WS_EVT_PONG,
  WS_EVT_ERROR,
  WS_EVT_DATA
};

enum AwsFrameType {
  WS_CONTINUATION,
  WS_TEXT,
  WS_BINARY,
  WS_DISCONNECT = 0x08,
  WS_PING,
  WS_PONG
};

struct AwsFrameInfo {
  uint8_t final;
  uint64_t index;
  uint64_t len;
  uint8_t opcode;
};

enum WebRequestMethod {
  HTTP_GET = 0b00000001,
  HTTP_POST = 0b00000010,
  HTTP_DELETE = 0b00000100,
  HTTP_PUT = 0b00001000,
  HTTP_PATCH = 0b00010000,
  HTTP_HEAD = 0b00100000,
  HTTP_OPTIONS = 0b01000000,
  HTTP_ANY = 0b01111111,
};

class AsyncWebHandler;
class AsyncWebSocket;
class AsyncWebSocketClient;
class AsyncWebServerRequest;
class AsyncWebServerRespons;
class AsyncWebServer;

typedef std::function<void(AsyncWebServerRequest* request)> ArRequestHandlerFunction;
typedef std::function<String(const String&)> AwsTemplateProcessor;
typedef std::function<void(AsyncWebSocket*, AsyncWebSocketClient*, AwsEventType, void*, uint8_t*, size_t)> AwsEventHandler;

class AsyncWebSocketClient {
  public:
  AsyncWebSocketClient() = default;
  virtual ~AsyncWebSocketClient() = default;
};

class AsyncWebHandler {
  public:
  AsyncWebHandler() = default;
  virtual ~AsyncWebHandler() = default;
};

class AsyncWebSocket : public AsyncWebHandler {
  public:
  String _url;

  AsyncWebSocket(String url)
      : _url(url) {};

  virtual ~AsyncWebSocket() = default;

  virtual void onEvent(AwsEventHandler handler) {};

  virtual void textAll(String) {};

  virtual String url()
  {
    return this->_url;
  }
};

class AsyncWebServerResponse {
  public:
  AsyncWebServerResponse() = default;
  virtual ~AsyncWebServerResponse() = default;

  virtual void addHeader(String, String) {};
};

class AsyncWebServerRequest {
  public:
  AsyncWebServerRequest() = default;
  virtual ~AsyncWebServerRequest() = default;

  virtual AsyncWebServerResponse* beginResponse(int code, String contentType = "", String content = "")
  {
    return new AsyncWebServerResponse();
  };

  virtual void send(AsyncWebServerResponse*) {};

  virtual AsyncWebServerResponse* beginResponse_P(int, const String, const uint8_t*, size_t, AwsTemplateProcessor = nullptr)
  {
    return new AsyncWebServerResponse();
  }
};

class AsyncWebServer {
  public:
  AsyncWebServer() = default;
  virtual ~AsyncWebServer() = default;

  virtual AsyncWebHandler& addHandler(AsyncWebHandler*)
  {
    return *(new AsyncWebHandler());
  }

  virtual AsyncWebHandler& on(String, int, ArRequestHandlerFunction)
  {
    return *(new AsyncWebHandler());
  }

  virtual AsyncWebHandler& on(String, ArRequestHandlerFunction)
  {
    return *(new AsyncWebHandler());
  }

  virtual bool removeHandler(AsyncWebHandler* h)
  {
    delete h;

    return true;
  }
};

#endif