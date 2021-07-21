#ifndef _HTML_ELEMENT_H_
#define _HTML_ELEMENT_H_

#include <Arduino.h>

#include "../IdGenerator.h"
#include "../ClassList/ClassList.h"

class HTMLElement
{
private:
  uint8_t width = 0;
  size_t id = 0;

protected:
  HTMLElement();

  /** 
   * @brief Intended to return component's filled HTML template.
   * 
   * Placeholder for further implementation.
   * 
   * @return String 
   */
  virtual String getHTML();

  /** 
   * @brief Emits an event.
   * 
   * Placeholder for further implementation.
   * 
   * @param id event id
   * @param value additional data that component may require
   * 
   * @return bool 
   */
  virtual bool emit(size_t id, String value = "") { return false; };

  /** 
   * @brief Returns an id of the element. The id is used to emit an event on certain component
   * @return size_t 
   */
  size_t getId();

  /** 
   * @brief Gets css class according to set width.
   * Uses 12-column grid layout. See "Introduction" section - https://materializecss.com/grid.html
   * @return String 
   */
  String getWidthClass();

public:
  ClassList classList;
  /** 
   * @brief Sets width of the component in HTML if it's possible.
   * Component must be in column to make this thing work.
   * Uses 12-column grid layout. See "Introduction" section - https://materializecss.com/grid.html
   * 
   * @param width width of the component. range: [0-12]. If 0 - component takes as much space as needed
   * 
   * @return void 
   */
  void setWidth(uint8_t width);

  /** 
   * @brief Gets width of the component in HTML. 
   * Uses 12-column grid layout. See "Introduction" section - https://materializecss.com/grid.html
   * @return uint8_t 
   */
  uint8_t getWidth();
};

#endif //_HTML_ELEMENT_H_