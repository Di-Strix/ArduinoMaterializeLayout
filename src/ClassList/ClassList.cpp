#include "ClassList.h"

void ClassList::add(String className, bool emit)
{
  className.trim();

  if (className.isEmpty())
    return;

  if (!this->contains(className)) {
    this->classList.push_back(className);
    if (emit)
      this->onChange.emit(className, "", ClassChangeType::add);
  }
}

bool ClassList::contains(String className)
{
  className.trim();

  if (className.isEmpty())
    return false;

  for (String c : this->classList) {
    if (c.equals(className))
      return true;
  }

  return false;
}

String ClassList::item(size_t index)
{
  if (index + 1 > this->classList.size())
    return "";

  auto ref = this->classList.cbegin();
  for (size_t i = 0; i < index; i++)
    ref++;

  return *ref;
}

void ClassList::remove(String className, bool emit)
{
  className.trim();

  if (className.isEmpty())
    return;

  auto it = std::find(this->classList.cbegin(), this->classList.cend(), className);
  if (it != this->classList.cend()) {
    this->classList.erase(it);
    if (emit)
      this->onChange.emit(className, "", ClassChangeType::remove);
  }
}

void ClassList::replace(String className, String newClassName, bool emit)
{
  className.trim();

  if (className.isEmpty())
    return;

  auto it = std::find(this->classList.begin(), this->classList.end(), className);
  if (*it == className) {
    *it = newClassName;
    if (emit)
      this->onChange.emit(className, newClassName, ClassChangeType::replace);
  }
}

void ClassList::toggle(String className, bool emit)
{
  className.trim();

  if (className.isEmpty())
    return;

  if (this->contains(className))
    this->remove(className, emit);
  else
    this->add(className, emit);
}

String ClassList::value()
{
  String result;
  for (String c : this->classList) {
    result += c + " ";
  }

  result.trim();

  return result;
}
