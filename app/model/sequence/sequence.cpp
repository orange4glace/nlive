#include "model/sequence/sequence.h"

#include "platform/logger/logger.h"
#include "platform/undo/undo_stack.h"
#include "renderer/video_renderer/renderer.h"
#include "renderer/video_renderer/command_buffer.h"
#include "renderer/video_renderer/simple_render_command.h"

#include <cstdlib>
#include <ctime>

namespace nlive {

namespace {
  int z = 0;
}

Sequence::Sequence(sptr<IUndoStack> undo_stack, int base_time, int sample_rate) :
  undo_stack_(undo_stack), time_base_(1, base_time), sample_rate_(sample_rate), 
  current_time_(0), invalidated_(false), width_(1920), height_(1080) {

  invalidateTimer_ = new QTimer();
  connect(invalidateTimer_, &QTimer::timeout, this, [this]() {
    if (invalidated_) {
      invalidated_ = false;
      doMakeDirty();
    }
  });
  invalidateTimer_->setInterval(33);
  invalidateTimer_->start();

}

QSharedPointer<video_renderer::CommandBuffer> Sequence::renderVideo(int64_t timecode) {
  QSharedPointer<video_renderer::CommandBuffer> command_buffer =
    QSharedPointer<video_renderer::CommandBuffer>(new video_renderer::CommandBuffer());
  for (auto track : tracks_) {
    track->render(command_buffer, timecode);
  }
  return command_buffer;
}

QSharedPointer<audio_renderer::CommandBuffer> Sequence::renderAudio(int64_t start_frame, int64_t end_frame) {
  QSharedPointer<audio_renderer::CommandBuffer> command_buffer =
    QSharedPointer<audio_renderer::CommandBuffer>(new audio_renderer::CommandBuffer());
  for (auto track : tracks_) {
    track->renderAudio(command_buffer, start_frame, end_frame);
  }
  return command_buffer;
}

void Sequence::doMakeDirty() {
  onDirty();
}

void Sequence::doInvalidate() {
  onInvalidate();
  invalidated_ = true;
}

QSharedPointer<Track> Sequence::addTrack() {
  return doAddTrack();
}

QSharedPointer<Track> Sequence::doAddTrack() {
  QSharedPointer<Track> track = QSharedPointer<Track>(new Track(undo_stack_, time_base_, sample_rate_));
  tracks_.push_back(track);
  std::vector<sig2_conn_t> connections;
  connections.push_back(track->onDidAddClip.connect(sig2_t<void (QSharedPointer<Clip>)>::slot_type(
    [this, track] (QSharedPointer<Clip> clip) {
    handleDidAddClip(track, clip);
  })));
  connections.push_back(track->onInvalidate.connect(sig2_t<void (void)>::slot_type(
    [this] () {
    doInvalidate();
  })));
  track_connections_.insert({track, connections});
  onDidAddTrack(track, tracks_.size() - 1);
  return track;
}

void Sequence::handleDidAddClip(QSharedPointer<Track> track, QSharedPointer<Clip> clip) {
  if (clip->end_time() > duration_) {
    doSetDuration(clip->end_time() + Rational::rescale(5000, Rational(1, 1000), time_base_));
  }
}

void Sequence::removeTrackAt(int index) {
  doRemoveTrackAt(index);
}

void Sequence::doRemoveTrackAt(int index) {
  Q_ASSERT(!(index < 0 || tracks_.size() >= index));
  // if (index < 0 || tracks_.size() >= index) {
  //   spdlog::get(LOGGER_DEFAULT)->warn(
  //     "[Sequence] Failed to doRemoveTrackAt. Track ID = {}, index = {}", id_, index);
  //   return;
  // }
  QSharedPointer<Track> track = tracks_[index];
  onWillRemoveTrack(track, index);
  auto connections = track_connections_.find(track);
  Q_ASSERT(connections != track_connections_.end());
  for (auto& connection : connections->second) connection.disconnect();
  track_connections_.erase(connections);
  tracks_.erase(tracks_.begin() + index);
}

void Sequence::doSetCurrentTime(int64_t value) {
  int64_t old = current_time_;
  current_time_ = value;
  doInvalidate();
  onDidChangeCurrentTime(old);
}

void Sequence::doSetDuration(int64_t value) {
  int64_t old = duration_;
  duration_ = value;
  doInvalidate();
  onDidChangeDuration(old);
}

QSharedPointer<Track> Sequence::getTrackAt(int index) {
  Q_ASSERT (0 <= index && index < tracks_.size());
  return tracks_[index];
}

void Sequence::setTimeBase(Rational time_base) {
  time_base_ = time_base;
}

void Sequence::setCurrentTime(int64_t value) {
  doSetCurrentTime(value);
}

void Sequence::setDuration(int64_t value) {
  doSetDuration(value);
} 

int64_t Sequence::getClipBTimecodeOffset(QSharedPointer<Clip> clip) const {
  return current_time_ - clip->start_time() + clip->b_time();
}

void Sequence::invalidate() {
  doInvalidate();
}

const std::string& Sequence::id() {
  return id_;
}

const Rational& Sequence::time_base() const {
  return time_base_;
}

int Sequence::base_time() const {
  return time_base_.den();
}

int64_t Sequence::current_time() const {
  return current_time_;
}

int64_t Sequence::duration() const {
  return duration_;
}

int Sequence::width() const {
  return width_;
}

int Sequence::height() const {
  return height_;
}

const std::vector<QSharedPointer<Track>>& Sequence::tracks() {
  return tracks_;
}

int Sequence::track_size() const {
  return tracks_.size();
}

sptr<IUndoStack> Sequence::undo_stack() {
  return undo_stack_;
};

}