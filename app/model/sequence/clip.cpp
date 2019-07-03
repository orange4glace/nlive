#include "model/sequence/clip.h"

namespace nlive {

namespace {
  int __next_id = 0;
}

Clip::Clip(int start_time, int end_time):
  start_time_(start_time), end_time_(end_time), id_(__next_id++) {

}

void Clip::setTime(int start_time, int end_time) {
  int old_start_time = start_time_;
  int old_end_time = end_time_;
  start_time_ = start_time;
  end_time_ = end_time;
  emit onDidChangeTime(old_start_time, old_end_time);
}

int Clip::start_time() const { return start_time_; }
int Clip::end_time() const { return end_time_; }
int Clip::id() const { return id_; }

bool Clip::operator<(const Clip& rhs) const {
  return id_ < rhs.id_;
}

}