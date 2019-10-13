#ifndef _NLIVE_SEQUENCE_H_
#define _NLIVE_SEQUENCE_H_

#include <QObject>
#include <QMetaObject>
#include <QUndoStack>
#include <QSharedPointer>
#include <QTimer>
#include <map>
#include <vector>

#include "base/common/sig.h"
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

  bool invalidated_;
  QTimer* invalidateTimer_;

  int width_;
  int height_;

  std::vector<QSharedPointer<Track>> tracks_;
  std::map<QSharedPointer<Track>, std::vector<sig2_conn_t>, TrackCompare> track_connections_;

  QSharedPointer<Track> doAddTrack();
  void doRemoveTrackAt(int index);
  void doSetCurrentTime(int64_t value);
  void doSetDuration(int64_t value);

  void handleDidAddClip(QSharedPointer<Track> track, QSharedPointer<Clip> clip);

  void doMakeDirty();
  void doInvalidate();

public:
  Sequence(QUndoStack* undo_stack, int base_time);

  QSharedPointer<Track> addTrack();
  void removeTrackAt(int index);
  QSharedPointer<Track> getTrackAt(int index);

  void setTimeBase(Rational time_base);
  void setCurrentTime(int64_t value);
  void setDuration(int64_t value);

  int64_t getClipBTimecodeOffset(QSharedPointer<Clip> clip) const;

  void renderVideoCommandBuffer(QSharedPointer<video_renderer::CommandBuffer> command_buffer);

  void invalidate();

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

  sig2_t<void (QSharedPointer<Track> track, int /*index*/)> onDidAddTrack;
  sig2_t<void (QSharedPointer<Track>, int /*index*/)> onWillRemoveTrack;
  sig2_t<void (int64_t /*old_current_time*/)> onDidChangeCurrentTime;
  sig2_t<void (int64_t /*old_duration*/)> onDidChangeDuration;
  sig2_t<void (QSharedPointer<video_renderer::CommandBuffer> /*command_buffer*/)> onDirty;

public slots:
  void renderVideo(QSharedPointer<video_renderer::Renderer> renderer, int64_t timecode);

};

}

#endif