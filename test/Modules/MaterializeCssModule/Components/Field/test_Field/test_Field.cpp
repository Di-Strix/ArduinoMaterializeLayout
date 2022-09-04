#include <Arduino.h>
#include <MaterializeLayoutTypes.h>
#include <Modules/MaterializeCssModule/Components/Field/Field.h>
#include <gtest/gtest.h>

class TestField : public ::testing::Test {
  protected:
  Field<HTMLElementArgs> field = Field<HTMLElementArgs>(HTMLElementArgs());
};

TEST_F(TestField, getHtml)
{
  ASSERT_GT(field.getHTML().length(), 0) << "Should have html by default";
}

TEST_F(TestField, renderClasses)
{
  ASSERT_GE(field.classList.value().indexOf("input-field"), 0) << "Should have input-field class by default";

  field.classList.add("custom-class");
  ASSERT_GT(field.getHTML().indexOf("custom-class"), 0) << "Should render its classes";
}

TEST_F(TestField, name)
{
  field.setName("Custom name");
  ASSERT_GT(field.getHTML().indexOf("Custom name"), 0) << "Should render its name";
  ASSERT_STREQ(field.getName().c_str(), "Custom name") << "Should return setted name";
}

TEST_F(TestField, defaultValue)
{
  field.setDefaultValue("Custom default value");
  ASSERT_GT(field.getHTML().indexOf("Custom default value"), 0) << "Should render its default value";
  ASSERT_STREQ(field.getDefaultValue().c_str(), "Custom default value") << "Should return setted default value";
}

TEST_F(TestField, currentValue)
{
  ASSERT_STREQ(field.getCurrentValue().c_str(), "") << "Current value should be empty by default";
  field.emit(field.getId(), "testValue");
  ASSERT_STREQ(field.getCurrentValue().c_str(), "testValue") << "Current value should set after event fired with its id";
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  if (RUN_ALL_TESTS())
    ;

  return 0;
}
