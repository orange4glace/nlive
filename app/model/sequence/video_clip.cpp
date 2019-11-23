#include "model/sequence/video_clip.h"

#include "base/common/memory.h"
#include "renderer/video_renderer/video_render_command.h"
#include "renderer/video_renderer/video_clip_render_command.h"
#include "renderer/video_renderer/command/transform_blit_render_command.h"
#include "renderer/video_renderer/command/mirror_render_command.h"
#include "renderer/video_renderer/command_buffer.h"
#include "model/effect/gray_scale_effect.h"

namespace nlive {

VideoClip::VideoClip(sptr<IUndoStack> undo_stack, sptr<VideoResource> video_resource,
    Rational time_base, int sample_rate, int64_t start_time, int64_t end_time, int64_t b_time) :
  Clip(undo_stack, time_base, sample_rate, start_time, end_time, b_time),
  resource_(video_resource) {
  sptr<effect::GrayScaleEffect> gse = 
      sptr<effect::GrayScaleEffect>(new effect::GrayScaleEffect());
  addEffect(gse);
}

VideoClip::VideoClip(const VideoClip& clip) :
  VideoClip(clip.undo_stack_, clip.resource_, clip.time_base_, clip.sample_rate_, clip.start_time_, clip.end_time_, clip.b_time_) {
  // TODO : Clone effects
}

void VideoClip::render(sptr<video_renderer::CommandBuffer> command_buffer, int64_t timecode) {
  int64_t pts = Rational::rescale(timecode, time_base_, resource_->time_base());
  auto position = transform_effect_->position()->getInterpolatedValue(timecode);
  auto pre = sptr<video_renderer::VideoClipPreRenderCommand>(
      new video_renderer::VideoClipPreRenderCommand(id(), resource_, pts));
  auto blit = sptr<video_renderer::TransformBlitRenderCommand>(
      new video_renderer::TransformBlitRenderCommand(position.x(), position.y()));
  auto post = sptr<video_renderer::VideoClipPostRenderCommand>(
      new video_renderer::VideoClipPostRenderCommand(pre->sharing, resource_));
  command_buffer->addCommand(pre);
  for (auto effect : effects_) effect->render(command_buffer, timecode);
  command_buffer->addCommand(blit);
  command_buffer->addCommand(post);
}

sptr<Clip> VideoClip::clone() const {
  return sptr<Clip>(new VideoClip(*this));
}

}