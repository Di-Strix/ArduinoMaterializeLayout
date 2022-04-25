/**
 * This example shows how to use Button component
 */

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <MaterializeLayout.h>

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

AsyncWebServer server(80);

MaterializeLayout page("Button example");

/**
 * To store a pointer to the component you can use MaterializeLayoutComponent<T> type, where T is desired component type
 */
MaterializeLayoutComponent<Button> button;

void setup()
{
  Serial.begin(115200);
  Serial.print("\n\n\n");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  auto connResult = WiFi.waitForConnectResult();

  if (connResult != WL_CONNECTED) {
    Serial.println("Connection failed, please, check entered WiFi SSID and password");
    delay(1000);
    ESP.reset();
  }

  Serial.println("Visit http://" + WiFi.localIP().toString() + "/index.html to view the page");

  page.registerInEspAsyncWebServer(&server);

  /**
   * Create and append Button
   */
  button = page.createAndAppendComponent<Button>(&page);

  /**
   * Set caption for the button
   */
  button->setCaption("Click me to say Hello from callback");

  /**
   * Set click callback.
   * The callback function is invoked when corresponding button is clicked on the page.
   * Callback takes no arguments
   */
  button->setCallback([]() {
    Serial.println("Hello from callback!");
  });

  /**
   * Button can be styled as well as all other MaterializeLayout components
   * For example:
   */
  button->setBackgroundColor(Color::grey);
  button->setWidth(12);

  server.begin();
}

void loop()
{
}