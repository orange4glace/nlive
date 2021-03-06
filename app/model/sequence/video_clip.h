#ifndef NLIVE_VIDEO_CLIP_H_
#define NLIVE_VIDEO_CLIP_H_

#include "base/common/memory.h"
#include "model/sequence/clip.h"
#include "model/resource/video_resource.h"
#include "model/resource/audio_resource.h"
#include "decoder/video_decoder.h"
#include "renderer/video_renderer/video_render_command.h"

namespace nlive {

class VideoClip : public Clip {

private:
  VideoClip() = default;
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & boost::serialization::base_object<Clip>(*this);
    ar & video_resource_ & audio_resource_;
  }

  sptr<VideoResource> video_resource_;
  sptr<AudioResource> audio_resource_;

public:
  VideoClip(sptr<IUndoStack> undo_stack, sptr<VideoResource> video_resource, sptr<AudioResource> audio_resource,
      Rational time_base, int sample_rate, int64_t start_time, int64_t end_time, int64_t b_time);
  VideoClip(const VideoClip&);

  void render(sptr<video_renderer::CommandBuffer> command_buffer, int64_t timecode) override;
  void renderAudio(sptr<audio_renderer::CommandBuffer> command_buffer, int64_t start_frame, int64_t end_frame) override;

  sptr<Clip> clone() const override;

};

}

#endif