#pragma once

#include <Arduino.h>
#include <map>

#include "ClassList/ClassList.h"
#include "EventEmitter/EventEmitter.h"
#include "IdGenerator.h"
#include "WebSourceHandler/WebSourceHandler.h"

#include "HTMLElementHelperFunctions.h"
#include "HTMLElemetTypes.h"

template <typename T>
class HTMLElement {
  static_assert(!std::is_pointer_v<T>, "Template argument for HTMLElement must not be a pointer");
  static_assert(std::is_base_of_v<HTMLElementArgsBase, std::remove_pointer_t<T>>, "Template arg of the HTMLElement must derive from HTMLElementArgsBase");

  private:
  T* argCollection;

  size_t id = 0;

  String widths[4];

  Color backgroundColor = Color::defaultColor;
  ColorShade backgroundColorShade = ColorShade::noShade;

  Color textColor = Color::defaultColor;
  ColorShade textColorShade = ColorShade::noShade;

  std::map<String, String> inlineStyles;
  std::map<String, String> attributes;

  String getTextColorClass();
  String getTextColorShadeClass();

  protected:
  virtual String getHandlerId() { return F("MaterializeCssHandler"); };

  std::list<HTMLElement*> children;

  /**
   * @brief Event handler that is called when event emitted on this component
   *
   * @param value Event data
   */
  virtual void onEmit(String value) {};

  /**
   * @brief Returns insert-ready inline css for the current node
   *
   * @return String
   */
  String getInlineStyles();

  /**
   * @brief Returns insert-ready attribute list
   *
   * @return String
   */
  String getAttributes();

  T* getArgCollection();

  public:
  ClassList classList;

  /**
   * @brief Event that is fired when inline CSS property changed
   *
   * @param String - Style key
   * @param String - Style value
   */
  EventEmitter<String, String> onInlineStylesChange;

  /**
   * @brief Event that is fired when inline CSS property changed
   *
   * @param String - Attribute key
   * @param String - Attribute value
   */
  EventEmitter<String, String> onAttributeChange;

  /**
   * @brief Event that is fired when child component is added or removed
   *
   * @param ChildChangeType type of the change
   * @param HTMLElement* pointer to the element that caused event
   */
  EventEmitter<ChildChangeType, HTMLElement*> onChildChange;

  HTMLElement(T* argCollection);

  virtual ~HTMLElement() = default;

  /**
   * @brief Returns an id of the element. The id is used to emit an event on certain component
   * @return size_t
   */
  size_t getId();

  void appendChild(HTMLElement* child);

  void removeChild(HTMLElement* child);

  std::list<HTMLElement*> removeAllChildren();

  /**
   * @brief Intended to write filled HTML template of the component using writer.
   *
   * Placeholder for further implementation.
   *
   * @return void
   */
  virtual void getHTML(ResponseWriter writer);

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
   * @brief Registers custom source for the page
   *
   * @param path Path to the course. The final source will be available by path /${Module handler id}/${node id}/${path}
   * @param content Source contents in PROGMEM
   * @param contentType Type of the content according to the MIME type. See https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types#types
   */
  WebSourceHandler* registerSource(String path, const uint8_t* content, size_t contentLength, String contentType);

  /**
   * @brief Sets width of the component in HTML if it's possible.
   * Doesn't work for row layout
   * Uses 12-column grid layout. See "Introduction" section - https://materializecss.com/grid.html
   *
   * @param width the width of the component. range: [0-12]. If 0, the component takes up as much space as needed
   * @param screenSize width that applies to the given screen size and bigger, if no values for bigger screen is set. See "Creating Responsive Layouts" section - https://materializecss.com/grid.html
   * @param emit whether to dispatch an event or not
   *
   * @return void
   */
  void setWidth(uint8_t width, ScreenSize screenSize = ScreenSize::small, bool emit = true);

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
   * @param emit whether to dispatch event or not
   * @return void
   */
  void setBackgroundColor(Color color, ColorShade colorShade = ColorShade::noShade, bool emit = true);

