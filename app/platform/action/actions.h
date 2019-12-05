#ifndef NLIVE_PLATFORM_ACTIONS_H_
#define NLIVE_PLATFORM_ACTIONS_H_

#include <any>
#include <optional>
#include "base/common/memory.h"
#include "base/common/sig.h"
#include "base/common/actions.h"
#include "platform/service/service.h"
#include "platform/context_key/context_key.h"
#include "platform/commands/commands.h"

namespace nlive {

struct ICommandAction {
  ICommandAction(std::string id, std::wstring title);

  const std::string id;
  const std::wstring title;
};

struct IMenuItem {
  IMenuItem(ICommandAction command, std::string group, int order);

  const ICommandAction command;
  const std::string group;
  const int order;
};

struct ISubmenuItem {
  ISubmenuItem(std::wstring title, std::string submenu, std::string group, int order);  

  const std::wstring title;
  const std::string submenu;
  const std::string group;
  const int order;
};

struct MenuItemable {
  std::string type;
  std::any item;

  static MenuItemable createMenuItem(ICommandAction command, std::string group, int order);
  static MenuItemable createSubmenuItem(std::wstring title, std::string submenu, std::string group, int order);

  bool isMenuItem() const;
  bool isSubmenuItem() const;
  IMenuItem asMenuItem() const;
  ISubmenuItem asSubmenuItem() const;
};

typedef std::pair<std::string, std::vector<MenuItemable>> MenuItemGroup;

class ExecuteCommandAction : public Action {

private:
  sptr<ICommandService> command_service_;

public:
  ExecuteCommandAction(std::string id, std::wstring label,
      sptr<ICommandService> command_service);

  virtual void run(IActionRunParam* param = nullptr) override;

};

class SubmenuItemAction : public Action {

public:
  const ISubmenuItem item;

  SubmenuItemAction(ISubmenuItem item);

};

class MenuItemAction : public ExecuteCommandAction {

private:
  sptr<IContextKeyService> context_key_service_;
  sptr<ICommandService> command_service_;

public:
  const ICommandAction item;

  MenuItemAction(
    ICommandAction item,
    sptr<IContextKeyService> context_key_service,
    sptr<ICommandService> command_service);

  void run(IActionRunParam* param = nullptr) override;

};

struct MenuItemActionable {
  int type;
  std::any item;

  bool isSubmenuItemAction() const;
  bool isMenuItemAction() const;
  sptr<SubmenuItemAction> asSubmenuItemAction();
  sptr<MenuItemAction> asMenuItemAction();

  static MenuItemActionable createSubmenuItemAction(ISubmenuItem submenu_item);
  static MenuItemActionable createMenuItemAction(ICommandAction command_action,
    sptr<IContextKeyService> context_key_service,
    sptr<ICommandService> command_service);
};

typedef std::pair<std::string, std::vector<MenuItemActionable>> MenuItemActionGroup;

class IMenu {

public:
  virtual std::vector<MenuItemActionGroup> getActions() = 0;
  sig2_t<void ()> onDidChange;

};

class IMenuService : public IService {

public:
  inline static std::string ID = "nlive.platform.service.MenuService";

  virtual sptr<IMenu> createMenu(std::string id, sptr<IContextKeyService>) = 0;

  inline std::string service_id() const override { return ID; }

};

class MenuRegistry : public Sig {

private:
  MenuRegistry();
  static sptr<MenuRegistry> instance_;

  std::map<std::string, ICommandAction> commands_;
  std::map<std::string, std::vector<MenuItemable>> menu_items_;

public:
  static sptr<MenuRegistry> instance();

  void addCommand(ICommandAction command);
  std::optional<ICommandAction> getCommand(std::string id);
  void appendMenuItem(std::string menu_id, MenuItemable item);
  const std::vector<MenuItemable>& getMenuItems(std::string menu_id);

  sig2_t<void ()> onDidChangeMenu;

};

}

#endif