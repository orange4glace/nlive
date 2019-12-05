#ifndef NLIVE_PLATFORM_COMMANDS_H_
#define NLIVE_PLATFORM_COMMANDS_H_

#include <functional>
#include <optional>
#include "base/common/memory.h"
#include "base/common/disposable.h"
#include "platform/service/service_locator.h"
#include "platform/service/service.h"

namespace nlive {

struct ICommandParam {

};

typedef std::function<void (sptr<ServiceLocator>, ICommandParam*)> ICommandHandler;

struct ICommand {
  ICommand(std::string id, ICommandHandler);
  const std::string id;
  const sptr<ICommandHandler> handler;
};

class ICommandService : public IService {

public:
  inline static const std::string ID = "nlive.platform.service.CommandService";

  virtual void executeCommand(std::string command_id, ICommandParam* param) = 0;

  inline std::string service_id() const override { return ID; }

};

class CommandRegistry {

private:
  static sptr<CommandRegistry> instance_;
  CommandRegistry() = default;

  std::map<std::string, ICommand> commands_;

public:
  static sptr<CommandRegistry> instance();

  sptr<IDisposable> registerCommand(std::string id,
      std::function<void (sptr<ServiceLocator>, ICommandParam*)> handler);
  sptr<IDisposable> registerCommand(ICommand command);

  std::optional<ICommand> getCommand(std::string id);

};

}

#endif