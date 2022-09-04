#include <Arduino.h>
#include <HTMLElement/HTMLElement.h>
#include <MaterializeLayoutTypes.h>
#include <functional>
#include <gmock/gmock.h>

template <typename T>
class HTMLElementMock : public HTMLElement<T> {
  public:
  using HTMLElement<T>::HTMLElement;
  virtual ~HTMLElementMock() = default;

  MOCK_METHOD(String, getHTML, (), (override));
  MOCK_METHOD(void, onEmit, (String value), (override));
  MOCK_METHOD(String, getHandlerId, (), (override));
};

class TestHTMLElement : public ::testing::Test {
  protected:
  HTMLElement<HTMLElementArgs> htmlElement = HTMLElement<HTMLElementArgs>(HTMLElementArgs());
};

using ::testing::Return;

MATCHER_P(AStringEq, target, "Equality of two arduino strings")
{
  return std::get<0>(arg) == String(target);
}

TEST_F(TestHTMLElement, childrenInitialState)
{
  ASSERT_STREQ(htmlElement.getHTML().c_str(), "") << "Should not have children by default";
}

TEST_F(TestHTMLElement, childrenAppend)
{
  auto child = HTMLElementMock<HTMLElementArgs>(HTMLElementArgs());
  EXPECT_CALL(child, getHTML).WillRepeatedly(Return("test1"));

  htmlElement.appendChild(&child);

  ASSERT_STREQ(htmlElement.getHTML().c_str(), "test1") << "Should render HTML of the children";

  auto child2 = HTMLElementMock<HTMLElementArgs>(HTMLElementArgs());
  EXPECT_CALL(child2, getHTML).WillRepeatedly(Return("test2"));

  htmlElement.appendChild(&child2);
  ASSERT_TRUE(htmlElement.getHTML().indexOf("test1") >= 0) << "Should render HTML of the child after adding 'child' and 'child2'";
  ASSERT_TRUE(htmlElement.getHTML().indexOf("test2") >= 0) << "Should render HTML of the child2 after adding 'child' and 'child2'";
}

TEST_F(TestHTMLElement, childDuplicate)
{
  auto child = HTMLElementMock<HTMLElementArgs>(HTMLElementArgs());
  EXPECT_CALL(child, getHTML).WillRepeatedly(Return("test1"));

  htmlElement.appendChild(&child);
  htmlElement.appendChild(&child);
  ASSERT_STREQ(htmlElement.getHTML().c_str(), "test1") << "Should not add already added child";
}

TEST_F(TestHTMLElement, childRemove)
{
  auto child = HTMLElementMock<HTMLElementArgs>(HTMLElementArgs());
  EXPECT_CALL(child, getHTML).WillRepeatedly(Return("test1"));

  auto child2 = HTMLElementMock<HTMLElementArgs>(HTMLElementArgs());
  EXPECT_CALL(child2, getHTML).WillRepeatedly(Return("test2"));

  htmlElement.appendChild(&child);
  htmlElement.appendChild(&child2);
  htmlElement.removeChild(&child);
  ASSERT_STREQ(htmlElement.getHTML().c_str(), "test2") << "Should remove child and as a result - do not render it's html";
}

TEST_F(TestHTMLElement, removeAllChildren)
{
  auto child = HTMLElementMock<HTMLElementArgs>(HTMLElementArgs());
  EXPECT_CALL(child, getHTML).WillRepeatedly(Return("test1"));

  auto child2 = HTMLElementMock<HTMLElementArgs>(HTMLElementArgs());
  EXPECT_CALL(child, getHTML).WillRepeatedly(Return("test2"));

  htmlElement.appendChild(&child);
  htmlElement.appendChild(&child2);
  htmlElement.removeAllChildren();

  ASSERT_STREQ(htmlElement.getHTML().c_str(), "") << "Should render nothing after removing all children";
}

TEST_F(TestHTMLElement, emitAll)
{
  auto child1 = HTMLElementMock<HTMLElementArgs>(HTMLElementArgs());
  EXPECT_CALL(child1, onEmit).Times(1);

  auto child2 = HTMLElementMock<HTMLElementArgs>(HTMLElementArgs());
  EXPECT_CALL(child2, onEmit).Times(0);

  htmlElement.appendChild(&child1);
  htmlElement.appendChild(&child2);
  htmlElement.emit(child1.getId());
}

TEST_F(TestHTMLElement, emitValue)
{
  auto child1 = HTMLElementMock<HTMLElementArgs>(HTMLElementArgs());
  EXPECT_CALL(child1, onEmit).With(AStringEq("abc")).Times(1);

  htmlElement.appendChild(&child1);
  htmlElement.emit(child1.getId(), "abc");
}

