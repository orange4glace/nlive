#ifndef _NLIVE_TRANSFORM_EFFECT_H_
#define _NLIVE_TRANSFORM_EFFECT_H_

#include <QSharedPointer>

#include "model/effect/effect.h"
#include "model/effect/property.h"
#include "model/effect/value/vector2.h"

namespace nlive {

namespace effect {

class TransformEffect : public Effect {

private:
  QSharedPointer<Property<value::Vector2>> position_;
  QSharedPointer<Property<value::Vector2>> scale_;

public:
  static const std::string TYPE;

  TransformEffect();

  QSharedPointer<Property<value::Vector2>> position();
  QSharedPointer<Property<value::Vector2>> scale();

};

}

}

#endif