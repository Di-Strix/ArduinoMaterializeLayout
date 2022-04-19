/**
 * This example shows how to use Field component
 * Button and Layout component is used. See corresponding examples if needed
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
MaterializeLayoutComponent<Field> field;

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
   * Create root Row
   */
  auto rootRow = page.createAndAppendComponent<Layout>(&page);
  rootRow->setLayoutType(LayoutType::ROW);
  rootRow->setVerticalAlign(true);

  /**
   * Create Field
   */
  field = page.createAndAppendComponent<Field>(rootRow);

  /**
   * Set the field name. It will be displayed above
   */
  field->setName("The text here will be printed to the serial");

  button = page.createAndAppendComponent<Button>(rootRow);
  button->setCaption("Print to serial");
  button->setCallback([]() {
    /**
     * You can call getCurrentValue anytime on the field to get current value
     */
    Serial.println(field->getCurrentValue());
  });

  /**
   * Field can be styled as well as all the other MaterializeLayout components
   * For example:
   */
  field->setWidth(9);

  button->setWidth(3);

  server.begin();
}

void loop()
{
  delay(1000);
}