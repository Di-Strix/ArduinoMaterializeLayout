#include <Arduino.h>
#include <MaterializeLayoutTypes.h>
#include <Modules/MaterializeCssModule/Components/Field/Field.h>
#include <unity.h>

auto field = Field<HTMLElementArgs>(HTMLElementArgs());

void setUp()
{
  field = Field<HTMLElementArgs>(HTMLElementArgs());
}

void test_get_html();
void test_render_classes();
void test_name();
void test_default_value();
void test_current_value();

void setup()
{
  UNITY_BEGIN();

  RUN_TEST(test_get_html);
  RUN_TEST(test_render_classes);
  RUN_TEST(test_name);
  RUN_TEST(test_default_value);
  RUN_TEST(test_current_value);

  UNITY_END();
}

void loop() { }

void test_get_html()
{
  TEST_ASSERT_GREATER_THAN_INT_MESSAGE(0, field.getHTML().length(), "Should have html by default");
}

void test_render_classes()
{
  TEST_ASSERT_GREATER_OR_EQUAL_MESSAGE(0, field.classList.value().indexOf("input-field"), "Should have input-field class by default");

  field.classList.add("custom-class");
  TEST_ASSERT_GREATER_THAN_INT_MESSAGE(0, field.getHTML().indexOf("custom-class"), "Should render its classes");
}

void test_name()
{
  field.setName("Custom name");
  TEST_ASSERT_GREATER_THAN_INT_MESSAGE(0, field.getHTML().indexOf("Custom name"), "Should render its name");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("Custom name", field.getName().c_str(), "Should return setted name");
}

void test_default_value()
{
  field.setDefaultValue("Custom default value");
  TEST_ASSERT_GREATER_THAN_INT_MESSAGE(0, field.getHTML().indexOf("Custom default value"), "Should render its default value");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("Custom default value", field.getDefaultValue().c_str(), "Should return setted default value");
}

void test_current_value()
{
  TEST_ASSERT_EQUAL_STRING_MESSAGE("", field.getCurrentValue().c_str(), "Current value should be empty by default");
  field.emit(field.getId(), "testValue");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("testValue", field.getCurrentValue().c_str(), "Current value should set after event fired with its id");
}