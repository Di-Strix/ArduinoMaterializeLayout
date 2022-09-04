#include <Arduino.h>
#include <HTMLElement/HTMLElement.h>
#include <MaterializeLayoutTypes.h>
#include <Modules/MaterializeCssModule/Components/TabGroup/TabGroup.h>
#include <gmock/gmock.h>

template <typename T>
class TabMock : public Tab<T> {
  public:
  using Tab<T>::Tab;
  virtual ~TabMock() = default;

  MOCK_METHOD(String, getHTML, (), (override));
  MOCK_METHOD(String, getTabName, (), (override));
};

class TestTabGroup : public ::testing::Test {
  protected:
  TabGroup<HTMLElementArgs> tabGroup = TabGroup<HTMLElementArgs>(HTMLElementArgs());
};

using ::testing::Return;

TEST_F(TestTabGroup, getHtml)
{
  ASSERT_GT(tabGroup.getHTML().length(), 0) << "Should have html by default";

  auto tab = TabMock<HTMLElementArgs>(HTMLElementArgs());

  EXPECT_CALL(tab, getHTML()).WillRepeatedly(Return("TestTab1HTML"));
  EXPECT_CALL(tab, getTabName()).WillRepeatedly(Return("TestTab1Name"));

  tabGroup.appendChild(&tab);

  ASSERT_GT(tabGroup.getHTML().indexOf("TestTab1HTML"), 0) << "Should render tab's html";
  ASSERT_GT(tabGroup.getHTML().indexOf("TestTab1Name"), 0) << "Should render tab's name";
  ASSERT_GT(tabGroup.getHTML().indexOf("#" + String(tab.getId())), 0) << "Should render tab's reference";
}

TEST_F(TestTabGroup, renderClasses)
{
  ASSERT_GT(tabGroup.classList.value().length(), 0) << "Should have classes by default";

  tabGroup.classList.add("custom-class");
  ASSERT_GT(tabGroup.getHTML().indexOf("custom-class"), 0) << "Should render its classes";
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS())
    ;

  return 0;
}
