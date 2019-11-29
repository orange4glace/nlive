#ifndef NLIVE_MENU_BAR_H_
#define NLIVE_MENU_BAR_H_

#include <QWidget>
#include "base/common/memory.h"
#include "browser/services/menu_bar/menu_bar_sevice.h"

namespace nlive {

class MenuBar {

private:
  sptr<MenuBarService> menu_bar_service_;

public:
  MenuBar(QWidget* widget, sptr<MenuBarService> menu_bar_service);

};

}

#endif