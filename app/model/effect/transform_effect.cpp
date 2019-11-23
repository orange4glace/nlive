#include "model/effect/transform_effect.h"

#include "model/effect/value/vector2.h"

namespace nlive {

namespace effect {

const std::string TransformEffect::TYPE = "nlive.effect.TransformEffect";

TransformEffect::TransformEffect() :
  Effect(TransformEffect::TYPE) {
  position_ = sptr<Property<value::Vector2>>(
    new Property<value::Vector2>(value::Vector2(0, 0)));
  scale_ = sptr<Property<value::Vector2>>(
    new Property<value::Vector2>(value::Vector2(0, 0)));

  connectProperty(position_);
  connectProperty(scale_);
}

sptr<Property<value::Vector2>> TransformEffect::position() {
  return position_;
}

sptr<Property<value::Vector2>> TransformEffect::scale() {
  return scale_;
}

}

}