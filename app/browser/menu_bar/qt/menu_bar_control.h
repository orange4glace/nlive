#ifndef NLIVE_BROWSER_MENU_BAR_QT_MENU_BAR_CONTROL_H_
#define NLIVE_BROWSER_MENU_BAR_QT_MENU_BAR_CONTROL_H_

#include <QMainWindow>
#include "browser/menu_bar/menu_bar_control.h"
#include "base/common/actions.h"
#include <QMenuBar>

namespace nlive {

class NativeMenuBarControl : public MenuBarControl {

private:
  QMainWindow* main_window_;
  QMenuBar* qmenu_bar_;

  void install(bool first_time);
  void updateActions(QWidget* menubar_or_menu, bool is_menubar,
    sptr<IMenu> menu, std::wstring top_level_title);

protected:
  void doUpdateMenuBar(bool first_time) override;

public:
  NativeMenuBarControl(
    QMainWindow* main_window,
    sptr<IMenuService> menu_service,
    sptr<IContextKeyService> context_key_service);

};

}

#endif