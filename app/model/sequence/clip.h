#ifndef _NLIVE_CLIP_H_
#define _NLIVE_CLIP_H_

#include <QObject>
#include <QDebug>
#include <QSharedPointer>
#include <QUndoStack>

#include "model/common/rational.h"

namespace nlive {

class Clip : public QObject {
  Q_OBJECT

private:
  QUndoStack* undo_stack_;
  int id_;

  Rational time_base_;

  int64_t start_time_;
  int64_t end_time_;
  int64_t b_time_;

signals:
  void onDidChangeTime(int64_t old_start_time, int64_t old_end_time, int64_t old_b_time);

public:
  Clip(QUndoStack* undo_stack, Rational time_base, int64_t start_time, int64_t end_time, int64_t b_time);
  Clip(const Clip&);

  void setTime(int64_t start_time, int64_t end_time, int64_t b_time);

  int64_t start_time() const;
  int64_t end_time() const;
  int64_t b_time() const;
  int id() const;

  QUndoStack* undo_stack();

  virtual QSharedPointer<Clip> clone() const;

  bool operator<(const Clip& rhs) const;

};

QDebug operator<<(QDebug dbg, const Clip &type);

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