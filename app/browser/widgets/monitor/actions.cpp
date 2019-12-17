#include "browser/widgets/monitor/actions.h"

namespace nlive {

namespace monitor_widget {

MonitorWidgetAction::MonitorWidgetAction(std::string id, std::wstring label,
      sptr<ActionContext> context) :
  Action(id, label), context_(context), playable_(nullptr) {
  context->onDidChangeSequencePlayable.connect(SIG2_TRACK(
    sig2_t<void (sptr<SequencePlayable>)>::slot_type([this](sptr<SequencePlayable> playable) {
      playable_ = playable;
      update();
      onPlayableChange(playable);
    })
  ));
  update();
}

void MonitorWidgetAction::update() {
  setEnabled(playable_ != nullptr);
}

sptr<SequencePlayable> MonitorWidgetAction::playable() {
  return playable_;
}

void MonitorWidgetAction::onPlayableChange(sptr<SequencePlayable> playable) {}

ToggleAction::ToggleAction(sptr<ActionContext> context,
  sptr<PlayService> play_service) :
    MonitorWidgetAction("monitor_widget.action.toggle", L"", context),
  play_service_(play_service) {
  update();
}

void ToggleAction::onPlayableChange(sptr<SequencePlayable> playable) {
  auto p = playable;
  playable_conn0_.disconnect();
  playable_conn1_.disconnect();
  setEnabled(p != nullptr);
  update();
  if (p) {
    playable_conn0_ = p->onPlay.connect(SIG2_TRACK(sig2_t<void ()>::slot_type(
      [this]() { update(); })));
    playable_conn1_ = p->onStop.connect(SIG2_TRACK(sig2_t<void ()>::slot_type(
      [this]() { update(); })));
  }
}

void ToggleAction::update() {
  auto p = playable();
  if (p) {
    if (p->playing()) {
      setIcon(L"\uf04d");
    }
    else {
      setIcon(L"\uf04b");
    }
  }
  else {
    setIcon(L"\uf04b");
  }
}

void ToggleAction::run(IActionRunParam* param) {
  Q_ASSERT(playable() != nullptr);
  play_service_->toggle(playable());
}

ForwardCurrnetTimeAction::ForwardCurrnetTimeAction(sptr<ActionContext> context, bool forward) :
    MonitorWidgetAction("monitor_widget.action.forwardCurrentTime", L"", context),
    forward_(forward) {
  if (forward_) setIcon(L"\uf04e");
  else setIcon(L"\uf04a");
}

void ForwardCurrnetTimeAction::run(IActionRunParam* param) {
  Q_ASSERT(playable() != nullptr);
  auto p = playable();
  auto seq = p->sequence();
  int r = forward_ ? 1 : -1;
  seq->setCurrentTime(seq->current_time()  + r);
}

}

}