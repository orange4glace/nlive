#include "model/effect/gray_scale_effect.h"

#include "renderer/video_renderer/command/gray_scale_render_command.h"

namespace nlive {

namespace effect {

const std::string GrayScaleEffect::TYPE = "nlive.effect.GrayScaleEffect";

GrayScaleEffect::GrayScaleEffect() :
  Effect(GrayScaleEffect::TYPE) {
  scale_ = QSharedPointer<Property<value::Scalar>>(
    new Property<value::Scalar>(value::Scalar(1)));

  connectProperty(scale_);
}

void GrayScaleEffect::render(QSharedPointer<video_renderer::CommandBuffer> command_buffer, int64_t timeoffset) {
  double v = scale_->getInterpolatedValue(timeoffset).value();
  auto cmd = QSharedPointer<video_renderer::GrayScaleRenderCommand>(
      new video_renderer::GrayScaleRenderCommand(v));
  command_buffer->addCommand(cmd);
}

QSharedPointer<Property<value::Scalar>> GrayScaleEffect::scale() {
  return scale_;
}

}

}