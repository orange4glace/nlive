#include "model/sequence/clip.h"

#include "model/effect/effect.h"

#include "renderer/video_renderer/simple_render_command.h"
#include "renderer/video_renderer/command_buffer.h"

namespace nlive {

namespace {
  int __next_id = 0;
}

Clip::Clip(QUndoStack* undo_stack, Rational time_base, int64_t start_time, int64_t end_time, int64_t b_time) :
  undo_stack_(undo_stack), time_base_(time_base), start_time_(start_time), end_time_(end_time), b_time_(b_time), id_(__next_id++) {

}

Clip::Clip(const Clip& clip) :
  Clip(clip.undo_stack_, clip.time_base_, clip.start_time_, clip.end_time_, clip.b_time_) {

}

void Clip::setTime(int64_t start_time, int64_t end_time, int64_t b_time) {
  int64_t old_start_time = start_time_;
  int64_t old_end_time = end_time_;
  int64_t old_b_time = b_time_;
  start_time_ = start_time;
  end_time_ = end_time;
  b_time_ = b_time;
  emit onDidChangeTime(old_start_time, old_end_time, old_b_time);
}

void Clip::addEffect(QSharedPointer<effect::Effect> effect) {
  effects_.push_back(effect);
  emit onDidAddEffect(effect);
}

void Clip::render(QSharedPointer<video_renderer::CommandBuffer> command_buffer, int64_t time) {
  video_renderer::SimpleRenderCommand* cmd = 
    new video_renderer::SimpleRenderCommand(0, 0, 0, -0.5f, -0.5f);
  command_buffer->addCommand(cmd);
}

int64_t Clip::start_time() const { return start_time_; }
int64_t Clip::end_time() const { return end_time_; }
int64_t Clip::b_time() const { return b_time_; }

const std::vector<QSharedPointer<effect::Effect>>& Clip::effects() { return effects_; }

int Clip::id() const { return id_; }

QUndoStack* Clip::undo_stack() {
  return undo_stack_;
}

QSharedPointer<Clip> Clip::clone() const {
  return QSharedPointer<Clip>(new Clip(*this));
}

QDebug operator<<(QDebug dbg, const Clip &clip) {
    dbg.nospace() << "Clip (start = " << clip.start_time() << ", end = " << clip.end_time() << ", b = " << clip.b_time() << ")\n";
    return dbg.maybeSpace();
}

}