#include "platform/menu_bar/qt/menu_bar.h"

#include <QMenu>

namespace nlive {

void NativeMenuBar::install() {
  if (qmenu_bar_ != nullptr) delete qmenu_bar_;

  qmenu_bar_ = new QMenuBar(0);
  auto file_menu = new QMenu();
}

void NativeMenuBar::setMenu(QMenu* menu, std::vector<MenuBarMenuItem> items) {
  for (auto& item : items) {
    if (item.isMenuBarMenuItemSeparator()) {

    }
    else if (item.isMenuBarMenuItemSubmenu()) {
      auto& submenu_item = item.asMenuBarMenuItemSubmenu();
      auto qsubmenu = new QMenu(menu);
      setMenu(qsubmenu, submenu_item.items);
      menu->addMenu(qsubmenu);
    }
    else if (item.isMenuBarMenuItemAction()) {
      auto& action_item = item.asMenuBarMenuItemAction();
    }
  }
}

QAction* NativeMenuBar::createMenuItem(QMenu* menu, std::wstring label, std::string id, bool enabled, bool checked) {
  QAction* qaction = new QAction(QString::fromStdWString(label), menu);
  QObject::connect(qaction, &QAction::triggered, [this]() {

  });
}

}