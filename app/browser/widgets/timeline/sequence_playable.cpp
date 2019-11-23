#include "browser/widgets/timeline/sequence_playable.h"

namespace nlive {

SequencePlayable::SequencePlayable(QObject* parent, QSharedPointer<Sequence> sequence) :
    sequence_(sequence), invalidated_(false), tick_flag_(false),
    sequence_video_renderer_(nullptr), audio_sequence_renderer_(nullptr) {
  invalidation_timer_ = new QTimer();

  audio_sequence_renderer_ = new audio_renderer::SequenceRenderer(sequence);

  invalidation_timer_->setInterval(30);
  QObject::connect(invalidation_timer_, &QTimer::timeout, [this]() {
    if (invalidated_) {
      onInvalidate();
      invalidated_ = false;
    }
  });
  sequence->onInvalidate.connect(SIG2_TRACK(sig2_t<void ()>::slot_type([this]() {
    if (playing() && !tick_flag_) stopSignal();
    invalidated_ = true;
  })));
  invalidation_timer_->start();
}

SequencePlayable::~SequencePlayable() {
  if (audio_sequence_renderer_) delete audio_sequence_renderer_;
  delete invalidation_timer_;
}

void SequencePlayable::setVideoRenderer(video_renderer::SequenceRenderer* renderer) {
  sequence_video_renderer_ = renderer;
}

void SequencePlayable::onInvalidate() {
  if (playing()) return;
  if (sequence_video_renderer_) sequence_video_renderer_->render();
  audio_sequence_renderer_->render();
}

void SequencePlayable::beforePlayStartCallback() {
  audio_sequence_renderer_->start();
}

void SequencePlayable::playStartCallback() {
  started_timecode_ = sequence_->current_time();
  playing_timecode_ = -1;
}

void SequencePlayable::playingCallback(int64_t elapsed_time) {
  int64_t elapsed_timecode = elapsed_time * sequence_->base_time() / 1000;
  int64_t timecode = started_timecode_ + elapsed_timecode;
  if (playing_timecode_ != timecode) {
    tick_flag_ = true;
    sequence_->setCurrentTime(started_timecode_ + elapsed_timecode);
    tick_flag_ = false;
    if (sequence_video_renderer_) sequence_video_renderer_->render();
    playing_timecode_ = timecode;
  }
}

void SequencePlayable::playStopCallback() {
  audio_sequence_renderer_->stop();
  invalidated_ = true;
}

}