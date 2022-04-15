#include <Arduino.h>
#include <MaterializeLayoutTypes.h>
#include <Modules/ChartistModule/Components/ChartLine/ChartLine.h>
#include <unity.h>

auto chartLine = ChartLine(24, []() {});

void setUp()
{
  chartLine = ChartLine(24, []() {});
}

void test_array_size();
void test_clear();
void test_dispatch();

void setup()
{
  UNITY_BEGIN();

  RUN_TEST(test_array_size);
  RUN_TEST(test_clear);
  RUN_TEST(test_dispatch);

  UNITY_END();
}

void loop() { }

void test_array_size()
{
  chartLine.setArraySize(5);

  chartLine.pushValue("v1");
  chartLine.pushValue("v2");
  chartLine.pushValue("v3");
  chartLine.pushValue("v4");
  chartLine.pushValue("v5");
  chartLine.pushValue("v6");

  TEST_ASSERT_EQUAL_MESSAGE(5, chartLine.getValues().size(), "Should contain no more values than specified");
  TEST_ASSERT_EQUAL_MESSAGE(5, chartLine.getArraySize(), "Should return current array size");
  TEST_ASSERT_FALSE_MESSAGE(chartLine.getValues().begin()->equals("v1"), "Should remove the oldest values when overflow");
  TEST_ASSERT_TRUE_MESSAGE(chartLine.getValues().begin()->equals("v2"), "Should keep newer values if they are fit");

  chartLine.setArraySize(2);
  TEST_ASSERT_EQUAL_MESSAGE(2, chartLine.getValues().size(), "Should trim array when smaller array size is set");
  TEST_ASSERT_FALSE_MESSAGE(chartLine.getValues().begin()->equals("v4"), "Should remove the oldest values when overflow");
  TEST_ASSERT_TRUE_MESSAGE(chartLine.getValues().begin()->equals("v5"), "Should keep newer values if they are fit");
}

void test_clear()
{
  chartLine.pushValue("v1");
  chartLine.pushValue("v2");
  chartLine.pushValue("v3");
  chartLine.pushValue("v4");
  chartLine.pushValue("v5");
  chartLine.pushValue("v6");

  chartLine.clear();

  TEST_ASSERT_EQUAL_MESSAGE(0, chartLine.getValues().size(), "Should be empty after clear");
}

void test_dispatch()
{
  size_t assertions = 0;

  chartLine = ChartLine(24, [&assertions]() { assertions++; });

  chartLine.pushValue("v");
  
  TEST_ASSERT_EQUAL_MESSAGE(1, assertions, "Dispatcher should be called after value push");
}