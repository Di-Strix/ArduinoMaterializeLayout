/**
 * This example shows what options are available in text components (DynamicText and StaticText) and how to use them
 * In this example used ESPAsyncWebServer.h which is used to provide the easiest page hosting setup
 */

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <MaterializeLayout.h>

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

AsyncWebServer server(80);

MaterializeLayout page("DynamicText example");

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
   * Create color-demo-row and text-type-demo-row to place topic-related demos in it
   */
  auto colorDemoRow = page.createAndAppendComponent<Layout>(MATERIALIZE_CSS_MODULE, MaterializeCssComponent::Layout, rootRow);
  colorDemoRow->setLayoutType(LayoutType::ROW);
  auto textTypeDemoRow = page.createAndAppendComponent<Layout>(MATERIALIZE_CSS_MODULE, MaterializeCssComponent::Layout, rootRow);
  textTypeDemoRow->setLayoutType(LayoutType::ROW);

  /**
   * Let's create several text components and set different colors to them
   */
  auto text1 = page.createAndAppendComponent<StaticText>(MATERIALIZE_CSS_MODULE, MaterializeCssComponent::StaticText, colorDemoRow);
  text1->setText("I have black background and white text");

  /**
   * Set the backround color to black. Black has no color shades available
   * 
   * See "Color Palette" section - https://materializecss.com/color.html
   */
  text1->setBackgroundColor(Color::black);
  /**
   * Set the text color to white. White has no color shades available too
   * 
   * See "Color Palette" section - https://materializecss.com/color.html
   */
  text1->setTextColor(Color::white);

  auto text2 = page.createAndAppendComponent<StaticText>(MATERIALIZE_CSS_MODULE, MaterializeCssComponent::StaticText, colorDemoRow);
  text2->setText("I have light-green color background with darken-4 shade and teal text with accent-3 shade");

  /**
   * Set the backround color to black. Black has no color shades available
   * 
   * See "Color Palette" section - https://materializecss.com/color.html
   */
  text2->setBackgroundColor(Color::lightGreen, ColorShade::darken4);
  /**
   * Set the text color to white. White has no color shades available too
   * 
   * See "Color Palette" section - https://materializecss.com/color.html
   */
  text2->setTextColor(Color::teal, ColorShade::accent3);

  /**
   * Now let's create text-type demos
   */
  auto text3 = page.createAndAppendComponent<StaticText>(MATERIALIZE_CSS_MODULE, MaterializeCssComponent::StaticText, textTypeDemoRow);
  text3->setText("I'm h2");
  /**
   * Set text type for it. For more info about text types go to: https://www.w3schools.com/html/html_headings.asp
   * By default text-type is paragraph: https://www.w3schools.com/html/html_paragraphs.asp
   */
  text3->setTextType(TextType::h2);

  auto text4 = page.createAndAppendComponent<StaticText>(MATERIALIZE_CSS_MODULE, MaterializeCssComponent::StaticText, textTypeDemoRow);
  text4->setText("I'm h3");
  text4->setTextType(TextType::h3);

  auto text5 = page.createAndAppendComponent<StaticText>(MATERIALIZE_CSS_MODULE, MaterializeCssComponent::StaticText, rootRow);
  text5->setText("Styles can be combined");
  text5->setTextType(TextType::h4);
  text5->setBackgroundColor(Color::grey, ColorShade::darken4);
  text5->setTextColor(Color::white);

  server.begin();
}

void loop()
{
}