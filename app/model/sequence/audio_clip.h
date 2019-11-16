#ifndef NLIVE_AUDIO_CLIP_H_
#define NLIVE_AUDIO_CLIP_H_

#include <QSharedPointer>
#include "model/sequence/clip.h"
#include "model/resource/audio_resource.h"

namespace nlive {

class AudioClip : public Clip {

private:
  QSharedPointer<AudioResource> resource_;

public:
  AudioClip(sptr<IUndoStack> undo_stack, QSharedPointer<AudioResource> audio_resource,
      Rational time_base, int sample_rate, int64_t start_time, int64_t end_time, int64_t b_time);
  AudioClip(const AudioClip&);

  void renderAudio(QSharedPointer<audio_renderer::CommandBuffer> command_buffer, int64_t start_frame, int64_t end_frame) override;

  QSharedPointer<Clip> clone() const override;

};

}

#endif