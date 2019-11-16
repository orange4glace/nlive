#ifndef NLIVE_BROWSER_PLAYABLE_H_
#define NLIVE_BROWSER_PLAYABLE_H_

namespace nlive {

class Playable {

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
  Playable() = default;

public:
  virtual inline void beforePlayStartCallback() {}
  virtual inline void playStartCallback() {}
  virtual inline void playingCallback(int64_t elapsed_time) {}
  virtual inline void playStopCallback() {}

  inline bool playing() const { return playing_; }

};

}

#endif