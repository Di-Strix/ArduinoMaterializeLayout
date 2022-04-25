/**
 * This example shows how to use Tabs
 * Text component is used. See corresponding examples if needed
 */

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <MaterializeLayout.h>

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

AsyncWebServer server(80);

MaterializeLayout page("Tabs example");

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
   * Tabs can be placed in any container. To create tabs first you need to create tab group
   */
  auto tabGroup = page.createAndAppendComponent<TabGroup>(&page);

  /**
   * Then create a few tabs and add them to just created tab group
   */
  auto tab1 = page.createAndAppendComponent<Tab>(tabGroup);
  auto tab2 = page.createAndAppendComponent<Tab>(tabGroup);

  /**
   * Set tab name
   */
  tab1->setTabName("First tab");
  tab2->setTabName("Second tab");

  /**
   * Add some contents for the tabs
   */
  auto textOnTab1 = page.createAndAppendComponent<StaticText>(tab1);
  auto textOnTab2 = page.createAndAppendComponent<StaticText>(tab2);
  textOnTab1->setText("Contents on the first page");
  textOnTab2->setText("Contents on the second page");

  server.begin();
}

void loop()
{
}