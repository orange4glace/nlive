#include "model/sequence/track.h"

#include <QDebug>

namespace nlive {

Track::Track(QUndoStack* undo_stack) :
  undo_stack_(undo_stack) {

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
  auto sptr = *clips_.find(clip);
  onWillRemoveClip(sptr);
  clips_.erase(sptr);
  clip_start_ordered_set_.erase(sptr);
  clip_end_ordered_set_.erase(sptr);
}

void Track::doClearTime(int start_time, int end_time) {
  // TODO : Improve time complexity
  qDebug() << "Clear " << start_time << " " << end_time << "\n";
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

void Track::render(QSharedPointer<video_renderer::CommandBuffer> command_buffer, int64_t time) {
  auto clip = getClipAt(time);
  if (clip == nullptr) return;
  clip->render(command_buffer, time);
}

void Track::invalidate() {
  doInvalidate();
}

const std::set<QSharedPointer<Clip>, ClipCompare>& Track::clips() {
  return clips_;
}

QUndoStack* Track::undo_stack() {
  return undo_stack_;
}

}