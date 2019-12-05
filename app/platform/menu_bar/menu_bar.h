#ifndef NLIVE_PLATFORM_MENU_BAR_MENU_BAR_H_
#define NLIVE_PLATFORM_MENU_BAR_MENU_BAR_H_

#include <any>
#include <map>
#include <vector>
#include <string>

namespace nlive {

struct MenuBarMenuItem;

class IMenuBarMenuItemAction {
private:
  friend struct MenuBarMenuItem;
  IMenuBarMenuItemAction() = default;
public:
  std::string id;
  std::wstring label;
  bool checked;
  bool enabled;
};

struct IMenuBarMenuItemSubmenu {
private:
  friend struct MenuBarMenuItem;
  IMenuBarMenuItemSubmenu() = default;
public:
  std::string id;
  std::wstring label;
  std::vector<MenuBarMenuItem> items;
};

struct IMenubarMenuItemSeparator {
private:
  friend struct MenuBarMenuItem;
  IMenubarMenuItemSeparator() = default;
};

struct MenuBarMenuItem {
  std::string type;
  std::any item;

  bool isMenuBarMenuItemSeparator();
  bool isMenuBarMenuItemSubmenu();
  bool isMenuBarMenuItemAction();

  IMenuBarMenuItemAction asMenuBarMenuItemAction();
  IMenuBarMenuItemSubmenu asMenuBarMenuItemSubmenu();
  IMenubarMenuItemSeparator asMenuBarMenuItemSeparator();
};

struct IMenuBarMenu {
  std::vector<MenuBarMenuItem> items;
};

struct IMenuBarData {
  std::map<std::string/*id*/, IMenuBarMenu> menus;
};

}

#endif