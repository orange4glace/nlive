#ifndef NLIVE_GRAY_SCALEEFFECT_H_
#define NLIVE_GRAY_SCALEEFFECT_H_

#include "base/common/memory.h"

#include "model/effect/effect.h"
#include "model/effect/property.h"
#include "model/effect/value/scalar.h"

namespace nlive {

namespace effect {

class GrayScaleEffect : public Effect {

private:
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