#include "model/sequence/clip.h"

#include "model/effect/effect.h"
#include "platform/undo/undo_stack.h"
#include "renderer/video_renderer/simple_render_command.h"
#include "renderer/video_renderer/video_clip_render_command.h"
#include "renderer/video_renderer/command_buffer.h"

namespace nlive {

namespace {
  int __next_id = 0;
}

Clip::Clip(sptr<IUndoStack> undo_stack, Rational time_base, int sample_rate, int64_t start_time, int64_t end_time, int64_t b_time) :
  undo_stack_(undo_stack), time_base_(time_base), sample_rate_(sample_rate), start_time_(start_time), end_time_(end_time), b_time_(b_time), id_(__next_id++) {
  sptr<effect::TransformEffect> transform_effect = 
    sptr<effect::TransformEffect>(new effect::TransformEffect());
  addEffect(transform_effect);
  transform_effect_ = transform_effect;
}

Clip::Clip(const Clip& clip) :
  Clip(clip.undo_stack_, clip.time_base_, clip.sample_rate_, clip.start_time_, clip.end_time_, clip.b_time_) {

}

void Clip::setTime(int64_t start_time, int64_t end_time, int64_t b_time) {
  int64_t old_start_time = start_time_;
  int64_t old_end_time = end_time_;
  int64_t old_b_time = b_time_;
  start_time_ = start_time;
  end_time_ = end_time;
  b_time_ = b_time;
  onDidChangeTime(old_start_time, old_end_time, old_b_time);
  onDidUpdate();
}

void Clip::addEffect(sptr<effect::Effect> effect) {
  effects_.push_back(effect);
  auto conn = effect->onDidUpdate.connect(sig2_t<void (void)>::slot_type(
    [this] { onDidUpdate(); }));
  effect_conn_.insert(std::make_pair(effect, conn));
  emit onDidAddEffect(effect);
  onDidUpdate();
}

sptr<effect::TransformEffect> Clip::transform() {
  return transform_effect_;
}

void Clip::render(sptr<video_renderer::CommandBuffer> command_buffer, int64_t time) {}
void Clip::renderAudio(sptr<audio_renderer::CommandBuffer> command_buffer, int64_t start_frame, int64_t end_frame) {}

int64_t Clip::start_time() const { return start_time_; }
int64_t Clip::end_time() const { return end_time_; }
int64_t Clip::b_time() const { return b_time_; }

const std::vector<sptr<effect::Effect>>& Clip::effects() { return effects_; }

int Clip::id() const { return id_; }

sptr<IUndoStack> Clip::undo_stack() {
  return undo_stack_;
}

sptr<Clip> Clip::clone() const {
  return sptr<Clip>(new Clip(*this));
}

QDebug operator<<(QDebug dbg, const Clip &clip) {
    dbg.nospace() << "Clip (start = " << clip.start_time() << ", end = " << clip.end_time() << ", b = " << clip.b_time() << ")\n";
    return dbg.maybeSpace();
}

}