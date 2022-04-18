#include <Arduino.h>
#include <ClassList/ClassList.h>
#include <unity.h>

ClassList classList;

void setUp()
{
  classList = ClassList();
}

void test_initial_state()
{
  TEST_ASSERT_EQUAL_STRING("", classList.value().c_str());
}

void test_add()
{
  classList.add("test");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("test", classList.value().c_str(), "Should add 'test' to the class list");
  classList.add("test2");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("test test2", classList.value().c_str(), "Both 'test2' and 'test' should be in class list");
}

void test_remove()
{
  classList.add("test");
  classList.remove("test");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("", classList.value().c_str(), "'test' should be removed from class list");

  classList.add("test");
  classList.add("test2");
  classList.remove("test");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("test2", classList.value().c_str(), "Only 'test2' should be in class list after 'test' removal");
}

void test_replace()
{
  classList.add("test");
  classList.replace("test", "test2");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("test2", classList.value().c_str(), "'test' should be replaced with 'test2'");

  setUp();

  classList.add("test");
  classList.replace("test3", "test2");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("test", classList.value().c_str(), "Should not throw an error or add an element if trying to replace a nonexistent element");

  setUp();

  classList.add("test1");
  classList.add("test2");
  classList.add("test3");
  classList.replace("test1", "test4");
  String result = classList.value();
  TEST_ASSERT_TRUE_MESSAGE(result.indexOf("test1") == -1, "'test1' should not be in class list after replacement");
  TEST_ASSERT_TRUE_MESSAGE(result.indexOf("test2") >= 0 && result.indexOf("test3") >= 0, "'test2' and 'test3' should remains in class list after 'test' replacement");
  TEST_ASSERT_TRUE_MESSAGE(result.indexOf("test4") >= 0, "'test4' should appear in class list after 'test' replacement")
}

void test_toggle()
{
  classList.toggle("test1");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("test1", classList.value().c_str(), "'test1' should appear in class list after toggle");

  classList.toggle("test1");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("", classList.value().c_str(), "'test1' should disappear in class list after second toggle");

  classList.add("test1");
  classList.toggle("test2");
  String result = classList.value();
  TEST_ASSERT_TRUE_MESSAGE(result.indexOf("test1") >= 0, "'test1' should remain in class list after toggle");
  TEST_ASSERT_TRUE_MESSAGE(result.indexOf("test2") >= 0, "'test2' should appear in class list after toggle");

  classList.toggle("test2");
  result = classList.value();
  TEST_ASSERT_TRUE_MESSAGE(result.indexOf("test1") >= 0, "'test1' should remain in class list after toggle");
  TEST_ASSERT_TRUE_MESSAGE(result.indexOf("test2") == -1, "'test2' should disappear in class list after toggle");
}

void test_contains()
{
  classList.add("test1");
  TEST_ASSERT_TRUE_MESSAGE(classList.contains("test1"), "Should return true since class list contains 'test1'");
  TEST_ASSERT_FALSE_MESSAGE(classList.contains("test2"), "Should return false since class list is not contains 'test2'");
}

void test_item()
{
  TEST_ASSERT_EQUAL_STRING_MESSAGE("", classList.item(0).c_str(), "Should return empty string since class list is empty");

  classList.add("test1");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("test1", classList.item(0).c_str(), "Should return 'test1' since 'test1' is only class in class list");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("", classList.item(1).c_str(), "Should return empty string since class list only contains one element");

  classList.add("test2");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("test1", classList.item(0).c_str(), "Should return 'test1' since 'test1' is the first element in class list");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("test2", classList.item(1).c_str(), "Should return 'test2' since 'test2' is the second element in class list");
}

void setup()
{
  UNITY_BEGIN();

  RUN_TEST(test_initial_state);
  RUN_TEST(test_add);
  RUN_TEST(test_remove);
  RUN_TEST(test_replace);
  RUN_TEST(test_toggle);
  RUN_TEST(test_contains);
  RUN_TEST(test_item);

  UNITY_END();
}

void loop() { }