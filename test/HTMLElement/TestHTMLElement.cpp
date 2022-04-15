#include <Arduino.h>
#include <HTMLElement/HTMLElement.h>
#include <MaterializeLayoutTypes.h>
#include <functional>
#include <unity.h>

template <typename T>
class TestHTMLElement : public HTMLElement<T> {
  public:
  using HTMLElement<T>::HTMLElement;

  String HTML;
  String handlerId;
  String getHTML() { return HTML; };
  std::function<void(String value)> onEmitHandler;

  virtual String getHandlerId()
  {
    return this->handlerId.isEmpty() ? HTMLElement<T>::getHandlerId() : this->handlerId;
  };

  virtual void onEmit(String value)
  {
    if (this->onEmitHandler)
      this->onEmitHandler(value);
  };
};

auto htmlElement = HTMLElement<HTMLElementArgs>(HTMLElementArgs());

void setUp()
{
  htmlElement = HTMLElement<HTMLElementArgs>(HTMLElementArgs());
}

void test_children_initial_state();
void test_children_append();
void test_children_remove();
void test_children_remove_all();

void test_emit_call();
void test_emit_value();

void test_dispatch_call();
void test_dispatch_args();
void test_dispatch_throttle();
void test_dispatch_custom_handler_id();

void test_width();

void test_background_color();
void test_text_color();

void setup()
{
  UNITY_BEGIN();

  RUN_TEST(test_children_initial_state);
  RUN_TEST(test_children_append);
  RUN_TEST(test_children_remove);
  RUN_TEST(test_children_remove_all);

  RUN_TEST(test_emit_call);
  RUN_TEST(test_emit_value);

  RUN_TEST(test_dispatch_call);
  RUN_TEST(test_dispatch_args);
  RUN_TEST(test_dispatch_throttle);
  RUN_TEST(test_dispatch_custom_handler_id);

  RUN_TEST(test_width);

  RUN_TEST(test_background_color);
  RUN_TEST(test_text_color);

  UNITY_END();
}

void loop() { }

void test_children_initial_state()
{
  TEST_ASSERT_EQUAL_STRING_MESSAGE("", htmlElement.getHTML().c_str(), "Should not have children by default");
}

void test_children_append()
{
  auto child = TestHTMLElement<HTMLElementArgs>(HTMLElementArgs());
  child.HTML = "test1";

  htmlElement.appendChild(&child);
  TEST_ASSERT_EQUAL_STRING_MESSAGE(child.HTML.c_str(), htmlElement.getHTML().c_str(), "Should render children's HTML");

  auto child2 = TestHTMLElement<HTMLElementArgs>(HTMLElementArgs());
  child.HTML = "test2";

  htmlElement.appendChild(&child);
  htmlElement.appendChild(&child2);
  TEST_ASSERT_TRUE_MESSAGE(htmlElement.getHTML().indexOf(child.HTML) >= 0, "Should render child's HTML after adding 'child' and 'child2'");
  TEST_ASSERT_TRUE_MESSAGE(htmlElement.getHTML().indexOf(child2.HTML) >= 0, "Should render child2's HTML after adding 'child' and 'child2'");

  setUp();

  htmlElement.appendChild(&child);
  htmlElement.appendChild(&child);
  TEST_ASSERT_EQUAL_STRING_MESSAGE(child.HTML.c_str(), htmlElement.getHTML().c_str(), "Should not add already added child");
}

void test_children_remove()
{
  auto child = TestHTMLElement<HTMLElementArgs>(HTMLElementArgs());
  child.HTML = "test1";
  auto child2 = TestHTMLElement<HTMLElementArgs>(HTMLElementArgs());
  child.HTML = "test2";

  htmlElement.appendChild(&child);
  htmlElement.removeChild(&child);
  TEST_ASSERT_EQUAL_STRING_MESSAGE(child2.HTML.c_str(), htmlElement.getHTML().c_str(), "Should remove child and as a result - not render it's html");
}

