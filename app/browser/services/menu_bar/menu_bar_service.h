// #ifndef NLIVE_MENU_BAR_SERVICE_H_
// #define NLIVE_MENU_BAR_SERVICE_H_

// #include <QAction>
// #include <QString>
// #include <QMenu>
// #include <QMenuBar>
// #include <map>
// #include "base/common/memory.h"
// #include "base/common/sig.h"
// #include "platform/service/service.h"
// #include "platform/theme/themeservice.h"
// #include "platform/context_key/context_key.h"

// namespace nlive {

// class ContextKeyAwareQAction : public Sig {

// private:
//   sptr<IContextKeyService> context_key_service_;
//   ContextKeyExprPtr precondition_;
//   QAction* action_;

// public:
//   ContextKeyAwareQAction(QAction* action, ContextKeyExprPtr precondition, 
//       sptr<IContextKeyService> context_key_service);

//   QAction* action();

// };

// struct MenuActionDescriptor {
//   std::string id;
//   QString title;
//   sptr<ContextKeyAwareQAction> action;
// };

// class Menu {

// private:
//   sptr<IContextKeyService> context_key_service_;

//   std::string id_;
//   QString title_;
//   QMenu* qmenu_;
//   std::map<std::string, MenuActionDescriptor> action_descriptors_;
//   std::map<std::string, Menu> submenus_;

// public:
//   Menu(std::string id, QString title, QMenu* qmenu, sptr<IContextKeyService> context_key_service);
//   void addAction(MenuActionDescriptor descriptor);
//   Menu* addSubmenu(std::string id, QString title);
//   MenuActionDescriptor* getAction(std::string id);

//   QMenu* qmenu();

// };

// class MenuBarService : public IService {

// private:
//   sptr<IContextKeyService> context_key_service_;

//   QMenuBar* qmenu_bar_;
//   std::map<std::string, Menu*> menus_;

// public:
//   static const std::string ID;

//   MenuBarService(QWidget* widget, sptr<IThemeService> theme_service, sptr<IContextKeyService> context_key_service);
//   MenuBarService(QMenuBar* qmenu_bar, sptr<IThemeService> theme_service, sptr<IContextKeyService> context_key_service);

//   Menu* addMenu(std::string id, QString title);
//   Menu* getMenu(std::string id);

//   QMenuBar* qmenu_bar();

//   inline std::string service_id() const override { return ID; }

// };

// }

// #endif