#include "browser/menu_bar/menu_bar_control.h"

namespace nlive {

MenuBarControl::MenuBarControl(
    sptr<IMenuService> menu_service,
    sptr<IContextKeyService> context_key_service) :
  created_(false),
  menu_service_(menu_service), context_key_service_(context_key_service) {
  menus_["File"] = menu_service->createMenu("File", context_key_service);
  top_level_titles_["File"] = L"&File";
}

void MenuBarControl::updateMenuBar() {
  doUpdateMenuBar(false);
}

}