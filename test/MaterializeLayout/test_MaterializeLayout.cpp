// TODO: Fix crash

// #include <Arduino.h>
// #include <ESPAsyncWebServer.h>
// #include <MaterializeLayout.h>
// #include <functional>
// #include <gmock/gmock.h>
// #include <iostream>
// #include <vector>

// class AsyncWebServerMock : public AsyncWebServer {
//   public:
//   AsyncWebServerMock() = default;
//   virtual ~AsyncWebServerMock() = default;

//   size_t registeredHandlers = 0;
//   std::vector<String> registeredHandlersUrl;

//   virtual AsyncWebHandler& on(String str, ArRequestHandlerFunction fn) override
//   {
//     this->registeredHandlers++;
//     this->registeredHandlersUrl.push_back(str);

//     return this->AsyncWebServer::on(str, fn);
//   }

//   virtual AsyncWebHandler& on(String url, int method, ArRequestHandlerFunction handler) override
//   {
//     return this->on(url, handler);
//   }

//   virtual AsyncWebHandler& addHandler(AsyncWebHandler* h) override
//   {
//     this->registeredHandlers++;
//     return this->AsyncWebServer::addHandler(h);
//   }

//   virtual bool removeHandler(AsyncWebHandler* h) override
//   {
//     this->registeredHandlers--;
//     registeredHandlersUrl.erase(this->registeredHandlersUrl.begin());

//     return this->AsyncWebServer::removeHandler(h);
//   }

//   bool handlerForUrlIsRegistered(String targetUrl, bool exactValue = true)
//   {
//     std::function<bool(String&, String&)> comparator = exactValue ? [](String& v, String& target) { return v == target; } : [](String& v, String& target) { return v.indexOf(target) >= 0; };

//     for (auto url : this->registeredHandlersUrl) {
//       if (comparator(url, targetUrl))
//         return true;
//     }

//     return false;
//   }
// };

// class TestMaterializeLayout : public ::testing::Test {
//   protected:
//   MaterializeLayout MLPage = MaterializeLayout("");
//   AsyncWebServerMock server = AsyncWebServerMock();
// };

// using ::testing::AtLeast;
// using ::testing::ReturnRef;

// TEST_F(TestMaterializeLayout, getHtml)
// {
//   ASSERT_GT(MLPage.getHTML().length(), 0) << "Should have html by default";
// }

// TEST_F(TestMaterializeLayout, serverRegister)
// {
//   MLPage.registerInEspAsyncWebServer(&server);

//   ASSERT_GT(server.registeredHandlers, 2) << "Should register at least 2 handlers";
// }

// TEST_F(TestMaterializeLayout, serverSwap)
// {
//   AsyncWebServerMock server1;

//   MLPage.registerInEspAsyncWebServer(&server1);
//   MLPage.registerInEspAsyncWebServer(&server);

//   ASSERT_EQ(server1.registeredHandlers, 0) << "Should remove all handers added by ML";
//   ASSERT_GT(server.registeredHandlers, 2) << "Should register at least 2 handlers";
// }

// TEST_F(TestMaterializeLayout, injection)
// {
//   uint8_t file = 1;

//   MaterializeLayoutModule customModule;
//   customModule.CSS.file = &file;
//   customModule.CSS.fileName = "customCSS";
//   customModule.JS.file = &file;
//   customModule.JS.fileName = "customJS";

//   MLPage.registerInEspAsyncWebServer(&server);

//   size_t handlersRegistered = server.registeredHandlers;

//   MLPage.injectModule(customModule);
//   MLPage.registerInEspAsyncWebServer(&server);
//   ASSERT_EQ(server.registeredHandlers, handlersRegistered + 2) << "Should register 2 more handlers for injected module";
// }

// TEST_F(TestMaterializeLayout, emptyFileInjection)
// {
//   uint8_t file = 1;

//   MaterializeLayoutModule customModule;
//   customModule.CSS.fileName = "customCSS";
//   customModule.JS.file = &file;
//   customModule.JS.fileName = "customJS";

//   MLPage.registerInEspAsyncWebServer(&server);

//   size_t handlersRegistered = server.registeredHandlers;

//   MLPage.injectModule(customModule);
//   MLPage.registerInEspAsyncWebServer(&server);
//   ASSERT_EQ(server.registeredHandlers, handlersRegistered + 1) << "Should register only 1 more handler for injected module";
// }

// TEST_F(TestMaterializeLayout, moduleUrlInjection)
// {
//   uint8_t file = 1;

//   MaterializeLayoutModule customModule;
//   customModule.CSS.file = &file;
//   customModule.CSS.fileName = "customCSS";
//   customModule.JS.file = &file;
//   customModule.JS.fileName = "customJS";

//   MLPage.injectModule(customModule);
//   MLPage.registerInEspAsyncWebServer(&server);

//   ASSERT_TRUE(server.handlerForUrlIsRegistered("customCSS.css", false)) << "Should register handler with css file name";
//   ASSERT_TRUE(server.handlerForUrlIsRegistered("customJS.js", false)) << "Should register handler with js file name";
// }

// int main(int argc, char** argv)
// {
//   ::testing::InitGoogleMock(&argc, argv);

//   if (RUN_ALL_TESTS())
//     ;

//   return 0;
// }
