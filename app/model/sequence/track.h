#ifndef _NLIVE_TRACK_H_
#define _NLIVE_TRACK_H_

#include <set>
#include <QObject>
#include <QSharedPointer>
#include <QDebug>
#include <QUndoStack>
#include <QMetaObject>
#include <QSharedPointer>

#include "base/common/sig.h"
#include "model/sequence/clip.h"

namespace nlive {

namespace video_renderer {
class CommandBuffer;
}

struct ClipStartCompare {
  using is_transparent = void;
  inline bool operator() (const QSharedPointer<Clip>& a, const QSharedPointer<Clip>& b) const
  { int lhs = a->start_time(); int rhs = b->start_time();
    if (lhs == rhs) return a.get() < b.get();
    return lhs < rhs; }
  inline bool operator() (const QSharedPointer<Clip>& a, const Clip* b) const
  { int lhs = a->start_time(); int rhs = b->start_time();
    if (lhs == rhs) return a.get() < b;
    return lhs < rhs; }
  inline bool operator() (const Clip* a, const QSharedPointer<Clip>& b) const
  { int lhs = a->start_time(); int rhs = b->start_time();
    if (lhs == rhs) return a < b.get();
    return lhs < rhs; }
};
struct ClipEndCompare {
  using is_transparent = void;
  inline bool operator() (const QSharedPointer<Clip>& a, const QSharedPointer<Clip>& b) const
  { int lhs = a->end_time(); int rhs = b->end_time();
    if (lhs == rhs) return a.get() < b.get();
    return lhs < rhs; }
  inline bool operator() (const QSharedPointer<Clip>& a, const Clip* b) const
  { int lhs = a->end_time(); int rhs = b->end_time();
    if (lhs == rhs) return a.get() < b;
    return lhs < rhs; }
  inline bool operator() (const Clip* a, const QSharedPointer<Clip>& b) const
  { int lhs = a->end_time(); int rhs = b->end_time();
    if (lhs == rhs) return a < b.get();
    return lhs < rhs; }
};

class Track : public QObject, public Sig {
  Q_OBJECT

private:
  QUndoStack* undo_stack_;
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
  Track(QUndoStack* undo_stack);

  void addClip(QSharedPointer<Clip> clip);
  void removeClip(QSharedPointer<Clip> clip);
  void attachClip(QSharedPointer<Clip> clip);
  void detachClip(QSharedPointer<Clip> clip);
  bool hasClip(QSharedPointer<Clip> clip) const;
  QSharedPointer<Clip> getClipAt(int64_t time);

  QSharedPointer<Clip> getNextClip(QSharedPointer<Clip> clip);
  QSharedPointer<Clip> getPrevClip(QSharedPointer<Clip> clip);

  int64_t getClipBTimecodeOffset(int64_t timecode, QSharedPointer<Clip> clip) const;

  void invalidate();

  void render(QSharedPointer<video_renderer::CommandBuffer> command_buffer, int64_t time);

  const std::set<QSharedPointer<Clip>, ClipCompare>& clips();

  QUndoStack* undo_stack();

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