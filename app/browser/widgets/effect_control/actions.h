#ifndef NLIVE_EFFECT_CONTROL_ACTIONS_H_
#define NLIVE_EFFECT_CONTROL_ACTIONS_H_

#include "base/common/memory.h"
#include "base/common/sig.h"
#include "base/common/actions.h"
#include "browser/commands.h"
#include "browser/widgets/effect_control/effect_control_widget.h"
#include "platform/include.h"

namespace nlive {

namespace effect_control {

class EffectControlCommand : public BrowserCommand {
protected:
  EffectControlCommand(std::string id, ICommandOptions opts);

public:
  void runCommand(sptr<ServiceLocator> service_locator, ICommandParam* param) override;
  virtual void runEffectControlCommand(
      EffectControlWidget* widget, sptr<ServiceLocator> service_locator) = 0;

};

class DeleteKeyframesCommand : public EffectControlCommand {

public:
  inline static const std::string ID = "nlive.browser.command.EffectControlDeleteKeyframes";
  DeleteKeyframesCommand();
  void runEffectControlCommand(
      EffectControlWidget* widget, sptr<ServiceLocator> service_locator) override;

};

void registerEffectControlContributions();

}

}

#endif