#ifndef NILVE_EFFECT_VALUE_SCALAR_H_
#define NILVE_EFFECT_VALUE_SCALAR_H_

#include <string>

namespace nlive {

namespace effect {

namespace value {

class Scalar {

private:
  double value_;

public:
  inline Scalar(double value = 0.0) :
    value_(value) {

  }

  inline Scalar(const Scalar& v) : Scalar(v.value_) {}

  inline double value() const {
    return value_;
  }

  inline static Scalar interpolate(const Scalar& lhs, const Scalar& rhs, double t) {
    double v = lhs.value_ + (rhs.value_ - lhs.value_) * t;
    return Scalar(v);
  }

};

}

}

}

#endif