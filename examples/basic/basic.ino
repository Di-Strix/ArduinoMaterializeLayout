/**
 * This example shows the basics of using this library
 */

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <MaterializeLayout.h>

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

// Create AsyncWebServer instance, listening on port 80 (HTTP)
AsyncWebServer server(80);

// Create MaterializeLayout instance with "Basic example" text as page title
MaterializeLayout page("Basic example");

void setup()
{
  // Serial setup
  Serial.begin(115200);
  // Print few blank lines to visually separate messanges from the serial noise
  Serial.print("\n\n\n");

  // WiFi setup
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Wait until the esp connects to the network
  auto connResult = WiFi.waitForConnectResult();

  // Reboot if connection failed
  if (connResult != WL_CONNECTED) {
    Serial.println("Connection failed, please, check entered WiFi SSID and password");
    delay(1000);
    ESP.reset();
  }

  // Print the ip address to the serial
  Serial.println("Visit http://" + WiFi.localIP().toString() + "/index.html to view the page");

  // Use automatic integration with ESPAsyncWebServer. Let the library do all the dirty work
  page.registerInEspAsyncWebServer(&server);

  /**
   * Let's create root layout
   * Layout type designed to align nested items as you want
   * Layouts can be nested.
   */
  /**
   * To create root layout we need to call component factory.
   * We can do this by calling 'createComponent' method from the page class.
   *
   * Factory creates and returns pointer to the component with MaterializeLayoutComponent<COMPONENT_TYPE> type
   * The pointer can be null if error had occurred
   */

  // MaterializeLayoutComponent<Layout> rootLayout = page.createComponent<Layout>(MATERIALIZE_CSS_MODULE, MaterializeCssComponent::Layout);
  auto rootLayout = page.createComponent<Layout>(); // The same as in the previous line, but with auto

  /**
   * Default module contains all the basic components that you can place on your page
   * Also there is a chartist module, which you can inject to place charts on your page, but we'll talk about it later.
   */

  /**
   * Now we have to append component that we have just created to the page
   */
  page.appendChild(rootLayout);

  /**
   * Provide anchor for the component as we need it to be nested in rootLayout.
   * If the anchor is null it would be set to the page's root by default.
   **/
  auto headingRow = page.createAndAppendComponent<Layout>(rootLayout);

  /**
   * Layouts can be one of three types
   * 1: none: just container without any styles
   * 2: row: container that takes all the available width and adds bottom margin under itself
   * 3: column: container has padding on the left and right sides. Takes up as much space as needed or takes specified with, if set
   *
   * So let's create a row to put the page heading into it.
   *
   * We can use createAndAppendComponent function to create and add a component to the specified component at once.
   * So lines
   *    auto rootLayout = page.createComponent<Layout>();
   *    page.appendChild(rootLayout);
   * and
   *    auto rootLayout = page.createAndAppendComponent<Layout>();
   * do the same
   */
  headingRow->setLayoutType(LayoutType::ROW); // Set layout type

  /**
   * There are two variants of text class that can be created
   * 1: Static text. Only updated on page re-render(when browser requests the page)
   * 2: Dynamic text. Text contents updates on the page every second
   *
   * For heading we don't need any live update, so create a static text
   */
  auto heading = page.createAndAppendComponent<StaticText>(headingRow);
  /**
   * Let's make text "Hello ArduinoMaterializeLayout!"
   * and make it bigger
   */
  heading->setText("Hello ArduinoMaterializeLayout!");
  heading->setTextType(TextType::h2);
  /**
   * About the text sizes you can read on https://www.w3schools.com/html/html_headings.asp
   */

  /**
   * Start the server
   */
  server.begin();
}

void loop()
{
}