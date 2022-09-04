#include <Arduino.h>
#include <MaterializeLayoutTypes.h>
#include <Modules/MaterializeCssModule/Components/Button/Button.h>
#include <gtest/gtest.h>

class TestButton : public ::testing::Test {
  protected:
  Button<HTMLElementArgs> button = Button(HTMLElementArgs());
};

TEST_F(TestButton, getHtml)
{
  ASSERT_GT(button.getHTML().length(), 0) << "Should have html by default";
}

TEST_F(TestButton, renderClasses)
{
  ASSERT_GT(button.classList.value().indexOf("btn"), 0) << "Should have btn class by default";

  button.classList.add("custom-class");
  ASSERT_GT(button.getHTML().indexOf("custom-class"), 0) << "Should render its classes";
}

TEST_F(TestButton, caption)
{
  button.setCaption("Button caption");
  ASSERT_GT(button.getHTML().indexOf("Button caption"), 0) << "Should render its caption";
  ASSERT_STREQ(button.getCaption().c_str(), "Button caption") << "getCaption should return current caption";
}

TEST_F(TestButton, callback)
{
  ASSERT_NO_THROW({ button.emit(button.getId()); }) << "Should not produce any errors if callback is not set";

  size_t assertions = 0;
  button.setCallback([&assertions]() { assertions++; });
  button.emit(button.getId());

  ASSERT_EQ(assertions, 1) << "Should call callback if event fired with button's id";
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  if (RUN_ALL_TESTS())
    ;

  return 0;
}
