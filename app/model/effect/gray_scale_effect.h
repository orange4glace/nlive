#ifndef _NLIVE_GRAY_SCALEEFFECT_H_
#define _NLIVE_GRAY_SCALEEFFECT_H_

#include <QSharedPointer>

#include "model/effect/effect.h"
#include "model/effect/property.h"
#include "model/effect/value/scalar.h"

namespace nlive {

namespace effect {

class GrayScaleEffect : public Effect {

private:
  QSharedPointer<Property<value::Scalar>> scale_;

public:
  static const std::string TYPE;

  GrayScaleEffect();

  void render(QSharedPointer<video_renderer::CommandBuffer> command_buffer, int64_t timeoffset) override;

  QSharedPointer<Property<value::Scalar>> scale();

};

}

}

#endif