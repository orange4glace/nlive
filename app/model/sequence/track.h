#ifndef _NLIVE_TRACK_H_
#define _NLIVE_TRACK_H_

#include <set>
#include <QObject>
#include "model/sequence/clip.h"

namespace nlive {

class Track : public QObject {
  Q_OBJECT

private:
  std::set<Clip*> clips_;
  
  void doAddClip(Clip* const clip);
  void doRemoveClip(Clip* const clip);

public:
  Track();

  void addClip(Clip* const clip);
  void removeClip(Clip* const clip);
  bool hasClip(Clip* const clip) const;

  const std::set<Clip*>& clips();

signals:
  void onDidAddClip(Clip* const clip);
  void onWillRemoveClip(Clip* const clip);

};

}

#endif