  /**
   * @brief Sets the text color of the component. See "Color Palette" section - https://materializecss.com/color.html
   *
   * @param color
   * @param colorShade
   * @param emit whether to dispatch event or not
   */
  void setTextColor(Color color, ColorShade colorShade = ColorShade::noShade, bool emit = true);

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

  /**
   * @brief Set style parameter for current node
   *
   * @param key Style key
   * @param value Value to be assigned
   * @param emit whether to dispatch event or not
   */
  void setStyle(CSSStyleKey key, String value, bool emit = true);

  /**
   * @brief Returns current value for the provided key
   *
   * @param key Style key to search value for
   * @return Value
   */
  String getStyle(CSSStyleKey key);

  /**
   * @brief Set the Attribute for the current node
   *
   * @param attr attribute name
   * @param value attribute value
   * @param emit whether to dispatch event or not
   */
  void setAttribute(String attr, String value, bool emit = true);

  /**
   * @brief Get the Attribute value for current node
   *
   * @param attr attribute name
   * @return String attribute value
   */
  String getAttribute(String attr);
};

// ======================= IMPLEMENTATION =======================

template <typename T>
HTMLElement<T>::HTMLElement(T* argCollection)
{
  this->argCollection = argCollection;
  this->id = IdGenerator::Instance().getId();
  this->setAttribute(F("data-id"), String(this->id));

  this->classList.onChange.subscribe([this](String, String, ClassChangeType) {
    this->setAttribute(F("class"), this->classList.value(), false);
  });

  this->onInlineStylesChange.subscribe([this](String, String) {
    this->setAttribute(F("style"), this->getInlineStyles(), false);
  });
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
  auto dispatchArgs = this->getArgCollection()->dispatch;

  static uint32_t lastDispatchTime = 0;
  if (dispatchArgs.dispatcher && millis() - lastDispatchTime >= dispatchArgs.throttleTime) {
    dispatchArgs.dispatcher(this->getHandlerId(), this->id, value);
    lastDispatchTime = millis();
  }
}

template <typename T>
WebSourceHandler* HTMLElement<T>::registerSource(String path, const uint8_t* content, size_t contentLength, String contentType)
{
  return this->getArgCollection()->registerSource("/" + this->getHandlerId() + "/" + String(this->getId()) + "/" + path, content, contentLength, contentType);
}

template <typename T>
void HTMLElement<T>::getHTML(ResponseWriter writer)
{
  for (auto el : this->children) {
    el->getHTML(writer);
  }
}

template <typename T>
void HTMLElement<T>::appendChild(HTMLElement* child)
{
  if (std::find_if(this->children.cbegin(), this->children.cend(), [child](HTMLElement* el) { return el == child; }) != this->children.cend())
    return;

  this->children.push_back(child);
  this->onChildChange.emit(ChildChangeType::add, child);
}

template <typename T>
void HTMLElement<T>::removeChild(HTMLElement* child)
{
  bool removed = false;
  this->children.remove_if([child, &removed](HTMLElement* el) { return removed = el == child; });

  if (removed)
    this->onChildChange.emit(ChildChangeType::remove, child);
}

template <typename T>
std::list<HTMLElement<T>*> HTMLElement<T>::removeAllChildren()
{
  auto children = this->children;
  this->children.clear();

  for (auto child : children) {
    this->onChildChange.emit(ChildChangeType::remove, child);
  }

  return children;
}

template <typename T>
String HTMLElement<T>::getTextColorClass()
{
  if (this->textColor == Color::defaultColor)
    return String();

  return colorToString(this->textColor) + F("-text");
}

template <typename T>
String HTMLElement<T>::getTextColorShadeClass()
{
  if (this->textColor == Color::defaultColor || this->textColorShade == ColorShade::noShade)
    return String();

  return F(" text-") + colorShadeToString(this->textColorShade);
}

