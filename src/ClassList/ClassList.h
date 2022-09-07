#pragma once

#include <Arduino.h>
#include <algorithm>
#include <list>

#include "EventEmitter/EventEmitter.h"

enum class ClassChangeType {
  add,
  remove,
  replace
};

class ClassList {
  private:
  std::list<String> classList;

  public:
  /**
   * @brief Event that is fired on class add or remove
   *
   * @param String class name
   * @param String new class name if replaced
   * @param ClassChangeType type of the change
   */
  EventEmitter<String, String, ClassChangeType> onChange;

  /**
   * @brief adds a class to the classList
   *
   * @param className
   * @param emit whether emit change event or not
   */
  void add(String className, bool emit = true);

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
   * @param emit whether emit change event or not
   */
  void remove(String className, bool emit = true);

  /**
   * @brief replaces class name with the provided one
   *
   * @param className
   * @param newClassName
   * @param emit whether emit change event or not
   */
  void replace(String className, String newClassName, bool emit = true);

  /**
   * @brief Removes the class name if it already in the array, and adds it if not
   *
   * @param className
   * @param emit whether emit change event or not
   */
  void toggle(String className, bool emit = true);

  /**
   * @brief Concatenates all classes that is in array to one string. Classes separated width ' '
   *
   * @return String
   */
  String value();
};
