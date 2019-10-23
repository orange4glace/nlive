#ifndef NLIVE_VIDEO_CLIP_H_
#define NLIVE_VIDEO_CLIP_H_

#include <QSharedPointer>
#include "model/sequence/clip.h"
#include "model/resource/video_resource.h"
#include "decoder/video_decoder.h"
#include "renderer/video_renderer/video_render_command.h"

namespace nlive {

class VideoClip : public Clip {

private:
  QSharedPointer<VideoResource> resource_;

public:
  VideoClip(QUndoStack* undo_stack, QSharedPointer<VideoResource> video_resource,
      Rational time_base, int64_t start_time, int64_t end_time, int64_t b_time);

  void render(QSharedPointer<video_renderer::CommandBuffer> command_buffer, int64_t timecode) override;

};

}

#endif