template <typename T>
void HTMLElement<T>::setWidth(uint8_t width, ScreenSize screenSize, bool emit)
{
  if (width > 12)
    return;

  String classPrefix = getWidthClassPrefix(screenSize);

  this->classList.remove(this->widths[(size_t)screenSize], emit);

  if (width != 0) {
    this->widths[(size_t)screenSize] = classPrefix + String(width);
    this->classList.add(this->widths[(size_t)screenSize], emit);
  }
}

template <typename T>
uint8_t HTMLElement<T>::getWidth(ScreenSize screenSize)
{
  String className = this->widths[(uint8_t)screenSize];
  if (className.isEmpty())
    return 0;

  size_t classPrefixLength = getWidthClassPrefix(screenSize).length();

  return className.substring(classPrefixLength).toInt();
}

template <typename T>
size_t HTMLElement<T>::getId()
{
  return this->id;
}

template <typename T>
void HTMLElement<T>::setBackgroundColor(Color color, ColorShade colorShade, bool emit)
{
  auto oldColor = colorToString(this->backgroundColor);
  auto oldColorShade = colorShadeToString(this->backgroundColorShade);

  this->backgroundColor = color;
  if (colorShadeIsValid(color, colorShade))
    this->backgroundColorShade = colorShade;
  else
    this->backgroundColorShade = ColorShade::noShade;

  if (this->classList.contains(oldColor))
    this->classList.replace(oldColor, colorToString(this->backgroundColor), emit);
  else
    this->classList.add(colorToString(this->backgroundColor), emit);
  if (this->classList.contains(oldColorShade))
    this->classList.replace(oldColorShade, colorShadeToString(this->backgroundColorShade), emit);
  else
    this->classList.add(colorShadeToString(this->backgroundColorShade), emit);
}

template <typename T>
void HTMLElement<T>::setTextColor(Color color, ColorShade colorShade, bool emit)
{
  auto oldColor = colorToString(this->backgroundColor);
  auto oldColorShade = colorShadeToString(this->backgroundColorShade);

  this->textColor = color;
  if (colorShadeIsValid(color, colorShade))
    this->textColorShade = colorShade;
  else
    this->textColorShade = ColorShade::noShade;

  if (this->classList.contains(oldColor))
    this->classList.replace(oldColor, this->getTextColorClass(), emit);
  else
    this->classList.add(this->getTextColorClass(), emit);
  if (this->classList.contains(oldColorShade))
    this->classList.replace(oldColorShade, this->getTextColorShadeClass(), emit);
  else
    this->classList.add(this->getTextColorShadeClass(), emit);
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

template <typename T>
void HTMLElement<T>::setStyle(CSSStyleKey key, String value, bool emit)
{
  String mapKey = getCSSStyleKey(key);

  if (mapKey.isEmpty())
    return;

  value.trim();
  value.replace(F(";"), F(""));

  if (value.isEmpty()) {
    this->inlineStyles.erase(mapKey);
  } else {
    this->inlineStyles[mapKey] = value;
  }

  if (emit)
    this->onInlineStylesChange.emit(mapKey, value);
}

template <typename T>
String HTMLElement<T>::getStyle(CSSStyleKey key)
{
  return this->inlineStyles[getCSSStyleKey(key)];
}

template <typename T>
String HTMLElement<T>::getInlineStyles()
{
  String result;

  for (auto style : this->inlineStyles) {
    result += style.first;
    result += F(":");
    result += style.second;
    result += F(";");
  }

  return result;
}

template <typename T>
T* HTMLElement<T>::getArgCollection()
{
  return this->argCollection;
}

template <typename T>
void HTMLElement<T>::setAttribute(String attr, String value, bool emit)
{
  attr.trim();
  this->attributes[attr] = value;

  if (emit)
    this->onAttributeChange.emit(attr, value);
}

template <typename T>
String HTMLElement<T>::getAttribute(String attr)
{
  attr.trim();
  return this->attributes[attr];
}

template <typename T>
String HTMLElement<T>::getAttributes()
{
  String result;

  for (auto attribute : this->attributes) {
    if (attribute.first.isEmpty())
      continue;

    result += attribute.first;
    result += F("=\"");
    result += attribute.second;
    result += F("\" ");
  }

  return result;
}
