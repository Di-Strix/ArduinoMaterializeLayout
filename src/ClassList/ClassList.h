#ifndef _MATERIALIZE_LAYOUT_CLASS_LIST_H_
#define _MATERIALIZE_LAYOUT_CLASS_LIST_H_

#include <Arduino.h>
#include <algorithm>
#include <list>

class ClassList {
  private:
  std::list<String> classList;

  public:
  /**
   * @brief adds a class to the classList
   * 
   * @param className 
   */
  void add(String className);

  /**
   * @brief Checks if provided class name if in the classList
   * 
   * @param className 
   * @return true if classList includes provided class name
   * @return false if classList doesn't includes provided class name
   */
  bool contains(String className);

  /**
   * @brief Returns the class name that is in the classList array under provided index
   * 
   * @param index 
   * @return String 
   */
  String item(size_t index);

  /**
   * @brief Removes provided class name from the classList
   * 
   * @param className 
   */
  void remove(String className);

  /**
   * @brief replaces class name with the provided one
   * 
   * @param className 
   * @param newClassName 
   */
  void replace(String className, String newClassName);

  /**
   * @brief Removes the class name if it already in the array, and adds it if not
   * 
   * @param className 
   */
  void toggle(String className);

  /**
   * @brief Concatenates all classes that is in array to one string. Classes separated width ' '
   * 
   * @return String 
   */
  String value();
};

#endif //_MATERIALIZE_LAYOUT_CLASS_LIST_H_