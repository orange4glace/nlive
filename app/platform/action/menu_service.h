#ifndef NLIVE_PLATFORM_MENU_SERVICE_H_
#define NLIVE_PLATFORM_MENU_SERVICE_H_

#include "platform/action/actions.h"

namespace nlive {

class MenuService : public IMenuService {

private:
  sptr<ICommandService> command_service_;

public:
  MenuService(sptr<ICommandService> command_service);

  sptr<IMenu> createMenu(std::string id, sptr<IContextKeyService>) override;

};

class Menu : public IMenu, public Sig {

private:
  sptr<ICommandService> command_service_;
  sptr<IContextKeyService> context_key_service_;

  std::string id_;
  std::vector<MenuItemGroup> menu_groups_;
  std::set<std::string> context_keys_;

  void build();

public:
  Menu(std::string menu_id, sptr<ICommandService> command_service,
      sptr<IContextKeyService> context_key_service);

  std::vector<MenuItemActionGroup> getActions() override;


};

}

#endif