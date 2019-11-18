#ifndef NLIVE_THEME_H_
#define NLIVE_THEME_H_

#include <string>
#include <QColor>

using namespace std;

namespace nlive {

class Theme {

private:
  QColor primaryColor_;
  QColor secondaryColor_;
  QColor surfaceColor_;
  QColor surfaceDarkColor_;
  QColor surfaceBrightColor_;
  QColor surfaceTextColor_;
  QColor surfaceContrastColor_;
  QColor buttonEnabled_;
  QColor buttonHighlighted_;
  QColor buttonDisabled_;


public:
  Theme(
    QColor primaryColor,
    QColor secondaryColor,
    QColor surfaceColor,
    QColor surfaceDarkColor,
    QColor surfaceBrightColor,
    QColor surfaceTextColor,
    QColor surfaceContrastColor,
    QColor buttonEnabled,
    QColor buttonHighlighted,
    QColor buttonDisabled) : 
    primaryColor_(primaryColor),
    secondaryColor_(secondaryColor),
    surfaceColor_(surfaceColor),
    surfaceDarkColor_(surfaceDarkColor),
    surfaceBrightColor_(surfaceBrightColor),
    surfaceTextColor_(surfaceTextColor),
    surfaceContrastColor_(surfaceContrastColor),
    buttonEnabled_(buttonEnabled),
    buttonHighlighted_(buttonHighlighted),
    buttonDisabled_(buttonDisabled) {
  }

  Theme(Theme const& rhs) : 
    primaryColor_(rhs.primaryColor_),
    secondaryColor_(rhs.secondaryColor_),
    surfaceColor_(rhs.surfaceColor_),
    surfaceDarkColor_(rhs.surfaceDarkColor_),
    surfaceBrightColor_(rhs.surfaceBrightColor_),
    surfaceTextColor_(rhs.surfaceTextColor_),
    surfaceContrastColor_(rhs.surfaceContrastColor_),
    buttonEnabled_(rhs.buttonEnabled_),
    buttonHighlighted_(rhs.buttonHighlighted_),
    buttonDisabled_(rhs.buttonDisabled_) {
  }

  inline const QColor& primaryColor() const { return primaryColor_; }
  inline const QColor& secondaryColor() const { return secondaryColor_; }
  inline const QColor& surfaceColor() const { return surfaceColor_; }
  inline const QColor& surfaceDarkColor() const { return surfaceDarkColor_; }
  inline const QColor& surfaceBrightColor() const { return surfaceBrightColor_; }
  inline const QColor& surfaceTextColor() const { return surfaceTextColor_; }
  inline const QColor& surfaceContrastColor() const { return surfaceContrastColor_; }
  inline const QColor& buttonEnabled() const { return buttonEnabled_; }
  inline const QColor& buttonHighlighted() const { return buttonHighlighted_; }
  inline const QColor& buttonDisabled() const { return buttonDisabled_; }

};

}

#endif