TEST_F(TestHTMLElement, unsetDispatcher)
{
  ASSERT_NO_THROW({ htmlElement.dispatch(""); }) << "Should not produce any errors if dispatcher is unset";

  size_t assertions = 0;
  HTMLElementArgs args;
  auto testElement = HTMLElementMock<HTMLElementArgs>(args);

  args.dispatch.dispatcher = [&assertions](String handlerId, size_t elementId, String value) {
    assertions++;
  };
  args.dispatch.throttleTime = 0;

  htmlElement = HTMLElement<HTMLElementArgs>(args);
}

TEST_F(TestHTMLElement, dispatcherCall)
{
  size_t assertions = 0;

  HTMLElementArgs args;
  args.dispatch.dispatcher = [&assertions](String handlerId, size_t elementId, String value) {
    assertions++;
  };
  args.dispatch.throttleTime = 0;

  htmlElement = HTMLElement<HTMLElementArgs>(args);
  htmlElement.dispatch("");
  ASSERT_EQ(assertions, 1) << "Dispatcher function should be called";
}

TEST_F(TestHTMLElement, dispatcherArgs)
{
  class HTMLElementMock : HTMLElement<HTMLElementArgs*> {
public:
    virtual ~HTMLElementMock() = default;
    using HTMLElement<HTMLElementArgs*>::HTMLElement;

    using HTMLElement<HTMLElementArgs*>::getHandlerId;
  };

  String payload = "abc";

  HTMLElementArgs args;
  auto testElement = HTMLElementMock(&args);

  auto htmlElement = HTMLElement<HTMLElementArgs*>(&args);

  args.dispatch.dispatcher = [payload, &testElement, &htmlElement](String handlerId, size_t elementId, String value) {
    ASSERT_STREQ(value.c_str(), payload.c_str()) << "Dispatcher should be called with value";
    ASSERT_STREQ(handlerId.c_str(), testElement.getHandlerId().c_str()) << "Dispatcher should be called with default handlerId";
    ASSERT_EQ(elementId, htmlElement.getId()) << "Dispatcher should be called with exact element id";
  };
  args.dispatch.throttleTime = 0;

  htmlElement.dispatch(payload);
}

TEST_F(TestHTMLElement, dispatchThrottle)
{
  size_t assertions = 0;

  HTMLElementArgs args;
  args.dispatch.dispatcher = [&assertions](String handlerId, size_t elementId, String value) {
    assertions++;
  };
  args.dispatch.throttleTime = 100;

  htmlElement = HTMLElement<HTMLElementArgs>(args);
  htmlElement.dispatch("");
  htmlElement.dispatch("");
  delay(100);
  htmlElement.dispatch("");

  ASSERT_EQ(assertions, 1) << "Dispatcher function should be called only 2 times";
}

TEST_F(TestHTMLElement, customHandlerId)
{
  String customHandlerId = "CustomHandlerId";
  size_t assertions = 0;

  HTMLElementArgs args;
  args.dispatch.dispatcher = [&assertions, customHandlerId](String handlerId, size_t elementId, String value) {
    assertions++;
    ASSERT_STREQ(handlerId.c_str(), customHandlerId.c_str()) << "Dispatcher should be called with custom handlerId";
  };
  args.dispatch.throttleTime = 0;

  auto testElement = HTMLElementMock<HTMLElementArgs>(args);
  EXPECT_CALL(testElement, getHandlerId).WillRepeatedly(Return(customHandlerId));

  testElement.dispatch("");
  ASSERT_EQ(assertions, 1) << "Dispatcher function should be called";
}

TEST_F(TestHTMLElement, width)
{
  htmlElement.setWidth(6, ScreenSize::small);
  ASSERT_FALSE(htmlElement.classList.item(0).isEmpty()) << "Class should be added";
  ASSERT_TRUE(htmlElement.classList.item(1).isEmpty()) << "Only one class should be added";

  ASSERT_EQ(htmlElement.getWidth(ScreenSize::small), 6) << "Getter should return setted width";
  ASSERT_EQ(htmlElement.getWidth(ScreenSize::medium), 0) << "Getter should return 0 if width is unset for provided screen size";

  htmlElement.setWidth(3, ScreenSize::medium);
  ASSERT_FALSE(htmlElement.classList.item(0).isEmpty()) << "Previous class should remain if screen size is different";
  ASSERT_FALSE(htmlElement.classList.item(1).isEmpty()) << "Second class should be added";
  ASSERT_EQ(htmlElement.getWidth(ScreenSize::small), 6) << "Getter should return setted width";
  ASSERT_EQ(htmlElement.getWidth(ScreenSize::medium), 3) << "Getter should return setted width";

  htmlElement.setWidth(10, ScreenSize::small);
  ASSERT_EQ(htmlElement.classList.value().indexOf("6"), -1) << "Should remove class with previous width for the same screen size";
}

