#include "platform/theme/themeservice-impl.h"

#include <QColor>
#include <QSharedPointer>

namespace nlive {

QSharedPointer<ThemeService> ThemeService::instance_ = nullptr;

void ThemeService::Initialize() {
  ThemeService::instance_ = QSharedPointer<ThemeService>(new ThemeService());
}

ThemeService::ThemeService() :
  theme_(
    QColor("#28aaff"),
    QColor("#ff5040"),
    QColor("#212624"),
    QColor("#1a1a1a"),
    QColor("#424242"),
    QColor("#BDBDBD"),
    QColor("#BDBDBD"),
    QColor("#b9b9b9"),
    QColor("#28aaff"),
    QColor("#343c39")) {

}

void ThemeService::setTheme(Theme const& theme) {
  theme_ = theme;
  onDidUpdate();
}

const Theme& ThemeService::getTheme() const {
  return theme_;
}

}