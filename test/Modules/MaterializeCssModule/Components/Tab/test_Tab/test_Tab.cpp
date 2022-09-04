#include <Arduino.h>
#include <MaterializeLayoutTypes.h>
#include <Modules/MaterializeCssModule/Components/Tab/Tab.h>
#include <gtest/gtest.h>

class TestTab : public ::testing::Test {
  protected:
  Tab<HTMLElementArgs> tab = Tab<HTMLElementArgs>(HTMLElementArgs());
};

TEST_F(TestTab, getHtml)
{
  ASSERT_EQ(tab.getHTML().length(), 0) << "Should not have any by default html";
}

TEST_F(TestTab, renderClasses)
{
  ASSERT_EQ(tab.classList.value().length(), 0) << "Should not have any classes";

  tab.classList.add("custom-class");
  ASSERT_EQ(tab.getHTML().length(), 0) << "Should not render its classes";
}

TEST_F(TestTab, tabName)
{
  ASSERT_GT(tab.getTabName().length(), 0) << "Should have name by default";

  tab.setTabName("   ");
  ASSERT_GT(tab.getTabName().length(), 0) << "Should not set empty name";

  tab.setTabName("abc");
  ASSERT_STREQ(tab.getTabName().c_str(), "abc") << "Should set name";
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  if (RUN_ALL_TESTS())
    ;

  return 0;
}
