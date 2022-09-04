#include <Arduino.h>
#include <MaterializeLayoutTypes.h>
#include <Modules/MaterializeCssModule/Components/Text/Text.h>
#include <gtest/gtest.h>

class TestDynamicText : public ::testing::Test {
  protected:
  DynamicText<HTMLElementArgs> text = DynamicText<HTMLElementArgs>(HTMLElementArgs());
};

TEST_F(TestDynamicText, getHtml)
{
  ASSERT_GT(text.getHTML().length(), 0) << "Should have html by default";
}

TEST_F(TestDynamicText, renderClasses)
{
  ASSERT_EQ(text.classList.value().length(), 0) << "Should not have any classes by default";

  text.classList.add("custom-class");
  ASSERT_GT(text.getHTML().indexOf("custom-class"), 0) << "Should render its classes";
}

TEST_F(TestDynamicText, text)
{
  ASSERT_EQ(text.getText().length(), 0) << "Should not have any text by default";

  text.setText("Custom text");
  ASSERT_GE(text.getHTML().indexOf("Custom text"), 0) << "Should render its text";
  ASSERT_STREQ(text.getText().c_str(), "Custom text") << "Should return current text";

  text.setText("    ");
  ASSERT_STREQ(text.getText().c_str(), "") << "Should trim spaces";
}

TEST_F(TestDynamicText, dispatch)
{
  size_t assertions = 0;
  String payload = "text";

  HTMLElementArgs args;
  args.dispatch.throttleTime = 0;
  args.dispatch.dispatcher = [&assertions, payload](String handlerId, size_t elementId, String value) {
    assertions++;
    ASSERT_STREQ(value.c_str(), payload.c_str()) << "Dispatched value should be the same (only trimmed) as provided";
  };

  text = DynamicText<HTMLElementArgs>(args);
  text.setText(payload);
  ASSERT_EQ(assertions, 1) << "Should dispatch text-change event";
}

TEST_F(TestDynamicText, textType)
{
  ASSERT_EQ(text.getTextType(), TextType::p) << "Should have paragraph text type by default";
  ASSERT_GT(text.getHTML().indexOf("<p"), -1) << "Should render setted tag";

  text.setTextType(TextType::h5);
  ASSERT_EQ(text.getTextType(), TextType::h5) << "Should have h5 text type";
  ASSERT_GT(text.getHTML().indexOf("<h5"), -1) << "Should render setted tag";
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  if (RUN_ALL_TESTS())
    ;

  return 0;
}
