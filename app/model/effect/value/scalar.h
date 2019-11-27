#ifndef NILVE_EFFECT_VALUE_SCALAR_H_
#define NILVE_EFFECT_VALUE_SCALAR_H_

#include <string>
#include "base/common/serialize.h"

namespace nlive {

namespace effect {

namespace value {

class Scalar {

private:
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar & value_;
  }

  double value_;

public:
  Scalar();
  Scalar(double value);
  Scalar(const Scalar& v);

  double value() const;

  static Scalar interpolate(const Scalar& lhs, const Scalar& rhs, double t);

  bool operator==(const Scalar &rhs) const;

};

}

}

}

#endif