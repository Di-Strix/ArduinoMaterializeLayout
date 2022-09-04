#include <Arduino.h>
#include <MaterializeLayoutTypes.h>
#include <Modules/MaterializeCssModule/Components/Layout/Layout.h>
#include <gtest/gtest.h>

class TestLayout : public ::testing::Test {
  protected:
  Layout<HTMLElementArgs> layout = Layout<HTMLElementArgs>(HTMLElementArgs());
};

TEST_F(TestLayout, getHtml)
{
  ASSERT_GT(layout.getHTML().length(), 0) << "Should have html by default";
}

TEST_F(TestLayout, renderClasses)
{
  ASSERT_EQ(layout.classList.value().length(), 0) << "Should not have classes by default";

  layout.classList.add("custom-class");
  ASSERT_GT(layout.getHTML().indexOf("custom-class"), 0) << "Should render its classes";
}

TEST_F(TestLayout, verticalAlign)
{
  layout.setVerticalAlign(true);
  ASSERT_GT(layout.classList.value().length(), 0) << "Should add vertical align wrapper class";

  layout.setVerticalAlign(false);
  ASSERT_EQ(layout.classList.value().length(), 0) << "Should remove vertical align wrapper class";
}

TEST_F(TestLayout, layoutType)
{
  layout.setLayoutType(LayoutType::COLUMN);
  ASSERT_GT(layout.classList.value().length(), 0) << "Should add column class";
  ASSERT_EQ(layout.getLayoutType(), LayoutType::COLUMN) << "Should return column layout type";

  layout.setLayoutType(LayoutType::NONE);
  ASSERT_EQ(layout.classList.value().length(), 0) << "Should not have any classes";
  ASSERT_EQ(layout.getLayoutType(), LayoutType::NONE) << "Should return 'none' layout type";
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  if (RUN_ALL_TESTS())
    ;

  return 0;
}
