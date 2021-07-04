#ifndef _FIELD_H_
#define _FIELD_H_

#include <Arduino.h>

#include "../HTMLElement/HTMLElement.h"
#include "../IdGenerator.h"

class Field : public HTMLElement
{
  template <typename T>
  friend class Container;

private:
  String name;
  String defaultValue;
  String currentValue = "";

protected:
  virtual String getHTML();
  virtual bool emit(size_t id, String value);

public:
  Field(String name, String defaultValue);

  String getName();
  void setName(String name);

  String getDefaultValue();
  void setDefaultValue(String defaultValue);

  String getCurrentValue();
};

#endif //_FIELD_H_