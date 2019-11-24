#ifndef NLIVE_AUDIO_CLIP_H_
#define NLIVE_AUDIO_CLIP_H_

#include "base/common/memory.h"
#include "model/sequence/clip.h"
#include "model/resource/audio_resource.h"

namespace nlive {

class AudioClip : public Clip {

private:
  AudioClip() = default;
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & boost::serialization::base_object<Clip>(*this);
    ar & resource_;
  }

  sptr<AudioResource> resource_;

public:
  AudioClip(sptr<IUndoStack> undo_stack, sptr<AudioResource> audio_resource,
      Rational time_base, int sample_rate, int64_t start_time, int64_t end_time, int64_t b_time);
  AudioClip(const AudioClip&);

  void renderAudio(sptr<audio_renderer::CommandBuffer> command_buffer, int64_t start_frame, int64_t end_frame) override;

  sptr<Clip> clone() const override;

};

}

#endif