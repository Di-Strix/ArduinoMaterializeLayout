#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <MaterializeLayout.h>
#include <functional>
#include <unity.h>
#include <vector>

class AsyncWebServerCountHandlers : public AsyncWebServer {
  public:
  AsyncWebServerCountHandlers() = default;
  virtual ~AsyncWebServerCountHandlers() = default;

  size_t handlersRegistered = 0;

  virtual AsyncWebHandler& on(String, ArRequestHandlerFunction)
  {
    handlersRegistered++;
    auto h = AsyncWebHandler();
    return h;
  }

  virtual AsyncWebHandler& on(String url, int method, ArRequestHandlerFunction handler)
  {
    return this->on(url, handler);
  }

  virtual AsyncWebHandler& addHandler(AsyncWebHandler* h)
  {
    handlersRegistered++;
    return this->AsyncWebServer::addHandler(h);
  }

  virtual bool removeHandler(AsyncWebHandler* h)
  {
    handlersRegistered--;
    return true;
  }
};

auto MLPage = MaterializeLayout("");
auto server = new AsyncWebServer();

template <class T = AsyncWebServer>
T* setupGlobalServerInstance()
{
  delete server;
  server = new T();

  return static_cast<T*>(server);
}

void setUp()
{
  MLPage = MaterializeLayout("");

  setupGlobalServerInstance<AsyncWebServer>();
}

void test_get_html();
void test_server_register();
void test_server_swap();
void test_injection();
void test_injection_empty_file();
void test_injection_url();

void setup()
{
  UNITY_BEGIN();

  RUN_TEST(test_get_html);
  RUN_TEST(test_server_register);
  RUN_TEST(test_server_swap);
  RUN_TEST(test_injection);
  RUN_TEST(test_injection_empty_file);
  RUN_TEST(test_injection_url);

  UNITY_END();
}

void loop() { }

void test_get_html()
{
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, MLPage.getHTML().length(), "Should have html by default");
}

void test_server_register()
{
  auto serverMock = setupGlobalServerInstance<AsyncWebServerCountHandlers>();

  MLPage.registerInEspAsyncWebServer(serverMock);

  TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(2, serverMock->handlersRegistered, "Should register at least 2 handlers");
}

void test_server_swap()
{
  AsyncWebServerCountHandlers firstMock;
  auto secondMock = setupGlobalServerInstance<AsyncWebServerCountHandlers>();

  MLPage.registerInEspAsyncWebServer(&firstMock);
  MLPage.registerInEspAsyncWebServer(secondMock);

  TEST_ASSERT_EQUAL_MESSAGE(0, firstMock.handlersRegistered, "Should remove all handers added by ML");
  TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(2, secondMock->handlersRegistered, "Should register at least 2 handlers");
}

void test_injection()
{
  MaterializeLayoutModule customModule;
  customModule.CSS.file = new uint8_t(1);
  customModule.CSS.fileName = "customCSS";
  customModule.JS.file = new uint8_t(1);
  customModule.JS.fileName = "customJS";

  auto serverMock = setupGlobalServerInstance<AsyncWebServerCountHandlers>();
  MLPage.registerInEspAsyncWebServer(serverMock);

  size_t handlersRegistered = serverMock->handlersRegistered;

  MLPage.injectModule(customModule);
  MLPage.registerInEspAsyncWebServer(serverMock);
  TEST_ASSERT_EQUAL_MESSAGE(handlersRegistered + 2, serverMock->handlersRegistered, "Should register 2 more handlers for injected module");

  delete customModule.CSS.file;
  delete customModule.JS.file;
}

void test_injection_empty_file()
{
  MaterializeLayoutModule customModule;
  customModule.CSS.fileName = "customCSS";
  customModule.JS.file = new uint8_t(1);
  customModule.JS.fileName = "customJS";

  auto serverMock = setupGlobalServerInstance<AsyncWebServerCountHandlers>();
  MLPage.registerInEspAsyncWebServer(serverMock);

  size_t handlersRegistered = serverMock->handlersRegistered;

  MLPage.injectModule(customModule);
  MLPage.registerInEspAsyncWebServer(serverMock);
  TEST_ASSERT_EQUAL_MESSAGE(handlersRegistered + 1, serverMock->handlersRegistered, "Should register only 1 more handler for injected module");

  delete customModule.JS.file;
}

void test_injection_url()
{

  class AsyncWebServerMock : public AsyncWebServerCountHandlers {
public:
    std::vector<String> registeredHandlersUrl;

    virtual AsyncWebHandler& on(String url, ArRequestHandlerFunction)
    {
      this->registeredHandlersUrl.push_back(url);
      return this->AsyncWebServerCountHandlers::on(String(), ArRequestHandlerFunction());
    }

    bool handlerForUrlIsRegistered(String targetUrl, bool exactValue = true)
    {
      std::function<bool(String&, String&)> comparator = exactValue ? [](String& v, String& target) { return v == target; } : [](String& v, String& target) { return v.indexOf(target) >= 0; };

      for (auto url : this->registeredHandlersUrl) {
        if (comparator(url, targetUrl))
          return true;
      }

      return false;
    }

    virtual bool removeHandler(AsyncWebHandler* h)
    {
      this->registeredHandlersUrl.erase(this->registeredHandlersUrl.begin());

      return this->AsyncWebServerCountHandlers::removeHandler(h);
    }
  };

  MaterializeLayoutModule customModule;
  customModule.CSS.file = new uint8_t(1);
  customModule.CSS.fileName = "customCSS";
  customModule.JS.file = new uint8_t(1);
  customModule.JS.fileName = "customJS";

  auto serverMock = setupGlobalServerInstance<AsyncWebServerMock>();
  MLPage.injectModule(customModule);
  MLPage.registerInEspAsyncWebServer(serverMock);

  TEST_ASSERT_TRUE_MESSAGE(serverMock->handlerForUrlIsRegistered("customCSS.css", false), "Should register handler with css file name");
  TEST_ASSERT_TRUE_MESSAGE(serverMock->handlerForUrlIsRegistered("customJS.js", false), "Should register handler with js file name");

  delete customModule.CSS.file;
  delete customModule.JS.file;
}
