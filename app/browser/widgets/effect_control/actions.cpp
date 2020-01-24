#include "browser/widgets/effect_control/actions.h"

#include "platform/service/service_locator.h"
#include "browser/services/widgets/widgets_service.h"

namespace nlive {

namespace effect_control {

EffectControlCommand::EffectControlCommand(std::string id, ICommandOptions opts) :
  BrowserCommand(id, opts) {

}

void EffectControlCommand::runCommand(sptr<ServiceLocator> service_locator, ICommandParam* param) {
  auto widgets_service = service_locator->getService<IWidgetsService>(IWidgetsService::ID);
  auto focused_widget = widgets_service->focused_widget();
  if (!focused_widget) return;
  if (focused_widget->id() != EffectControlWidget::ID) return;
  auto effect_control_widget = static_cast<EffectControlWidget*>(focused_widget);
  runEffectControlCommand(effect_control_widget, service_locator);
}

DeleteKeyframesCommand::DeleteKeyframesCommand() :
  EffectControlCommand(ID, {}) {

}

void DeleteKeyframesCommand::runEffectControlCommand(
    EffectControlWidget* widget, sptr<ServiceLocator> service_locator) {
  auto kf_ctrl = widget->keyframes_controller();
  kf_ctrl->remove();
}

void registerEffectControlContributions() {
  { auto command = new DeleteKeyframesCommand(); command->regist(); }
}

}

}