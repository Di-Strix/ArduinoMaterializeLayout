#include "ChartLine.h"

ChartLine::ChartLine(size_t arraySize, std::function<void()> dispatcher)
{
  this->arraySize = arraySize;
  this->dispatcher = dispatcher;
}

void ChartLine::setArraySize(size_t newSize)
{
  this->arraySize = newSize;

  while (this->array.size() > this->arraySize) {
    this->array.pop_front();
  }
}

size_t ChartLine::getArraySize()
{
  return this->arraySize;
}

void ChartLine::clear()
{
  this->array.clear();

  this->dispatcher();
}

void ChartLine::pushValue(String value)
{
  if (this->array.size() == this->arraySize) {
    this->array.pop_front();
  }
  this->array.push_back(value);

  this->dispatcher();
}

std::list<String> ChartLine::getValues()
{
  return this->array;
}
