#ifndef NLIVE_MENU_BAR_SERVICE_H_
#define NLIVE_MENU_BAR_SERVICE_H_

#include <QAction>
#include <QString>
#include <QMenu>
#include <QMenuBar>
#include <map>
#include "base/common/memory.h"
#include "platform/service/service.h"
#include "platform/theme/themeservice.h"

namespace nlive {

struct MenuActionDescriptor {
  std::string id;
  QString title;
  QAction* action;
};

class Menu {

private:
  std::string id_;
  QString title_;
  QMenu* qmenu_;
  std::map<std::string, MenuActionDescriptor> action_descriptors_;
  std::map<std::string, Menu> submenus_;

public:
  Menu(std::string id, QString title, QMenu* qmenu = nullptr);
  void addAction(MenuActionDescriptor descriptor);
  Menu* addSubmenu(std::string id, QString title);
  MenuActionDescriptor* getAction(std::string id);

  QMenu* qmenu();

};

class MenuBarService : public IService {

private:
  QMenuBar* qmenu_bar_;
  std::map<std::string, Menu*> menus_;

public:
  static const std::string ID;

  MenuBarService(QWidget* widget, sptr<IThemeService> theme_service);
  MenuBarService(QMenuBar* qmenu_bar, sptr<IThemeService> theme_service);

  Menu* addMenu(std::string id, QString title);
  Menu* getMenu(std::string id);

  QMenuBar* qmenu_bar();

  inline std::string service_id() const override { return ID; }

};

}

#endif