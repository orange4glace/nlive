#ifndef NILVE_EFFECT_VALUE_VECTOR2_H_
#define NILVE_EFFECT_VALUE_VECTOR2_H_

#include <string>

namespace nlive {

namespace effect {

namespace value {

class Vector2 {

private:
  double x_;
  double y_;

public:
  inline Vector2(double x = 0.0, double y = 0.0) :
    x_(x), y_(y) {

  }

  inline Vector2(const Vector2& v) : Vector2(v.x_, v.y_) {}

  inline double x() const {
    return x_;
  }

  inline double y() const {
    return y_;
  }

  inline static Vector2 interpolate(const Vector2& lhs, const Vector2& rhs, double t) {
    double x = lhs.x_ + (rhs.x_ - lhs.x_) * t;
    double y = lhs.y_ + (rhs.y_ - lhs.y_) * t;
    return Vector2(x, y);
  }

};

}

}

}

#endif