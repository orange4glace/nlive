#include "model/effect/value/vector2.h"

namespace nlive {

namespace effect {

namespace value {

Vector2::Vector2() : Vector2(0.0, 0.0) {}
Vector2::Vector2(double x, double y) :
  x_(x), y_(y) {
}
Vector2::Vector2(const Vector2& v) : Vector2(v.x_, v.y_) {}

double Vector2::x() const {
  return x_;
}

double Vector2::y() const {
  return y_;
}

Vector2 Vector2::interpolate(const Vector2& lhs, const Vector2& rhs, double t) {
  double x = lhs.x_ + (rhs.x_ - lhs.x_) * t;
  double y = lhs.y_ + (rhs.y_ - lhs.y_) * t;
  return Vector2(x, y);
}

bool Vector2::operator==(const Vector2 &rhs) const {
  return x_ == rhs.x_ && y_ == rhs.y_;
}

}

}

}