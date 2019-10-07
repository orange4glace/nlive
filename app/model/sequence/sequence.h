#ifndef _NLIVE_SEQUENCE_H_
#define _NLIVE_SEQUENCE_H_

#include <QObject>
#include <QMetaObject>
#include <QUndoStack>
#include <QSharedPointer>
#include <map>
#include <vector>

#include "model/common/rational.h"
#include "model/sequence/track.h"

namespace nlive {

namespace video_renderer {
class Renderer;
class CommandBuffer;
}

class Sequence : public QObject {
  Q_OBJECT

private:
  std::string id_;
  QUndoStack* undo_stack_;
  Rational time_base_;
  int64_t current_time_;
  int64_t duration_;

  int width_;
  int height_;

  std::vector<QSharedPointer<Track>> tracks_;
  std::map<QSharedPointer<Track>, std::vector<QMetaObject::Connection*>> track_connections_;

  QSharedPointer<Track> doAddTrack();
  void doRemoveTrackAt(int index);
  void doSetCurrentTime(int64_t value);
  void doSetDuration(int64_t value);

  void handleDidAddClip(QSharedPointer<Track> track, QSharedPointer<Clip> clip);

public:
  Sequence(QUndoStack* undo_stack, int base_time);

  QSharedPointer<Track> addTrack();
  void removeTrackAt(int index);
  QSharedPointer<Track> getTrackAt(int index);

  void setTimeBase(Rational time_base);
  void setCurrentTime(int64_t value);
  void setDuration(int64_t value);

  void renderVideoCommandBuffer(QSharedPointer<video_renderer::CommandBuffer> command_buffer);

  const std::string& id();
  const Rational& time_base() const;
  int base_time() const;
  int64_t current_time() const;
  int64_t duration() const;
  int width() const;
  int height() const;
  const std::vector<QSharedPointer<Track>>& tracks();
  int track_size() const;
  QUndoStack* undo_stack();

public slots:
  void renderVideo(QSharedPointer<video_renderer::Renderer> renderer, int64_t timecode);

signals:
  void onDidAddTrack(QSharedPointer<Track> track, int index);
  void onWillRemoveTrack(QSharedPointer<Track> track, int index);
  void onDidChangeCurrentTime(int64_t old_current_time);
  void onDidChangeDuration(int64_t old_duration);
  void onDirty(QSharedPointer<video_renderer::CommandBuffer> command_buffer);

};

}

#endif