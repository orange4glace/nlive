#ifndef NILVE_EFFECT_VALUE_VECTOR2_H_
#define NILVE_EFFECT_VALUE_VECTOR2_H_

#include <QDebug>
#include <string>
#include "base/common/serialize.h"

namespace nlive {

namespace effect {

namespace value {

class Vector2 {

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & x_ & y_;
  }

  double x_;
  double y_;

public:
  Vector2();
  Vector2(double x, double y);
  Vector2(const Vector2& v);

  double x() const;
  double y() const;

  static Vector2 interpolate(const Vector2& lhs, const Vector2& rhs, double t);

  bool operator==(const Vector2 &rhs) const;

};

}

}

}

#endif