#include "WebSourceHandler.h"

WebSourceHandler::WebSourceHandler(String path, const uint8_t* content, size_t contentLength, String contentType)
{
  this->path = path;
  this->content = content;
  this->contentLength = contentLength;
  this->contentType = contentType;
}

WebSourceHandler::~WebSourceHandler()
{
  this->cancelRegistration();
}

void WebSourceHandler::cancelRegistration()
{
  if (this->server != nullptr && this->handler != nullptr)
    this->server->removeHandler(this->handler);

  this->handler = nullptr;
}

AsyncWebHandler* WebSourceHandler::switchContent(const uint8_t* content, String contentType)
{
  if (this->server == nullptr)
    return nullptr;

  this->content = content;
  this->contentType = contentType;

  if (this->handler == nullptr)
    return this->performRegistration(this->server);

  return this->handler;
}

AsyncWebHandler* WebSourceHandler::performRegistration(AsyncWebServer* s)
{
  if (this->path.isEmpty() || this->content == nullptr || this->contentType.isEmpty())
    return nullptr;

  this->cancelRegistration();

  this->server = s;
  this->handler = &this->server->on(this->path.c_str(), HTTP_GET, [=](AsyncWebServerRequest* request) {
    AsyncWebServerResponse* res = request->beginResponse_P(200, this->contentType.c_str(), this->content, this->contentLength);
    res->addHeader(F("X-Content-Type-Options"), F("nosniff"));
    request->send(res);
  });

  return this->handler;
}

String WebSourceHandler::getPath()
{
  return this->path;
}

const uint8_t* WebSourceHandler::getContent()
{
  return this->content;
}

String WebSourceHandler::getContentType()
{
  return this->contentType;
}