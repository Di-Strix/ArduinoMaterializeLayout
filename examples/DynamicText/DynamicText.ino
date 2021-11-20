/**
 * This example shows how to use DynamicText component
 * In this example used ESPAsyncWebServer.h which is used to provide the easiest page hosting setup
 */

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <MaterializeLayout.h>

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

AsyncWebServer server(80);

MaterializeLayout page("DynamicText example");

/**
 * To store a pointer to the component you can use MaterializeLayoutComponent<T> type, where T is desired component type
 */
MaterializeLayoutComponent<StaticText> staticText;
MaterializeLayoutComponent<DynamicText> dynamicText;

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

  auto rootLayout = page.createAndAppendComponent<Layout>(MATERIALIZE_CSS_MODULE, MaterializeCssComponent::Layout, &page);
  auto rootRow = page.createAndAppendComponent<Layout>(MATERIALIZE_CSS_MODULE, MaterializeCssComponent::Layout, rootLayout);
  rootRow->setLayoutType(LayoutType::ROW);

  /**
   * Unlike StaticText, DynamicText updates its contents on the page dynamically, without reloading the page
   * At this moment DynamicText updates only its contents, without styles, so if you change text style you'll have to reload the page to see new styles
   * 
   * Let's create StaticText and DynamicText components to demonstrate the difference
   */
  staticText = page.createAndAppendComponent<StaticText>(MATERIALIZE_CSS_MODULE, MaterializeCssComponent::StaticText, rootRow);
  dynamicText = page.createAndAppendComponent<DynamicText>(MATERIALIZE_CSS_MODULE, MaterializeCssComponent::DynamicText, rootRow);

  server.begin();
}

void loop()
{
  staticText->setText("I'm static, my counter: " + String(millis() / 1000));
  dynamicText->setText("I'm dynamic, my counter: " + String(millis() / 1000));

  delay(1000);
}