#pragma once

#include <Arduino.h>

#include "../BaseClass/MaterializeCssBaseClass.h"

enum ImageType {
  JPEG,
  PNG,
  GIF,
  WEBP,
  SVG_XML
};

template <typename T>
class Image : public MaterializeCssBaseClass<T> {
  private:
  using HTMLElement<T>::appendChild;
  using HTMLElement<T>::removeAllChildren;
  using HTMLElement<T>::removeChild;

  protected:
  WebSourceHandler* sourceHandler = nullptr;

  public:
  Image(T* argCollection);
  virtual ~Image() = default;

  virtual void getHTML(ResponseWriter writer) override;

  /**
   * @brief Sets the image source
   *
   * @param newSrc new image source
   */
  virtual void setSrc(String newSrc);

  /**
   * @brief Sets the image source to load provided image from PROGMEM
   *
   * @param content image content in PROGMEM
   * @param imageType Type of the image
   */
  virtual void setSrc(const uint8_t* content, size_t contentLength, ImageType imageType);

  /**
   * @brief Sets the alternative text for the image
   *
   * @param newAlt new alternative text
   */
  virtual void setAlt(String newAlt);

  /**
   * @brief Gets the current image source
   *
   * @return String
   */
  virtual String getSrc();

  /**
   * @brief Gets the current alternative text
   *
   * @return String
   */
  virtual String getAlt();
};

// ======================= IMPLEMENTATION =======================

template <typename T>
Image<T>::Image(T* argCollection)
    : MaterializeCssBaseClass<T>(argCollection)
{
  this->classList.add(F("materialboxed"));
}

template <typename T>
void Image<T>::getHTML(ResponseWriter writer)
{
  String id = (String)this->getId();

  writer(F("<img "));
  writer(this->getAttributes());
  writer(F(">"));
}

template <typename T>
void Image<T>::setSrc(String newSrc)
{
  if (this->sourceHandler != nullptr) {
    this->sourceHandler->cancelRegistration();
  }

  this->setAttribute("src", newSrc);
}

template <typename T>
void Image<T>::setSrc(const uint8_t* content, size_t contentLength, ImageType imageType)
{
  String MIMEType;
  switch (imageType) {
  case ImageType::JPEG:
    MIMEType = F("image/jpeg");
    break;
  case ImageType::PNG:
    MIMEType = F("image/png");
    break;
  case ImageType::GIF:
    MIMEType = F("image/gif");
    break;
  case ImageType::WEBP:
    MIMEType = F("image/webp");
    break;
  case ImageType::SVG_XML:
    MIMEType = F("image/svg+xml");
    break;
  default:
    return;
  }

  if (this->sourceHandler != nullptr) {
    this->sourceHandler->switchContent(content, MIMEType);
  } else {
    this->sourceHandler = this->registerSource(F("img") + String(millis()), content, contentLength, MIMEType);
    this->setAttribute("src", this->sourceHandler->getPath());
  }
}

template <typename T>
void Image<T>::setAlt(String newAlt)
{
  this->setAttribute("alt", newAlt);
}

template <typename T>
String Image<T>::getSrc()
{
  return this->getAttribute("src");
}

template <typename T>
String Image<T>::getAlt()
{
  return this->getAttribute("alt");
}
