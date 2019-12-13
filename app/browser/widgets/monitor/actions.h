#ifndef NLIVE_MONTIOR_WIDGET_ACTIONS_H_
#define NLIVE_MONTIOR_WIDGET_ACTIONS_H_

#include <QDebug>
#include <QObject>
#include <QAction>
#include <QWidget>
#include "base/common/memory.h"
#include "base/common/sig.h"
#include "base/common/actions.h"
#include "base/layout/action_bar.h"
#include "model/sequence/sequence.h"
#include "browser/widgets/timeline/sequence_playable.h"
#include "browser/services/play/play_service.h"
#include "platform/include.h"

namespace nlive {

namespace monitor_widget {

class ActionContext : public Sig {

private:
  sptr<Sequence> sequence_;
  sptr<SequencePlayable> sequence_playable_;

public:
  inline ActionContext() {
    sequence_ = nullptr;
    sequence_playable_ = nullptr;
  }

  inline void setSequence(sptr<Sequence> sequence) {
    sequence_ = sequence;
    onDidChangeSequence(sequence);
  }

  inline void setSequencePlayable(sptr<SequencePlayable> playable) {
    sequence_playable_ = playable;
    onDidChangeSequencePlayable(playable);
  }

  sig2_t<void (sptr<Sequence>)> onDidChangeSequence;
  sig2_t<void (sptr<SequencePlayable>)> onDidChangeSequencePlayable;

};

class MonitorWidgetAction : public Action {

protected:
  sptr<ActionContext> context_;

public:
  MonitorWidgetAction(std::string id, std::wstring label,
      sptr<ActionContext> context);

};

class ToggleAction : public MonitorWidgetAction {

private:
  sptr<PlayService> play_service_;
  sptr<SequencePlayable> playable_;

public:
  ToggleAction(sptr<ActionContext> context, sptr<PlayService> play_service);

  void run(IActionRunParam* param) override;

};

}

}

#endif