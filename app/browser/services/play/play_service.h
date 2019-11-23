#ifndef NLIVE_BROWSER_PLAY_SERVICE_H_
#define NLIVE_BROWSER_PLAY_SERVICE_H_

#include "base/common/memory.h"
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

  sptr<Playable> current_playable_;
  sig2_conn_t current_playable_stop_conn_;

  int64_t started_time_;

  bool playing_;

public:
  PlayService(QObject* parent);
  void play(sptr<Playable> playable);
  void toggle(sptr<Playable> playable);
  void stop();
  void stop(sptr<Playable> playable);

  inline sptr<Playable> current_playable() { return current_playable_; }
  inline bool playing() const { return playing_; }

};

}

#endif