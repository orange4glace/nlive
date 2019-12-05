#include "browser/services/commands/command_service.h"

#include "platform/action/actions.h"

namespace nlive {

CommandService::CommandService(sptr<ServiceLocator> service_locator) :
  service_locator_(service_locator) {}

void CommandService::executeCommand(std::string command_id, ICommandParam* param) {
  auto command = CommandRegistry::instance()->getCommand(command_id);
  if (command) {
    (*command->handler)(service_locator_, param);
  }
}

}