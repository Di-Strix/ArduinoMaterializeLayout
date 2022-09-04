#include "HTMLElementHelperFunctions.h"

String getWidthClassPrefix(ScreenSize screenSize)
{
  switch (screenSize) {
  default:
  case ScreenSize::small:
    return "s";
    break;
  case ScreenSize::medium:
    return "m";
    break;
  case ScreenSize::large:
    return "l";
    break;
  case ScreenSize::extraLarge:
    return "xl";
    break;
  }
}

String colorShadeToString(ColorShade cs)
{
  switch (cs) {
  case ColorShade::lighten5:
    return F("lighten-5");
    break;
  case ColorShade::lighten4:
    return F("lighten-4");
    break;
  case ColorShade::lighten3:
    return F("lighten-3");
    break;
  case ColorShade::lighten2:
    return F("lighten-2");
    break;
  case ColorShade::lighten1:
    return F("lighten-1");
    break;
  default:
  case ColorShade::noShade:
    return "";
    break;
  case ColorShade::darken1:
    return F("darken-1");
    break;
  case ColorShade::darken2:
    return F("darken-2");
    break;
  case ColorShade::darken3:
    return F("darken-3");
    break;
  case ColorShade::darken4:
    return F("darken-4");
    break;
  case ColorShade::accent1:
    return F("accent-1");
    break;
  case ColorShade::accent2:
    return F("accent-2");
    break;
  case ColorShade::accent3:
    return F("accent-3");
    break;
  case ColorShade::accent4:
    return F("accent-4");
    break;
  }
}

bool colorShadeIsValid(Color c, ColorShade cs)
{
  switch (c) {
  case Color::brown:
  case Color::grey:
  case Color::blueGrey:
    if (!colorShadeToString(cs).startsWith(F("accent")))
      return true;
    break;

  case Color::black:
  case Color::white:
  case Color::transparent:
    break;

  default:
    return true;
    break;
  }

  return false;
}

String colorToString(Color c)
{
  switch (c) {
  default:
  case Color::defaultColor:
    return "";
    break;
  case Color::red:
    return F("red");
    break;
  case Color::pink:
    return F("pink");
    break;
  case Color::purple:
    return F("purple");
    break;
  case Color::deepPurple:
    return F("deep-purple");
    break;
  case Color::indigo:
    return F("indigo");
    break;
  case Color::blue:
    return F("blue");
    break;
  case Color::lightBlue:
    return F("light-blue");
    break;
  case Color::cyan:
    return F("cyan");
    break;
  case Color::teal:
    return F("teal");
    break;
  case Color::green:
    return F("green");
    break;
  case Color::lightGreen:
    return F("light-green");
    break;
  case Color::lime:
    return F("lime");
    break;
  case Color::yellow:
    return F("yellow");
    break;
  case Color::amber:
    return F("amber");
    break;
  case Color::orange:
    return F("orange");
    break;
  case Color::deepOrange:
    return F("deep-orange");
    break;
  case Color::brown:
    return F("brown");
    break;
  case Color::grey:
    return F("grey");
    break;
  case Color::blueGrey:
    return F("blue-grey");
    break;
  case Color::black:
    return F("black");
    break;
  case Color::white:
    return F("white");
    break;
  case Color::transparent:
    return F("transparent");
    break;
  }
}