TEST_F(TestHTMLElement, backgroundColor)
{
  String classes;

  htmlElement.setBackgroundColor(Color::amber);
  classes = htmlElement.classList.value();
  ASSERT_GT(classes.length(), 0) << "Background color class should be set";
  ASSERT_EQ(htmlElement.getBackgroundColor(), Color::amber) << "Background color getter should return amber color";

  htmlElement.setBackgroundColor(Color::amber, ColorShade::accent4);
  htmlElement.setBackgroundColor(Color::defaultColor);
  classes = htmlElement.classList.value();
  ASSERT_EQ(classes.length(), 0) << "Background color class should be removed";
  ASSERT_EQ(htmlElement.getBackgroundColor(), Color::defaultColor) << "Background color getter should return default color";

  htmlElement.setBackgroundColor(Color::deepOrange, ColorShade::darken3);
  ASSERT_TRUE(
      htmlElement.classList.item(0).length() > 0 && htmlElement.classList.item(1).length() > 0)
      << "Class list should contain two classes after applying background color with color shade";
  ASSERT_EQ(htmlElement.getBackgroundColor(), Color::deepOrange) << "Background color getter should return deep orange color";
  ASSERT_EQ(htmlElement.getBackgroundColorShade(), ColorShade::darken3) << "Background color shade getter should return darken3 color shade";

  htmlElement.setBackgroundColor(Color::amber, ColorShade::noShade);
  ASSERT_EQ(htmlElement.classList.item(1).length(), 0) << "Background color shade class should be removed if noShade";
  ASSERT_EQ(htmlElement.getBackgroundColorShade(), ColorShade::noShade) << "Background color shade getter should return no shade";

  htmlElement.setBackgroundColor(Color::black, ColorShade::darken3);
  ASSERT_EQ(htmlElement.classList.item(1).length(), 0) << "Shade should be ingored if invalid with specified color";
  ASSERT_EQ(htmlElement.getBackgroundColorShade(), ColorShade::noShade) << "Background color shade getter should return no shade";

  htmlElement.setBackgroundColor(Color::deepOrange, ColorShade::accent2);
  htmlElement.setBackgroundColor(Color::brown, ColorShade::accent2);
  ASSERT_EQ(htmlElement.classList.item(1).length(), 0) << "Shade should be removed if invalid new color";
  ASSERT_EQ(htmlElement.getBackgroundColorShade(), ColorShade::noShade) << "Background color shade getter should return no shade";
}

TEST_F(TestHTMLElement, textColor)
{
  String classes;

  htmlElement.setTextColor(Color::amber);
  classes = htmlElement.classList.value();
  ASSERT_GT(classes.length(), 0) << "Text color class should be set";
  ASSERT_EQ(htmlElement.getTextColor(), Color::amber) << "Text color getter should return amber color";

  htmlElement.setTextColor(Color::amber, ColorShade::accent4);
  htmlElement.setTextColor(Color::defaultColor);
  classes = htmlElement.classList.value();
  ASSERT_EQ(classes.length(), 0) << "Text color class should be removed";
  ASSERT_EQ(htmlElement.getTextColor(), Color::defaultColor) << "Text color getter should return default color";
  ASSERT_EQ(htmlElement.getTextColorShade(), ColorShade::noShade) << "Text color shade getter should return no shade";

  htmlElement.setTextColor(Color::deepOrange, ColorShade::darken3);
  ASSERT_TRUE(
      htmlElement.classList.item(0).length() > 0 && htmlElement.classList.item(1).length() > 0)
      << "Class list should contain two classes after applying text color with color shade";
  ASSERT_EQ(htmlElement.getTextColorShade(), ColorShade::darken3) << "Text color shade getter should return darken3";

  htmlElement.setTextColor(Color::amber, ColorShade::noShade);
  ASSERT_EQ(htmlElement.classList.item(1).length(), 0) << "Text color shade class should be removed if noShade";
  ASSERT_EQ(htmlElement.getTextColor(), Color::amber) << "Text color getter should return amber";
  ASSERT_EQ(htmlElement.getTextColorShade(), ColorShade::noShade) << "Text color shade getter should return no shade";

  htmlElement.setTextColor(Color::black, ColorShade::darken3);
  ASSERT_EQ(htmlElement.classList.item(1).length(), 0) << "Shade should be ingored if invalid with specified color";
  ASSERT_EQ(htmlElement.getTextColor(), Color::black) << "Text color getter should return black";
  ASSERT_EQ(htmlElement.getTextColorShade(), ColorShade::noShade) << "Text color shade getter should return no shade";

  htmlElement.setTextColor(Color::deepOrange, ColorShade::accent2);
  htmlElement.setTextColor(Color::brown, ColorShade::accent2);
  ASSERT_EQ(htmlElement.classList.item(1).length(), 0) << "Shade should be removed if invalid new color";
  ASSERT_EQ(htmlElement.getTextColor(), Color::brown) << "Text color getter should return brown";
  ASSERT_EQ(htmlElement.getTextColorShade(), ColorShade::noShade) << "Text color shade getter should return no shade";
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS())
    ;

  return 0;
}
