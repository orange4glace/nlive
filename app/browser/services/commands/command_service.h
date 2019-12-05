#ifndef NLIVE_BROWSER_COMMAND_SERVICE_H_
#define NLIVE_BROWSER_COMMAND_SERVICE_H_

#include "platform/commands/commands.h"
#include "platform/service/service_locator.h"

namespace nlive {

class CommandService : public ICommandService {

private:
  sptr<ServiceLocator> service_locator_;

public:
  CommandService(sptr<ServiceLocator> service_locator);

  void executeCommand(std::string command_id, ICommandParam* param) override;

};

}

#endif