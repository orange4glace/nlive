#include <gtest/gtest.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "base/common/serialize.h"
#include "model/effect/value/scalar.h"
#include "model/effect/value/vector2.h"
#include "model/effect/property.h"
#include "model/effect/transform_effect.h"

using namespace nlive::effect;

TEST(Serialize, Scalar) {
  value::Scalar out_scalar(30);

  std::ofstream ofs("test.save");
  boost::archive::text_oarchive oa(ofs);
  oa << out_scalar;
  ofs.close();

  value::Scalar in_scalar;

  std::ifstream ifs("test.save");
  boost::archive::text_iarchive ia(ifs);
  ia >> in_scalar;
  ifs.close();

  EXPECT_EQ(in_scalar.value(), out_scalar.value());
}

TEST(Serialize, Vector2) {
  value::Vector2 out_value(10, 20);

  std::ofstream ofs("test.save");
  boost::archive::text_oarchive oa(ofs);
  oa << out_value;
  ofs.close();

  value::Vector2 in_value;

  std::ifstream ifs("test.save");
  boost::archive::text_iarchive ia(ifs);
  ia >> in_value;
  ifs.close();

  EXPECT_EQ(in_value.x(), out_value.x());
  EXPECT_EQ(in_value.y(), out_value.y());
}

TEST(Serialize, ScalarProperty) {
  Property<value::Scalar> out_property(value::Scalar(10), true);
  out_property.upsertKeyframe(10, value::Scalar(10));
  out_property.upsertKeyframe(20, value::Scalar(58.25));
  out_property.upsertKeyframe(100, value::Scalar(90.175));

  std::ofstream ofs("test.save");
  boost::archive::text_oarchive oa(ofs);
  oa << out_property;
  ofs.close();

  Property<value::Scalar> in_property(value::Scalar(0));

  std::ifstream ifs("test.save");
  boost::archive::text_iarchive ia(ifs);
  ia >> in_property;
  ifs.close();

  EXPECT_EQ(in_property.default_value(), out_property.default_value());
  EXPECT_EQ(in_property.animatable(), out_property.animatable());
  EXPECT_EQ(in_property.animated(), out_property.animated());
  EXPECT_EQ(in_property.keyframes().size(), out_property.keyframes().size());
  for (int i = 0; i < 10; i ++) {
    EXPECT_DOUBLE_EQ(in_property.getInterpolatedValue(i * 5 + i).value(),
        out_property.getInterpolatedValue(i * 5 + i).value());
  }
}

TEST(Serialize, TransformEffect) {
  sptr<TransformEffect> out_transform_effect =
    sptr<TransformEffect>(new TransformEffect());
  out_transform_effect->position()->upsertKeyframe(10, value::Vector2(10, 10));
  out_transform_effect->position()->upsertKeyframe(20, value::Vector2(40, -10));
  out_transform_effect->position()->upsertKeyframe(30, value::Vector2(93.125, 18.25));
  out_transform_effect->position()->upsertKeyframe(40, value::Vector2(-15.35, 88.75));
  out_transform_effect->scale()->upsertKeyframe(40, value::Vector2(0.5, 15.25));
  out_transform_effect->scale()->upsertKeyframe(20, value::Vector2(95.22, -0.55));
  out_transform_effect->scale()->upsertKeyframe(30, value::Vector2(30.15, 88.95));
  out_transform_effect->scale()->upsertKeyframe(40, value::Vector2(-125, 95));
  sptr<Effect> out_effect = sptr<Effect>(out_transform_effect);

  std::ofstream ofs("test.save");
  boost::archive::text_oarchive oa(ofs);
  oa << out_effect;
  ofs.close();

  sptr<Effect> in_effect;
  sptr<TransformEffect> in_transform_effect;

  std::ifstream ifs("test.save");
  boost::archive::text_iarchive ia(ifs);
  ia >> in_effect;
  ifs.close();

  ASSERT_EQ(in_effect->type(), out_effect->type());
  in_transform_effect = std::static_pointer_cast<TransformEffect>(in_effect);
  for (int i = 0; i < 10; i ++) {
    EXPECT_DOUBLE_EQ(in_transform_effect->position()->getInterpolatedValue(i * 5 + i).x(),
        out_transform_effect->position()->getInterpolatedValue(i * 5 + i).x());
    EXPECT_DOUBLE_EQ(in_transform_effect->position()->getInterpolatedValue(i * 5 + i).y(),
        out_transform_effect->position()->getInterpolatedValue(i * 5 + i).y());
    EXPECT_DOUBLE_EQ(in_transform_effect->scale()->getInterpolatedValue(i * 5 + i).x(),
        out_transform_effect->scale()->getInterpolatedValue(i * 5 + i).x());
    EXPECT_DOUBLE_EQ(in_transform_effect->scale()->getInterpolatedValue(i * 5 + i).y(),
        out_transform_effect->scale()->getInterpolatedValue(i * 5 + i).y());
  }
}