#ifndef NILVE_EFFECT_PROPERTY_H_
#define NILVE_EFFECT_PROPERTY_H_

#include <QObject>
#include <stdint.h>
#include <map>
#include <string>
#include "base/common/sig.h"
#include "base/common/memory.h"
#include "base/common/serialize.h"
#include "model/effect/keyframe.h"

namespace nlive {

namespace effect {

class IProperty {

public:
  virtual void addKeyframeAt(int64_t time, sptr<IKeyframe> keyframe) = 0;
  virtual void removeKeyframe(sptr<IKeyframe> keyframe) = 0;
  virtual bool removeKeyframeAt(int64_t time) = 0;
  virtual bool moveKeyframeTo(sptr<IKeyframe> keyframe, int64_t time) = 0;
  virtual sptr<IKeyframe> getKeyframeAt(int64_t time) = 0;

  virtual void setAnimated(bool value) = 0;
  virtual void setAnimatable(bool value) = 0;

  virtual bool animatable() const = 0;
  virtual bool animated() const = 0;

  virtual std::map<int64_t, sptr<IKeyframe>> const& keyframes() = 0;

  sig2_t<void (bool)> onDidChangeAnimated;
  sig2_t<void (bool)> onDidChangeAnimatable;
  sig2_t<void (void)> onDidUpdate;

  sig2_t<void (sptr<IKeyframe>)> onDidAddKeyframe;
  sig2_t<void (sptr<IKeyframe>)> onWillRemoveKeyframe;
  sig2_t<void (sptr<IKeyframe>, int old_time)> onDidChangeKeyframeTime;
  sig2_t<void (sptr<IKeyframe>)> onDidInvalidate;

};

#define CAST_KF(V) std::static_pointer_cast<Keyframe<T>>(V)

template <class T>
class Property : public IProperty {

private:
  Property() = default;
  friend class boost::serialization::access;


  std::string type_;
  T default_value_;
  std::map<int64_t, sptr<IKeyframe>> keyframes_;

  bool animatable_;
  bool animated_;

  void doUpsertKeyframe(int64_t time, const T& value) {
    if (!animated_) return doSetDefaultValue(value);
    auto match_it = keyframes_.find(time);
    if (match_it == keyframes_.end()) return doCreateKeyframe(time, value);
    else return doUpdateKeyframe(time, value);
  }
  
  void doCreateKeyframe(int64_t time, const T& value) {
    auto match_it = keyframes_.find(time);
    Q_ASSERT(match_it == keyframes_.end());
    auto kf = sptr<Keyframe<T>>(new Keyframe<T>(time, value));
    keyframes_.insert(make_pair(time, kf));
    onDidUpdate();
    onDidAddKeyframe(kf);
  }

  void doUpdateKeyframe(int64_t time, const T& value) {
    auto match_it = keyframes_.find(time);
    Q_ASSERT(match_it != keyframes_.end());
    sptr<Keyframe<T>> kf = CAST_KF(match_it->second);
    kf->setValue(value);
    onDidUpdate();
  }

  void doSetDefaultValue(const T& value) {
    default_value_ = value;
    onDidUpdate();
  }

public:
  struct access_;
  // https://stackoverflow.com/questions/30594917/get-private-data-members-for-non-intrusive-boost-serialization-c/30595430#30595430

  Property(T default_value, bool animatable = true) :
    default_value_(default_value), animatable_(animatable),
    animated_(false) {

  }

  void setDefaultValue(T value) {
    default_value = value;
    onDidUpdate();
  }

  void addKeyframeAt(int64_t time, sptr<IKeyframe> keyframe) override {
    auto match_it = keyframes_.find(time);
    Q_ASSERT(match_it == keyframes_.end());
    auto kf = keyframe;
    kf->setTime(time);
    keyframes_.insert(make_pair(time, keyframe));
    onDidUpdate();
    onDidAddKeyframe(keyframe);
  }

