#ifndef NLIVE_PLATFORM_MENU_BAR_QT_MENU_BAR_H_
#define NLIVE_PLATFORM_MENU_BAR_QT_MENU_BAR_H_

#include <QMenuBar>
#include "platform/menu_bar/menu_bar.h"

namespace nlive {
  
class NativeMenuBar {

private:
  QMenuBar* qmenu_bar_;
  IMenuBarData menu_bar_data_;

  void install();
  void setMenuById(QMenu* menu, std::string menu_id);
  void setMenu(QMenu* menu, std::vector<MenuBarMenuItem> items);
  QAction* createMenuItem(QMenu* menu, std::wstring label, std::string id, bool enabled, bool checked);

public:
  void updateMenu(IMenuBarData menu_bar_data);


};

}

#endif