#include "platform/commands/commands.h"

namespace nlive {

ICommand::ICommand(std::string _id,
    ICommandHandler _handler) :
  id(_id), handler(std::make_shared<ICommandHandler>(_handler)) {
}

sptr<CommandRegistry> CommandRegistry::instance_ = sptr<CommandRegistry>(
  new CommandRegistry());

sptr<CommandRegistry> CommandRegistry::instance() { return instance_; }

sptr<IDisposable> CommandRegistry::registerCommand(std::string id,
    std::function<void (sptr<ServiceLocator>, ICommandParam*)> handler) {
  commands_.emplace(std::piecewise_construct,
  std::forward_as_tuple(id), std::forward_as_tuple(id, handler));
  return Disposable::create([this, id]() {
    commands_.erase(id);
  });
}

sptr<IDisposable> CommandRegistry::registerCommand(ICommand command) {
  auto id = command.id;
  commands_.insert({id, command});
  return Disposable::create([this, id]() {
    commands_.erase(id);
  });
}

std::optional<ICommand> CommandRegistry::getCommand(std::string id) {
  auto it = commands_.find(id);
  if (it == commands_.end()) return {};
  return it->second;
}

}