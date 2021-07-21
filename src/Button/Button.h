#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Arduino.h>
#include <functional>

#include "../HTMLElement/HTMLElement.h"

typedef std::function<void()> ButtonCallback;

class Button : public HTMLElement
{
  template <typename T>
  friend class Container;

private:
  String caption;
  ButtonCallback callback;

protected:
  virtual String getHTML();
  virtual bool emit(size_t id, String value);

public:
  /**
  * @brief Constructs a new Button object
  * 
  * @param caption the caption of the button
  * @param callback callback that is called when the button is clicked
  */
  Button(String caption, ButtonCallback callback);

  /**
   * @brief Gets the caption of the button
   * 
   * @return String 
   */
  String getCaption();

  /**
   * @brief Sets the caption of the button
   * 
   * @param caption the new caption of the button
   * 
   * @return void
   */
  void setCaption(String caption);

  /**
   * @brief Sets callback that is called when the button is clicked
   * 
   * @param callback 
   * 
   * @return void
   */
  void setCallback(ButtonCallback callback);
};

#endif //_BUTTON_H_