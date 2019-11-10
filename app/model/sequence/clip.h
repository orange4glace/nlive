#ifndef NLIVE_CLIP_H_
#define NLIVE_CLIP_H_

#include <vector>
#include <QObject>
#include <QDebug>
#include <QSharedPointer>

#include "base/common/sig.h"
#include "platform/undo/undo_stack.h"
#include "model/common/rational.h"
#include "model/effect/transform_effect.h"
#include "renderer/video_renderer/command_buffer.h"
#include "renderer/audio_renderer/command_buffer.h"

namespace nlive {

namespace effect {

class Effect;

}

class Clip : public QObject, public Sig {
  Q_OBJECT

private:
  int id_;

protected:
  sptr<IUndoStack> undo_stack_;

  Rational time_base_;

  int64_t start_time_;
  int64_t end_time_;
  int64_t b_time_;

  std::vector<QSharedPointer<effect::Effect>> effects_;
  std::map<QSharedPointer<effect::Effect>, sig2_conn_t> effect_conn_;
  // Every clip has EXACTLY 1 TransformEffect
  QSharedPointer<effect::TransformEffect> transform_effect_;

public:
  Clip(sptr<IUndoStack> undo_stack, Rational time_base, int64_t start_time, int64_t end_time, int64_t b_time);
  Clip(const Clip&);

  void setTime(int64_t start_time, int64_t end_time, int64_t b_time);
  void addEffect(QSharedPointer<effect::Effect> effect);

  virtual void render(QSharedPointer<video_renderer::CommandBuffer> command_buffer, int64_t time);
  virtual void renderAudio(QSharedPointer<audio_renderer::CommandBuffer> command_buffer, int64_t time);

  QSharedPointer<effect::TransformEffect> transform();

  int64_t start_time() const;
  int64_t end_time() const;
  int64_t b_time() const;

  const std::vector<QSharedPointer<effect::Effect>>& effects();

  int id() const;

  sptr<IUndoStack> undo_stack();

  virtual QSharedPointer<Clip> clone() const;

  bool operator<(const Clip& rhs) const;

  sig2_t<void (int64_t /*old_start_time*/, int64_t /*old_end_time*/,
    int64_t /*old_b_time*/)> onDidChangeTime;
  sig2_t<void (void)> onDidUpdate;

signals:
  void onDidAddEffect(QSharedPointer<effect::Effect> effect);


};

QDebug operator<<(QDebug dbg, const Clip &type);



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
struct ClipCompare {
  using is_transparent = void;
  inline bool operator() (const QSharedPointer<Clip>& a, const QSharedPointer<Clip>& b) const
  { return a.get() < b.get(); }
  inline bool operator() (const QSharedPointer<Clip>& a, const Clip* b) const
  { return a.get() < b; }
  inline bool operator() (const Clip* a, const QSharedPointer<Clip>& b) const
  { return a < b.get(); }
};

}

#endif