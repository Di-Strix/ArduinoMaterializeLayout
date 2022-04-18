#include <Arduino.h>
#include <MaterializeLayoutTypes.h>
#include <Modules/MaterializeCssModule/Components/Button/Button.h>
#include <unity.h>

auto button = Button<HTMLElementArgs>(HTMLElementArgs());

void setUp()
{
  button = Button(HTMLElementArgs());
}

void test_get_html();
void test_render_classes();
void test_caption();
void test_callback();

void setup()
{
  UNITY_BEGIN();

  RUN_TEST(test_get_html);
  RUN_TEST(test_render_classes);
  RUN_TEST(test_caption);
  RUN_TEST(test_callback);

  UNITY_END();
}

void loop() { }

void test_get_html()
{
  TEST_ASSERT_GREATER_THAN_INT_MESSAGE(0, button.getHTML().length(), "Should have html by default");
}

void test_render_classes()
{
  TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(0, button.classList.value().indexOf("btn"), "Should have btn class by default");

  button.classList.add("custom-class");
  TEST_ASSERT_GREATER_THAN_INT_MESSAGE(0, button.getHTML().indexOf("custom-class"), "Should render its classes");
}

void test_caption()
{
  button.setCaption("Button caption");
  TEST_ASSERT_GREATER_THAN_INT_MESSAGE(0, button.getHTML().indexOf("Button caption"), "Should render its caption");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("Button caption", button.getCaption().c_str(), "getCaption should return current caption");
}

void test_callback()
{
  button.emit(button.getId());
  TEST_ASSERT_TRUE_MESSAGE(true, "Should not produce any errors if callback is not set");

  size_t assertions = 0;
  button.setCallback([&assertions]() { assertions++; });
  button.emit(button.getId());
  TEST_ASSERT_EQUAL_MESSAGE(1, assertions, "Should call callback if event fired with button's id");
}