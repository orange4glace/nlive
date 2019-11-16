#include "browser/widgets/monitor/sequence_playable.h"

namespace nlive {

namespace monitor_widget {

SequencePlayable::SequencePlayable(QObject* parent, QSharedPointer<Sequence> sequence) :
    QObject(parent), sequence_(sequence), invalidated_(false),
    sequence_video_renderer_(nullptr), audio_sequence_renderer_(nullptr) {
  invalidation_timer_ = new QTimer(this);

  audio_sequence_renderer_ = new audio_renderer::SequenceRenderer(sequence);

  invalidation_timer_->setInterval(30);
  QObject::connect(invalidation_timer_, &QTimer::timeout, [this]() {
    if (invalidated_) {
      onInvalidate();
      invalidated_ = false;
    }
  });
  sequence->onInvalidate.connect(SIG2_TRACK(sig2_t<void ()>::slot_type([this]() {
    invalidated_ = true;
  })));
  invalidation_timer_->start();
}

SequencePlayable::~SequencePlayable() {
  if (sequence_video_renderer_) delete sequence_video_renderer_;
  if (audio_sequence_renderer_) delete audio_sequence_renderer_;
}

void SequencePlayable::initializeVideoRenderer(QOpenGLContext* context) {
  sequence_video_renderer_ =
      new video_renderer::SequenceRenderer(sequence_, context);
  sequence_video_renderer_->initialize();
}

void SequencePlayable::onInvalidate() {
  if (playing()) return;
  sequence_video_renderer_->render();
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
  int64_t elapsed_timecode = elapsed_time / sequence_->base_time();
  // qDebug() << elapsed_time << sequence_->base_time() << started_timecode_ << elapsed_timecode;
  int64_t timecode = started_timecode_ + elapsed_timecode;
  if (playing_timecode_ != timecode) {
    sequence_->setCurrentTime(started_timecode_ + elapsed_timecode);
    sequence_video_renderer_->render();
    playing_timecode_ = timecode;
  }
}

void SequencePlayable::playStopCallback() {
  audio_sequence_renderer_->stop();
}

}

}