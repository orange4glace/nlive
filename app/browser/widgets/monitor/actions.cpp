#include "browser/widgets/monitor/actions.h"

namespace nlive {

namespace monitor_widget {

MonitorWidgetAction::MonitorWidgetAction(std::string id, std::wstring label,
      sptr<ActionContext> context) :
  Action(id, label), context_(context) {

}

ToggleAction::ToggleAction(sptr<ActionContext> context,
  sptr<PlayService> play_service) :
    MonitorWidgetAction("monitor_widget.action.toggle", L"", context),
  play_service_(play_service), playable_(nullptr) {
  setEnabled(playable_ != nullptr);
  setIcon(L"\uf04b");
  context->onDidChangeSequencePlayable.connect(SIG2_TRACK(
    sig2_t<void (sptr<SequencePlayable>)>::slot_type([this](sptr<SequencePlayable> playable) {
      playable_ = playable;
      setEnabled(playable_ != nullptr);
    })
  ));
}

void ToggleAction::run(IActionRunParam* param) {
  Q_ASSERT(playable_ != nullptr);
  play_service_->toggle(playable_);
}

}

}