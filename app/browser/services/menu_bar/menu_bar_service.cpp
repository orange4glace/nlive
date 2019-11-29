#include "browser/services/menu_bar/menu_bar_service.h"

#include <assert.h>

namespace nlive {

Menu::Menu(std::string id, QString title, QMenu* qmenu) {
  if (qmenu == nullptr) qmenu = new QMenu(title);
  id_ = id;
  title_ = title;
  qmenu_ = qmenu;
}

void Menu::addAction(MenuActionDescriptor desc) {
  assert(action_descriptors_.count(desc.id) == 0);
  action_descriptors_[desc.id] = desc;
  qmenu_->addAction(desc.action);
}

Menu* Menu::addSubmenu(std::string id, QString title) {
  assert(action_descriptors_.count(id) == 0);
  QMenu* sub_menu = qmenu_->addMenu(title);
  QAction* sub_menu_action = sub_menu->menuAction();
  Menu* menu = new Menu(id, title, sub_menu);
  MenuActionDescriptor desc;
  desc.id = id;
  desc.title = title;
  desc.action = sub_menu_action;
  action_descriptors_[desc.id] = desc;
  return menu;
}
  
MenuActionDescriptor* Menu::getAction(std::string id) {
  if (action_descriptors_.count(id))
    return &action_descriptors_[id];
  return nullptr;
}

QMenu* Menu::qmenu() { return qmenu_; }

const std::string MenuBarService::ID = "nlive.browser.service.MenuBarService";

MenuBarService::MenuBarService(QWidget* widget, sptr<IThemeService> theme_service) :
  MenuBarService(new QMenuBar(widget), theme_service) {
}

MenuBarService::MenuBarService(QMenuBar* qmenu_bar, sptr<IThemeService> theme_service) {
  qmenu_bar_ = qmenu_bar;
  auto theme = theme_service->getTheme();
  qmenu_bar_->setStyleSheet(
      "QMenuBar {"
        "background-color: " + theme.surfaceDarkColor().name() + ";"
        "color: " + theme.surfaceTextColor().name() + ";"
      "}"
      "QMenuBar::item {"
        "padding: 7px 7px;"
      "}"
      "QMenuBar::item:selected {"
        "background-color: " + theme.surfaceBrightColor().name() + ";"
        "color: " + theme.primaryColor().name() + ";"
      "}");
}

Menu* MenuBarService::addMenu(std::string id, QString title) {
  assert(menus_.count(id) == 0);
  Menu* menu = new Menu(id, title);
  menus_[id] = menu;
  qmenu_bar_->addMenu(menu->qmenu());
  return menu;
}

Menu* MenuBarService::getMenu(std::string id) {
  if (menus_.count(id)) return menus_[id];
  return nullptr;
}

QMenuBar* MenuBarService::qmenu_bar() {
  return qmenu_bar_;
}

}
