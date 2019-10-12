#include "model/sequence/video_clip.h"

#include <QSharedPointer>
#include "renderer/video_renderer/video_render_command.h"
#include "renderer/video_renderer/video_clip_render_command.h"
#include "renderer/video_renderer/simple_render_command.h"
#include "renderer/video_renderer/command_buffer.h"

namespace nlive {

VideoClip::VideoClip(QUndoStack* undo_stack, QSharedPointer<VideoResource> video_resource,
    Rational time_base, int64_t start_time, int64_t end_time, int64_t b_time) :
  Clip(undo_stack, time_base, start_time, end_time, b_time),
  resource_(video_resource) {
  decoder_ = QSharedPointer<VideoDecoder>(new VideoDecoder(video_resource->path()));
}

void VideoClip::render(QSharedPointer<video_renderer::CommandBuffer> command_buffer, int64_t timecode) {
  int64_t pts = Rational::rescale(timecode, time_base_, resource_->time_base());
  auto pre = QSharedPointer<video_renderer::VideoClipPreRenderCommand>(
      new video_renderer::VideoClipPreRenderCommand(resource_, decoder_, pts));
  auto post = QSharedPointer<video_renderer::VideoClipPostRenderCommand>(
      new video_renderer::VideoClipPostRenderCommand(resource_));
  command_buffer->addCommand(pre);
  command_buffer->addCommand(post);
}

}