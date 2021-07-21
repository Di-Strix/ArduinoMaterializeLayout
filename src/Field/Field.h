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
  /**
   * @brief Constructs a new Field object
   * 
   * @param name the name of the field
   * @param defaultValue default value of the field
   */
  Field(String name, String defaultValue);

  /**
   * @brief Gets the name of the field
   * 
   * @return String 
   */
  String getName();

  /**
   * @brief Sets the name of the field
   * 
   * @param name the new name of the field
   * 
   * @return void
   */
  void setName(String name);

  /**
   * @brief Gets the default value of the field
   * 
   * @return String 
   */
  String getDefaultValue();

  /**
   * @brief Sets the default value of the field
   * 
   * @param defaultValue default value of the field
   * 
   * @return void
   */
  void setDefaultValue(String defaultValue);

  /**
   * @brief Gets the text that is currently written in the field
   * 
   * @return String 
   */
  String getCurrentValue();
};

#endif //_FIELD_H_