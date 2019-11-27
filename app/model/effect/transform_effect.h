#ifndef NLIVE_TRANSFORM_EFFECT_H_
#define NLIVE_TRANSFORM_EFFECT_H_

#include "base/common/memory.h"

#include "model/effect/effect.h"
#include "model/effect/property.h"
#include "model/effect/value/vector2.h"

namespace nlive {

namespace effect {

class TransformEffect : public Effect {

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & boost::serialization::base_object<Effect>(*this);
    ar & position_ & scale_;
  }

  sptr<Property<value::Vector2>> position_;
  sptr<Property<value::Vector2>> scale_;

public:
  static const std::string TYPE;

  TransformEffect();

  sptr<Property<value::Vector2>> position();
  sptr<Property<value::Vector2>> scale();

};

}

}

#endif