void test_children_remove_all()
{
  auto child = TestHTMLElement<HTMLElementArgs>(HTMLElementArgs());
  child.HTML = "test1";
  auto child2 = TestHTMLElement<HTMLElementArgs>(HTMLElementArgs());
  child.HTML = "test2";

  htmlElement.appendChild(&child);
  htmlElement.appendChild(&child2);
  htmlElement.removeAllChildren();
  TEST_ASSERT_EQUAL_STRING_MESSAGE("", htmlElement.getHTML().c_str(), "Should render nothing after removing all children");
}

void test_emit_call()
{
  size_t assertions = 0;
  auto child1 = TestHTMLElement<HTMLElementArgs>(HTMLElementArgs());
  auto child2 = TestHTMLElement<HTMLElementArgs>(HTMLElementArgs());
  child1.onEmitHandler = [&assertions](String value) { assertions++; };
  child2.onEmitHandler = child1.onEmitHandler;

  htmlElement.appendChild(&child1);
  htmlElement.appendChild(&child2);
  htmlElement.emit(child1.getId());
  TEST_ASSERT_EQUAL_MESSAGE(1, assertions, "Emit handler should be called if id matches");
}

void test_emit_value()
{
  String payload = "abc";
  auto child1 = TestHTMLElement<HTMLElementArgs>(HTMLElementArgs());
  child1.onEmitHandler = [payload](String value) {
    TEST_ASSERT_EQUAL_STRING_MESSAGE(payload.c_str(), value.c_str(), "Should call 'onEmit' with value");
  };

  htmlElement.appendChild(&child1);
  htmlElement.emit(child1.getId(), payload);
}

void test_dispatch_call()
{
  htmlElement.dispatch("");
  TEST_ASSERT_TRUE_MESSAGE(true, "Should not produce any errors if dispatcher is unset");

  size_t assertions = 0;
  HTMLElementArgs args;
  auto testElement = TestHTMLElement<HTMLElementArgs>(args);

  args.dispatch.dispatcher = [&assertions](String handlerId, size_t elementId, String value) {
    assertions++;
  };
  args.dispatch.throttleTime = 0;

  htmlElement = HTMLElement<HTMLElementArgs>(args);
  htmlElement.dispatch("");
  TEST_ASSERT_EQUAL_MESSAGE(1, assertions, "Dispatcher function should be called");
}

void test_dispatch_args()
{
  String payload = "abc";
  HTMLElementArgs args;
  auto testElement = TestHTMLElement<HTMLElementArgs*>(&args);
  auto htmlElement = HTMLElement<HTMLElementArgs*>(&args);

  args.dispatch.dispatcher = [payload, &testElement, &htmlElement](String handlerId, size_t elementId, String value) {
    TEST_ASSERT_EQUAL_STRING_MESSAGE(payload.c_str(), value.c_str(), "Dispatcher should be called with value");
    TEST_ASSERT_EQUAL_STRING_MESSAGE(testElement.getHandlerId().c_str(), handlerId.c_str(), "Dispatcher should be called with default handlerId");
    TEST_ASSERT_EQUAL_MESSAGE(htmlElement.getId(), elementId, "Dispatcher should be called with exact element id");
  };
  args.dispatch.throttleTime = 0;

  htmlElement.dispatch(payload);
}

void test_dispatch_throttle()
{
  size_t assertions = 0;
  HTMLElementArgs args;
  auto testElement = TestHTMLElement<HTMLElementArgs>(args);

  args.dispatch.dispatcher = [&assertions](String handlerId, size_t elementId, String value) {
    assertions++;
  };
  args.dispatch.throttleTime = 100;

  htmlElement = HTMLElement<HTMLElementArgs>(args);
  htmlElement.dispatch("");
  htmlElement.dispatch("");
  delay(100);
  htmlElement.dispatch("");
  TEST_ASSERT_EQUAL_MESSAGE(1, assertions, "Dispatcher function should be called only 2 times");
}

