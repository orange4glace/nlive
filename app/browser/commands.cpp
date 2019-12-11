#include "browser/commands.h"

#include <QDialog>
#include <QFileDialog>
#include "platform/action/actions.h"
#include "platform/encoder/encoding_service_impl.h"
#include "platform/logger/logger.h"
#include "model/storage/sequence_storage_item.h"
#include "browser/services/projects/projects_service.h"
#include "browser/services/sequences/sequences_service.h"

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
  { auto command = new SaveCommand(); command->regist(); }
  { auto command = new EncodeCommand(); command->regist(); }
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

class EncodeDialog : public QDialog {

private:
  sptr<EncodingTask> task_;

public:
  EncodeDialog(sptr<EncodingTask> task) :
    task_(task) {

  }

};

EncodeCommand::EncodeCommand() :
  BrowserCommand(ID, {
    ICommandMenubarOptions{
      "File",
      "project",
      0,
      L"Encode"
    }
  }) {

}

void EncodeCommand::runCommand(sptr<ServiceLocator> service_locator, ICommandParam* param) {
  auto sequences_service = service_locator->getService<SequencesService>(SequencesService::ID);
  auto encoding_service = service_locator->getService<IEncodingService>(IEncodingService::ID);
  auto ssi = sequences_service->target_sequence();
  if (!ssi) return;
  auto sequence = ssi->sequence();
  auto filename = QFileDialog::getSaveFileName().toStdWString();
  qDebug() << encoding_service.get() << ssi.get() << sequence.get();
  encoding_service->encode(sequence, filename);
}

}