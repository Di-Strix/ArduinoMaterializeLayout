#include <Arduino.h>
#include <MaterializeLayoutTypes.h>
#include <Modules/ChartistModule/Components/Chart.h>
#include <unity.h>

auto chart = Chart<HTMLElementArgs>(HTMLElementArgs());

void setUp()
{
  chart = Chart<HTMLElementArgs>(HTMLElementArgs());
}

void test_get_html();
void test_render_classes();
void test_aspect_ratio();
void test_array_size();
void test_render_chart_lines_values();

void setup()
{
  UNITY_BEGIN();

  RUN_TEST(test_get_html);
  RUN_TEST(test_render_classes);
  RUN_TEST(test_aspect_ratio);
  RUN_TEST(test_array_size);
  RUN_TEST(test_render_chart_lines_values);

  UNITY_END();
}

void loop() { }

void test_get_html()
{
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, chart.getHTML().length(), "Should have html by default");
}

void test_render_classes()
{
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, chart.classList.value().length(), "Should have classes by default");

  chart.classList.add("custom-class");
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, chart.getHTML().indexOf("custom-class"), "Should render its classes");
}

void test_aspect_ratio()
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
  TEST_ASSERT_EQUAL_MESSAGE(classCount, getClassCount(chart.classList), "Class count should remain the same");
  TEST_ASSERT_EQUAL_MESSAGE(ChartAspectRatio::majorSecond, chart.getAspectRatio(), "Should return current aspect ratio");
}

void test_array_size()
{
  chart.setArraySize(5);
  TEST_ASSERT_EQUAL_MESSAGE(5, chart.getArraySize(), "Should return current array size");

  auto chartLine = chart.createLine();
  TEST_ASSERT_EQUAL_MESSAGE(5, chartLine->getArraySize(), "Should pass current array size to the chartLines");

  chart.setArraySize(4);
  TEST_ASSERT_EQUAL_MESSAGE(4, chartLine->getArraySize(), "Should change current array size for existing chartLines");

  chart.pushLegend("legend1");
  chart.pushLegend("legend2");
  chart.pushLegend("legend3");
  chart.pushLegend("legend4");
  chart.pushLegend("legend5");

  TEST_ASSERT_EQUAL_MESSAGE(-1, chart.collectChartData().indexOf("legend1"), "legend1 is out of range so it should be removed");
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, chart.collectChartData().indexOf("legend2"), "legend2 should exists");
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, chart.collectChartData().indexOf("legend5"), "legend5 should exists");

  chart.setArraySize(2);
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, chart.collectChartData().indexOf("legend5"), "legend5 should exists");
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, chart.collectChartData().indexOf("legend4"), "legend4 should exists");
  TEST_ASSERT_EQUAL_MESSAGE(-1, chart.collectChartData().indexOf("legend3"), "legend3 is out of range so it should be removed");
}

void test_render_chart_lines_values()
{
  auto chartLine = chart.createLine();

  chartLine->pushValue("chlVal1");
  chartLine->pushValue("chlVal2");

  TEST_ASSERT_GREATER_THAN_MESSAGE(0, chart.collectChartData().indexOf("chlVal1"), "Should contains chartLine's values");
  TEST_ASSERT_GREATER_THAN_MESSAGE(0, chart.collectChartData().indexOf("chlVal2"), "Should contains chartLine's values");
}