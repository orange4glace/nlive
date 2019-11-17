#ifndef NLIVE_BROWSER_PLAYABLE_H_
#define NLIVE_BROWSER_PLAYABLE_H_

#include "base/common/sig.h"

namespace nlive {

class Playable : public Sig {

friend class PlayService;

private:
  bool playing_;

  inline void play() {
    playing_ = true;
  }

  inline void stop() {
    playing_ = false;
  }

protected:
  inline Playable() : playing_(false) {}

public:
  virtual inline void beforePlayStartCallback() {}
  virtual inline void playStartCallback() {}
  virtual inline void playingCallback(int64_t elapsed_time) {}
  virtual inline void playStopCallback() {}

  inline bool playing() const { return playing_; }

  sig2_t<void ()> onPlay;
  sig2_t<void ()> onStop;

};

}

#endif