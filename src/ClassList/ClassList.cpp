#include "ClassList.h"

void ClassList::add(String className)
{
  className.trim();

  if (className.isEmpty())
    return;

  if (!this->contains(className))
    this->classList.push_back(className);
}

bool ClassList::contains(String className)
{
  className.trim();

  if (className.isEmpty())
    return false;

  for (String c : this->classList)
  {
    if (c.equals(className))
      return true;
  }

  return false;
}

String ClassList::item(size_t index)
{
  if (index + 1 > this->classList.capacity())
    return "";

  return this->classList[index];
}

void ClassList::remove(String className)
{
  className.trim();

  if (className.isEmpty())
    return;

  for (auto i = this->classList.cbegin(); i < this->classList.cend(); i++)
  {
    if (*i == className)
      this->classList.erase(i);
  }
}

void ClassList::replace(String className, String newClassName)
{
  className.trim();

  if (className.isEmpty())
    return;

  for (auto i = this->classList.begin(); i < this->classList.end(); i++)
  {
    if (*i == className)
      *i = newClassName;
  }
}

void ClassList::toggle(String className)
{
  className.trim();

  if (className.isEmpty())
    return;

  if (this->contains(className))
    this->remove(className);
  else
    this->add(className);
}

String ClassList::value()
{
  String result;
  for (String c : this->classList)
  {
    result += c + " ";
  }

  result.trim();

  return result;
}
