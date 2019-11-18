#ifndef NLIVE_SEQUENCE_H_
#define NLIVE_SEQUENCE_H_

#include <QObject>
#include <QString>
#include <QMetaObject>
#include <QSharedPointer>
#include <QTimer>
#include <map>
#include <vector>

#include "base/common/sig.h"
#include "platform/undo/undo_stack.h"
#include "model/common/rational.h"
#include "model/sequence/track.h"
#include "renderer/video_renderer/command_buffer.h"
#include "renderer/audio_renderer/command_buffer.h"

namespace nlive {

namespace video_renderer {
class Renderer;
class CommandBuffer;
}

class Sequence : public QObject {
  Q_OBJECT

private:
  std::string id_;
  QString name_;
  sptr<IUndoStack> undo_stack_;
  Rational time_base_;
  int sample_rate_;
  int64_t current_time_;
  int64_t duration_;

  bool invalidated_;

  int width_;
  int height_;

  std::vector<QSharedPointer<Track>> tracks_;
  std::map<QSharedPointer<Track>, std::vector<sig2_conn_t>, TrackCompare> track_connections_;

  QSharedPointer<Track> doAddTrack();
  void doRemoveTrackAt(int index);
  void doSetCurrentTime(int64_t value);
  void doSetDuration(int64_t value);

  void handleDidAddClip(QSharedPointer<Track> track, QSharedPointer<Clip> clip);

  void doInvalidate();

public:
  Sequence(sptr<IUndoStack> undo_stack, int base_time, int sample_rate);

  QSharedPointer<Track> addTrack();
  void removeTrackAt(int index);
  QSharedPointer<Track> getTrackAt(int index);

  void setTimeBase(Rational time_base);
  void setCurrentTime(int64_t value);
  void setDuration(int64_t value);

  int64_t getClipBTimecodeOffset(QSharedPointer<Clip> clip) const;

  QSharedPointer<video_renderer::CommandBuffer> renderVideo(int64_t timecode);
  QSharedPointer<audio_renderer::CommandBuffer> renderAudio(int64_t start_frame, int64_t end_frame);

  void invalidate();

  const std::string& id();
  inline const QString& name() { return name_; }
  const Rational& time_base() const;
  int base_time() const;
  inline int sample_rate() const { return sample_rate_; }
  int64_t current_time() const;
  int64_t duration() const;
  int64_t duration_in_seconds() const;
  int width() const;
  int height() const;
  const std::vector<QSharedPointer<Track>>& tracks();
  int track_size() const;
  sptr<IUndoStack> undo_stack();

  sig2_t<void (QSharedPointer<Track> track, int /*index*/)> onDidAddTrack;
  sig2_t<void (QSharedPointer<Track>, int /*index*/)> onWillRemoveTrack;
  sig2_t<void (int64_t /*old_current_time*/)> onDidChangeCurrentTime;
  sig2_t<void (int64_t /*old_duration*/)> onDidChangeDuration;
  sig2_t<void ()> onInvalidate;

};

}

#endif