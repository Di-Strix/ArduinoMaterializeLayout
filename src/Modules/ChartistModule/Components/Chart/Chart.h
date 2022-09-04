#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <list>

#include "HTMLElement/HTMLElement.h"

#include "ChartLine/ChartLine.h"

enum class ChartAspectRatio {
  square, // 1:1
  minorSecond, // 15:16
  majorSecond, // 8:9
  minorThird, // 5:6
  majorThird, // 4:5
  perfectFourth, // 3:4
  perfectFifth, // 2:3
  minorSixth, // 5:8
  goldenSection, // 1:1.618
  majorSixth, // 3:5
  minorSeventh, // 9:16
  majorSeventh, // 8:15
  octave, // 1:2
  majorTenth, // 2:5
  majorEleventh, // 3:8
  majorTwelfth, // 1:3
  doubleOctave // 1:4
};

template <typename T>
class Chart : public HTMLElement<T> {
  private:
  size_t arraySize = 24;
  std::list<ChartLine*> lines;
  std::list<String> legendNames;

  ChartAspectRatio aspectRatio = ChartAspectRatio::goldenSection;

  String getAspectRatioClass(ChartAspectRatio ar);

  protected:
  virtual String getHandlerId() { return F("ChartistHandler"); };

  public:
  Chart(T argCollection);
  ~Chart();

  String collectChartData();

  String getHTML();

  void setArraySize(size_t newSize);
  size_t getArraySize();

  void pushLegend(String legendName);

  void setAspectRatio(ChartAspectRatio ar);
  ChartAspectRatio getAspectRatio();

  ChartLine* createLine();
};

// ======================= IMPLEMENTATION =======================

template <typename T>
Chart<T>::Chart(T argCollection)
    : HTMLElement<T>(argCollection)
{
  this->classList.add(F("ct-chart"));
  this->classList.add(F("chart-overflow-visible"));
  this->setAspectRatio(this->aspectRatio);
}

template <typename T>
Chart<T>::~Chart()
{
  for (auto chl : this->lines) {
    delete chl;
  }
}

template <typename T>
void Chart<T>::setArraySize(size_t newSize)
{
  this->arraySize = newSize;
  for (auto chl : this->lines) {
    if (chl)
      chl->setArraySize(newSize);
  }

  while (this->legendNames.size() > this->arraySize) {
    this->legendNames.pop_front();
  }
}

template <typename T>
size_t Chart<T>::getArraySize()
{
  return this->arraySize;
}

template <typename T>
ChartLine* Chart<T>::createLine()
{
  auto chl = new ChartLine(this->arraySize, [this]() { this->dispatch(this->collectChartData()); });
  this->lines.push_back(chl);
  return chl;
}

template <typename T>
void Chart<T>::pushLegend(String legendName)
{
  if (this->legendNames.size() == this->arraySize) {
    this->legendNames.pop_front();
  }
  this->legendNames.push_back(legendName);

  this->dispatch(this->collectChartData());
}

template <typename T>
String Chart<T>::getHTML()
{
  String html = F("<div class=\"");
  html += this->classList.value();
  html += F("\" data-chart=\"true\" data-type=\"chart\" data-id=\"");
  html += (String)this->getId();
  html += F("\" ");
  html += F("style=\"");
  html += this->getInlineStyles();
  html += F("\"></div>");

  return html;
}

template <typename T>
void Chart<T>::setAspectRatio(ChartAspectRatio ar)
{
  this->classList.remove(this->getAspectRatioClass(this->aspectRatio));
  this->classList.add(this->getAspectRatioClass(ar));
  this->aspectRatio = ar;
}

template <typename T>
ChartAspectRatio Chart<T>::getAspectRatio()
{
  return this->aspectRatio;
}

template <typename T>
String Chart<T>::getAspectRatioClass(ChartAspectRatio ar)
{
  switch (ar) {
  case ChartAspectRatio::square:
    return F("square");

  case ChartAspectRatio::minorSecond:
    return F("minor-second");

  case ChartAspectRatio::majorSecond:
    return F("major-second");

  case ChartAspectRatio::minorThird:
    return F("minor-third");

  case ChartAspectRatio::majorThird:
    return F("major-third");

  case ChartAspectRatio::perfectFourth:
    return F("perfect-fourth");

  case ChartAspectRatio::perfectFifth:
    return F("perfect-fifth");

  case ChartAspectRatio::minorSixth:
    return F("minor-sixth");

  case ChartAspectRatio::goldenSection:
    return F("golden-section");

  case ChartAspectRatio::majorSixth:
    return F("major-sixth");

  case ChartAspectRatio::minorSeventh:
    return F("minor-seventh");

  case ChartAspectRatio::majorSeventh:
    return F("major-seventh");

  case ChartAspectRatio::octave:
    return F("octave");

  case ChartAspectRatio::majorTenth:
    return F("major-tenth");

  case ChartAspectRatio::majorEleventh:
    return F("major-eleventh");

  case ChartAspectRatio::majorTwelfth:
    return F("major-twelfth");

  case ChartAspectRatio::doubleOctave:
    return F("double-octave");

  default:
    return "";
  }
}

template <typename T>
String Chart<T>::collectChartData()
{
  size_t size = JSON_ARRAY_SIZE(this->arraySize * (this->lines.size() + 1));
  for (auto chartLine : this->lines) {
    auto lineValues = chartLine->getValues();
    for (auto v : lineValues) {
      size += JSON_STRING_SIZE(v.length());
    }
  }
  for (auto l : this->legendNames) {
    size += JSON_STRING_SIZE(l.length());
  }

  DynamicJsonDocument doc(size + 128);

  if (doc.capacity() == 0) {
    return "";
  }

  auto labels = doc.createNestedArray(F("labels"));
  auto series = doc.createNestedArray(F("series"));

  for (auto chartLine : this->lines) {
    auto arr = series.createNestedArray();

    auto lineValues = chartLine->getValues();
    for (auto v : lineValues) {
      arr.add(v);
    }
  }

  for (auto l : this->legendNames) {
    labels.add(l);
  }

  doc.shrinkToFit();

  String res;
  serializeJson(doc, res);

  return res;
}
