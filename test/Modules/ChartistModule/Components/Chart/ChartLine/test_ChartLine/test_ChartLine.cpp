#include <Arduino.h>
#include <MaterializeLayoutTypes.h>
#include <Modules/ChartistModule/Components/Chart/ChartLine/ChartLine.h>
#include <gmock/gmock.h>

class TestChartLine : public ::testing::Test {
  protected:
  ChartLine chartLine = ChartLine(24, []() {});
};

TEST_F(TestChartLine, arraySize)
{
  chartLine.setArraySize(5);

  chartLine.pushValue("v1");
  chartLine.pushValue("v2");
  chartLine.pushValue("v3");
  chartLine.pushValue("v4");
  chartLine.pushValue("v5");
  chartLine.pushValue("v6");

  ASSERT_EQ(chartLine.getValues().size(), 5) << "Should contain no more values than specified";
  ASSERT_EQ(chartLine.getArraySize(), 5) << "Should return current array size";
  ASSERT_FALSE(chartLine.getValues().begin()->equals("v1")) << "Should remove the oldest values when overflow";
  ASSERT_TRUE(chartLine.getValues().begin()->equals("v2")) << "Should keep newer values if they are fit";

  chartLine.setArraySize(2);
  ASSERT_EQ(chartLine.getValues().size(), 2) << "Should trim array when smaller array size is set";
  ASSERT_FALSE(chartLine.getValues().begin()->equals("v4")) << "Should remove the oldest values when overflow";
  ASSERT_TRUE(chartLine.getValues().begin()->equals("v5")) << "Should keep newer values if they are fit";
}

TEST_F(TestChartLine, clear)
{
  chartLine.pushValue("v1");
  chartLine.pushValue("v2");
  chartLine.pushValue("v3");
  chartLine.pushValue("v4");
  chartLine.pushValue("v5");
  chartLine.pushValue("v6");

  chartLine.clear();

  ASSERT_EQ(chartLine.getValues().size(), 0) << "Should be empty after clear";
}

TEST_F(TestChartLine, dispatch)
{
  size_t assertions = 0;

  chartLine = ChartLine(24, [&assertions]() { assertions++; });

  chartLine.pushValue("v");

  ASSERT_EQ(assertions, 1) << "Dispatcher should be called after value push";
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  if (RUN_ALL_TESTS())
    ;

  return 0;
}
