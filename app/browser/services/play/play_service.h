#ifndef NLIVE_BROWSER_PLAY_SERVICE_H_
#define NLIVE_BROWSER_PLAY_SERVICE_H_

#include <QSharedPointer>
#include <QObject>
#include <QMetaObject>
#include <QTimer>
#include <QElapsedTimer>
#include "browser/services/play/playable.h"

namespace nlive {

class PlayService : public QObject {
  Q_OBJECT

private:
  QObject* parent_;
  QTimer* timer_;
  QElapsedTimer elapsed_timer_;
  QMetaObject::Connection timer_conn_;

  Playable* current_playable_;

  int64_t started_time_;

  bool playing_;

public:
  PlayService(QObject* parent);
  void play(Playable* playable);
  void stop();

  inline bool playing() const { return playing_; }

};

}

#endif