#include "platform/action/menu_service.h"

#include <QDebug>

namespace nlive {

MenuService::MenuService(sptr<ICommandService> command_service) :
  command_service_(command_service) {

}

sptr<IMenu> MenuService::createMenu(std::string id, sptr<IContextKeyService> context_key_service) {
  return std::make_shared<Menu>(id, command_service_, context_key_service);
}

Menu::Menu(std::string menu_id, sptr<ICommandService> command_service,
    sptr<IContextKeyService> context_key_service) :
  id_(menu_id), command_service_(command_service),
  context_key_service_(context_key_service) {
  build();
}

struct MenuItemCompare {
  bool operator()(const MenuItemable& a, const MenuItemable& b) {
    std::string ag, bg;
    if (a.isMenuItem()) ag = a.asMenuItem().group;
    else ag = a.asSubmenuItem().group;
    if (b.isMenuItem()) bg = b.asMenuItem().group;
    else bg = b.asSubmenuItem().group;

    return ag.compare(bg) > 0;
  }
};

void Menu::build() {
  menu_groups_.clear();
  auto menu_items = MenuRegistry::instance()->getMenuItems(id_);
  std::sort(menu_items.begin(), menu_items.end(), MenuItemCompare());

  std::string last_group_name = "";

  for (auto& item : menu_items) {
    std::string group_name;
    if (item.isMenuItem()) {
      group_name = item.asMenuItem().group;
    }
    else if (item.isSubmenuItem()) {
      group_name = item.asSubmenuItem().group;
    }
    if (last_group_name == "" || last_group_name != group_name) {
      menu_groups_.push_back({group_name, {}});
      last_group_name = group_name;
    }
    auto& group = menu_groups_.back();
    group.second.push_back(item);
  }
}

std::vector<MenuItemActionGroup> Menu::getActions() {
  std::vector<std::pair<std::string, std::vector<MenuItemActionable>>> result;
  for (auto group : menu_groups_) {
    auto& [id, items] = group;
    auto it = result.insert(result.end(), {id, {}});
    auto& vec = it->second;
    for (auto& item : items) {
      // TODO : Check ContextKeyService
      if (item.isMenuItem()) {
        auto action = MenuItemActionable::createMenuItemAction(
            item.asMenuItem().command,
            context_key_service_, command_service_);
        vec.push_back(action);
      }
      else {
        auto action = MenuItemActionable::createSubmenuItemAction(
          item.asSubmenuItem());
        vec.push_back(action);
      }
    }
    if (vec.size() == 0) result.erase(it);
  }
  return result;
}

}