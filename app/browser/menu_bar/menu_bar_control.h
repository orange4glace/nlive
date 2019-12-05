#ifndef NLIVE_BROWSER_MENU_BAR_MENU_BAR_CONTROL_H_
#define NLIVE_BROWSER_MENU_BAR_MENU_BAR_CONTROL_H_

#include "platform/action/actions.h"
#include "platform/context_key/context_key.h"
#include "base/common/sig.h"

namespace nlive {

class MenuBarControl : public Sig {

private:
  bool created_;

protected:
  sptr<IMenuService> menu_service_;
  sptr<IContextKeyService> context_key_service_;

  std::map<std::string, sptr<IMenu>> menus_;
  std::map<std::string, std::wstring> top_level_titles_;

  void updateMenuBar();
  virtual void doUpdateMenuBar(bool first_time) = 0;

public:
  MenuBarControl(
    sptr<IMenuService> menu_service,
    sptr<IContextKeyService> context_key_service);

};

}

#endif