void test_dispatch_custom_handler_id()
{
  String customHandlerId = "CustomHandlerId";
  size_t assertions = 0;

  HTMLElementArgs args;
  args.dispatch.dispatcher = [&assertions, customHandlerId](String handlerId, size_t elementId, String value) {
    assertions++;
    TEST_ASSERT_EQUAL_STRING_MESSAGE(customHandlerId.c_str(), handlerId.c_str(), "Dispatcher should be called with custom handlerId");
  };
  args.dispatch.throttleTime = 0;

  auto testElement = TestHTMLElement<HTMLElementArgs>(args);
  testElement.handlerId = customHandlerId;

  testElement.dispatch("");
  TEST_ASSERT_EQUAL_MESSAGE(1, assertions, "Dispatcher function should be called");
}

void test_width()
{
  htmlElement.setWidth(6, ScreenSize::small);
  TEST_ASSERT_FALSE_MESSAGE(htmlElement.classList.item(0).isEmpty(), "Class should be added");
  TEST_ASSERT_TRUE_MESSAGE(htmlElement.classList.item(1).isEmpty(), "Only one class should be added")

  TEST_ASSERT_EQUAL_MESSAGE(6, htmlElement.getWidth(ScreenSize::small), "Getter should return setted width");
  TEST_ASSERT_EQUAL_MESSAGE(0, htmlElement.getWidth(ScreenSize::medium), "Getter should return 0 if width is unset for provided screen size");

  htmlElement.setWidth(3, ScreenSize::medium);
  TEST_ASSERT_FALSE_MESSAGE(htmlElement.classList.item(0).isEmpty(), "Previous class should remain if screen size is different");
  TEST_ASSERT_FALSE_MESSAGE(htmlElement.classList.item(1).isEmpty(), "Second class should be added");
  TEST_ASSERT_EQUAL_MESSAGE(6, htmlElement.getWidth(ScreenSize::small), "Getter should return setted width");
  TEST_ASSERT_EQUAL_MESSAGE(3, htmlElement.getWidth(ScreenSize::medium), "Getter should return setted width");

  htmlElement.setWidth(10, ScreenSize::small);
  TEST_ASSERT_EQUAL_MESSAGE(-1, htmlElement.classList.value().indexOf("6"), "Should remove class with previous width for the same screen size");
}

void test_background_color()
{
  String classes;

  htmlElement.setBackgroundColor(Color::amber);
  classes = htmlElement.classList.value();
  TEST_ASSERT_GREATER_THAN_INT_MESSAGE(0, classes.length(), "Background color class should be set");
  TEST_ASSERT_EQUAL_MESSAGE(Color::amber, htmlElement.getBackgroundColor(), "Background color getter should return amber color");

  htmlElement.setBackgroundColor(Color::amber, ColorShade::accent4);
  htmlElement.setBackgroundColor(Color::defaultColor);
  classes = htmlElement.classList.value();
  TEST_ASSERT_EQUAL_MESSAGE(0, classes.length(), "Background color class should be removed");
  TEST_ASSERT_EQUAL_MESSAGE(Color::defaultColor, htmlElement.getBackgroundColor(), "Background color getter should return default color");

  htmlElement.setBackgroundColor(Color::deepOrange, ColorShade::darken3);
  TEST_ASSERT_TRUE_MESSAGE(
      htmlElement.classList.item(0).length() > 0 && htmlElement.classList.item(1).length() > 0,
      "Class list should contain two classes after applying background color with color shade");
  TEST_ASSERT_EQUAL_MESSAGE(Color::deepOrange, htmlElement.getBackgroundColor(), "Background color getter should return deep orange color");
  TEST_ASSERT_EQUAL_MESSAGE(ColorShade::darken3, htmlElement.getBackgroundColorShade(), "Background color shade getter should return darken3 color shade");

  htmlElement.setBackgroundColor(Color::amber, ColorShade::noShade);
  TEST_ASSERT_EQUAL_MESSAGE(0, htmlElement.classList.item(1).length(), "Background color shade class should be removed if noShade");
  TEST_ASSERT_EQUAL_MESSAGE(ColorShade::noShade, htmlElement.getBackgroundColorShade(), "Background color shade getter should return no shade");

  htmlElement.setBackgroundColor(Color::black, ColorShade::darken3);
  TEST_ASSERT_EQUAL_MESSAGE(0, htmlElement.classList.item(1).length(), "Shade should be ingored if invalid with specified color");
  TEST_ASSERT_EQUAL_MESSAGE(ColorShade::noShade, htmlElement.getBackgroundColorShade(), "Background color shade getter should return no shade");

  htmlElement.setBackgroundColor(Color::deepOrange, ColorShade::accent2);
  htmlElement.setBackgroundColor(Color::brown, ColorShade::accent2);
  TEST_ASSERT_EQUAL_MESSAGE(0, htmlElement.classList.item(1).length(), "Shade should be removed if invalid new color");
  TEST_ASSERT_EQUAL_MESSAGE(ColorShade::noShade, htmlElement.getBackgroundColorShade(), "Background color shade getter should return no shade");
}

