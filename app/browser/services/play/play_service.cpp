#include "browser/services/play/play_service.h"

#include <QDebug>

namespace nlive {

namespace {
  int x = 0;
}

PlayService::PlayService(QObject* parent) :
  QObject(parent), playing_(false) {
  timer_ = new QTimer(this);
  elapsed_timer_.start();
}

void PlayService::play(QSharedPointer<Playable> playable) {
  if (playing_) stop();
  current_playable_ = playable;
  current_playable_stop_conn_ =
    current_playable_->stopSignal.connect(sig2_t<void ()>::slot_type([this]() {
      stop();
    }));
  current_playable_->beforePlayStartCallback();
  started_time_ = elapsed_timer_.elapsed();
  current_playable_->play();
  current_playable_->playStartCallback();
  timer_conn_ = connect(timer_, &QTimer::timeout, this, [this]() {
    int64_t elapsed_time = elapsed_timer_.elapsed() - started_time_;
    current_playable_->playingCallback(elapsed_time);
  });
  timer_->start();
  playing_ = true;
}

void PlayService::toggle(QSharedPointer<Playable> playable) {
  if (playable == current_playable_) stop();
  else {
    stop();
    play(playable);
  }
}

void PlayService::stop(QSharedPointer<Playable> playable) {
  if (playable == current_playable_) stop();
}

void PlayService::stop() {
  if (!playing_) return;
  timer_->stop();
  current_playable_stop_conn_.disconnect();
  current_playable_->stop();
  current_playable_->playStopCallback();
  playing_ = false;
  current_playable_ = nullptr;
  disconnect(timer_conn_);
}
  
}