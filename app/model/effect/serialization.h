#ifndef NLIVE_EFFECT_SERIALIZATION_H_
#define NLIVE_EFFECT_SERIALIZATION_H_

#include "model/effect/value/serialization.h"
#include "model/effect/transform_effect.h"
#include "model/effect/gray_scale_effect.h"

BOOST_CLASS_EXPORT_GUID(nlive::effect::TransformEffect, "nlive.model.effect.TransformEffect")
BOOST_CLASS_EXPORT_GUID(nlive::effect::GrayScaleEffect, "nlive.model.effect.GrayScaleEffect")

#endif