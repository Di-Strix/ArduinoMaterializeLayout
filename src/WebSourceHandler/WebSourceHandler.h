#pragma once

#include <ESPAsyncWebServer.h>

class WebSourceHandler {
  protected:
  String path;
  const uint8_t* content = nullptr;
  size_t contentLength;
  String contentType;
  AsyncWebHandler* handler = nullptr;
  AsyncWebServer* server = nullptr;

  public:
  /**
   * @brief Construct a new Web Source Handler object
   *
   * @param path Path which will be reserved for the source
   * @param content Source contents in PROGMEM
   * @param contentType Type of the content according to the MIME type.
   *  See https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types#types
   */
  WebSourceHandler(String path, const uint8_t* content, size_t contentLength, String contentType);

  /**
   * @brief Destroy the Web Source Handler object and cancels registration;
   *
   */
  virtual ~WebSourceHandler();

  /**
   * @brief Cancels source registration. After that handler's source can't be reached until
   * 'performRegistration' or 'switchContent' is called
   *
   */
  void cancelRegistration();

  /**
   * @brief Construct a new switch Content object
   *
   * @param content new source contents
   * @param contentType Type of the content according to the MIME type.
   *  See https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types#types
   */
  AsyncWebHandler* switchContent(const uint8_t* content, String contentType);

  /**
   * @brief Performs handler registration in provided server
   *
   * @param s pointer to the server
   * @return AsyncWebHandler* pointer to the registered web handler
   */
  AsyncWebHandler* performRegistration(AsyncWebServer* s);

  /**
   * @brief Path to the content on the server
   *
   * @return String path
   */
  String getPath();

  /**
   * @brief Served content in PROGMEM
   *
   * @return const char*
   */
  const uint8_t* getContent();

  /**
   * @brief MIME type of the content
   *
   * @return String type
   */
  String getContentType();
};