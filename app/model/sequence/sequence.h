#ifndef _NLIVE_SEQUENCE_H_
#define _NLIVE_SEQUENCE_H_

#include <QObject>
#include <QUndoStack>
#include <QSharedPointer>
#include <vector>

#include "model/common/timebase.h"
#include "model/sequence/track.h"

namespace nlive {

class Sequence : public QObject {
  Q_OBJECT

private:
  QUndoStack* undo_stack_;
  Timebase timebase_;
  int duration_;

  std::vector<QSharedPointer<Track>> tracks_;

  QSharedPointer<Track> doAddTrack();
  void doRemoveTrackAt(int index);

public:
  Sequence(QUndoStack* undo_stack, int basetime);

  QSharedPointer<Track> addTrack();
  void removeTrackAt(int index);
  QSharedPointer<Track> getTrackAt(int index);

  void setTimebase(Timebase timebase);
  const Timebase& timebase() const;
  int basetime() const;

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