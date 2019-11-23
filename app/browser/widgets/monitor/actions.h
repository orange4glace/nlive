#ifndef NLIVE_MONTIOR_WIDGET_ACTIONS_H_
#define NLIVE_MONTIOR_WIDGET_ACTIONS_H_

#include <QDebug>
#include <QObject>
#include <QAction>
#include "base/common/memory.h"
#include "base/common/sig.h"
#include "model/sequence/sequence.h"
#include "browser/widgets/timeline/sequence_playable.h"
#include "browser/services/play/play_service.h"

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

class Action : public QAction, public Sig {

protected:
  sptr<ActionContext> context_;

public:
  inline Action(QObject* parent, sptr<ActionContext> context) :
    QAction(parent), context_(context) {

  }

};

class PlayPauseAction : public Action {

private:
  sptr<PlayService> play_service_;
  sptr<SequencePlayable> playable_;

public:
  inline PlayPauseAction(QObject* parent,
    sptr<ActionContext> context,
    sptr<PlayService> play_service) :
    Action(parent, context), play_service_(play_service), playable_(nullptr) {
    setEnabled(playable_ != nullptr);
    context->onDidChangeSequencePlayable.connect(SIG2_TRACK(
      sig2_t<void (sptr<SequencePlayable>)>::slot_type([this](sptr<SequencePlayable> playable) {
        playable_ = playable;
        setEnabled(playable_ != nullptr);
      })
    ));

    connect(this, &QAction::triggered, [this]() {
      Q_ASSERT(playable_ != nullptr);
      play_service_->toggle(playable_);
    });
  }

};

}

}

#endif