#include <Arduino.h>
#include <MaterializeLayoutTypes.h>
#include <Modules/MaterializeCssModule/Components/Text/Text.h>
#include <unity.h>

auto text = StaticText<HTMLElementArgs>(HTMLElementArgs());

void setUp()
{
  text = StaticText<HTMLElementArgs>(HTMLElementArgs());
}

void test_get_html();
void test_render_classes();
void test_text();
void test_dispatch();
void test_text_type();

void setup()
{
  UNITY_BEGIN();

  RUN_TEST(test_get_html);
  RUN_TEST(test_render_classes);
  RUN_TEST(test_text);
  RUN_TEST(test_dispatch);
  RUN_TEST(test_text_type);

  UNITY_END();
}

void loop() { }

void test_get_html()
{
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, text.getHTML().length(), "Should have html by default");
}

void test_render_classes()
{
  TEST_ASSERT_EQUAL_MESSAGE(0, text.classList.value().length(), "Should not have any classes by default");

  text.classList.add("custom-class");
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, text.getHTML().indexOf("custom-class"), "Should render its classes");
}

void test_text()
{
  TEST_ASSERT_EQUAL_MESSAGE(0, text.getText().length(), "Should not have any text by default");

  text.setText("Custom text");
  TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(0, text.getHTML().indexOf("Custom text"), "Should render its text");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("Custom text", text.getText().c_str(), "Should return current text");

  text.setText("    ");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("", text.getText().c_str(), "Should trim spaces");
}

void test_dispatch()
{
  size_t assertions = 0;

  HTMLElementArgs args;
  args.dispatch.throttleTime = 0;
  args.dispatch.dispatcher = [&assertions](String handlerId, size_t elementId, String value) { assertions++; };

  text = StaticText<HTMLElementArgs>(args);
  text.setText("text");
  TEST_ASSERT_EQUAL_MESSAGE(0, assertions, "Should not dispatch text-change event");
}

void test_text_type()
{
  TEST_ASSERT_EQUAL_MESSAGE(TextType::p, text.getTextType(), "Should have paragraph text type by default");
  TEST_ASSERT_GREATER_THAN_MESSAGE(-1, text.getHTML().indexOf("<p"), "Should render setted tag");

  text.setTextType(TextType::h5);
  TEST_ASSERT_EQUAL_MESSAGE(TextType::h5, text.getTextType(), "Should have h5 text type");
  TEST_ASSERT_GREATER_THAN_MESSAGE(-1, text.getHTML().indexOf("<h5"), "Should render setted tag");
}