#ifndef NLIVE_GRAY_SCALE_EFFECT_H_
#define NLIVE_GRAY_SCALE_EFFECT_H_

#include "base/common/memory.h"

#include "model/effect/effect.h"
#include "model/effect/value/scalar.h"
#include "model/effect/property.h"

namespace nlive {

namespace effect {

class GrayScaleEffect : public Effect {

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & boost::serialization::base_object<Effect>(*this);
    ar & scale_;
  }

  sptr<Property<value::Scalar>> scale_;

public:
  static const std::string TYPE;

  GrayScaleEffect();

  void render(sptr<video_renderer::CommandBuffer> command_buffer, int64_t timeoffset) override;

  sptr<Property<value::Scalar>> scale();

};

}

}

#endif