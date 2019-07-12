#ifndef _NLIVE_CLIP_H_
#define _NLIVE_CLIP_H_

#include <QObject>
#include <QDebug>
#include <QSharedPointer>
#include <QUndoStack>

#include "model/common/timebase.h"

namespace nlive {

class Clip : public QObject {
  Q_OBJECT

private:
  QUndoStack* undo_stack_;
  int id_;

  Timebase timebase_;

  int start_time_;
  int end_time_;
  int b_time_;

signals:
  void onDidChangeTime(int old_start_time, int old_end_time, int old_b_time);

public:
  Clip(QUndoStack* undo_stack, Timebase timebase, int start_time, int end_time, int b_time);
  Clip(const Clip&);

  void setTime(int start_time, int end_time, int b_time);

  int start_time() const;
  int end_time() const;
  int b_time() const;
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