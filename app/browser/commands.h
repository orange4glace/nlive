#ifndef NLIVE_BROWSER_COMMANDS_H_
#define NLIVE_BROWSER_COMMANDS_H_

#include <string>
#include <optional>
#include "base/common/memory.h"
#include "platform/commands/commands.h"
#include "platform/service/service_locator.h"

namespace nlive {

struct ICommandMenubarOptions {
  std::string menu_id;
  std::string group;
  int order;
  std::wstring title;
};

struct ICommandOptions {
  std::optional<ICommandMenubarOptions> menu_bar_opts;
};

class BrowserCommand {

private:
  std::string id_;
  std::optional<ICommandMenubarOptions> menu_bar_opts_;

public:
  BrowserCommand(std::string id, ICommandOptions opts);

  void regist();
  virtual void runCommand(sptr<ServiceLocator> service_locator, ICommandParam* param) = 0;

  static void install();

};

class SaveCommand : public BrowserCommand {
public:
  inline static const std::string ID = "nlive.browser.command.SaveCommand";
  SaveCommand();
  void runCommand(sptr<ServiceLocator> service_locator, ICommandParam* param) override;
};

class EncodeCommand : public BrowserCommand {
public:
  inline static const std::string ID = "nlive.browser.command.Encode";
  EncodeCommand();
  void runCommand(sptr<ServiceLocator> service_locator, ICommandParam* param) override;
};

}

#endif