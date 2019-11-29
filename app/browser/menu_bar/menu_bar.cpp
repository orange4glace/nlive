#include "browser/menu_bar/menu_bar.h"

namespace nlive {

MenuBar::MenuBar(QWigdet* widget, sptr<MenuBarService> menu_bar_service) :
  menu_bar_service_(menu_bar_service) {

}

}