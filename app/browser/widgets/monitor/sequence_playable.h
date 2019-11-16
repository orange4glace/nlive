#ifndef NLIVE_MONTIOR_WIDGET_SEQUENCE_PLAYABLE_H_
#define NLIVE_MONTIOR_WIDGET_SEQUENCE_PLAYABLE_H_

#include <QSharedPointer>
#include <QObject>
#include <QTimer>
#include "base/common/sig.h"
#include "browser/services/play/playable.h"
#include "model/sequence/sequence.h"

#include "renderer/video_renderer/sequence_renderer.h"
#include "renderer/audio_renderer/sequence_renderer.h"

namespace nlive {

namespace monitor_widget {

class SequencePlayable : public QObject, public Playable, public Sig {

private:
  QSharedPointer<Sequence> sequence_;
  QTimer* invalidation_timer_;
  bool invalidated_;

  video_renderer::SequenceRenderer* sequence_video_renderer_;
  audio_renderer::SequenceRenderer* audio_sequence_renderer_;

  int64_t started_timecode_;
  int64_t playing_timecode_;

  void onInvalidate();

public:
  SequencePlayable(QObject* parent, QSharedPointer<Sequence> sequence);
  ~SequencePlayable();

  void initializeVideoRenderer(QOpenGLContext* context);

  void beforePlayStartCallback() override;
  void playStartCallback() override;
  void playingCallback(int64_t elapsed_time) override;
  void playStopCallback() override;

  inline video_renderer::SequenceRenderer* sequence_video_renderer() { return sequence_video_renderer_; };

};

}

}

#endif