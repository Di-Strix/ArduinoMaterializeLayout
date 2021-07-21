#ifndef _HTML_ELEMENT_H_
#define _HTML_ELEMENT_H_

#include <Arduino.h>

#include "../IdGenerator.h"
#include "../ClassList/ClassList.h"

class HTMLElement
{
private:
  size_t id = 0;

  String widths[4];

  String getWidthClassPrefix(ScreenSize screenSize);

protected:
  HTMLElement();

  /** 
   * @brief Intended to return a filled HTML template of the component.
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

public:
  ClassList classList;
  /** 
   * @brief Sets width of the component in HTML if it's possible.
   * Doesn't work for row layout
   * Uses 12-column grid layout. See "Introduction" section - https://materializecss.com/grid.html
   * 
   * @param width the width of the component. range: [0-12]. If 0, the component takes up as much space as needed
   * @param screenSize width that applies to the given screen size and bigger, if no values for bigger screen is set. See "Creating Responsive Layouts" section - https://materializecss.com/grid.html
   * 
   * @return void 
   */
  void setWidth(uint8_t width, ScreenSize screenSize = ScreenSize::small);

  /** 
   * @brief Gets the width of the component in HTML for the provided screen size. 
   * Uses 12-column grid layout. See "Introduction" section - https://materializecss.com/grid.html
   * @param screenSize width that is currently set for the provided screen size. See "Creating Responsive Layouts" section - https://materializecss.com/grid.html
   * @return uint8_t 
   */
  uint8_t getWidth(ScreenSize screenSize = ScreenSize::small);


};

#endif //_HTML_ELEMENT_H_