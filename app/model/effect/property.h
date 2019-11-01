#ifndef NILVE_EFFECT_PROPERTY_H_
#define NILVE_EFFECT_PROPERTY_H_

#include <QObject>
#include <QSharedPointer>
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
  std::map<int64_t, QSharedPointer<Keyframe<T>>> keyframes_;

  bool animatable_;
  bool animated_;

  void doUpsertKeyframe(int64_t time, T& value) {
    if (!animated_) return doSetDefaultValue(value);
    auto match_it = keyframes_.find(time);
    if (match_it == keyframes_.end()) return doCreateKeyframe(time, value);
    else return doUpdateKeyframe(time, value);
  }
  
  void doCreateKeyframe(int64_t time, T& value) {
    auto match_it = keyframes_.find(time);
    Q_ASSERT(match_it == keyframes_.end());
    auto kf = QSharedPointer<Keyframe<T>>(new Keyframe<T>(time, value));
    keyframes_.insert(make_pair(time, kf));
    onDidUpdate();
    onDidAddKeyframe(kf);
  }

  void doUpdateKeyframe(int64_t time, T& value) {
    auto match_it = keyframes_.find(time);
    Q_ASSERT(match_it != keyframes_.end());
    QSharedPointer<Keyframe<T>> kf = match_it->second;
    kf->setValue(value);
    onDidUpdate();
  }

  void doSetDefaultValue(T& value) {
    default_value_ = value;
    onDidUpdate();
  }

public:
  Property(T default_value, bool animatable = true) :
    default_value_(default_value), animatable_(animatable),
    animated_(false) {

  }

  void setDefaultValue(T value) {
    default_value = value;
    onDidUpdate();
  }

  void upsertKeyframe(int64_t time, T& value) {
    doUpsertKeyframe(time, value);
  }

  T getInterpolatedValue(int64_t time) {
    if (!animated_ || keyframes_.size() == 0) {
      return default_value_;
    }
    auto next = keyframes_.lower_bound(time);
    if (next == keyframes_.end()) {
      QSharedPointer<Keyframe<T>> kf = (*(keyframes_.rbegin())).second;
      return kf->value();
    }
    if (next == keyframes_.begin()) {
      return (*next).second->value();
    }
    auto prev = std::prev(next);
    auto& prev_value = (*prev).second->value();
    auto prev_time = (*prev).first;
    auto& next_value = (*next).second->value();
    auto next_time = (*next).first;
    double t = (double)(time - prev_time) / (next_time - prev_time);
    return T::interpolate(prev_value, next_value, t);
  }

  void setAnimated(bool value) {
    animated_ = value;
    onDidChangeAnimated(value);
    onDidUpdate();
  }

  void setAnimatable(bool value) {
    animatable_ = value;
    onDidChangeAnimatable(value);
    onDidUpdate();
  }

  std::map<int64_t, QSharedPointer<Keyframe<T>>> const& keyframes() {
    return keyframes_;
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
  sig2_t<void (void)> onDidUpdate;
  sig2_t<void (QSharedPointer<Keyframe<T>>)> onDidAddKeyframe;
  sig2_t<void (QSharedPointer<Keyframe<T>>)> onWillRemoveKeyframe;
  sig2_t<void (QSharedPointer<Keyframe<T>>)> onDidChangeKeyframeTime;

};

}

}

#endif