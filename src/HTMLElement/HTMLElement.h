#ifndef _HTML_ELEMENT_H_
#define _HTML_ELEMENT_H_

#include <Arduino.h>

#include "../IdGenerator.h"
#include "../ClassList/ClassList.h"

enum class Color
{
  defaultColor,
  red,
  pink,
  purple,
  deepPurple,
  indigo,
  blue,
  lightBlue,
  cyan,
  teal,
  green,
  lightGreen,
  lime,
  yellow,
  amber,
  orange,
  deepOrange,
  brown,
  grey,
  blueGrey,
  black,
  white,
  transparent
};

enum class ColorShade
{
  lighten5,
  lighten4,
  lighten3,
  lighten2,
  lighten1,
  noShade,
  darken1,
  darken2,
  darken3,
  darken4,
  accent1,
  accent2,
  accent3,
  accent4
};

enum class ScreenSize
{
  small,
  medium,
  large,
  extraLarge
};

class HTMLElement
{
private:
  size_t id = 0;

  String widths[4];

  Color backgroundColor = Color::defaultColor;
  ColorShade backgroundColorShade = ColorShade::noShade;

  Color textColor = Color::defaultColor;
  ColorShade textColorShade = ColorShade::noShade;

  String getWidthClassPrefix(ScreenSize screenSize);
  String colorToString(Color c);
  String colorShadeToString(ColorShade cs);
  String getBackgroundColorClass();
  String getTextColorClass();
  bool colorShadeIsValid(Color c, ColorShade cs);

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

  /**
   * @brief Sets the background color of the component. See "Color Palette" section - https://materializecss.com/color.html
   * 
   * @param color 
   * @param colorShade 
   * @return void
   */
  void setBackgroundColor(Color color, ColorShade colorShade = ColorShade::noShade);

  /**
   * @brief Sets the text color of the component. See "Color Palette" section - https://materializecss.com/color.html
   * 
   * @param color 
   * @param colorShade 
   */
  void setTextColor(Color color, ColorShade colorShade = ColorShade::noShade);

  /**
   * @brief Gets current background color
   * 
   * @return Color 
   */
  Color getBackgroundColor();

  /**
   * @brief Gets current text color
   * 
   * @return Color 
   */
  Color getTextColor();

  /**
   * @brief Gets current background color shade
   * 
   * @return ColorShade 
   */
  ColorShade getBackgroundColorShade();

  /**
   * @brief Gets current text color shade
   * 
   * @return ColorShade 
   */
  ColorShade getTextColorShade();
};

#endif //_HTML_ELEMENT_H_