String getCSSStyleKey(CSSStyleKey k)
{
  switch (k) {
  case CSSStyleKey::alignContent:
    return F("align-content");
  case CSSStyleKey::alignItems:
    return F("align-items");
  case CSSStyleKey::alignSelf:
    return F("align-self");
  case CSSStyleKey::animation:
    return F("animation");
  case CSSStyleKey::animationDelay:
    return F("animation-delay");
  case CSSStyleKey::animationDirection:
    return F("animation-direction");
  case CSSStyleKey::animationDuration:
    return F("animation-duration");
  case CSSStyleKey::animationFillMode:
    return F("animation-fill-mode");
  case CSSStyleKey::animationIterationCount:
    return F("animation-iteration-count");
  case CSSStyleKey::animationName:
    return F("animation-name");
  case CSSStyleKey::animationPlayState:
    return F("animation-play-state");
  case CSSStyleKey::animationTimingFunction:
    return F("animation-timing-function");
  case CSSStyleKey::backfaceVisibility:
    return F("backface-visibility");
  case CSSStyleKey::background:
    return F("background");
  case CSSStyleKey::backgroundAttachment:
    return F("background-attachment");
  case CSSStyleKey::backgroundClip:
    return F("background-clip");
  case CSSStyleKey::backgroundColor:
    return F("background-color");
  case CSSStyleKey::backgroundImage:
    return F("background-image");
  case CSSStyleKey::backgroundOrigin:
    return F("background-origin");
  case CSSStyleKey::backgroundPosition:
    return F("background-position");
  case CSSStyleKey::backgroundRepeat:
    return F("background-repeat");
  case CSSStyleKey::backgroundSize:
    return F("background-size");
  case CSSStyleKey::border:
    return F("border");
  case CSSStyleKey::borderBottom:
    return F("border-bottom");
  case CSSStyleKey::borderBottomColor:
    return F("border-bottom-color");
  case CSSStyleKey::borderBottomLeftRadius:
    return F("border-bottom-left-radius");
  case CSSStyleKey::borderBottomRightRadius:
    return F("border-bottom-right-radius");
  case CSSStyleKey::borderBottomStyle:
    return F("border-bottom-style");
  case CSSStyleKey::borderBottomWidth:
    return F("border-bottom-width");
  case CSSStyleKey::borderCollapse:
    return F("border-collapse");
  case CSSStyleKey::borderColor:
    return F("border-color");
  case CSSStyleKey::borderImage:
    return F("border-image");
  case CSSStyleKey::borderImageOutset:
    return F("border-image-outset");
  case CSSStyleKey::borderImageRepeat:
    return F("border-image-repeat");
  case CSSStyleKey::borderImageSlice:
    return F("border-image-slice");
  case CSSStyleKey::borderImageSource:
    return F("border-image-source");
  case CSSStyleKey::borderImageWidth:
    return F("border-image-width");
  case CSSStyleKey::borderLeft:
    return F("border-left");
  case CSSStyleKey::borderLeftColor:
    return F("border-left-color");
  case CSSStyleKey::borderLeftStyle:
    return F("border-left-style");
  case CSSStyleKey::borderLeftWidth:
    return F("border-left-width");
  case CSSStyleKey::borderRadius:
    return F("border-radius");
  case CSSStyleKey::borderRight:
    return F("border-right");
  case CSSStyleKey::borderRightColor:
    return F("border-right-color");
  case CSSStyleKey::borderRightStyle:
    return F("border-right-style");
  case CSSStyleKey::borderRightWidth:
    return F("border-right-width");
  case CSSStyleKey::borderSpacing:
    return F("border-spacing");
  case CSSStyleKey::borderStyle:
    return F("border-style");
  case CSSStyleKey::borderTop:
    return F("border-top");
  case CSSStyleKey::borderTopColor:
    return F("border-top-color");
  case CSSStyleKey::borderTopLeftRadius:
    return F("border-top-left-radius");
  case CSSStyleKey::borderTopRightRadius:
    return F("border-top-right-radius");
  case CSSStyleKey::borderTopStyle:
    return F("border-top-style");
  case CSSStyleKey::borderTopWidth:
    return F("border-top-width");
  case CSSStyleKey::borderWidth:
    return F("border-width");
  case CSSStyleKey::bottom:
    return F("bottom");
  case CSSStyleKey::boxShadow:
    return F("box-shadow");
  case CSSStyleKey::boxSizing:
    return F("box-sizing");
  case CSSStyleKey::captionSide:
    return F("caption-side");
  case CSSStyleKey::clear:
    return F("clear");
  case CSSStyleKey::clip:
    return F("clip");
  case CSSStyleKey::color:
    return F("color");
  case CSSStyleKey::columnCount:
    return F("column-count");
  case CSSStyleKey::columnFill:
    return F("column-fill");
  case CSSStyleKey::columnGap:
    return F("column-gap");
  case CSSStyleKey::columnRule:
    return F("column-rule");
  case CSSStyleKey::columnRuleColor:
    return F("column-rule-color");
  case CSSStyleKey::columnRuleStyle:
    return F("column-rule-style");
  case CSSStyleKey::columnRuleWidth:
    return F("column-rule-width");
  case CSSStyleKey::columnSpan:
    return F("column-span");
  case CSSStyleKey::columnWidth:
    return F("column-width");
  case CSSStyleKey::columns:
    return F("columns");
  case CSSStyleKey::content:
    return F("content");
  case CSSStyleKey::counterIncrement:
    return F("counter-increment");
  case CSSStyleKey::counterReset:
    return F("counter-reset");
  case CSSStyleKey::cursor:
    return F("cursor");
  case CSSStyleKey::direction:
    return F("direction");
  case CSSStyleKey::display:
    return F("display");
  case CSSStyleKey::emptyCells:
    return F("empty-cells");
  case CSSStyleKey::flex:
    return F("flex");
  case CSSStyleKey::flexBasis:
    return F("flex-basis");
  case CSSStyleKey::flexDirection:
    return F("flex-direction");
  case CSSStyleKey::flexFlow:
    return F("flex-flow");
  case CSSStyleKey::flexGrow:
    return F("flex-grow");
  case CSSStyleKey::flexShrink:
    return F("flex-shrink");
  case CSSStyleKey::flexWrap:
    return F("flex-wrap");
  case CSSStyleKey::float_:
    return F("float");
  case CSSStyleKey::font:
    return F("font");
  case CSSStyleKey::fontFamily:
    return F("font-family");
  case CSSStyleKey::fontSize:
    return F("font-size");
  case CSSStyleKey::fontSizeAdjust:
    return F("font-size-adjust");
  case CSSStyleKey::fontStretch:
    return F("font-stretch");
  case CSSStyleKey::fontStyle:
    return F("font-style");
  case CSSStyleKey::fontVariant:
    return F("font-variant");
  case CSSStyleKey::fontWeight:
    return F("font-weight");
  case CSSStyleKey::height:
    return F("height");
  case CSSStyleKey::justifyContent:
    return F("justify-content");
  case CSSStyleKey::left:
    return F("left");
  case CSSStyleKey::letterSpacing:
    return F("letter-spacing");
  case CSSStyleKey::lineHeight:
    return F("line-height");
  case CSSStyleKey::listStyle:
    return F("list-style");
  case CSSStyleKey::listStyleImage:
    return F("list-style-image");
  case CSSStyleKey::listStylePosition:
    return F("list-style-position");
  case CSSStyleKey::listStyleType:
    return F("list-style-type");
  case CSSStyleKey::margin:
    return F("margin");
  case CSSStyleKey::marginBottom:
    return F("margin-bottom");
  case CSSStyleKey::marginLeft:
    return F("margin-left");
  case CSSStyleKey::marginRight:
    return F("margin-right");
  case CSSStyleKey::marginTop:
    return F("margin-top");
  case CSSStyleKey::maxHeight:
    return F("max-height");
  case CSSStyleKey::maxWidth:
    return F("max-width");
  case CSSStyleKey::minHeight:
    return F("min-height");
  case CSSStyleKey::minWidth:
    return F("min-width");
  case CSSStyleKey::opacity:
    return F("opacity");
  case CSSStyleKey::order:
    return F("order");
  case CSSStyleKey::outline:
    return F("outline");
  case CSSStyleKey::outlineColor:
    return F("outline-color");
  case CSSStyleKey::outlineOffset:
    return F("outline-offset");
  case CSSStyleKey::outlineStyle:
    return F("outline-style");
  case CSSStyleKey::outlineWidth:
    return F("outline-width");
  case CSSStyleKey::overflow:
    return F("overflow");
  case CSSStyleKey::overflowX:
    return F("overflow-x");
  case CSSStyleKey::overflowY:
    return F("overflow-y");
  case CSSStyleKey::padding:
    return F("padding");
  case CSSStyleKey::paddingBottom:
    return F("padding-bottom");
  case CSSStyleKey::paddingLeft:
    return F("padding-left");
  case CSSStyleKey::paddingRight:
    return F("padding-right");
  case CSSStyleKey::paddingTop:
    return F("padding-top");
  case CSSStyleKey::pageBreakAfter:
    return F("page-break-after");
  case CSSStyleKey::pageBreakBefore:
    return F("page-break-before");
  case CSSStyleKey::pageBreakInside:
    return F("page-break-inside");
  case CSSStyleKey::perspective:
    return F("perspective");
  case CSSStyleKey::perspectiveOrigin:
    return F("perspective-origin");
  case CSSStyleKey::position:
    return F("position");
  case CSSStyleKey::quotes:
    return F("quotes");
  case CSSStyleKey::resize:
    return F("resize");
  case CSSStyleKey::right:
    return F("right");
  case CSSStyleKey::tabSize:
    return F("tab-size");
  case CSSStyleKey::tableLayout:
    return F("table-layout");
  case CSSStyleKey::textAlign:
    return F("text-align");
  case CSSStyleKey::textAlignLast:
    return F("text-align-last");
  case CSSStyleKey::textDecoration:
    return F("text-decoration");
  case CSSStyleKey::textDecorationColor:
    return F("text-decoration-color");
  case CSSStyleKey::textDecorationLine:
    return F("text-decoration-line");
  case CSSStyleKey::textDecorationStyle:
    return F("text-decoration-style");
  case CSSStyleKey::textIndent:
    return F("text-indent");
  case CSSStyleKey::textJustify:
    return F("text-justify");
  case CSSStyleKey::textOverflow:
    return F("text-overflow");
  case CSSStyleKey::textShadow:
    return F("text-shadow");
  case CSSStyleKey::textTransform:
    return F("text-transform");
  case CSSStyleKey::top:
    return F("top");
  case CSSStyleKey::transform:
    return F("transform");
  case CSSStyleKey::transformOrigin:
    return F("transform-origin");
  case CSSStyleKey::transformStyle:
    return F("transform-style");
  case CSSStyleKey::transition:
    return F("transition");
  case CSSStyleKey::transitionDelay:
    return F("transition-delay");
  case CSSStyleKey::transitionDuration:
    return F("transition-duration");
  case CSSStyleKey::transitionProperty:
    return F("transition-property");
  case CSSStyleKey::transitionTimingFunction:
    return F("transition-timing-function");
  case CSSStyleKey::verticalAlign:
    return F("vertical-align");
  case CSSStyleKey::visibility:
    return F("visibility");
  case CSSStyleKey::whiteSpace:
    return F("white-space");
  case CSSStyleKey::width:
    return F("width");
  case CSSStyleKey::wordBreak:
    return F("word-break");
  case CSSStyleKey::wordSpacing:
    return F("word-spacing");
  case CSSStyleKey::wordWrap:
    return F("word-wrap");
  case CSSStyleKey::zIndex:
    return F("z-index");
  default:
    return "";
  }
}
