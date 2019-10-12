#ifndef NILVE_EFFECT_KEYFRAME_H_
#define NILVE_EFFECT_KEYFRAME_H_

#include <stdint.h>
#include <string>

namespace nlive {

namespace effect {

template <class T>
class Keyframe {
  
private:
  int64_t time_;
  T value_;

public:
  Keyframe(int64_t time, T&& value) :
    time_(time), value_(value) {
  }

  Keyframe(int64_t time, T& value) :
    time_(time), value_(value) {

  }

  void setValue(T& value) {
    value_ = value;
  }

  int64_t time() const {
    return time_;
  }

  const T& value() const {
    return value_;
  }

};

}

}

#endif