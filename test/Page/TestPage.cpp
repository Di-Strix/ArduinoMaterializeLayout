#include <Arduino.h>
#include <HTMLElement/HTMLElement.h>
#include <MaterializeLayoutTypes.h>
#include <Page/Page.h>
#include <unity.h>

template <typename T>
class HTMLElementMock : public HTMLElement<T> {
  public:
  using HTMLElement<T>::HTMLElement;

  String HTML;

  String getHTML() { return HTML; }
};

auto page = Page<HTMLElementArgs>(HTMLElementArgs(), "");

void setUp()
{
  page = Page<HTMLElementArgs>(HTMLElementArgs(), "");
}

void test_get_html();
void test_render_classes();
void test_page_title();
void test_src();
void test_children();

void setup()
{
  UNITY_BEGIN();

  RUN_TEST(test_get_html);
  RUN_TEST(test_render_classes);
  RUN_TEST(test_page_title);
  RUN_TEST(test_src);
  RUN_TEST(test_children);

  UNITY_END();
}

void loop() { }

void test_get_html()
{
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, page.getHTML().length(), "Should have html by default");
}

void test_render_classes()
{
  TEST_ASSERT_EQUAL_MESSAGE(0, page.classList.value().length(), "Should not have any classes by default");

  page.classList.add("custom-class");
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, page.getHTML().indexOf("custom-class"), "Should render its classes");
}

void test_page_title()
{
  page.setPageTitle("    abc  ");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("abc", page.getPageTitle().c_str(), "Should return current page title. It should be trimmed");

  page = Page<HTMLElementArgs>(HTMLElementArgs(), "   123123    ");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("123123", page.getPageTitle().c_str(), "Trimmed page title should be set through constructor");

  TEST_ASSERT_GREATER_THAN_MESSAGE(0, page.getHTML().indexOf("123123"), "Should render page title");

  page.setPageTitle("Custom title");
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, page.getHTML().indexOf("Custom title"), "Should render page title");
}

void test_src()
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

  TEST_ASSERT_GREATER_THAN_MESSAGE(0, page.getHTML().indexOf("Custom style1 filename"), "Should render links to styles");
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, page.getHTML().indexOf("Custom style1 inline styles"), "Should render inline styles");

  TEST_ASSERT_GREATER_THAN_MESSAGE(0, page.getHTML().indexOf("Custom script1 filename"), "Should render links to script files");
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, page.getHTML().indexOf("Custom script1 inline script"), "Should render inline scripts");

  TEST_ASSERT_EQUAL_MESSAGE(4, assertions, "Src getter should be called at every render");
}

void test_children()
{
  auto child = HTMLElementMock<HTMLElementArgs>(HTMLElementArgs());
  page.appendChild(&child);
  child.HTML = "Custom child's HTML";

  TEST_ASSERT_GREATER_THAN_MESSAGE(0, page.getHTML().indexOf("Custom child's HTML"), "Should render children's HTML");
}