#include "model/sequence/track.h"

#include <QDebug>

namespace nlive {

Track::Track(sptr<IUndoStack> undo_stack, Rational time_base, int sample_rate) :
  undo_stack_(undo_stack), time_base_(time_base), sample_rate_(sample_rate) {

}

void Track::addClip(QSharedPointer<Clip> clip) {
  doAddClip(clip);
}

void Track::doAddClip(QSharedPointer<Clip> clip) {
  if (hasClip(clip)) return;
  doClearTime(clip->start_time(), clip->end_time());
  std::vector<sig2_conn_t> connections;
  auto c1 = clip->onDidChangeTime.connect(sig2_t<void (int64_t, int64_t, int64_t)>::slot_type(
    [this, clip](int64_t old_start_time, int64_t old_end_time, int64_t old_b_time) {
    handleOnDidChangeClipTime(clip, old_start_time, old_end_time, old_b_time);
  }));
  auto c2 = clip->onDidUpdate.connect(sig2_t<void (void)>::slot_type(
    [this]() {
    doInvalidate();
  }));
  connections.push_back(c1);
  connections.push_back(c2);
  clip_connections_.insert({ clip, connections });
  clips_.insert(clip);
  clip_start_ordered_set_.insert(clip);
  clip_end_ordered_set_.insert(clip);
  onDidAddClip(clip);
}

void Track::handleOnDidChangeClipTime(QSharedPointer<Clip> clip, int old_start_time, int old_end_time, int old_b_time) {
  detachClip(clip);
  doClearTime(clip->start_time(), clip->end_time());
  attachClip(clip);
  onDidChangeClipTime(clip, old_start_time, old_end_time, old_b_time); 
  doInvalidate();
}

void Track::doInvalidate() {
  onInvalidate();
}

void Track::removeClip(QSharedPointer<Clip> clip) {
  doRemoveClip(clip);
}

void Track::doRemoveClip(QSharedPointer<Clip> clip) {
  if (!hasClip(clip)) return;
  auto connections = clip_connections_.find(clip);
  Q_ASSERT(connections != clip_connections_.end());
  for (auto& connection : connections->second) connection.disconnect();
  clip_connections_.erase(connections);
  auto sp = *clips_.find(clip);
  onWillRemoveClip(sp);
  clips_.erase(sp);
  clip_start_ordered_set_.erase(sp);
  clip_end_ordered_set_.erase(sp);
}

void Track::doClearTime(int start_time, int end_time) {
  // TODO : Improve time complexity
  auto copied_clips_ = clips_;
  for (auto& clip : copied_clips_) {
    if (detached_clips_.count(clip)) continue;
    if (end_time <= clip->start_time() || clip->end_time() <= start_time) continue;
    if (start_time <= clip->start_time() && clip->end_time() <= end_time) {
      // Cut entire
      doRemoveClip(clip);
    }
    else if (start_time <= clip->start_time() && end_time < clip->end_time()) {
      // Cut left side
      qDebug() << "Cut left\n";
      clip->setTime(end_time,  clip->end_time(), clip->b_time() + (end_time - clip->start_time()));
    }
    else if (start_time <= clip->end_time() && clip->end_time() < end_time) {
      // Cut right side
      qDebug() << "Cut right " << clip << "\n";
      clip->setTime(clip->start_time(), start_time, clip->b_time());
    }
    else if (clip->start_time() < start_time && end_time < clip->end_time()) {
      // Cut both side and keep the middle
      qDebug() << "Cut both\n";
      auto left_side_clip = clip->clone();
      auto right_side_clip = clip->clone();
      left_side_clip->setTime(clip->start_time(), start_time, clip->b_time());
      right_side_clip->setTime(end_time, clip->end_time(), clip->b_time() + (end_time - clip->start_time()));
      doRemoveClip(clip);
      doAddClip(left_side_clip);
      qDebug() << *left_side_clip << " " << left_side_clip << "\n";
      qDebug() << *right_side_clip << "\n";
      doAddClip(right_side_clip);
      qDebug() << *left_side_clip << "\n";
      qDebug() << *right_side_clip << "\n";
      qDebug() << start_time << " " << end_time << "\n";
    }
  }
}

void Track::attachClip(QSharedPointer<Clip> clip) {
  doAttachClip(clip);
}

void Track::doAttachClip(QSharedPointer<Clip> clip) {
  if (!hasClip(clip)) return;
  detached_clips_.erase(clip);
}

void Track::detachClip(QSharedPointer<Clip> clip) {
  doDetachClip(clip);
}

void Track::doDetachClip(QSharedPointer<Clip> clip) {
  if (!hasClip(clip)) return;
  detached_clips_.insert(clip);
}

bool Track::hasClip(QSharedPointer<Clip> clip) const {
  return clips_.count(clip) > 0;
}
  
// TODO : Remove O(N) complexity
QSharedPointer<Clip> Track::getClipAt(int64_t time) {
  for (auto clip : clips_) {
    if (clip->start_time() <= time && time < clip->end_time()) return clip;
  }
  return nullptr;
}

// TODO : Remove O(N) complexity
std::vector<QSharedPointer<Clip>> Track::getClipsBetween(int64_t from, int64_t to) {
  std::vector<QSharedPointer<Clip>> result;
  for (auto clip : clips_) {
    if (clip->start_time() <= to && from < clip->end_time()) result.push_back(clip);
  }
  return result;
}

QSharedPointer<Clip> Track::getNextClip(QSharedPointer<Clip> clip) {
  if (!hasClip(clip)) return nullptr;
  auto it = clip_end_ordered_set_.upper_bound(clip);
  if (it == clip_end_ordered_set_.end()) return nullptr;
  return *it;
}

QSharedPointer<Clip> Track::getPrevClip(QSharedPointer<Clip> clip) {
  if (!hasClip(clip)) return nullptr;
  auto it = clip_start_ordered_set_.find(clip);
  if (it == clip_start_ordered_set_.begin()) return nullptr;
  return *std::prev(it);
}

int64_t Track::getClipBTimecodeOffset(int64_t timecode, QSharedPointer<Clip> clip) const {
  return timecode - clip->start_time() + clip->b_time();
}

int64_t Track::audioFrameToTimecode(int64_t frame) const {
  return Rational::rescale(frame, Rational(1, sample_rate_), time_base_);
}

int64_t Track::timecodeToAudioFrame(int64_t timecode) const {
  return Rational::rescale(timecode, time_base_, Rational(1, sample_rate_));
}

void Track::render(QSharedPointer<video_renderer::CommandBuffer> command_buffer, int64_t time) {
  auto clip = getClipAt(time);
  if (clip == nullptr) return;
  int64_t clip_timecode = getClipBTimecodeOffset(time, clip);
  clip->render(command_buffer, clip_timecode);
}

void Track::renderAudio(QSharedPointer<audio_renderer::CommandBuffer> command_buffer, int64_t start_frame, int64_t end_frame) {
  int64_t start_timecode = audioFrameToTimecode(start_frame);
  int64_t end_timecode = audioFrameToTimecode(end_frame);
  auto clips = getClipsBetween(start_timecode, end_timecode);
  for (auto& clip : clips) {
    int64_t b = -clip->start_time() + clip->b_time();
    int64_t b_frame = timecodeToAudioFrame(b);
    clip->renderAudio(command_buffer, start_frame + b_frame, end_frame + b_frame);
  }
}

void Track::invalidate() {
  doInvalidate();
}

const std::set<QSharedPointer<Clip>, ClipCompare>& Track::clips() {
  return clips_;
}

sptr<IUndoStack> Track::undo_stack() {
  return undo_stack_;
}

}