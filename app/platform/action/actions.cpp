#include "platform/action/actions.h"

#include <assert.h>

namespace nlive {

ICommandAction::ICommandAction(std::string _id, std::wstring _title) :
  id(_id), title(_title) {}

IMenuItem::IMenuItem(ICommandAction _command, std::string _group, int _order) :
  command(_command), group(_group), order(_order) {}

ISubmenuItem::ISubmenuItem(std::wstring _title, std::string _submenu, std::string _group, int _order) :
  title(_title), submenu(_submenu), group(_group), order(_order) {}

MenuItemable MenuItemable::createMenuItem(ICommandAction command, std::string group, int order) {
  MenuItemable itemable;
  itemable.item = IMenuItem(command, group, order);
  itemable.type = "MenuItem";
  return itemable;
}

MenuItemable MenuItemable::createSubmenuItem(std::wstring title, std::string submenu, std::string group, int order) {
  MenuItemable itemable;
  itemable.item = ISubmenuItem(title, submenu, group, order);
  itemable.type = "SubmenuItem";
  return itemable;
}

bool MenuItemable::isMenuItem() const {
  return type == "MenuItem";
}

bool MenuItemable::isSubmenuItem() const {
  return type == "SubmenuItem";
}

IMenuItem MenuItemable::asMenuItem() const {
  assert(type == "MenuItem");
  return std::any_cast<IMenuItem>(item);
}

ISubmenuItem MenuItemable::asSubmenuItem() const {
  assert(type == "SubmenuItem");
  return std::any_cast<ISubmenuItem>(item);
}

ExecuteCommandAction::ExecuteCommandAction(std::string id, std::wstring label,
      sptr<ICommandService> command_service) : 
  Action(id, label), command_service_(command_service) {

}

void ExecuteCommandAction::run(IActionRunParam* param) {
  command_service_->executeCommand(id(), nullptr);
}

SubmenuItemAction::SubmenuItemAction(ISubmenuItem item) :
  Action("submenu", item.title), item(item) {

}

MenuItemAction::MenuItemAction(ICommandAction item,
    sptr<IContextKeyService> context_key_service,
    sptr<ICommandService> command_service) :
  ExecuteCommandAction(item.id, item.title, command_service), item(item),
  context_key_service_(context_key_service), command_service_(command_service) {
  // TODO
}

void MenuItemAction::run(IActionRunParam* param) {
  ExecuteCommandAction::run(param);
}

bool MenuItemActionable::isSubmenuItemAction() const {
  return type == 0;
}

bool MenuItemActionable::isMenuItemAction() const {
  return type == 1;
}

sptr<SubmenuItemAction> MenuItemActionable::asSubmenuItemAction() {
  assert(type == 0);
  return std::any_cast<sptr<SubmenuItemAction>>(item);
}

sptr<MenuItemAction> MenuItemActionable::asMenuItemAction() {
  assert(type == 1);
  return std::any_cast<sptr<MenuItemAction>>(item);
}

MenuItemActionable MenuItemActionable::createSubmenuItemAction(ISubmenuItem submenu_item) {
  MenuItemActionable actionable;
  actionable.type =  0;
  actionable.item = std::make_shared<SubmenuItemAction>(submenu_item);
  return actionable;
}

MenuItemActionable MenuItemActionable::createMenuItemAction(ICommandAction command_action,
  sptr<IContextKeyService> context_key_service,
  sptr<ICommandService> command_service) {
  MenuItemActionable actionable;
  actionable.type =  1;
  actionable.item = std::make_shared<MenuItemAction>(
    command_action, context_key_service, command_service);
  return actionable;
}



sptr<MenuRegistry> MenuRegistry::instance_ = 
  sptr<MenuRegistry>(new MenuRegistry());

MenuRegistry::MenuRegistry() {

}

sptr<MenuRegistry> MenuRegistry::instance() {
  return instance_;
}

void MenuRegistry::addCommand(ICommandAction command) {
  commands_.insert({command.id, command});
}

std::optional<ICommandAction> MenuRegistry::getCommand(std::string id) {
  auto it = commands_.find(id);
  if (it == commands_.end()) return {};
  return it->second;
}

void MenuRegistry::appendMenuItem(std::string menu_id, MenuItemable item) {
  auto array_it = menu_items_.find(menu_id);
  if (array_it == menu_items_.end()) {
    auto p = menu_items_.insert({menu_id, {}});
    array_it = p.first;
  }
  array_it->second.push_back(item);
}

const std::vector<MenuItemable>& MenuRegistry::getMenuItems(std::string menu_id) {
  auto array_it = menu_items_.find(menu_id);
  // TODO : C4172
  if (array_it == menu_items_.end()) return {};
  return array_it->second;
}

}

