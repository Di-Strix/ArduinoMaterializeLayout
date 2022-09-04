#include <Arduino.h>
#include <ClassList/ClassList.h>
#include <gtest/gtest.h>

class TestClassList : public ::testing::Test {
  protected:
  ClassList classList;
};

TEST_F(TestClassList, initialState)
{
  ASSERT_STREQ(classList.value().c_str(), "") << "Should be empty by default";
}

TEST_F(TestClassList, add)
{
  classList.add("test");
  ASSERT_STREQ(classList.value().c_str(), "test") << "Should add 'test' to the class list";
  classList.add("test2");
  ASSERT_STREQ(classList.value().c_str(), "test test2") << "Both 'test2' and 'test' should be in class list";
}

TEST_F(TestClassList, remove)
{
  classList.add("test");
  classList.remove("test");
  ASSERT_STREQ(classList.value().c_str(), "") << "'test' should be removed from class list";
  classList.add("test");
  classList.add("test2");
  classList.remove("test");
  ASSERT_STREQ(classList.value().c_str(), "test2") << "Only 'test2' should be in class list after 'test' removal";
}

TEST_F(TestClassList, replace)
{
  classList.add("test");
  classList.replace("test", "test2");
  ASSERT_STREQ(classList.value().c_str(), "test2") << "'test' should be replaced with 'test2'";
}

TEST_F(TestClassList, replaceNonExistent)
{
  classList.add("test");
  classList.replace("test3", "test2");
  ASSERT_STREQ(classList.value().c_str(), "test") << "Should not throw an error or add an element if trying to replace a nonexistent element";
}

TEST_F(TestClassList, replaceWhenMultipleItemsAreInTheList)
{
  classList.add("test1");
  classList.add("test2");
  classList.add("test3");
  classList.replace("test1", "test4");
  String result = classList.value();
  ASSERT_TRUE(result.indexOf("test1") == -1) << "'test1' should not be in class list after replacement";
  ASSERT_TRUE(result.indexOf("test2") >= 0 && result.indexOf("test3") >= 0) << "'test2' and 'test3' should remains in class list after 'test' replacement";
  ASSERT_TRUE(result.indexOf("test4") >= 0) << "'test4' should appear in class list after 'test' replacement";
}

TEST_F(TestClassList, toggle)
{
  classList.toggle("test1");
  ASSERT_STREQ(classList.value().c_str(), "test1") << "'test1' should appear in class list after toggle";

  classList.toggle("test1");
  ASSERT_STREQ(classList.value().c_str(), "") << "'test1' should disappear in class list after second toggle";

  classList.add("test1");
  classList.toggle("test2");
  String result = classList.value();
  ASSERT_TRUE(result.indexOf("test1") >= 0) << "'test1' should remain in class list after toggle";
  ASSERT_TRUE(result.indexOf("test2") >= 0) << "'test2' should appear in class list after toggle";

  classList.toggle("test2");
  result = classList.value();
  ASSERT_TRUE(result.indexOf("test1") >= 0) << "'test1' should remain in class list after toggle";
  ASSERT_TRUE(result.indexOf("test2") == -1) << "'test2' should disappear in class list after toggle";
}

TEST_F(TestClassList, contains)
{
  classList.add("test1");
  ASSERT_TRUE(classList.contains("test1")) << "Should return true since class list contains 'test1'";
  ASSERT_FALSE(classList.contains("test2")) << "Should return false since class list is not contains 'test2'";
}

TEST_F(TestClassList, item)
{
  ASSERT_STREQ(classList.item(0).c_str(), "") << "Should return empty string since class list is empty";

  classList.add("test1");
  ASSERT_STREQ(classList.item(0).c_str(), "test1") << "Should return 'test1' since 'test1' is only class in class list";
  ASSERT_STREQ(classList.item(1).c_str(), "") << "Should return empty string since class list only contains one element";

  classList.add("test2");
  ASSERT_STREQ(classList.item(0).c_str(), "test1") << "Should return 'test1' since 'test1' is the first element in class list";
  ASSERT_STREQ(classList.item(1).c_str(), "test2") << "Should return 'test2' since 'test2' is the second element in class list";
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  if (RUN_ALL_TESTS())
    ;

  return 0;
}
