#include <Arduino.h>
#include <HTMLElement/HTMLElement.h>
#include <MaterializeLayoutTypes.h>
#include <Page/Page.h>
#include <gmock/gmock.h>

template <typename T>
class HTMLElementMock : public HTMLElement<T> {
  public:
  using HTMLElement<T>::HTMLElement;
  virtual ~HTMLElementMock() = default;

  MOCK_METHOD(String, getHTML, (), (override));
};

class TestPage : public ::testing::Test {
  protected:
  Page<HTMLElementArgs> page = Page<HTMLElementArgs>(HTMLElementArgs(), "");
};

using ::testing::Return;

TEST_F(TestPage, getHTML)
{
  ASSERT_GT(page.getHTML().length(), 0) << "Should have html by default";
}

TEST_F(TestPage, renderClasses)
{
  ASSERT_EQ(page.classList.value().length(), 0) << "Should not have any classes by default";

  page.classList.add("custom-class");
  ASSERT_GT(page.getHTML().indexOf("custom-class"), 0) << "Should render its classes";
}

TEST_F(TestPage, pageTitle)
{
  page.setPageTitle("    abc  ");
  ASSERT_STREQ(page.getPageTitle().c_str(), "abc") << "Should return current page title. It should be trimmed";

  page = Page<HTMLElementArgs>(HTMLElementArgs(), "   123123    ");
  ASSERT_STREQ(page.getPageTitle().c_str(), "123123") << "Trimmed page title should be set through constructor";

  ASSERT_GT(page.getHTML().indexOf("123123"), 0) << "Should render page title";

  page.setPageTitle("Custom title");
  ASSERT_GT(page.getHTML().indexOf("Custom title"), 0) << "Should render page title";
}

TEST_F(TestPage, src)
{
  size_t assertions = 0;

  PageSources srcs;
  srcs.styles.push_back({ "Custom style1 filename", "Custom style1 inline styles" });
  srcs.scripts.push_back({ "Custom script1 filename", "Custom script1 inline script" });

  class PageInheritor : public Page<HTMLElementArgs> {
public:
    using Page::Page;

    std::function<PageSources()> srcGetter;

    PageSources compileSrc()
    {
      if (this->srcGetter)
        return this->srcGetter();
      return PageSources();
    }
  };

  auto page = PageInheritor(HTMLElementArgs(), "");
  page.srcGetter = [&srcs, &assertions]() {assertions++; return srcs; };

  ASSERT_GT(page.getHTML().indexOf("Custom style1 filename"), 0) << "Should render links to styles";
  ASSERT_GT(page.getHTML().indexOf("Custom style1 inline styles"), 0) << "Should render inline styles";

  ASSERT_GT(page.getHTML().indexOf("Custom script1 filename"), 0) << "Should render links to script files";
  ASSERT_GT(page.getHTML().indexOf("Custom script1 inline script"), 0) << "Should render inline scripts";

  ASSERT_EQ(assertions, 4) << "Src getter should be called at every render";
}

TEST_F(TestPage, children)
{
  auto child = HTMLElementMock<HTMLElementArgs>(HTMLElementArgs());
  EXPECT_CALL(child, getHTML).Times(1).WillRepeatedly(Return("Custom child's HTML"));
  page.appendChild(&child);

  ASSERT_GT(page.getHTML().indexOf("Custom child's HTML"), 0) << "Should render children's HTML";
}

TEST_F(TestPage, pageLanguage)
{
  ASSERT_GT(page.getPageLanguage().length(), 0) << "Should have lang code by default";

  String langCode = "SomeLanguageCode";
  page.setPageLanguage(langCode);
  ASSERT_GT(page.getHTML().indexOf(langCode), 0) << "Should render page's lang code";
  ASSERT_STREQ(page.getPageLanguage().c_str(), langCode.c_str()) << "Should return current page lang code";

  page.setPageLanguage(String("   ") + langCode + String("     "));

  ASSERT_STREQ(page.getPageLanguage().c_str(), langCode.c_str()) << "Should trim spaces";
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS())
    ;

  return 0;
}
