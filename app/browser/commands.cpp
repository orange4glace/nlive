#include "browser/commands.h"

#include "platform/action/actions.h"
#include "browser/services/projects/projects_service.h"
#include "platform/logger/logger.h"

namespace nlive {

BrowserCommand::BrowserCommand(std::string id, ICommandOptions opts) :
  id_(id),
  menu_bar_opts_(opts.menu_bar_opts) {
  
}
  
void BrowserCommand::regist() {
  if (menu_bar_opts_) {
    MenuRegistry::instance()->appendMenuItem(menu_bar_opts_->menu_id,
        MenuItemable::createMenuItem(ICommandAction(id_, menu_bar_opts_->title),
        menu_bar_opts_->group, menu_bar_opts_->order));
  }
  CommandRegistry::instance()->registerCommand(ICommand{
    id_,
    [this](sptr<ServiceLocator> service_locator, ICommandParam* param) {
      runCommand(service_locator, param);
    }
  });
}

void BrowserCommand::install() {
  {
    auto command = new SaveCommand();
    command->regist();
  }
}

SaveCommand::SaveCommand() :
  BrowserCommand(ID, {
    ICommandMenubarOptions{
      "File",
      "project",
      0,
      L"Save"
    }
  }) {

}

void SaveCommand::runCommand(sptr<ServiceLocator> service_locator, ICommandParam* param) {
  auto projects_service = service_locator->getService<IProjectsService>(IProjectsService::ID);
  spdlog::get(LOGGER_DEFAULT)->info("{}", projects_service->target_project() ? "Exist" : "Non-exist");
}

}