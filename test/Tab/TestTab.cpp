#include <Arduino.h>
#include <MaterializeLayoutTypes.h>
#include <Modules/MaterializeCssModule/Components/Tab/Tab.h>
#include <unity.h>

auto tab = Tab<HTMLElementArgs>(HTMLElementArgs());

void setUp()
{
  tab = Tab<HTMLElementArgs>(HTMLElementArgs());
}

void test_get_html();
void test_render_classes();
void test_tab_name();

void setup()
{
  UNITY_BEGIN();

  RUN_TEST(test_get_html);
  RUN_TEST(test_render_classes);
  RUN_TEST(test_tab_name);

  UNITY_END();
}

void loop() { }

void test_get_html()
{
  TEST_ASSERT_EQUAL_MESSAGE(0, tab.getHTML().length(), "Should not have any by default html");
}

void test_render_classes()
{
  TEST_ASSERT_EQUAL_MESSAGE(0, tab.classList.value().length(), "Should not have any classes");

  tab.classList.add("custom-class");
  TEST_ASSERT_EQUAL_MESSAGE(0, tab.getHTML().length(), "Should not render its classes");
}

void test_tab_name()
{
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, tab.getTabName().length(), "Should have name by default");

  tab.setTabName("   ");
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, tab.getTabName().length(), "Should not set empty name");

  tab.setTabName("abc");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("abc", tab.getTabName().c_str(), "Should set name");
}