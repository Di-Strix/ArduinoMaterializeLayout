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
  Button(String caption, ButtonCallback callback);

  String getCaption();
  void setCaption(String caption);

  void setCallback(ButtonCallback callback);
};

#endif //_BUTTON_H_