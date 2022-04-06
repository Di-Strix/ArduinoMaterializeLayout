/**
 * This example shows what options are available to almost every component and how to use them
 */

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <MaterializeLayout.h>

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

AsyncWebServer server(80);

MaterializeLayout page("DynamicText example");

void createTextColorExample(MaterializeLayoutComponent<Layout> root);
void createLayoutExample(MaterializeLayoutComponent<Layout> root);

MaterializeLayoutComponent<Layout> createSection(String sectionHeading, MaterializeLayoutComponent<Layout> root, bool createDivider = true);

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

  auto rootLayout = page.createAndAppendComponent<Layout>();

  /**
   * list of available settings:
   *  background color
   *  layout type
   *  text color
   *  vertical align
   *  width
   */

  /**
   * 1. Background color can be changed with setBackgroundColor function.
   * as parameters it takes up to two arguments: color and color shade
   * 
   * See "Color Palette" section - https://materializecss.com/color.html
   */
  page.setBackgroundColor(Color::grey, ColorShade::darken4);

  /**
   * 2. Text color can be changed with setTextColor function.
   * This option is derivied by all nested elements, if other isn't specified
   */
  page.setTextColor(Color::white);

  /**
   * Create a component set for text color example 
   */
  auto colorExampleSection = createSection("Text color example", rootLayout, false);
  createTextColorExample(colorExampleSection);

  auto layoutExampleSection = createSection("Layout example", rootLayout);
  createLayoutExample(layoutExampleSection);

  server.begin();
}

void loop()
{
}

MaterializeLayoutComponent<Layout> createSection(String sectionHeading, MaterializeLayoutComponent<Layout> root, bool createDivider)
{
  /**
   * Create container for the section
   */
  auto sectionContainer = page.createAndAppendComponent<Layout>(root);
  sectionContainer->setLayoutType(LayoutType::ROW);

  /**
   * If divider is needed - create it
   * layout component with LayoutType=NONE is plain div(just container)
   */
  if (createDivider) {
    auto divider = page.createAndAppendComponent<Layout>(sectionContainer);
    /**
     * MaterializeCss provide 'divider' class to make a divider, so just use it
     * 
     * https://materializecss.com/grid.html
     */
    divider->classList.add("divider");
  }

  /**
   * Create section heading
   */
  auto sectionContainerHeading = page.createAndAppendComponent<StaticText>(sectionContainer);
  sectionContainerHeading->setText(sectionHeading);
  sectionContainerHeading->setTextType(TextType::h4);

  /**
   * Create container for contents and set its type to column with class 'offset-s1' to add left margin
   */
  auto sectionContentsContainer = page.createAndAppendComponent<Layout>(sectionContainer);
  sectionContentsContainer->setLayoutType(LayoutType::COLUMN);
  sectionContentsContainer->classList.add("offset-s1");
  sectionContentsContainer->setWidth(12, ScreenSize::small);

  /**
   * Return pointer to the contents container
   */
  return sectionContentsContainer;
}

void createTextColorExample(MaterializeLayoutComponent<Layout> root)
{
  /**
   * Here we create coloredTextContainer in which we will specify text color and which will override 
   * page's color setting and will be applied to all nested components
   */
  auto coloredTextContainer = page.createAndAppendComponent<Layout>(root);
  coloredTextContainer->setTextColor(Color::teal);

  auto coloredText = page.createAndAppendComponent<StaticText>(coloredTextContainer);
  coloredText->setText("I and all my nested components will have teal text color");
  auto nestedComponent = page.createAndAppendComponent<Layout>(coloredTextContainer);
  auto nestedText = page.createAndAppendComponent<StaticText>(nestedComponent);
  nestedText->setText("I got text color from my parent, because I don't have my own");
}

void createLayoutExample(MaterializeLayoutComponent<Layout> root)
{
  /**
   * There are three layout types: None, Row, Column
   *  1. None: works just like plain container or folder for the nested elements. It doesn't have any default styles
   *  2. Row: takes up as mush space as POSSIBLE by horizontal axis
   *  3. Column: takes as much as NEEDED space by horizontal axis, so other columns can be placed next to it if required space is available
   *     Column also adds a little paddings on the left and right sides, but if nested layout is row, the padding for the row won't be added
   */
  
  /**
   * So let's create a row to place in it two columns
   */
  auto rootRow = page.createAndAppendComponent<Layout>(root);
  rootRow->setLayoutType(LayoutType::ROW);

  auto column1 = page.createAndAppendComponent<Layout>(rootRow);
  column1->setLayoutType(LayoutType::COLUMN);
  auto column2 = page.createAndAppendComponent<Layout>(rootRow);
  column2->setLayoutType(LayoutType::COLUMN);

  /**
   * We want row to contain only two equal-size columns. We won't take care of adaptive design at this moment, 
   * so we'll set width setting for all display sizes
   * 
   * Width is calculated using 12-column grid, where all avaliable width id divided into 12 columns.
   * For more info see 'Introduction' section - https://materializecss.com/grid.html
   * 
   * As we want to make TWO equal-sized columns we need to set with to 6 for both columns (6 + 6 = 12)
   * For now set screen size to small for the reason I've mentioned before
   */
  column1->setWidth(6, ScreenSize::small);
  column2->setWidth(6, ScreenSize::small);

  /**
   * And place some text into them to see the layout
   */
  auto text1 = page.createAndAppendComponent<StaticText>(column1);
  text1->setText("I am in the first column");
  /**
   * And add some lorem text to make one column height bigger than the other one
   */
  auto lorem = page.createAndAppendComponent<StaticText>(column1);
  lorem->setText("Lorem ipsum dolor sit amet consectetur adipisicing elit. Beatae necessitatibus eaque magnam ratione ipsa veritatis? Unde eum vel quibusdam pariatur eaque nesciunt reiciendis expedita maiores iste. Saepe nihil in ea.");

  auto text2 = page.createAndAppendComponent<StaticText>(column2);
  text2->setText("I am in the second column and aligned vertically");

  /**
   * As one column has height bigger than the other one, we want to align these two columns vertically.
   * We can set vertical align to true for parent component of these columns.
   */
  rootRow->setVerticalAlign(true);
}
