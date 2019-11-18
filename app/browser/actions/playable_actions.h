#ifndef NLIVE_BROWSER_PLAYABLE_ACTIONS_H_
#define NLIVE_BROWSER_PLAYABLE_ACTIONS_H_

#include <QObject>
#include <QAction>
#include <QSharedPointer>
#include "base/ui/svg_sprite.h"
#include "browser/services/play/playable.h"

namespace nlive {

class PlayablePlayPauseAction : public QAction {

private:
  QSharedPointer<Playable> playable_;

public:
  inline PlayablePlayPauseAction(QObject* parent, QSharedPointer<Playable> playable) :
    QAction(parent), playable_(playable) {
  }

  inline void setPlayable(QSharedPointer<Playable> playable) {
    playable_ = playable;
    setEnabled(playable != nullptr);
  }

};

}

#endif