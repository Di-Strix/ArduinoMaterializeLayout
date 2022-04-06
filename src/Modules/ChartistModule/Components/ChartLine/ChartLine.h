#pragma once

#include <Arduino.h>
#include <list>

class ChartLine {
  private:
  std::list<String> array;
  size_t arraySize;

  public:
  ChartLine(size_t arraySize);

  void setArraySize(size_t newSize);
  size_t getArraySize();

  void pushValue(String value);

  void clear();

  std::list<String> getValues();
};
