#include "model/sequence/audio_clip.h"

#include <QSharedPointer>

namespace nlive {

AudioClip::AudioClip(sptr<IUndoStack> undo_stack, QSharedPointer<AudioResource> audio_resource,
    Rational time_base, int64_t start_time, int64_t end_time, int64_t b_time) :
  Clip(undo_stack, time_base, start_time, end_time, b_time),
  resource_(audio_resource) {
}

AudioClip::AudioClip(const AudioClip& clip) :
  AudioClip(clip.undo_stack_, clip.resource_, clip.time_base_, clip.start_time_, clip.end_time_, clip.b_time_) {
  // TODO : Clone effects
}

void AudioClip::renderAudio(QSharedPointer<audio_renderer::CommandBuffer> command_buffer, int64_t timecode) {
  int64_t pts = Rational::rescale(timecode, time_base_, resource_->time_base());
}

QSharedPointer<Clip> AudioClip::clone() const {
  return QSharedPointer<Clip>(new AudioClip(*this));
}

}