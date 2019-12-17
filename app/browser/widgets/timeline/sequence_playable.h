#ifndef NLIVE_MONTIOR_WIDGET_SEQUENCE_PLAYABLE_H_
#define NLIVE_MONTIOR_WIDGET_SEQUENCE_PLAYABLE_H_

#include "base/common/memory.h"
#include <QObject>
#include <QTimer>
#include "base/common/sig.h"
#include "browser/services/play/playable.h"
#include "model/sequence/sequence.h"

#include "renderer/video_renderer/sequence_renderer.h"
#include "renderer/audio_renderer/sequence_renderer.h"

namespace nlive {

class SequencePlayable : public Playable {

private:
  sptr<Sequence> sequence_;
  QTimer* invalidation_timer_;
  bool invalidated_;

  sig2_conn_t stop_conn_;
  bool tick_flag_;

  video_renderer::SequenceRenderer* sequence_video_renderer_;
  audio_renderer::SequenceRenderer* audio_sequence_renderer_;

  int64_t started_timecode_;
  int64_t playing_timecode_;

  void onInvalidate();

public:
  SequencePlayable(QObject* parent, sptr<Sequence> sequence);
  ~SequencePlayable();

  void setVideoRenderer(video_renderer::SequenceRenderer* renderer);

  void beforePlayStartCallback() override;
  void playStartCallback() override;
  void playingCallback(int64_t elapsed_time) override;
  void playStopCallback() override;

  sptr<Sequence> sequence();
  inline video_renderer::SequenceRenderer* sequence_video_renderer() { return sequence_video_renderer_; };

};

}

#endif