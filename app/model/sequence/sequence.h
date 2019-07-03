#ifndef _NLIVE_SEQUENCE_H_
#define _NLIVE_SEQUENCE_H_

#include <QObject>
#include <vector>
#include "model/sequence/track.h"

namespace nlive {

class Sequence : public QObject {
  Q_OBJECT

private:
  int basetime_;
  int duration_;

  std::vector<Track*> tracks_;

  Track* const doAddTrack();
  void doRemoveTrackAt(int index);

public:
  Sequence();

  Track* const addTrack();
  void removeTrackAt(int index);
  Track* const getTrackAt(int index);

  void setBasetime(int value);
  int basetime() const;

  void setDuration(int value);
  int duration() const;

  const std::vector<Track*>& tracks();
  int track_size() const;

signals:
  void onDidAddTrack(Track* const track, int index);
  void onWillRemoveTrack(Track* const track, int index);

};

}

#endif