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

void PlayService::play(Playable* playable) {
  if (playing_) stop();
  current_playable_ = playable;
  playing_ = true;
  current_playable_->beforePlayStartCallback();
  started_time_ = elapsed_timer_.elapsed();
  current_playable_->play();
  current_playable_->playStartCallback();
  timer_conn_ = connect(timer_, &QTimer::timeout, this, [this]() {
    int64_t elapsed_time = elapsed_timer_.elapsed() - started_time_;
    current_playable_->playingCallback(elapsed_time);
  });
  timer_->start();
}

void PlayService::stop() {
  if (!playing_) return;
  timer_->stop();
  current_playable_->stop();
  playing_ = false;
  disconnect(timer_conn_);
}
  
}