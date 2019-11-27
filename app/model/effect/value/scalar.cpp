#include "model/effect/value/scalar.h"

namespace nlive {

namespace effect {

namespace value {

Scalar::Scalar() : Scalar(0.0) {}

Scalar::Scalar(double value) : value_(value) {}

Scalar::Scalar(const Scalar& v) : Scalar(v.value_) {}

double Scalar::value() const {
  return value_;
}

Scalar Scalar::interpolate(const Scalar& lhs, const Scalar& rhs, double t) {
  double v = lhs.value_ + (rhs.value_ - lhs.value_) * t;
  return Scalar(v);
}

bool Scalar::operator==(const Scalar &rhs) const {
  return value_ == rhs.value_;
}

}

}

}