#ifndef _NLIVE_SEQUENCE_H_
#define _NLIVE_SEQUENCE_H_

#include <QObject>
#include <QUndoStack>
#include <QSharedPointer>
#include <vector>

#include "model/common/rational.h"
#include "model/sequence/track.h"

namespace nlive {

class Sequence : public QObject {
  Q_OBJECT

private:
  QUndoStack* undo_stack_;
  Rational time_base_;
  int duration_;

  std::vector<QSharedPointer<Track>> tracks_;

  QSharedPointer<Track> doAddTrack();
  void doRemoveTrackAt(int index);

public:
  Sequence(QUndoStack* undo_stack, int base_time);

  QSharedPointer<Track> addTrack();
  void removeTrackAt(int index);
  QSharedPointer<Track> getTrackAt(int index);

  void setTimeBase(Rational time_base);
  const Rational& time_base() const;
  int base_time() const;

  void setDuration(int value);
  int duration() const;

  const std::vector<QSharedPointer<Track>>& tracks();
  int track_size() const;

  QUndoStack* undo_stack();

signals:
  void onDidAddTrack(QSharedPointer<Track> track, int index);
  void onWillRemoveTrack(QSharedPointer<Track> track, int index);

};

}

#endif