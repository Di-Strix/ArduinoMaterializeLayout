/**
 * This example shows how to use Layout component
 * Text component is used. See corresponding examples if needed
 */

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <MaterializeLayout.h>

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

AsyncWebServer server(80);

MaterializeLayout page("Layout example");

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
   * Page creates root row by default
   */

  /**
   * Layout could be one of three types:
   *  - Column:
   *    Takes as much space as needed horizontally OR the specified amount (set with setWidth function):
   *    Adds small paddings on the left and right sides. Removed for child layout with 'row' layout type
   *
   *  - Row:
   *    Takes up all available space horizontally. Adds margin at the bottom.
   *
   *  - None:
   *    Just plain container
   *
   */

  /**
   * Let's create row and two columns in it
   */
  auto columnsGroupRow = page.createAndAppendComponent<Layout>(&page);
  columnsGroupRow->setLayoutType(LayoutType::ROW);
  auto column1 = page.createAndAppendComponent<Layout>(columnsGroupRow);
  column1->setLayoutType(LayoutType::COLUMN);
  auto column2 = page.createAndAppendComponent<Layout>(columnsGroupRow);
  column2->setLayoutType(LayoutType::COLUMN);

  /**
   * And place text inside them
   */
  auto textInColumn1 = page.createAndAppendComponent<StaticText>(column1);
  textInColumn1->setText("I'm located in the first column");
  auto textInColumn2 = page.createAndAppendComponent<StaticText>(column2);
  textInColumn2->setText("I'm located in the second column");

  /**
   * Create row to prevent columns that we will add from stack with the existing
   */
  auto columnsGroupRow2 = page.createAndAppendComponent<Layout>(&page);
  columnsGroupRow2->setLayoutType(LayoutType::ROW);

  /**
   * Create two more columns
   */
  auto column3 = page.createAndAppendComponent<Layout>(columnsGroupRow2);
  column3->setLayoutType(LayoutType::COLUMN);
  auto column4 = page.createAndAppendComponent<Layout>(columnsGroupRow2);
  column4->setLayoutType(LayoutType::COLUMN);

  /**
   * Set width 6 for both columns. Each will take up only half of available space (6 + 6 = 12)
   */
  column3->setWidth(6);
  column4->setWidth(6);

  /**
   * Add text
   */
  auto textInColumn3 = page.createAndAppendComponent<StaticText>(column3);
  textInColumn3->setText("I'm located in the first column");
  auto textInColumn4 = page.createAndAppendComponent<StaticText>(column4);
  textInColumn4->setText("I'm located in the second column");

  /**
   * Layout can be styled as well as other MaterializeLayout components. For example color:
   *
   * See "Color Palette" section - https://materializecss.com/color.html
   */
  column3->setBackgroundColor(Color::grey);
  column3->setTextColor(Color::white);

  server.begin();
}

void loop()
{
}