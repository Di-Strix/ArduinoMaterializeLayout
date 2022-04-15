#include <Arduino.h>
#include <MaterializeLayoutTypes.h>
#include <Modules/MaterializeCssModule/Components/Layout/Layout.h>
#include <unity.h>

auto layout = Layout<HTMLElementArgs>(HTMLElementArgs());

void setUp()
{
  layout = Layout<HTMLElementArgs>(HTMLElementArgs());
}

void test_get_html();
void test_render_classes();
void test_vertical_align();
void test_layout_type();

void setup()
{
  UNITY_BEGIN();

  RUN_TEST(test_get_html);
  RUN_TEST(test_render_classes);
  RUN_TEST(test_vertical_align);
  RUN_TEST(test_layout_type);

  UNITY_END();
}

void loop() { }

void test_get_html()
{
  TEST_ASSERT_GREATER_THAN_INT_MESSAGE(0, layout.getHTML().length(), "Should have html by default");
}

void test_render_classes()
{
  TEST_ASSERT_EQUAL_MESSAGE(0, layout.classList.value().length(), "Should not have classes by default");

  layout.classList.add("custom-class");
  TEST_ASSERT_GREATER_THAN_INT_MESSAGE(0, layout.getHTML().indexOf("custom-class"), "Should render its classes");
}

void test_vertical_align()
{
  layout.setVerticalAlign(true);
  TEST_ASSERT_GREATER_THAN_INT_MESSAGE(0, layout.classList.value().length(), "Should add vertical align wrapper class");

  layout.setVerticalAlign(false);
  TEST_ASSERT_EQUAL_MESSAGE(0, layout.classList.value().length(), "Should remove vertical align wrapper class");
}

void test_layout_type()
{
  layout.setLayoutType(LayoutType::COLUMN);
  TEST_ASSERT_GREATER_THAN_INT_MESSAGE(0, layout.classList.value().length(), "Should add column class");
  TEST_ASSERT_EQUAL_MESSAGE(LayoutType::COLUMN, layout.getLayoutType(), "Should return column layout type");

  layout.setLayoutType(LayoutType::NONE);
  TEST_ASSERT_EQUAL_MESSAGE(0, layout.classList.value().length(), "Should not have any classes");
  TEST_ASSERT_EQUAL_MESSAGE(LayoutType::NONE, layout.getLayoutType(), "Should return 'none' layout type");
}