#pragma once

#include <Arduino.h>

#include "ClassList/ClassList.h"
#include "IdGenerator.h"

#include "HTMLElementHelperFunctions.h"
#include "HTMLElemetTypes.h"

template <typename T>
class HTMLElement {
  private:
  T argCollection;

  size_t id = 0;

  String widths[4];

  Color backgroundColor = Color::defaultColor;
  ColorShade backgroundColorShade = ColorShade::noShade;

  Color textColor = Color::defaultColor;
  ColorShade textColorShade = ColorShade::noShade;

  String getBackgroundColorClass();
  String getTextColorClass();

  protected:
  virtual String getHandlerId() { return F("MaterializeCssHandler"); };

  std::list<HTMLElement*> children;

  virtual void onEmit(String value) {};

  public:
  ClassList classList;

  HTMLElement(T argCollection);
  ~HTMLElement() = default;

  /**
   * @brief Returns an id of the element. The id is used to emit an event on certain component
   * @return size_t
   */
  size_t getId();

  void appendChild(HTMLElement* child);

  void removeChild(HTMLElement* child);

  std::list<HTMLElement*> removeAllChildren();

  /**
   * @brief Intended to return a filled HTML template of the component.
   *
   * Placeholder for further implementation.
   *
   * @return String
   */
  virtual String getHTML();

  /**
   * @brief Emits an event on the current node.
   *
   * @param id event id
   * @param value additional data that component may require
   *
   * @return bool
   */
  bool emit(size_t id, String value = "");

  /**
   * @brief Dispatches event to the page
   *
   * @param value
   */
  void dispatch(String value);

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

// ======================= IMPLEMENTATION =======================

template <typename T>
HTMLElement<T>::HTMLElement(T argCollection)
{
  this->argCollection = argCollection;
  this->id = IdGenerator::Instance().getId();
}

template <typename T>
bool HTMLElement<T>::emit(size_t id, String value)
{
  if (this->getId() == id) {
    this->onEmit(value);
    return true;
  }

  bool found = false;

  for (auto el : this->children) {
    found = el->emit(id, value);

    if (found)
      break;
  }

  return found;
}

template <typename T>
void HTMLElement<T>::dispatch(String value)
{
  static uint32_t lastDispatchTime = 0;
  if (millis() - lastDispatchTime >= this->argCollection->dispatch.throttleTime) {
    this->argCollection->dispatch.dispatcher(this->getHandlerId(), this->id, value);
    lastDispatchTime = millis();
  }
}

template <typename T>
String HTMLElement<T>::getHTML()
{
  String html;

  for (auto el : this->children) {
    html += el->getHTML();
  }

  return html;
}

template <typename T>
void HTMLElement<T>::appendChild(HTMLElement* child)
{
  if (std::find_if(this->children.cbegin(), this->children.cend(), [child](HTMLElement* el) { return el == child; }) != this->children.cend())
    return;

  this->children.push_back(child);
}

template <typename T>
void HTMLElement<T>::removeChild(HTMLElement* child)
{
  this->children.remove_if([child](HTMLElement* el) { return el == child; });
}

template <typename T>
std::list<HTMLElement<T>*> HTMLElement<T>::removeAllChildren()
{
  auto children = this->children;
  this->children.clear();
  return children;
}

template <typename T>
String HTMLElement<T>::getBackgroundColorClass()
{
  return colorToString(this->backgroundColor) + F(" ") + colorShadeToString(this->backgroundColorShade);
}

template <typename T>
String HTMLElement<T>::getTextColorClass()
{
  if (this->textColor == Color::defaultColor)
    return String();

  String cClass = colorToString(this->textColor) + F("-text");

  if (this->textColorShade != ColorShade::noShade)
    cClass += F(" text-") + colorShadeToString(this->textColorShade);

  return cClass;
}

template <typename T>
void HTMLElement<T>::setWidth(uint8_t width, ScreenSize screenSize)
{
  if (width > 12)
    return;

  String classPrefix = getWidthClassPrefix(screenSize);

  this->classList.remove(this->widths[(size_t)screenSize]);

  if (width != 0) {
    this->widths[(size_t)screenSize] = classPrefix + String(width);
    this->classList.add(this->widths[(size_t)screenSize]);
  }
}

template <typename T>
uint8_t HTMLElement<T>::getWidth(ScreenSize screenSize)
{
  String className = this->widths[(uint8_t)screenSize];

  if (className.isEmpty())
    return 0;

  size_t classPrefixLength = getWidthClassPrefix(screenSize).length();

  return className.substring(classPrefixLength - 1).toInt();
}

template <typename T>
size_t HTMLElement<T>::getId()
{
  return this->id;
}

template <typename T>
void HTMLElement<T>::setBackgroundColor(Color color, ColorShade colorShade)
{
  this->classList.remove(this->getBackgroundColorClass());

  this->backgroundColor = color;
  if (colorShadeIsValid(color, colorShade))
    this->backgroundColorShade = colorShade;

  this->classList.add(this->getBackgroundColorClass());
}

template <typename T>
void HTMLElement<T>::setTextColor(Color color, ColorShade colorShade)
{
  this->classList.remove(this->getTextColorClass());

  this->textColor = color;
  if (colorShadeIsValid(color, colorShade))
    this->textColorShade = colorShade;

  this->classList.add(this->getTextColorClass());
}

template <typename T>
Color HTMLElement<T>::getBackgroundColor()
{
  return this->backgroundColor;
}

template <typename T>
Color HTMLElement<T>::getTextColor()
{
  return this->textColor;
}

template <typename T>
ColorShade HTMLElement<T>::getBackgroundColorShade()
{
  return this->backgroundColorShade;
}

template <typename T>
ColorShade HTMLElement<T>::getTextColorShade()
{
  return this->textColorShade;
}
