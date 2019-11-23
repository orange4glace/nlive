#include "model/sequence/audio_clip.h"

#include "base/common/memory.h"
#include "renderer/audio_renderer/command/audio_resource_render_command.h"

namespace nlive {

AudioClip::AudioClip(sptr<IUndoStack> undo_stack, sptr<AudioResource> audio_resource,
    Rational time_base, int sample_rate, int64_t start_time, int64_t end_time, int64_t b_time) :
  Clip(undo_stack, time_base, sample_rate, start_time, end_time, b_time),
  resource_(audio_resource) {
}

AudioClip::AudioClip(const AudioClip& clip) :
  AudioClip(clip.undo_stack_, clip.resource_, clip.time_base_, clip.sample_rate_, clip.start_time_, clip.end_time_, clip.b_time_) {
  // TODO : Clone effects
}

void AudioClip::renderAudio(sptr<audio_renderer::CommandBuffer> command_buffer, int64_t start_frame, int64_t end_frame) {
  auto cmd = sptr<audio_renderer::AudioResourceRenderCommand>(new 
    audio_renderer::AudioResourceRenderCommand(resource_, sample_rate_, id(), start_frame, end_frame));
  command_buffer->addCommand(cmd);
}

sptr<Clip> AudioClip::clone() const {
  return sptr<Clip>(new AudioClip(*this));
}

}