  void removeKeyframe(sptr<IKeyframe> keyframe) override {
    auto match_it = keyframes_.find(keyframe->time());
    Q_ASSERT(match_it != keyframes_.end());
    Q_ASSERT(match_it->second == keyframe);
    onWillRemoveKeyframe(keyframe);
    keyframes_.erase(match_it);
  }

  bool removeKeyframeAt(int64_t time) override {
    auto it = keyframes_.find(time);
    if (it == keyframes_.end()) return false;
    onWillRemoveKeyframe(it->second);
    keyframes_.erase(it);
  }

  bool moveKeyframeTo(sptr<IKeyframe> keyframe, int64_t time) override {
    Q_ASSERT(
      keyframes_.count(keyframe->time()) &&
      keyframes_[keyframe->time()] == keyframe);
    auto it = keyframes_.find(time);
    if (it != keyframes_.end()) return false;
    int old_time = time;
    keyframes_.erase(keyframe->time());
    keyframe->setTime(time);
    keyframes_.insert(make_pair(time, keyframe));
    onDidChangeKeyframeTime(keyframe, old_time);
    onDidInvalidate(keyframe);
    return true;
  }

  sptr<IKeyframe> getKeyframeAt(int64_t time) override {
    auto it = keyframes_.find(time);
    if (it == keyframes_.end()) return nullptr;
    return it->second;
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
      sptr<Keyframe<T>> kf = CAST_KF((*(keyframes_.rbegin())).second);
      return kf->value();
    }
    if (next == keyframes_.begin()) {
      sptr<Keyframe<T>> kf = CAST_KF((*next).second);
      return kf->value();
    }
    auto prev = std::prev(next);
    auto& prev_value = CAST_KF((*prev).second)->value();
    auto prev_time = (*prev).first;
    auto& next_value = CAST_KF((*next).second)->value();
    auto next_time = (*next).first;
    double t = (double)(time - prev_time) / (next_time - prev_time);
    return T::interpolate(prev_value, next_value, t);
  }

  void setAnimated(bool value) override {
    animated_ = value;
    onDidChangeAnimated(value);
    onDidUpdate();
  }

  void setAnimatable(bool value) override {
    animatable_ = value;
    onDidChangeAnimatable(value);
    onDidUpdate();
  }

  std::map<int64_t, sptr<IKeyframe>> const& keyframes() override {
    return keyframes_;
  }

  const std::string& type() const {
    return type_;
  }

  const T& default_value() const {
    return default_value_;
  }

  bool animatable() const override {
    return animatable_;
  }

  bool animated() const override {
    return animated_;
  }

};

#undef CAST_KF

template <class T>
struct Property<T>::access_ {
  template <class Archive>
  static void save(Archive& ar, const Property<T>& t, const unsigned int version) {
    ar & t.type_ & t.default_value_ & t.animatable_ & t.animated_;
    ar & t.keyframes_;
  }
  template <class Archive>
  static void load(Archive& ar, Property<T>& t, const unsigned int version) {
    ar & t.type_ & t.default_value_ & t.animatable_ & t.animated_;
    std::map<int64_t, sptr<Keyframe<T>>> keyframes;
    ar & keyframes;
    for (auto const& kv : keyframes)
      t.doCreateKeyframe(kv.second->time(), kv.second->value());
  }
};

}

}

namespace boost {
namespace serialization{
template <class Archive, class T>
inline void save(Archive& ar, const nlive::effect::Property<T>& t, const unsigned int version) {
  nlive::effect::Property<T>::access_::save(ar, t, version);
}
template <class Archive, class T>
inline void load(Archive& ar, nlive::effect::Property<T>& t, const unsigned int version) {
  nlive::effect::Property<T>::access_::load(ar, t, version);
}
template <class Archive, class T>
inline void serialize(Archive& ar, nlive::effect::Property<T>& t, const unsigned int version) {
  boost::serialization::split_free(ar, t, version);
}
}
}

#endif