#include "model/sequence/clip.h"

namespace nlive {

namespace {
  int __next_id = 0;
}

Clip::Clip(QUndoStack* undo_stack, Timebase timebase, int start_time, int end_time, int b_time) :
  undo_stack_(undo_stack), timebase_(timebase), start_time_(start_time), end_time_(end_time), b_time_(b_time), id_(__next_id++) {

}

Clip::Clip(const Clip& clip) :
  Clip(clip.undo_stack_, clip.timebase_, clip.start_time_, clip.end_time_, clip.b_time_) {

}

void Clip::setTime(int start_time, int end_time, int b_time) {
  int old_start_time = start_time_;
  int old_end_time = end_time_;
  int old_b_time = b_time_;
  start_time_ = start_time;
  end_time_ = end_time;
  b_time_ = b_time;
  emit onDidChangeTime(old_start_time, old_end_time, old_b_time);
}

int Clip::start_time() const { return start_time_; }
int Clip::end_time() const { return end_time_; }
int Clip::b_time() const { return b_time_; }
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