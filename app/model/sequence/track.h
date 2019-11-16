#ifndef NLIVE_TRACK_H_
#define NLIVE_TRACK_H_

#include <set>
#include <QObject>
#include <QSharedPointer>
#include <QDebug>
#include <QMetaObject>
#include <QSharedPointer>

#include "base/common/sig.h"
#include "platform/undo/undo_stack.h"
#include "model/sequence/clip.h"
#include "renderer/video_renderer/command_buffer.h"
#include "renderer/audio_renderer/command_buffer.h"

namespace nlive {

class Track : public QObject, public Sig {
  Q_OBJECT

private:
  sptr<IUndoStack> undo_stack_;

  Rational time_base_;
  int sample_rate_;

  std::set<QSharedPointer<Clip>, ClipCompare> clips_;
  std::set<QSharedPointer<Clip>, ClipStartCompare> clip_start_ordered_set_;
  std::set<QSharedPointer<Clip>, ClipEndCompare> clip_end_ordered_set_;
  std::set<QSharedPointer<Clip>, ClipCompare> detached_clips_;
  std::map<QSharedPointer<Clip>, std::vector<sig2_conn_t>, ClipCompare> clip_connections_;
  
  void doAddClip(QSharedPointer<Clip> clip);
  void doRemoveClip(QSharedPointer<Clip> clip);
  void doClearTime(int start_time, int end_time);
  void doAttachClip(QSharedPointer<Clip> clip);
  void doDetachClip(QSharedPointer<Clip> clip);

  void handleOnDidChangeClipTime(QSharedPointer<Clip> clip, int old_start_time, int old_end_time, int old_b_time);

  void doInvalidate();

public:
  Track(sptr<IUndoStack> undo_stack, Rational time_base, int sample_rate);

  void addClip(QSharedPointer<Clip> clip);
  void removeClip(QSharedPointer<Clip> clip);
  void attachClip(QSharedPointer<Clip> clip);
  void detachClip(QSharedPointer<Clip> clip);
  bool hasClip(QSharedPointer<Clip> clip) const;
  QSharedPointer<Clip> getClipAt(int64_t time);
  std::vector<QSharedPointer<Clip>> getClipsBetween(int64_t from, int64_t to);

  QSharedPointer<Clip> getNextClip(QSharedPointer<Clip> clip);
  QSharedPointer<Clip> getPrevClip(QSharedPointer<Clip> clip);

  int64_t getClipBTimecodeOffset(int64_t timecode, QSharedPointer<Clip> clip) const;
  int64_t audioFrameToTimecode(int64_t frame) const;
  int64_t timecodeToAudioFrame(int64_t timecode) const;

  void invalidate();

  void render(QSharedPointer<video_renderer::CommandBuffer> command_buffer, int64_t time);
  void renderAudio(QSharedPointer<audio_renderer::CommandBuffer> command_buffer, int64_t start_frame, int64_t end_frame);

  const std::set<QSharedPointer<Clip>, ClipCompare>& clips();

  sptr<IUndoStack> undo_stack();
  inline Rational const& time_base() const { return time_base_; }
  inline int sample_rate() const { return sample_rate_; }

  sig2_t<void (QSharedPointer<Clip>)> onDidAddClip;
  sig2_t<void (QSharedPointer<Clip>)> onWillRemoveClip;
  sig2_t<void (QSharedPointer<Clip>,
    int /*old_start_time*/, int /*old_end_time*/, int /*old_b_time*/)> onDidChangeClipTime;
  sig2_t<void (void)> onInvalidate;

signals:

};

struct TrackCompare {
  using is_transparent = void;
  inline bool operator() (const QSharedPointer<Track>& a, const QSharedPointer<Track>& b) const
  { return a.get() < b.get(); }
  inline bool operator() (const QSharedPointer<Track>& a, const Track* b) const
  { return a.get() < b; }
  inline bool operator() (const Track* a, const QSharedPointer<Track>& b) const
  { return a < b.get(); }
};

}

#endif