#include <Arduino.h>
#include <MaterializeLayoutTypes.h>
#include <Modules/ChartistModule/Components/Chart/Chart.h>
#include <gtest/gtest.h>

class TestChart : public ::testing::Test {
  protected:
  Chart<HTMLElementArgs> chart = Chart(HTMLElementArgs());
};

TEST_F(TestChart, getHtml)
{
  ASSERT_GT(chart.getHTML().length(), 0) << "Should have html by default";
}

TEST_F(TestChart, renderClasses)
{
  ASSERT_GT(chart.classList.value().length(), 0) << "Should have classes by default";

  chart.classList.add("custom-class");
  ASSERT_GT(chart.getHTML().indexOf("custom-class"), 0) << "Should render its classes";
}

TEST_F(TestChart, aspectRatio)
{
  std::function<size_t(ClassList&)> getClassCount = [](ClassList& cl) -> size_t {
    String v = cl.value();
    v.trim();
    if (v.isEmpty())
      return 0;

    size_t pos = 0;
    size_t classCount = 1;

    while (v.indexOf(" ", pos) >= 0) {
      pos = v.indexOf(" ", pos) + 1;
      classCount++;
    }

    return classCount;
  };

  size_t classCount = getClassCount(chart.classList);

  chart.setAspectRatio(ChartAspectRatio::majorSecond);
  ASSERT_EQ(getClassCount(chart.classList), classCount) << "Class count should remain the same";
  ASSERT_EQ(chart.getAspectRatio(), ChartAspectRatio::majorSecond) << "Should return current aspect ratio";
}

TEST_F(TestChart, arraySize)
{
  chart.setArraySize(5);
  ASSERT_EQ(chart.getArraySize(), 5) << "Should return current array size";

  auto chartLine = chart.createLine();
  ASSERT_EQ(chartLine->getArraySize(), 5) << "Should pass current array size to the chartLines";

  chart.setArraySize(4);
  ASSERT_EQ(chartLine->getArraySize(), 4) << "Should change current array size for existing chartLines";

  chart.pushLegend("legend1");
  chart.pushLegend("legend2");
  chart.pushLegend("legend3");
  chart.pushLegend("legend4");
  chart.pushLegend("legend5");

  ASSERT_EQ(chart.collectChartData().indexOf("legend1"), -1) << "legend1 is out of range so it should be removed";
  ASSERT_GT(chart.collectChartData().indexOf("legend2"), 0) << "legend2 should exists";
  ASSERT_GT(chart.collectChartData().indexOf("legend5"), 0) << "legend5 should exists";

  chart.setArraySize(2);
  ASSERT_GT(chart.collectChartData().indexOf("legend5"), 0) << "legend5 should exists";
  ASSERT_GT(chart.collectChartData().indexOf("legend4"), 0) << "legend4 should exists";
  ASSERT_EQ(chart.collectChartData().indexOf("legend3"), -1) << "legend3 is out of range so it should be removed";
}

TEST_F(TestChart, chartLinesValues)
{
  auto chartLine = chart.createLine();

  chartLine->pushValue("chlVal1");
  chartLine->pushValue("chlVal2");

  ASSERT_GT(chart.collectChartData().indexOf("chlVal1"), 0) << "Should contains chartLine's values";
  ASSERT_GT(chart.collectChartData().indexOf("chlVal2"), 0) << "Should contains chartLine's values";
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  if (RUN_ALL_TESTS())
    ;

  return 0;
}
