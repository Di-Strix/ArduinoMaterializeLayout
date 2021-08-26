#ifndef _MATERIALIZE_LAYOUT_CHARTIST_LINE_H_
#define _MATERIALIZE_LAYOUT_CHARTIST_LINE_H_

#include <Arduino.h>
#include <list>

class ChartLine
{
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

#endif //_MATERIALIZE_LAYOUT_CHARTIST_LINE_H_