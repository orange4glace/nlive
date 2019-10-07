#ifndef NILVE_EFFECT_PROPERTY_H_
#define NILVE_EFFECT_PROPERTY_H_

#include <QObject>
#include <stdint.h>
#include <map>
#include <string>

#include "base/common/sig.h"
#include "model/effect/keyframe.h"

namespace nlive {

namespace effect {

template <class T>
class Property {
  
private:
  std::string type_;
  T default_value_;
  std::map<int64_t, Keyframe<T>> keyframes_;

  bool animatable_;
  bool animated_;

public:
  Property(T default_value, bool animatable = true) :
    default_value_(default_value), animatable_(animatable),
    animated_(false) {

  }

  void setDefaultValue(T value) {
    default_value = value;
  }

  T getInterpolatedValue(int64_t time) {
    if (keyframes_.size() == 0) return default_value_;
    auto next = keyframes_.lower_bound(time);
    if (next == keyframes_.end()) {
      Keyframe<T> const& kf = (*(keyframes_.begin())).second;
      return kf.value();
    }
    if (next == keyframes_.begin()) {
      return (*next).second.value();
    }
    auto prev = std::prev(next);
    auto& prev_value = (*prev).second.value();
    auto prev_time = (*prev).first;
    auto& next_value = (*next).second.value();
    auto next_time = (*prev).first;
    double t = (double)(time - prev_time) / (next_time - prev_time);
    return T::interpolate(prev_value, next_value, t);
  }

  void setAnimatable(bool value) {
    animatable_ = value;
    emit onDidChangeAnimatable(value);
  }

  const std::string& type() const {
    return type_;
  }

  const T& default_value() const {
    return default_value_;
  }

  bool animatable() const {
    return animatable_;
  }

  bool animated() const {
    return animated_;
  }

  sig2_t<void (bool)> onDidChangeAnimated;
  sig2_t<void (bool)> onDidChangeAnimatable;

};

}

}

#endif