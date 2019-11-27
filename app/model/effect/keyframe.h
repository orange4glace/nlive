#ifndef NILVE_EFFECT_KEYFRAME_H_
#define NILVE_EFFECT_KEYFRAME_H_

#include <stdint.h>
#include <string>
#include "base/common/serialize.h"

namespace nlive {

namespace effect {

template <class T>
class Keyframe {
  
private:
  Keyframe() = default;
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & time_ & value_;
  }

  int64_t time_;
  T value_;

public:
  Keyframe(int64_t time, const T&& value) :
    time_(time), value_(value) {
  }

  Keyframe(int64_t time, const T& value) :
    time_(time), value_(value) {

  }

  void setValue(const T& value) {
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