#include "model/sequence/sequence.h"

#include "platform/logger/logger.h"
#include "renderer/video_renderer/renderer.h"
#include "renderer/video_renderer/command_buffer.h"
#include "renderer/video_renderer/simple_render_command.h"

#include <QTimer>

#include <cstdlib>
#include <ctime>

namespace nlive {

Sequence::Sequence(QUndoStack* undo_stack, int base_time) :
  undo_stack_(undo_stack), time_base_(1, base_time), current_time_(0),
  width_(1080), height_(720) {

    srand((unsigned int)time(NULL));
  QTimer* t = new QTimer();
  connect(t, &QTimer::timeout, this, [this]() {
    auto cb = QSharedPointer<video_renderer::CommandBuffer>(new video_renderer::CommandBuffer());
    cb->addCommand(
      new video_renderer::SimpleRenderCommand(
        0, 0, 0, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX
      ));
    this->onDirty(cb);
  });
  t->setInterval(2000);
  t->start();

}

QSharedPointer<Track> Sequence::addTrack() {
  return doAddTrack();
}

QSharedPointer<Track> Sequence::doAddTrack() {
  QSharedPointer<Track> track = QSharedPointer<Track>(new Track(undo_stack_));
  tracks_.emplace_back(track);
  std::vector<QMetaObject::Connection> connections;
  connections.emplace_back(connect(track.get(), &Track::onDidAddClip, this, [this, track](QSharedPointer<Clip> clip) {
    handleDidAddClip(track, clip);
  }));
  emit onDidAddTrack(track, tracks_.size() - 1);
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
  if (index < 0 || tracks_.size() >= index) {
    spdlog::get(LOGGER_DEFAULT)->warn(
      "[Sequence] Failed to doRemoveTrackAt. Track ID = {}, index = {}", id_, index);
    return;
  }
  QSharedPointer<Track> track = tracks_[index];
  emit onWillRemoveTrack(track, index);
  tracks_.erase(tracks_.begin() + index);
}

void Sequence::doSetCurrentTime(int64_t value) {
  int64_t old = current_time_;
  current_time_ = value;
  emit onDidChangeCurrentTime(old);
}

void Sequence::doSetDuration(int64_t value) {
  int64_t old = duration_;
  duration_ = value;
  emit onDidChangeDuration(old);
}

QSharedPointer<Track> Sequence::getTrackAt(int index) {
  if (index < 0 || index >= tracks_.size()) return nullptr;
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

void Sequence::renderVideoCommandBuffer(QSharedPointer<video_renderer::CommandBuffer> command_buffer) {
  
}

void Sequence::renderVideo(QSharedPointer<video_renderer::Renderer> renderer, int64_t timecode) {
  QSharedPointer<video_renderer::CommandBuffer> command_buffer
    = QSharedPointer<video_renderer::CommandBuffer>(new video_renderer::CommandBuffer());
  renderVideoCommandBuffer(command_buffer);
}

const std::string& Sequence::id() {
  return id_;
}

const Rational& Sequence::time_base() const {
  return time_base_;
}

int Sequence::base_time() const {
  return time_base_.num();
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

QUndoStack* Sequence::undo_stack() {
  return undo_stack_;
};

}