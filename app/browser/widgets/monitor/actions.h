#ifndef NLIVE_MONTIOR_WIDGET_ACTIONS_H_
#define NLIVE_MONTIOR_WIDGET_ACTIONS_H_

#include <QDebug>
#include <QObject>
#include <QAction>
#include <QSharedPointer>
#include "base/common/sig.h"
#include "model/sequence/sequence.h"
#include "browser/widgets/timeline/sequence_playable.h"
#include "browser/services/play/play_service.h"

namespace nlive {

namespace monitor_widget {

class ActionContext : public Sig {

private:
  QSharedPointer<Sequence> sequence_;
  QSharedPointer<SequencePlayable> sequence_playable_;

public:
  inline ActionContext() {
    sequence_ = nullptr;
    sequence_playable_ = nullptr;
  }

  inline void setSequence(QSharedPointer<Sequence> sequence) {
    sequence_ = sequence;
    onDidChangeSequence(sequence);
  }

  inline void setSequencePlayable(QSharedPointer<SequencePlayable> playable) {
    sequence_playable_ = playable;
    onDidChangeSequencePlayable(playable);
  }

  sig2_t<void (QSharedPointer<Sequence>)> onDidChangeSequence;
  sig2_t<void (QSharedPointer<SequencePlayable>)> onDidChangeSequencePlayable;

};

class Action : public QAction, public Sig {

protected:
  QSharedPointer<ActionContext> context_;

public:
  inline Action(QObject* parent, QSharedPointer<ActionContext> context) :
    QAction(parent), context_(context) {

  }

};

class PlayPauseAction : public Action {

private:
  QSharedPointer<PlayService> play_service_;
  QSharedPointer<SequencePlayable> playable_;

public:
  inline PlayPauseAction(QObject* parent,
    QSharedPointer<ActionContext> context,
    QSharedPointer<PlayService> play_service) :
    Action(parent, context), play_service_(play_service), playable_(nullptr) {
    setEnabled(playable_ != nullptr);
    context->onDidChangeSequencePlayable.connect(SIG2_TRACK(
      sig2_t<void (QSharedPointer<SequencePlayable>)>::slot_type([this](QSharedPointer<SequencePlayable> playable) {
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