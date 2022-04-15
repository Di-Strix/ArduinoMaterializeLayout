#include <Arduino.h>
#include <HTMLElement/HTMLElement.h>
#include <MaterializeLayoutTypes.h>
#include <Modules/MaterializeCssModule/Components/TabGroup/TabGroup.h>
#include <unity.h>

template <typename T>
class TabMock : public HTMLElement<T> {
  public:
  using HTMLElement<T>::HTMLElement;

  String tabName, HTML;

  String getTabName() { return tabName; };
  String getHTML() { return HTML; }
};

auto tabGroup = TabGroup<HTMLElementArgs>(HTMLElementArgs());

void setUp()
{
  tabGroup = TabGroup<HTMLElementArgs>(HTMLElementArgs());
}

void test_get_html();
void test_render_classes();

void setup()
{
  UNITY_BEGIN();

  RUN_TEST(test_get_html);
  RUN_TEST(test_render_classes);

  UNITY_END();
}

void loop() { }

void test_get_html()
{
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, tabGroup.getHTML().length(), "Should have html by default");

  auto tab = TabMock<HTMLElementArgs>(HTMLElementArgs());
  tabGroup.appendChild(&tab);
  tab.tabName = "TestTab1Name";
  tab.HTML = "TestTab1HTML";

  TEST_ASSERT_GREATER_THAN_MESSAGE(0, tabGroup.getHTML().indexOf("TestTab1HTML"), "Should render tab's html");
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, tabGroup.getHTML().indexOf("TestTab1Name"), "Should render tab's name");
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, tabGroup.getHTML().indexOf("#" + String(tab.getId())), "Should render tab's reference");
}

void test_render_classes()
{
  TEST_ASSERT_EQUAL_MESSAGE(0, tabGroup.classList.value().length(), "Should not have any classes by default");

  tabGroup.classList.add("custom-class");
  TEST_ASSERT_EQUAL_MESSAGE(-1, tabGroup.getHTML().indexOf("custom-class"), "Should not render its classes");
}