void test_text_color()
{
  String classes;

  htmlElement.setTextColor(Color::amber);
  classes = htmlElement.classList.value();
  TEST_ASSERT_GREATER_THAN_INT_MESSAGE(0, classes.length(), "Text color class should be set");
  TEST_ASSERT_EQUAL_MESSAGE(Color::amber, htmlElement.getTextColor(), "Text color getter should return amber color");

  htmlElement.setTextColor(Color::amber, ColorShade::accent4);
  htmlElement.setTextColor(Color::defaultColor);
  classes = htmlElement.classList.value();
  TEST_ASSERT_EQUAL_MESSAGE(0, classes.length(), "Text color class should be removed");
  TEST_ASSERT_EQUAL_MESSAGE(Color::defaultColor, htmlElement.getTextColor(), "Text color getter should return default color");
  TEST_ASSERT_EQUAL_MESSAGE(ColorShade::noShade, htmlElement.getTextColorShade(), "Text color shade getter should return no shade");

  htmlElement.setTextColor(Color::deepOrange, ColorShade::darken3);
  TEST_ASSERT_TRUE_MESSAGE(
      htmlElement.classList.item(0).length() > 0 && htmlElement.classList.item(1).length() > 0,
      "Class list should contain two classes after applying text color with color shade");
  TEST_ASSERT_EQUAL_MESSAGE(ColorShade::darken3, htmlElement.getTextColorShade(), "Text color shade getter should return darken3");

  htmlElement.setTextColor(Color::amber, ColorShade::noShade);
  TEST_ASSERT_EQUAL_MESSAGE(0, htmlElement.classList.item(1).length(), "Text color shade class should be removed if noShade");
  TEST_ASSERT_EQUAL_MESSAGE(Color::amber, htmlElement.getTextColor(), "Text color getter should return amber");
  TEST_ASSERT_EQUAL_MESSAGE(ColorShade::noShade, htmlElement.getTextColorShade(), "Text color shade getter should return no shade");

  htmlElement.setTextColor(Color::black, ColorShade::darken3);
  TEST_ASSERT_EQUAL_MESSAGE(0, htmlElement.classList.item(1).length(), "Shade should be ingored if invalid with specified color");
  TEST_ASSERT_EQUAL_MESSAGE(Color::black, htmlElement.getTextColor(), "Text color getter should return black");
  TEST_ASSERT_EQUAL_MESSAGE(ColorShade::noShade, htmlElement.getTextColorShade(), "Text color shade getter should return no shade");

  htmlElement.setTextColor(Color::deepOrange, ColorShade::accent2);
  htmlElement.setTextColor(Color::brown, ColorShade::accent2);
  TEST_ASSERT_EQUAL_MESSAGE(0, htmlElement.classList.item(1).length(), "Shade should be removed if invalid new color");
  TEST_ASSERT_EQUAL_MESSAGE(Color::brown, htmlElement.getTextColor(), "Text color getter should return brown");
  TEST_ASSERT_EQUAL_MESSAGE(ColorShade::noShade, htmlElement.getTextColorShade(), "Text color shade getter should return no shade");
}