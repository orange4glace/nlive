#include "model/sequence/sequence.h"

namespace nlive {

Sequence::Sequence(QUndoStack* undo_stack, int base_time) :
  undo_stack_(undo_stack), time_base_(1, base_time) {
}

QSharedPointer<Track> Sequence::addTrack() {
  return doAddTrack();
}

QSharedPointer<Track> Sequence::doAddTrack() {
  // Stack allocation would be enough but leave it now
  QSharedPointer<Track> track = QSharedPointer<Track>(new Track(undo_stack_));
  tracks_.emplace_back(track);
  onDidAddTrack(track, tracks_.size() - 1);
  return track;
}

void Sequence::removeTrackAt(int index) {
  doRemoveTrackAt(index);
}

void Sequence::doRemoveTrackAt(int index) {
  if (index < 0 || tracks_.size() >= index) return;
  QSharedPointer<Track> track = tracks_[index];
  onWillRemoveTrack(track, index);
  tracks_.erase(tracks_.begin() + index);
}

QSharedPointer<Track> Sequence::getTrackAt(int index) {
  if (index < 0 || index >= tracks_.size()) return nullptr;
  return tracks_[index];
}

void Sequence::setTimeBase(Rational time_base) {
  time_base_ = time_base;
}

const Rational& Sequence::time_base() const {
  return time_base_;
}

int Sequence::base_time() const {
  return time_base_.num();
}

void Sequence::setDuration(int value) {
  duration_ = value;
}

int Sequence::duration() const {
  return duration_;
}

const std::vector<QSharedPointer<Track>>& Sequence::tracks() {
  return tracks_;
}

int Sequence::track_size() const {
  return tracks_.size();
}

QUndoStack* Sequence::undo_stack() {
  return undo_stack_;
};

}