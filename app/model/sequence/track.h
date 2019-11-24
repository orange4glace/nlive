#ifndef NLIVE_TRACK_H_
#define NLIVE_TRACK_H_

#include <set>
#include <QObject>
#include "base/common/memory.h"
#include <QDebug>
#include <QMetaObject>
#include "base/common/memory.h"
#include "base/common/serialize.h"
#include "base/common/sig.h"
#include "platform/undo/undo_stack.h"
#include "model/sequence/clip.h"
#include "renderer/video_renderer/command_buffer.h"
#include "renderer/audio_renderer/command_buffer.h"

namespace nlive {

class Track : public QObject, public Sig {
  Q_OBJECT

private:
  Track() = default;
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const {
    ar & time_base_ & sample_rate_;
    ar & clips_;
  }
  template<class Archive>
  void load(Archive & ar, const unsigned int version) {
    ar & time_base_ & sample_rate_;
    std::set<sptr<Clip>, ClipCompare> clips;
    ar & clips;
    for (auto clip : clips) doAddClip(clip);
  }
  BOOST_SERIALIZATION_SPLIT_MEMBER()

  sptr<IUndoStack> undo_stack_;

  Rational time_base_;
  int sample_rate_;

  std::set<sptr<Clip>, ClipCompare> clips_;
  std::set<sptr<Clip>, ClipStartCompare> clip_start_ordered_set_;
  std::set<sptr<Clip>, ClipEndCompare> clip_end_ordered_set_;
  std::set<sptr<Clip>, ClipCompare> detached_clips_;
  std::map<sptr<Clip>, std::vector<sig2_conn_t>, ClipCompare> clip_connections_;
  
  void doAddClip(sptr<Clip> clip);
  void doRemoveClip(sptr<Clip> clip);
  void doClearTime(int start_time, int end_time);
  void doAttachClip(sptr<Clip> clip);
  void doDetachClip(sptr<Clip> clip);

  void handleOnDidChangeClipTime(sptr<Clip> clip, int old_start_time, int old_end_time, int old_b_time);

  void doInvalidate();

public:
  Track(sptr<IUndoStack> undo_stack, Rational time_base, int sample_rate);

  void addClip(sptr<Clip> clip);
  void removeClip(sptr<Clip> clip);
  void attachClip(sptr<Clip> clip);
  void detachClip(sptr<Clip> clip);
  bool hasClip(sptr<Clip> clip) const;
  sptr<Clip> getClipAt(int64_t time);
  sptr<Clip> getClipByID(int id);
  std::vector<sptr<Clip>> getClipsBetween(int64_t from, int64_t to);

  sptr<Clip> getNextClip(sptr<Clip> clip);
  sptr<Clip> getPrevClip(sptr<Clip> clip);

  int64_t getClipBTimecodeOffset(int64_t timecode, sptr<Clip> clip) const;
  int64_t audioFrameToTimecode(int64_t frame) const;
  int64_t timecodeToAudioFrame(int64_t timecode) const;

  void invalidate();

  void render(sptr<video_renderer::CommandBuffer> command_buffer, int64_t time);
  void renderAudio(sptr<audio_renderer::CommandBuffer> command_buffer, int64_t start_frame, int64_t end_frame);

  const std::set<sptr<Clip>, ClipCompare>& clips();

  sptr<IUndoStack> undo_stack();
  inline Rational const& time_base() const { return time_base_; }
  inline int sample_rate() const { return sample_rate_; }

  sig2_t<void (sptr<Clip>)> onDidAddClip;
  sig2_t<void (sptr<Clip>)> onWillRemoveClip;
  sig2_t<void (sptr<Clip>,
    int /*old_start_time*/, int /*old_end_time*/, int /*old_b_time*/)> onDidChangeClipTime;
  sig2_t<void (void)> onInvalidate;

signals:

};

struct TrackCompare {
  using is_transparent = void;
  inline bool operator() (const sptr<Track>& a, const sptr<Track>& b) const
  { return a.get() < b.get(); }
  inline bool operator() (const sptr<Track>& a, const Track* b) const
  { return a.get() < b; }
  inline bool operator() (const Track* a, const sptr<Track>& b) const
  { return a < b.get(); }
};

}

#endif