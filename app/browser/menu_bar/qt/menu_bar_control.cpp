#include "browser/menu_bar/qt/menu_bar_control.h"

#include <QDebug>

namespace nlive {

NativeMenuBarControl::NativeMenuBarControl(
  QMainWindow* main_window,
  sptr<IMenuService> menu_service,
  sptr<IContextKeyService> context_key_service) :
  main_window_(main_window),
  MenuBarControl(menu_service, context_key_service), qmenu_bar_(nullptr) {
  doUpdateMenuBar(true);
}

void NativeMenuBarControl::install(bool first_time) {
  if (qmenu_bar_ != nullptr) delete qmenu_bar_;
  qmenu_bar_ = new QMenuBar(0);
  if (main_window_) main_window_->setMenuBar(qmenu_bar_);
  
  for (auto& kv : top_level_titles_) {
    auto menu = menus_[kv.first];
    auto qmenu = new QMenu(qmenu_bar_);
    qmenu->setTitle(QString::fromStdWString(kv.second));
    updateActions(qmenu, false, menu, kv.second);
    qmenu_bar_->addMenu(qmenu);
  }
}

void NativeMenuBarControl::doUpdateMenuBar(bool first_time) {
  install(first_time);
}

void NativeMenuBarControl::updateActions(QWidget* menubar_or_menu, bool is_menubar,
    sptr<IMenu> menu, std::wstring top_level_title) {
  auto addAction = [](QWidget* menubar_or_menu, bool is_menubar, QAction* action) {
    if (is_menubar) {
      static_cast<QMenuBar*>(menubar_or_menu)->addAction(action);
    }
    else {
      static_cast<QMenu*>(menubar_or_menu)->addAction(action);
    }
  };
  auto& groups = menu->getActions();
  for (auto& group : groups) {
    auto& id = group.first;
    auto& actions = group.second;
    for (auto& action : actions) {
      if (action.isMenuItemAction()) {
        auto item_action = action.asMenuItemAction();
        auto qaction = new QAction(menubar_or_menu);
        qaction->setText(QString::fromStdWString(item_action->label()));
        QObject::connect(qaction, &QAction::triggered, [this, item_action]() {
          item_action->run(nullptr);
        });
        addAction(menubar_or_menu, is_menubar, qaction);
      }
      else {
        auto submenu_action = action.asSubmenuItemAction();
        sptr<IMenu> submenu;
        if (menus_.count(submenu_action->item.submenu) > 0) {
          submenu = menus_[submenu_action->item.submenu];
        }
        else {
          submenu = menus_[submenu_action->item.submenu] =
            menu_service_->createMenu(submenu_action->item.submenu, context_key_service_);
          submenu->onDidChange.connect(SIG2_TRACK(sig2_t<void ()>::slot_type([this]() {
            // TODO (menubarControl.ts:596)
          })));
        }

        auto qsubmenu = new QMenu(menubar_or_menu);
        updateActions(qsubmenu, false, submenu, top_level_title);
        addAction(menubar_or_menu, is_menubar, qsubmenu->menuAction());
      }
    }
  }
  qDebug() << "[NativeMenuBarControl] Done";
}

}