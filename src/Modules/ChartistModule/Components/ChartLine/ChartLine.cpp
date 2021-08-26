#include "ChartLine.h"

ChartLine::ChartLine(size_t arraySize)
{
  this->arraySize = arraySize;
}

void ChartLine::setArraySize(size_t newSize)
{
  this->arraySize = newSize;
}

size_t ChartLine::getArraySize()
{
  return this->arraySize;
}

void ChartLine::clear()
{
  this->array.clear();
}

void ChartLine::pushValue(String value)
{
  if (this->array.size() == this->arraySize)
  {
    this->array.pop_front();
  }
  this->array.push_back(value);
}

std::list<String> ChartLine::getValues()
{
  return this->array;
}
